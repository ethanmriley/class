    .data
n:
    .word 0
ask:
    .asciiz "What is n? "

    .text
    .globl main
main:
    la $a0, ask
    li $v0, 4
    syscall 

    li $v0, 5
    syscall

    sw $v0, n

    li $v0, 1
    lw $a0, n
    syscall

    jr $ra
