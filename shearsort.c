//	Guy Kabiri
//	312252224

#include <string.h>
#include <math.h>
#include "shearsort.h"

void shearsort(int num_proc, int row, int col, void* my_data, Orientation orientation,
				MPI_Datatype data_type,
				MPI_Comm comm,
				void (*min)(void*, void*),
				void (*max)(void*, void*))
{
	int dim = sqrt(num_proc);
	int num_iterations = 2 * ceil(log2(num_proc)) + 1;

	int location, left, right;
	Orientation even_odd_orientation;

	for (int i = 0; i < num_iterations; i++)
	{
		if (i % 2 == 0)	//	even iteration
		{				//	each row is sorted, the orientation depend on if it is even or odd
			location 	= col;		//	process location in this row
			MPI_Cart_shift(comm, 1, 1, &left, &right);

			//	if even row, sort ASCENDING, else DESCENDING
			if (orientation == DESCENDING)
				even_odd_orientation = (row % 2 == 0) ? DESCENDING : ASCENDING;
			else
				even_odd_orientation = (row % 2 == 0) ? ASCENDING : DESCENDING;

		}
		else	//	odd iteration
		{		//	each column is sorted in ascending orientation
			location 	= row;		//	process location in this column
			MPI_Cart_shift(comm, 0, 1, &left, &right);

			even_odd_orientation	= orientation;
		}
		odd_even_sort(dim, location, left, right, even_odd_orientation, my_data, data_type, comm, min, max);
	}
}


int* collect_values(void* sorted, void* arr, size_t type_size, int rows, int cols, MPI_Comm comm2d)
{
	int rank = 0;
	int sortedIdx = 0;
	int coords[NUM_DIMS];

	for (int i = 0; i < rows; i++)
	{
		coords[0] = i;

		if (i % 2 == 0)
		{
			for (int j = 0; j < cols; j++)
			{
				coords[1] = j;
				MPI_Cart_rank(comm2d, coords, &rank);
				void* copy_to = sorted + (sortedIdx * type_size);
				void* copy_from = arr + (rank * type_size);
				memcpy(copy_to, copy_from, type_size);
				sortedIdx++;
//				sorted[sortedIdx++] = arr[rank];
			}
		}
		else
		{
			for (int j = cols - 1; j >= 0; j--)
			{
				coords[1] = j;
				MPI_Cart_rank(comm2d, coords, &rank);
				void* copy_to = sorted + (sortedIdx * type_size);
				void* copy_from = arr + (rank * type_size);
				memcpy(copy_to, copy_from, type_size);
				sortedIdx++;
//				sorted[sortedIdx++] = arr[rank];
			}
		}
	}

	return sorted;
}

/*
 * when creating mpi cartesian, it might create a matrix like shown below (the number represent the processes rank):
 *
 * |  0 |  1 |  2 |  3 |
 * |  4 |  5 |  6 |  7 |
 * |  8 |  9 | 10 | 11 |
 * | 12 | 13 | 14 | 15 |
 *
 *	but, it might create a matrix as shows below as well:
 *
 * | 14 |  1 | 13 |  8 |
 * |  4 | 11 |  6 |  7 |
 * | 12 |  9 | 10 |  5 |
 * |  3 |  2 |  0 | 15 |
 *
 * 	when using MPI_Gather, all the values are placed in the array order by their rank as:
 *
 *	|  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 |
 *
 *	and not like below:
 *
 *	| 14 |  1 | 13 |  8 |  4 | 11 |  6 |  7 | 12 |  9 | 10 |  5 |  3 |  2 |  0 | 15 |
 *
 *
 *	so when running through the array to collect the values by `snake` shape
 *	the values should be collected by their cartesian order
 *	it means that it is necessary to calculate the rank of the process by its cartesian values
 *	as a process with rank 14 has cartesian values of (0, 0)
 *	and process with rank 0 has cartesian values of (3, 2)
 * */
