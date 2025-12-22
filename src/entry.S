.section .entry_os, "ax"
.globl _entry
_entry:
  /* Initialize global pointer for small-data accesses. */
  la gp, __global_pointer$

  /* Set up a simple boot stack. */
  la sp, stack_top

  /* Jump into C++ kernel main. */
  call main

1: j 1b

.section .bss
.align 16
stack:
  .skip 4096
stack_top:


