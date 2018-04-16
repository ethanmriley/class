---
layout: post
title: Lab 10
description: Lab 10
category: lab
date: April 10, 2018
urlcolor: cyan
header-includes:
    \usepackage{listings}
    \lstset{basicstyle=\ttfamily}
---
[lab10.zip](https://www.evernote.com/l/AAKHQCGkWA9MYIbeAF66rm5y7FtFnUPLzSg)
contains all of the starter template code for constructing your programs.

## Cache slot count

Figuring out how many slots or lines exist in your direct-mapped cache is straightforward.
Simply determine how many possible index bit combinations exist.
For example, with two bits we have 00, 01, 10, and 11, or $2^2$.

## Determine the index of a would-be cache entry

Given an address, a starting point for your index, and the ending point of your index, find the appropriate index in a direct-mapped cache.
You may leverage some of your code from lab 9 if you wish.

## Determine the size of a cache line

Given the starting and ending point of your index as well as the block offset bits (BOB), determine the total cache line size in BITS.
