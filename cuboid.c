#include "cuboid.h"

//	read cuboids from a file
//	returns array of cuboids, size of array and the order to sort
Cuboid* read_cuboids_from_file(FILE* file, int* size, int* order)
{
    if (!file)
        return NULL;

    if (fscanf(file, "%d", size) != 1)	return NULL;
    if (fscanf(file, "%d", order) != 1)	return NULL;

    if (*size < 1 || (*order != 0 && *order != 1))  //  if number of cuboids is less than 1 or order is undefined
        return NULL;

    Cuboid* arr = malloc(sizeof(Cuboid) * *size);
    if (!arr)
        return NULL;

    for (int i = 0; i < *size; i++)
    {
        if (!read_cuboid_from_file(file, &(arr[i])))
            return NULL;
    }

    return arr;
}


//	read a single cuboid from a file
Cuboid* read_cuboid_from_file(FILE* file, Cuboid* readInto)
{
    if (!file || !readInto)
        return NULL;

    if (fscanf(file, "%d %f %f %f",
    		&readInto->id,
			&readInto->height,
			&readInto->width,
			&readInto->depth) != 4)		//	if could not read all values
    	return NULL;

    return readInto;
}


//	return reference to array on success, NULL otherwise
Cuboid* write_cuboids_ids_to_file(FILE* file, Cuboid* arr, int size)
{
	if (!file || !arr)
		return NULL;

	for (int i = 0; i < size; i++)
	{
		if (fprintf(file, "%d ", arr[i].id) < 0)
			return NULL;
	}

	return arr;
}


//	calculate the area of a cubiod
double calc_surface_area(Cuboid* c)
{
	double side1 = c->width * c->height;
	double side2 = c->height * c->depth;
	double side3 = c->width * c->depth;
	double surface_area = 2 * (side1 + side2 + side3);
	return surface_area;
}


//	returns value greater than 0 if c1 is greater, smaller than 0 if c1 if smaller, otherwise 0
int compare_cuboids(Cuboid* c1, Cuboid* c2)
{
	double area1 = calc_surface_area(c1);
	double area2 = calc_surface_area(c2);
	
	if (area1 > area2)
		return 1;
	if (area2 > area1)
		return -1;

	return c1->width - c2->width;
}


//	will save the min cuboid in c1
void get_min_cuboid(void* c1, void* c2)
{
	if (compare_cuboids(c1, c2) > 0)
		swap_cuboids(c1, c2);	//	c2 smaller than c1
}


///	will save the max cuboid in c1
void get_max_cuboid(void* c1, void* c2)
{
	if (compare_cuboids(c1, c2) < 0)
		swap_cuboids(c1, c2);	//	c2 bigger than c1
}


//	swap between c1 and c2 values
void swap_cuboids(Cuboid* c1, Cuboid* c2)
{
	c1->id		= c2->id;
	c1->width	= c2->width;
	c1->height	= c2->height;
	c1->depth	= c2->depth;
}


//	print single cubiod
void print_cuboid(Cuboid* c)
{
	printf("%3d: %.3f, %.3f, %.3f", c->id, c->height, c->width, c->depth);
}


void print_cuboids_arr(Cuboid* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("(%2d, %6g)\n", arr[i].id, calc_surface_area(&arr[i]));
	}
}


void print_cuboids_arr_as_mat(Cuboid* arr, int rows, int cols)
{
	int idx = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++, idx++)
		{
			printf("(%2d, %6g)", arr[idx].id, calc_surface_area(&arr[idx]));
			if (j < cols - 1)
				printf(", ");
		}
		printf("\n");
	}
}
