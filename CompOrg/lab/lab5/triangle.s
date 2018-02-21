    .data
ask:
    .asciiz "What is n? "

    .text
    .globl main
main:
    la $a0, ask #print string asking for input
    li $v0, 4
    syscall 

    li $v0, 5 #read int to $v0
    syscall

    move $t1, $v0 #store entered int in t1
    li   $t0, 0   #use t0 as a counter, initialize to 0
    
header:
    beq $t0, $t1, done
    j body

body:
   j latch 

latch:
    addi $t0, $t0, 1 #increment our counter
    j header

done:
     jr $ra   
