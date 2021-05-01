//	Guy Kabiri
//	312252224

#ifndef __ODD_EVEN_SORT_H__
#define __ODD_EVEN_SORT_H__

#include "mpi.h"
#include "cuboid.h"

typedef enum e_orientation { DESCENDING, ASCENDING } Orientation;

void odd_even_sort(int num_proc, int location, int left, int right, Orientation orientation,
					void* my_data,
					MPI_Datatype data_type,
					MPI_Comm comm,
					void (*min)(void*, void*),
					void (*max)(void*, void*));

#endif	//	__ODD_EVEN_SORT_H__
