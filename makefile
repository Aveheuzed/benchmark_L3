bin/benchmark: src/asm.s src/ref.s src/asmdriver.c
	clang -O2 -o $@ $^
