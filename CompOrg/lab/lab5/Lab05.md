---
layout: post
title: Lab 05
description: Lab 05
category: lab
date: February 21, 2018
urlcolor: cyan
header-includes:
    - \usepackage{listings}
    - \lstset{basicstyle=\ttfamily}
---

One thing that confused many students during Lab 4 was the `jr $ra` instruction at the end of the `main()` function.
When our `main()` function gets called (and yes, it *must* get called from somewhere) it's called via a `jal main` instruction.
Recall from lecture that `jal` stores the address of the next instruction in the `$ra` (return address) register.
When the `main()` function concludes, we should jump back to where we were called from which will be stored in `$ra.`
This is discussed on the slides dealing with leaf / non-leaf functions.

Additionally, some of you were exiting SPIM completely and re-starting it every time.
This is unnecessary: you can simply go to the File menu and select "Reinitialize and Load File."
This should reset your simulator to a pristine state every time you wish to run it.

## Control Flow Graphs

Control flow graphs (CFGs) are flow diagrams for computer programs.
From [Wikipedia](https://en.wikipedia.org/wiki/Basic_block): "$\ldots$ a basic block is a straight-line code sequence with no branches in except to the entry and no branches out except at the exit. This restricted form makes a basic block highly amenable to analysis. Compilers usually decompose programs into their basic blocks as a first step in the analysis process. Basic blocks form the vertices or nodes in a control flow graph."

With MIPS instructions, when we encounter a branch instruction such as a `beq`, we either jump to the label or go to the next instruction.
In this lab, we are going to use `branch`/`jump` pairs to force explicit flow control.
In the event that we don't follow our `branch` condition, we will explicitly jump to another basic block.
Therefore, the following code must arrive at either the `next` label or the `after` label.

~~~
bne $s0, $t0, next
j after
~~~

Loops have three primary basic blocks: the loop header, the latch, and one or more basic blocks composing the loop body.
The loop header determines whether or not to continue the loop body (e.g., whether `i < 5` evaluates to true).
The loop body performs some operations and jumps directly to the latch block.
The latch block modifies the loop variable (e.g., `i++`) and jumps directly to the loop header.


## MIPS Triangle (part 1)

In [Lab 01](https://piazza.com/class_profile/get_resource/j6fema68yeu70x/j78z2f0hn9q5jj) we had a problem that asks you to program a triangle:

`What is n? 4`  
`*`  
`**`  
`***`  
`****`  

*Draw* the CFG corresponding to this program using the guidelines given above and the example CFG.

## MIPS Triangle (part 2)

Implement this following the CFG you constructed from the previous checkpoint.
