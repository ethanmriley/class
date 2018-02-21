    .data
ask:
    .asciiz "What is n? "
ast:
    .asciiz "*"
newline:
    .asciiz "\n"

    .text
    .globl main
main:
    la $a0, ask #print string asking for input
    li $v0, 4
    syscall 

    li $v0, 5 #read int to $v0
    syscall

    move $t2, $v0 #store entered int in t1
    li   $t1, 1   #use t1 as a counter, initialize to 1
    
header1:
    bgt $t1, $t2, exit
    li  $t0, 1
    j header2

header2:
    bgt $t0, $t1, latch1
    j print

print:
    li $v0, 4
    la $a0, ast
    syscall
    j latch2   

latch2:
    addi $t0, $t0, 1
    j header2 

latch1:
    addi $t1, $t1, 1 #increment our counter

    li $v0, 4
    la $a0, newline    
    syscall

    j header1

exit:
     jr $ra   
