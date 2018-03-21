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

**ANSWER: (any three are fine)**

* Simplicity favors regularity. MIPS instructions are all 32 bits.
* Smaller is faster. MIPS has 32, 32 bit registers.
* Good Design Demands Good Compromise. To maintain the same instruction size, MIPS has different format types.
* Execute the common case fast. MIPS puts the constant in the instruction.

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

**ANSWER:**

We told them during the quiz to assume `$s0` as the base address (i.e., `matrix`) but it's fine if they used something else.

~~~
la $t0, 0($s0)
li $t1, 4          # We need to look at matrix[1] which is actually 4 off the base address
add $t2, $t1, $t0
lw $t3, 0($t2)
addi $t3, $t3, 12  # We need to look at matrix[1][3] which is 12 off the address of matrix[1]
li $t4, 14
sw $t4, 0($t3)
~~~

None of the particular registers they use are of any consequence but make sure they are consistent.
The difference between `la` and `li` is subtle so don't penalize heavily for that.

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

**ANSWER:**

Branch instructions only have 16 bits allocated for branch targets.
Therefore these instructions have a more restricted range than jump instructions.
As there are no possible targets on 1, 2 or 3-byte boundaries, we can get a range of 2^18 (2^17 in either direction).
They don't need to say all of that but basically have the correct idea i.e., jumps can reach further away than branches.

## 3.2 [15 points]

How can you rewrite this code to prevent this problem?

**ANSWER:**

~~~
foo:
        bne $t0, $t1, close_label
        j bar

close_label:
        add $t2, $t3, $t4
        ...

bar:
        add $t5, $t6, $t7
~~~


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

**ANSWER:**

~~~
mystrchr:
        add $t0, $zero, $a0

header:
        lb $t1, 0($t0)
        bne $t1, $a1, body
        j end

body:
        beq $t1, $zero, null_exit
        j latch

latch:
        addi $t0, $t0, 1
        j header

end:
        move $v0, $t0
        jr $ra

null_exit:
        move $v0, $zero
        jr $ra
~~~

Look for loop structures (header, body, latch).
Make sure they do at least one load, small penalty for `lw` instead of `lb`.
Be lenient.

\pagebreak
\centerline{\bf Extra space page. Please denote which problem you are answering here.}
