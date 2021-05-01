# Cuboids-Shearsort-MPI

An implementation of `Shearsort` algorithm for parallal sorting an array.
This implementation is using other parallal sorting algorithm which is `Odd-Even sort`.

In this implementation a banch of cuboids is read from an input file into an array,
which contain each cuboid's id, width, height, and depth.
The cuboids are beign sorted by the volum of each, or by the width if the volum is equals.

Each process gets a cuboid by `MPI_Scatter` and returns its value in the end of the algorithm with `MPI_Gather`.

##  Shearsort Iterations
This algorithm sorting an array of `nXn` values with `nXn` processes after $log(n)$ iterations, each iteration is $O(n)$ complexity, (in this case $n=\sqrt{n}$) so the final complexity time is $O(\sqrt{n}log(n))$.

### Initial
The processes are beign cartesian ordered by `MPI_Cart_create`.

```
| 14 |  1 | 13 |  8 |  
|  4 | 11 |  6 |  7 |  
| 12 |  9 | 10 |  5 |  
|  3 |  2 |  0 | 15 |
```

### Even Iterations
In even iteration each row of the matrix beign sorted in ascending order if it is an even row, or in descending order if it is an odd row.
```
|  1 |  8 | 13 | 14 |    ->
| 11 |  7 |  6 |  4 |    <-
|  5 |  9 | 10 | 12 |    ->
| 15 |  3 |  2 |  0 |    <-
```

### Odd Iterations
In odd iteration each column of the matrix beign sorted in ascending order regardless if it is even or odd column.

```
   ^    ^    ^    ^
|  1 |  3 |  2 |  0 |
|  5 |  7 |  6 |  4 |
| 11 |  8 | 10 | 12 |
| 15 |  9 | 13 | 14 |
```


### End of Iterations
At the end of the iterations the matrix will be as below.

```
|  0 |  1 |  2 |  3 |
|  7 |  6 |  5 |  4 |
|  8 |  9 | 10 | 11 |
| 15 | 14 | 13 | 12 |
```

All that left is to collect the values by a sanke shape

```
|  0 |  1 |  2 |  3 |   >
|  7 |  6 |  5 |  4 |   <
|  8 |  9 | 10 | 11 |   > 
| 15 | 14 | 13 | 12 |   <
```


##  Odd-Even Sort Iterations
This algorithm sorting an array of `n` values with `n` processes after `n` iterations. Complexity time $O(n)$
### Initial
The initial array will be as below.

```
Rank:   |  0 |  1 |  2 |  3 |

Value:  |  3 |  2 |  0 |  1 |     <-  the actual array
```

### Even Iterations
In even iterations an even-ranked process is communicates with his right odd-ranked neighbghr.
If discussing ascending order, the even-ranked process will save the smaller value, while the odd-ranked process will save the greater one.

```
    >-<       >-<
|  3 |  2 |  0 |  1 |
```

So after the iteration the array will look like below:

```
|  2 |  3 |  0 |  1 |
```

### Odd Iterations
In odd iterations an odd-ranked process will communicates with his right even-ranked process.
The odd-ranked process will save the smaller value while the even-ranked process will save the grater one.

```
         >-<
|  2 |  3 |  0 |  1 |
```

So after the iteration the array will look like below:

```
|  2 |  0 |  3 |  1 |
```

### End of Iterations
At the end of the iterations the array will be as below.

```
|  0 |  1 |  2 |  3 |
```
