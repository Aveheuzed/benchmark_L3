.text

.type ref_good, @function
.globl ref_good
ref_good:
    # %rdi -> line length
    # %rsi -> number of lines
    # %rdx -> buffer

    pushq %rbx
    movq $1, %r8

.outer_loop_ref_good:
    movq %rdx, %rbx
    movq %rdi, %rcx
.inner_loop_ref_good:
    nop
    nop # movb (%rbx), %al # There is the single memory access of the function
    addq %r8, %rbx
    decq %rcx
    jnz .inner_loop_ref_good
    addq %rdi, %rdx
    decq %rsi
    jnz .outer_loop_ref_good

.end_ref_good:
    popq %rbx
    ret
.size ref_good, .-ref_good


.type ref_bad, @function
.globl ref_bad
ref_bad:
    # %rdi -> line length
    # %rsi -> number of lines
    # %rdx -> buffer

    pushq %rbx
    movq $1, %r8

.outer_loop_ref_bad:
    movq %rdx, %rbx
    movq %rsi, %rcx
.inner_loop_ref_bad:
    nop
    nop # movb (%rbx), %al # There is the single memory access of the function
    addq %rdi, %rbx
    decq %rcx
    jnz .inner_loop_ref_bad
    addq %r8, %rdx
    decq %rdi
    jnz .outer_loop_ref_bad

.end_ref_bad:
    popq %rbx
    ret
.size ref_bad, .-ref_bad


.section .note.GNU-stack, "", @progbits
.section .note.gnu.property, "a"
