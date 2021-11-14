bin/benchmark: src/asm.s src/asmdriver.c
	clang -O2 -o $@ $^
