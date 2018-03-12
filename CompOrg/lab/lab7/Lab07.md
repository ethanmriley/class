---
layout: post
title: Lab 07
description: Lab 07
category: lab
date: March 7, 2018
urlcolor: cyan
header-includes:
    - \usepackage{listings}
    - \lstset{basicstyle=\ttfamily}
---

In lecture on Momday we covered some material on logic design.
In C, we have a variety of bitwise operations at our disposal.
For example, when we are trying to construct an AND gate, we can use the `&` operator.
For OR gates, we can use `|` operator.
XOR gates use the `^` operator.
NOT gates use the `!` operator (and is the only unary operator).

## One bit adder

The [Wikipedia Adder](https://en.wikipedia.org/wiki/Adder_(electronics)) page details how to construct an Adder.
[lab07.zip] contains all of the starter template code for constructing your adder programs.
The `1-bit` directory contains the starter code and [Makefile](https://en.wikipedia.org/wiki/Makefile) for a one-bit adder.
Using the Wikipedia link above, use those formulae for your implementation.

## Create a four bit adder by stringing them together

Using both of the above and the starter code from the `rca` directory, construct a four-bit adder.
You can use `for` loops or whatever method you like but you must be able to add two four-bit values.
Note: pay attention to your loop ordering.
If you ask for values starting from zero, you probably want to start your computation with index 3 and propagate it forward.
