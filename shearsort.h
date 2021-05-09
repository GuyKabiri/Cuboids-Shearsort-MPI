#ifndef __SHEARSORT_H__
#define __SHEARSORT_H__

#include "mpi.h"
#include "odd_even_sort.h"

#define NUM_DIMS 2

void shearsort(int num_proc, int row, int col, void* my_data, Orientation orientation,
				MPI_Datatype* data_type,
				MPI_Comm* comm,
				void (*min)(void*, void*),
				void (*max)(void*, void*));

int* collect_values(void* sorted, void* arr, size_t type_size, int rows, int cols, MPI_Comm* comm2d);

#endif	//	__SHEARSORT_H__
