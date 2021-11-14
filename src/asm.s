    .text

.type good, @function
.globl good
good:
    # %rdi -> line length
    # %rsi -> number of lines
    # %rdx -> buffer

    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    movq %rdx, %rbp # %rdx is going to be overwritten by MUL

    xorq %rbx, %rbx # %rbx -> line number (from 0 to %rsi-1)
.outer_loop_good:
    xorq %rcx, %rcx # %rcx -> column number (from 0 to %rdi-1)
.inner_loop_good:
    # compute memory access
    # (pointer computation could be optimized)
    # base + line number*stride + column number
    movq %rbx, %rax
    mulq %rdi
    addq %rbp, %rax
    addq %rcx, %rax
    movb (%rax), %al
    # %al -> target for the MOVs

    incq %rcx
    cmpq %rcx, %rdi
    ja .inner_loop_good

    incq %rbx
    cmpq %rbx, %rsi
    ja .outer_loop_good

.end_good:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    ret
.size good, .-good


.type bad, @function
.globl bad
bad:
    # %rdi -> line length
    # %rsi -> number of lines
    # %rdx -> buffer

    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    movq %rdx, %rbp # %rdx is going to be overwritten by MUL

    xorq %rcx, %rcx # %rcx -> column number (from 0 to %rdi-1)
.outer_loop_bad:
    xorq %rbx, %rbx # %rbx -> line number (from 0 to %rsi-1)
.inner_loop_bad:
    # compute memory access
    # (pointer computation could be optimized)
    # base + line number*stride + column number
    movq %rbx, %rax
    mulq %rdi
    addq %rbp, %rax
    addq %rcx, %rax
    movb (%rax), %al
    # %al -> target for the MOVs

    incq %rbx
    cmpq %rbx, %rsi
    ja .inner_loop_bad

    incq %rcx
    cmpq %rcx, %rdi
    ja .outer_loop_bad

.end_bad:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    ret
.size bad, .-bad


.section .note.GNU-stack, "", @progbits
.section .note.gnu.property, "a"
