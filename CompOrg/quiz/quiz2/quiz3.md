---
layout: default
title: CSCI 2500 Quiz 3
description: CSCI 2500 Quiz 3
date: October 13, 2017
header-includes:
    - \usepackage{listings}
    - \lstset{basicstyle=\ttfamily}
---

### NAME and RCS ID:\underline{\hspace{4.0in}}
### YOUR EMAIL IS (RCS ID) @ RPI.EDU

### THIS QUIZ IS CLOSED BOOK, CLOSED NOTES, and NO CALCULATOR.
### STAY AWAY FROM THE MARGINS AS THEY MAY GET TRIMMED OFF.

\pagebreak

# 1.  MIPS Design Principles [15 points]

Name three (and only three!) out of the four MIPS design principles.
For each principle indicate an example within the MIPS instruction set architecture of how it is used.

\pagebreak

# 2.  Matrix Indexing [25 points]

~~~
int** matrix = (int**)malloc(sizeof(int*) * 4);
for (int i = 0; i < 4; i++) {
    matrix[i] = (int*)malloc(sizeof(int) * 4);
}
~~~

The above code is different than the way that matrices were supported in our homework.
There our memory was allocated in a 16-element one-dimensional array but we simulated a 4x4 array.
Given the approach above, how would we go about setting `matrix[1][3]` to 14?

\pagebreak

# 3.  MIPS Branches [25 points]

## 3.1 [10 points]

Recall our discussion of MIPS branch instructions.
The code below is completely valid but may not always work.
Explain why.
**Hint: refer to the image below.**

~~~
foo:
        beq $t0, $t1, bar
        add $t2, $t3, $t4
        ...

bar:
        add $t5, $t6, $t7
~~~

![Instruction Reference](InstructionsCard.PNG)

\vspace{1.5in}

## 3.2 [15 points]

How can you rewrite this code to prevent this problem?

\pagebreak

# 4.  Convert C to MIPS [35 points]

From the man page: The `strchr()` function locates the first occurrence of `c` (converted to a
`char`) in the string pointed to by `s`.
The functions `strchr()` returns a pointer to the located character, or `NULL` if the character does not appear in the string.
Assume `NULL` = 0.

Convert the following function from C to MIPS assembly.
It may be helpful to use basic blocks to outline your program (especially your loop).
You are allowed to use MIPS pseudo-instructions e.g., `blt`, `bgt`, etc.
Additionally, assume the array always has at least one element.
Per convention, `s` will be in `$a0` and `c` will be in `$a1`.
Your return value should be placed in `$v0`.

~~~
char *mystrchr(char *s, int c) {
    while (*s != (char) c) {
        if (!*s++) {
            return NULL;
        }
    }
    return (char *)s;
}
~~~

\pagebreak
\centerline{\bf Extra space page. Please denote which problem you are answering here.}
