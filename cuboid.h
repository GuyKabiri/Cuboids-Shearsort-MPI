//	Guy Kabiri
//	312252224

#ifndef __CUBOID_H__
#define __CUBOID_H__

#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_PARAMS 4

typedef struct cuboid {
    int id;
    float height;
    float width;
    float depth;
} Cuboid;

Cuboid* read_cuboids_from_file(FILE* file, int* size, int* order);
Cuboid* read_cuboid_from_file(FILE* file, Cuboid* readInto);
Cuboid* write_cuboids_ids_to_file(FILE* file, Cuboid* arr, int size);
double calc_area(Cuboid* c);
int compare_cuboids(Cuboid* c1, Cuboid* c2);
void get_min_cuboid(Cuboid* c1, Cuboid* c2);
void get_max_cuboid(Cuboid* c1, Cuboid* c2);
void swap_cuboids(Cuboid* c1, Cuboid* c2);
void print_cuboid(Cuboid* c);
void print_cuboids_arr(Cuboid* arr, int size);
void print_cuboids_arr_as_mat(Cuboid* arr, int rows, int cols);

#endif // __CUBOID_H__
