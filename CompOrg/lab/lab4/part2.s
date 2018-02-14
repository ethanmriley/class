      .text
      .globl main
main: li  $t1, 5
      li  $t0, 2
      slt $t2, $t0, $t1
      beq $t2, $zero, skip
      add $t0, $t0, $t1
skip: li  $v0, 1
      move $a0, $t0
      syscall
      jr $ra
