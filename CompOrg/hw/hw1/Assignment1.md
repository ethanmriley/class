---
layout: default
title: CSCI 2500 Assignment 1
description: CSCI 2500 Assignment 1
urlcolor: cyan
header-includes:
    - \usepackage{listings}
    - \lstset{basicstyle=\ttfamily}
---

# Matrix Multiplication

## Due Date:Friday, February 2nd, 11:59:59 PM

For this assignment we will be performing [matrix multiplication](https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm).
We will be revisiting this topic in the future but for now we are implementing the algorithm in C.

Following the template below, ask for the dimensions of both matrices.
Once it has been confirmed that the matrices can be multiplied, ask for the values contained within each matrix.
Your program should then output the first and second matrix as well as the resulting matrix product.

Your task is to fill in the functions to complete the program skeleton given in the template file.
This is a relatively short program to ensure everyone is up to speed on C programming topics.
Your program should accept non-negative integer values for each element in the matrix.
It should exit immediately upon invalid matrix dimensions.

The majority of your points (85) are coming from a program that a) compiles, and b) runs correctly.
The remaining 15 points are allocated for proper commenting and style.
These comments are to be provided by you.
At the very least you should provide a comment above any function definition or anywhere your code might get more complicated.

Your code should have a clear, logical organization.
In general, each function should easily fit on a single screen and have a clear purpose.
Variable names and function names should be intuitive and meaningful.
Use a consistent, logical method of indentation to make your program easy to read.

Below are some sample output.
Note that your program should match these samples exactly.

~~~
bash-3.2$ ./a.out
How many rows are in the first matrix?
2
How many columns are in the first matrix?
3
How many rows are in the second matrix?
3
How many columns are in the second matrix?
1

Please enter the values for the 2 x 3 matrix:
1 2 3
4 5 6
Please enter the values for the 3 x 1 matrix:
7
8
9

1 2 3
4 5 6

multiplied by

7
8
9

is:

50
122
bash-3.2$ ./a.out
How many rows are in the first matrix?
4
How many columns are in the first matrix?
5
How many rows are in the second matrix?
6
How many columns are in the second matrix?
7

Invalid matrix multiplication!
bash-3.2$ ./a.out
How many rows are in the first matrix?
2
How many columns are in the first matrix?
2
How many rows are in the second matrix?
2
How many columns are in the second matrix?
2

Please enter the values for the 2 x 2 matrix:
1 2
3 4
Please enter the values for the 2 x 2 matrix:
5 6
7 8

1 2
3 4

multiplied by

5 6
7 8

is:

19 22
43 50
bash-3.2$ exit
~~~
