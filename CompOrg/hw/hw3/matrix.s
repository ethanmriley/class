#############################################################################
#############################################################################
## Assignment 3: Your Name Goes Here!
#############################################################################
#############################################################################

#############################################################################
#############################################################################
## Data segment
#############################################################################
#############################################################################

.data

matrix_a:    .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
matrix_b:    .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
result:      .word 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0

newline:     .asciiz "\n"
tab:         .asciiz "\t"


#############################################################################
#############################################################################
## Text segment
#############################################################################
#############################################################################

.text                  # this is program code
.align 2               # instructions must be on word boundaries
.globl main            # main is a global label
.globl multiply
.globl matrix_multiply
.globl matrix_print
.globl matrix_ask

#############################################################################
matrix_ask:
#############################################################################
# Ask the user for the current matrix residing in the $a0 register
    sub $sp, $sp, 4
    sw $ra, 0($sp)

    # init our counter
    li $t0, 0
    # t1 holds our the address of our matrix
    move $t1, $a0

ma_head:
# if counter less than 16, go to ma_body
# else go to exit
    li $t2, 16
    blt $t0, $t2, ma_body
    j ma_exit

ma_body:
    # read int
    li $v0, 5
    syscall
    li $t2, 4
    # ints are 4 bytes
    multu $t0, $t2
    mflo $t2
    add $t2, $t2, $t1
    sw $v0, 0($t2)
    j ma_latch

ma_latch:
    addi $t0, $t0, 1
    j ma_head

ma_exit:
    lw $ra, 0($sp)
    add $sp, $sp, 4
    jr $ra

#############################################################################
main:
#############################################################################
    # alloc stack and store $ra
    sub $sp, $sp, 4
    sw $ra, 0($sp)

    # load A, B, and result into arg regs
    la $a0, matrix_a
    jal matrix_ask
    la $a0, matrix_b
    jal matrix_ask

    la $a0, matrix_a
    la $a1, matrix_b
    la $a2, result
    jal matrix_multiply

    la $a0, result
    jal matrix_print

    # restore $ra, free stack and return
    lw $ra, 0($sp)
    add $sp, $sp, 4
    jr $ra

##############################################################################
multiply:
##############################################################################  
# mult subroutine $a0 times $a1 and returns in $v0

    # start with $t0 = 0
    add $t0,$zero,$zero
mult_loop:
    # loop on a1
    beq $a1,$zero,mult_eol

    add $t0,$t0,$a0
    sub $a1,$a1,1
    j mult_loop

mult_eol:
    # put the result in $v0
    add $v0,$t0,$zero

    jr $ra

##############################################################################
matrix_multiply: 
##############################################################################
# mult matrices A and B together of square size N and store in result.

    # alloc stack and store regs
    sub $sp, $sp, 24
    sw $ra, 0($sp)
    sw $a0, 4($sp)
    sw $a1, 8($sp)
    sw $s0, 12($sp)
    sw $s1, 16($sp)
    sw $s2, 20($sp)

    #setup for i loop

    #setup for j loop

    #setup for k loop

    # compute A[i][k] address and load into $t3

    # compute B[k][j] address and load into $t4

    # call the multiply function

    # increment k and jump back or exit

    #increment j and jump back or exit

    #increment i and jump back or exit

    # retore saved regs from stack
    lw $s2, 20($sp)
    lw $s1, 16($sp)
    lw $s0, 12($sp)
    lw $a1, 8($sp)
    lw $a0, 4($sp)
    lw $ra, 0($sp)

    # free stack and return
    add $sp, $sp, 24
    jr $ra

##############################################################################
matrix_print:
##############################################################################

    # alloc stack and store regs.
    sub $sp, $sp, 16
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $a0, 12($sp)

    li $t0, 4 # size of array

    # do your two loops here

    # setup to jump back and return

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $a0, 12($sp)
    add $sp, $sp, 16
    jr $ra
