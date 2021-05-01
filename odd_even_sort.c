//	Guy Kabiri
//	312252224

#include "odd_even_sort.h"

//	if process is in the edge of the array, it's neighbor should be a negative number
void odd_even_sort(int num_proc, int location, int left, int right, Orientation orientation,
					void* my_data,
					MPI_Datatype* data_type,
					MPI_Comm* comm,
					void (*min)(void*, void*),
					void (*max)(void*, void*))
{
	MPI_Status status;

	//	Default orientation is ASCENDING
	void (*f1)(void*, void*) = min;
	void (*f2)(void*, void*) = max;

	if (orientation == DESCENDING)
	{
		f1 = max;
		f2 = min;
	}

	Cuboid other_data;
	int other_source;
	void (*func)(void*, void*);

	for (int i = 0; i < num_proc; i++)
	{
		if (location % 2 == 0) 	//	even location
		{
			if (i % 2 == 0)	//	even iteration -> communicate with right side
			{
				other_source = right;
				func = f1;
			}
			else	//	odd iteration -> communicate with left side
			{
				other_source = left;
				func = f2;
			}
		}
		else //	odd location
		{
			if (i % 2 == 0)//	even iteration -> communicate with left side
			{
				other_source = left;
				func = f2;
			}
			else	//	odd iteration -> communicate with right side
			{
				other_source = right;
				func = f1;
			}
		}

		//	if this process's location is first or last, it will not communicate during some of the iterations
		if (other_source >= 0)
		{
//			MPI_Send(my_data, 1, data_type, other_source, 0, comm);
//			MPI_Recv(other_data, 1, data_type, other_source, 0, comm, &status);

			MPI_Sendrecv(my_data, 1, *data_type, other_source, 0, &other_data, 1, *data_type, other_source, 0, *comm, &status);

			func(my_data, &other_data);
		}
	}
}
