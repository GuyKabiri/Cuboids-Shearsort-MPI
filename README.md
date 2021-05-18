# Cuboids-Shearsort-MPI

An implementation of the `Shearsort` algorithm for parallel sorting an array.
This implementation is using another parallel sorting algorithm which is `Odd-Even sort`.

In this implementation, a bench of cuboids is read from an input file into an array,
which contain each cuboid's id, width, height, and depth.
The cuboids are being sorted by the volume of each, or by the width if the volume is equals.

Each process gets a cuboid by `MPI_Scatter` and returns its value in the end of the algorithm with `MPI_Gather`.

##  Shearsort Iterations
This algorithm sorting an array of `nXn` values with `nXn` processes after 
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;log(n)" title="\inline log(n)" /> 
iterations, each iteration is 
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;O(n)" title="\inline O(n)" /> 
complexity, (in this case 
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;n=\sqrt{n}" title="\inline n=\sqrt{n}" /> 
) so the final complexity time is 
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;O(\sqrt{n}log(n))" title="\inline O(\sqrt{n}log(n))" />.

### Initial
The processes are cartesian ordered by `MPI_Cart_create`.

```
| 14 |  1 | 13 |  8 |  
|  4 | 11 |  6 |  7 |  
| 12 |  9 | 10 |  5 |  
|  3 |  2 |  0 | 15 |
```

### Even Iterations
In even iteration, each row of the matrix is sorted in ascending order if it is an even row, or in descending order if it is an odd row.
```
|  1 |  8 | 13 | 14 |    ->
| 11 |  7 |  6 |  4 |    <-
|  5 |  9 | 10 | 12 |    ->
| 15 |  3 |  2 |  0 |    <-
```

### Odd Iterations
In odd iteration, each column of the matrix is sorted in ascending order regardless if it is an even or odd column.

```
   ^    ^    ^    ^
|  1 |  3 |  2 |  0 |
|  5 |  7 |  6 |  4 |
| 11 |  8 | 10 | 12 |
| 15 |  9 | 13 | 14 |
```


### End of Iterations
At the end of the iterations, the matrix will be as below.

```
|  0 |  1 |  2 |  3 |
|  7 |  6 |  5 |  4 |
|  8 |  9 | 10 | 11 |
| 15 | 14 | 13 | 12 |
```

All that left is to collect the values by a snake shape.

```
|  0 |  1 |  2 |  3 |   >
|  7 |  6 |  5 |  4 |   <
|  8 |  9 | 10 | 11 |   > 
| 15 | 14 | 13 | 12 |   <
```


##  Odd-Even Sort Iterations
This algorithm sorting an array of `n` values with `n` processes after `n` iterations. Complexity time 
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;O(n)" title="\inline O(n)" />.
### Initial
The initial array will be as below.

```
Rank:   |  0 |  1 |  2 |  3 |

Value:  |  3 |  2 |  0 |  1 |     <-  the actual array
```

### Even Iterations
In even iterations, an even-ranked process is communicating with his right odd-ranked neighbor.
In ascending order, the even-ranked process will save the smaller value, while the odd-ranked process will save the greater one.

```
    >-<       >-<
|  3 |  2 |  0 |  1 |
```

So after the iteration, the array will look like below:

```
|  2 |  3 |  0 |  1 |
```

### Odd Iterations
In odd iterations, an odd-ranked process will communicate with his right even-ranked process.
The odd-ranked process will save the smaller value while the even-ranked process will save the greater one.

```
         >-<
|  2 |  3 |  0 |  1 |
```

So after the iteration, the array will look like below:

```
|  2 |  0 |  3 |  1 |
```

### End of Iterations
At the end of the iterations, the array will be as below.

```
|  0 |  1 |  2 |  3 |
```
