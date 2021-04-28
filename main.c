//	Guy Kabiri
//	312252224

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "main.h"

#include "odd_even_sort.h"
#include "shearsort.h"

#define INPUT_FILE_NAME "../src/cuboids.dat"
#define OUTPUT_FILE_NAME "../src/result.dat"

#define ROOT 0


int main(int argc, char* argv[])
{
	int  my_rank;			//	rank of process
	int  num_processes;     //	number of processes

	MPI_Init(&argc, &argv);	//	start MPI

	/* create a type for struct Cuboid */
	MPI_Datatype 	mpi_cuboid_type;
	
					//	number of blocks for each parameter
	int          	blocklengths[NUM_OF_PARAMS] = {1, 1, 1, 1};
	
					//	offset of each parameter, calculated by size of previous parameters
	MPI_Aint 		displacements[NUM_OF_PARAMS] = { 0, sizeof(int), sizeof(int) + sizeof(float), sizeof(int) + 2 * sizeof(float) };
	MPI_Datatype 	types[NUM_OF_PARAMS] = { MPI_INT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT };

	MPI_Type_create_struct(NUM_OF_PARAMS, blocklengths, displacements, types, &mpi_cuboid_type);
	MPI_Type_commit(&mpi_cuboid_type);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);		//	get process rank
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);	//	get number of processes

	int dim = sqrt(num_processes);

	Cuboid* arr = NULL;
	Cuboid my_cuboid;	//	cuboid struct for each process
	int size = 0;		//	size of array
	int order = 0;		//	order of sorting

	//	ROOT process
	if (my_rank == ROOT)
	{
		FILE* input = fopen(INPUT_FILE_NAME, "r");
		if (!input)
		{
			printf("Could not open input file '%s'\n", INPUT_FILE_NAME);
			MPI_Type_free(&mpi_cuboid_type);
			MPI_Abort(MPI_COMM_WORLD, 2);
			exit(-1);
		}

		arr = read_cuboids_from_file(input, &size, &order);
		fclose(input);
		
		if (!arr)
		{
			printf("Error reading input file or allocating space\n");
			MPI_Type_free(&mpi_cuboid_type);
			MPI_Abort(MPI_COMM_WORLD, 2);
			exit(-1);
		}

		if (size != num_processes)
		{
			printf("Number of processes is different from number of cuboids in the file\n");
			free(arr);
			MPI_Type_free(&mpi_cuboid_type);
			MPI_Abort(MPI_COMM_WORLD, 2);
			exit(-1);
		}

		print_cuboids_arr_as_mat(arr, dim, dim);
		printf("\n");
	}

	//	ROOT process send the array of cubiods to children (one for each)
	MPI_Scatter(arr, 1, mpi_cuboid_type, &my_cuboid, 1, mpi_cuboid_type, ROOT, MPI_COMM_WORLD);

	//	initiate cartesian coordinates
	int dims[] = { dim, dim };
	int periods[] = { 0, 0 };
	MPI_Comm comm_2d;
	MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, 0, &comm_2d);

	int my_coords[] = { 0, 0 };		//	store coordinates of each process
	MPI_Cart_coords(comm_2d, my_rank, NUM_DIMS, my_coords);

	//perform shearsort of the cuboids array
	shearsort(num_processes, my_coords[0], my_coords[1], &my_cuboid, mpi_cuboid_type, comm_2d);

	//	Gather back all the cuboids to the ROOT process
	MPI_Gather(&my_cuboid, 1, mpi_cuboid_type, arr, 1, mpi_cuboid_type, ROOT, MPI_COMM_WORLD);


	if (my_rank == 0)
	{
		//	allocating space to store the sorted values
		Cuboid* sorted = malloc(sizeof(Cuboid) * num_processes);
		if (!sorted)
		{
			printf("Error allocating space\n");
			MPI_Type_free(&mpi_cuboid_type);
			free(arr);
			MPI_Abort(MPI_COMM_WORLD, 2);
			exit(-1);
		}

		//	collect the values by snake shape
		collect_values(sorted, arr, num_processes, comm_2d);

		print_cuboids_arr_as_mat(arr, dim, dim);
		printf("\n");

		printf("The sorted array:\n");
		print_cuboids_arr(sorted, num_processes);
		printf("\n");

		//	open the output file to write the cuboids ids
		FILE* output = fopen(OUTPUT_FILE_NAME, "w");
		if (!output)
		{
			printf("Could not open output file '%s'\n", OUTPUT_FILE_NAME);
			MPI_Type_free(&mpi_cuboid_type);
			free(sorted);
			free(arr);
			MPI_Abort(MPI_COMM_WORLD, 2);
			exit(-1);
		}

		//	if writing ids failed
		if (!write_cuboids_ids_to_file(output, sorted, num_processes))
		{
			printf("Error writing to output file '%s'\n", OUTPUT_FILE_NAME);
		}

		fclose(output);
		free(sorted);
		free(arr);
	}

    MPI_Type_free(&mpi_cuboid_type);
    MPI_Finalize();

    exit(0);
}
