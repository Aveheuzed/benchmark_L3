bin/benchmark: src/asm.s src/ref.s src/asmdriver.c
	mkdir -p bin
	clang -O2 -o $@ $^

clean:
	rm -f bin/benchmark
