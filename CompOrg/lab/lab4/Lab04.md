---
layout: post
title: Lab 04
description: Lab 04
category: lab
date: February 14, 2018
urlcolor: cyan
---

We are starting our section on assembly language programming .
Our textbook has selected MIPS due largely to its simplicity.
Also, feel free to peruse the [MIPS Quick Tutorial](http://logos.cs.uic.edu/366/notes/mips%20quick%20tutorial.htm) from UIC.

## Install SPIM

SPIM is the MIPS simulator.
You can download the appropriate installer [here](https://sourceforge.net/projects/spimsimulator/files/).
After installing it, run [helloworld.s](https://piazza.com/class_profile/get_resource/j6fema68yeu70x/j829rfwudxf539) in the SPIM simulator.
Modify it from saying, "Hello World" to "Hello CSCI-2500."

## MIPS translation

Re-write the snippet below in MIPS using `$t0` as the location holding `x`.
Print out the result for initial values of `$t0` of 2, 4, 6, and 8.
(It may be helpful to look at [this page](http://students.cs.tamu.edu/tanzir/csce350/reference/syscalls.html) for help on printing out values.)
This may require the `slt` (set on less than) and `beq` (branch equal) instructions and their counterparts.
You can read more about MIPS control-flow instructions [here](https://courses.cs.washington.edu/courses/cse378/02au/Lectures/07controlI.pdf).
You may also find the [instruction reference](https://piazza.com/class_profile/get_resource/j6fema68yeu70x/j829r4n76i64wj) from the front of your book useful.

~~~
if (x < 5) {
	x += 5;
}
~~~


