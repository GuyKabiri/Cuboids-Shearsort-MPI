//	Guy Kabiri
//	312252224

#ifndef __SHEARSORT_H__
#define __SHEARSORT_H__

#include "mpi.h"
#include "odd_even_sort.h"

void shearsort(int num_proc, int row, int col, Cuboid* my_c, MPI_Datatype data_type, MPI_Comm comm);
Cuboid* collect_values(Cuboid* sorted, Cuboid* arr, int size);

#endif	//	__SHEARSORT_H__
