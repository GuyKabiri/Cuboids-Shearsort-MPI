//	Guy Kabiri
//	312252224

#include <math.h>
#include "shearsort.h"

void shearsort(int num_proc, int row, int col, Cuboid* my_data, MPI_Datatype data_type, MPI_Comm comm)
{
	int dim = sqrt(num_proc);
	int num_iterations = 2 * ceil(log2(num_proc)) + 1;

	int location, left, right;
	Orientation orientation;

	for (int i = 0; i < num_iterations; i++)
	{
		if (i % 2 == 0)	//	even iteration
		{				//	each row is sorted, the orientation depend on if it is even or odd
			location 	= col;		//	process location in this row
			MPI_Cart_shift(comm, 1, 1, &left, &right);

			//	if even row, sort ASCENDING, else DESCENDING
			orientation = (row % 2 == 0) ? ASCENDING : DESCENDING;

		}
		else	//	odd iteration
		{		//	each column is sorted in ascending orientation
			location 	= row;		//	process location in this column
			MPI_Cart_shift(comm, 0, 1, &left, &right);

			orientation	= ASCENDING;
		}
		odd_even_sort(dim, location, left, right, orientation, my_data, data_type, comm);
	}
}


Cuboid* collect_values(Cuboid* sorted, Cuboid* arr, int size)
{
	int dim = sqrt(size);

	int arrIdx = 0;
	int sortedIdx = 0;

	for (int i = 0; i < dim; i++)
	{
		if (i % 2 == 0)
		{
			for (int j = 0; j < dim; j++)
			{
				arrIdx = i * dim + j;
				sorted[sortedIdx++] = arr[arrIdx];
			}
		}
		else
		{
			for (int j = dim - 1; j >= 0; j--)
			{
				arrIdx = i * dim + j;
				sorted[sortedIdx++] = arr[arrIdx];
			}
		}
	}

	return sorted;
}
