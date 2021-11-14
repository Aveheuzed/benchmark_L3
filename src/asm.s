    .text

.type good, @function
.globl good
good:
    # %rdi -> line length
    # %rsi -> number of lines
    # %rdx -> buffer

    pushq %rbx
    movq $1, %r8

.outer_loop_good:
    movq %rdx, %rbx
    movq %rdi, %rcx
.inner_loop_good:
    movb (%rbx), %al # There is the single memory access of the function
    addq %r8, %rbx
    decq %rcx
    jnz .inner_loop_good
    addq %rdi, %rdx
    decq %rsi
    jnz .outer_loop_good

.end_good:
    popq %rbx
    ret
.size good, .-good


.type bad, @function
.globl bad
bad:
    # %rdi -> line length
    # %rsi -> number of lines
    # %rdx -> buffer

    pushq %rbx
    movq $1, %r8

.outer_loop_bad:
    movq %rdx, %rbx
    movq %rsi, %rcx
.inner_loop_bad:
    movb (%rbx), %al # There is the single memory access of the function
    addq %rdi, %rbx
    decq %rcx
    jnz .inner_loop_bad
    addq %r8, %rdx
    decq %rdi
    jnz .outer_loop_bad

.end_bad:
    popq %rbx
    ret
.size bad, .-bad


.section .note.GNU-stack, "", @progbits
.section .note.gnu.property, "a"
