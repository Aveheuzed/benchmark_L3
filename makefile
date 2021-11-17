bin/benchmark: src/asm.s src/ref.s src/asmdriver.c
	mkdir -p bin
	clang -O2 -o $@ $^ -lm

clean:
	rm -f bin/benchmark
