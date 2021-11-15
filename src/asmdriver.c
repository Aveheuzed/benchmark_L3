#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NBRUNS 1000

// this is a function, not a function pointer
// this works fine, unlike function pointers witch caused artifacts
typedef void (TimedF)(size_t stride, size_t len, char const* buffer);

TimedF ref_good, ref_bad, good, bad;

static inline clock_t timeit(TimedF kern, size_t stride, size_t len, char const* buffer) {
  clock_t total = 0;

  total -= clock();
  for (int i=0; i<NBRUNS; i++){
    kern(stride, len, buffer);
  }
  total += clock();
  return total/NBRUNS;
}

int main(int argc, char* argv[]) {
  if (argc != 3) exit(1);
  const size_t stride = atoi(argv[1]);
  const size_t len = atoi(argv[2]);
  const size_t bufsize = stride*len;
  char* buffer = malloc(bufsize);

  float avg_memtime_good =
    +timeit(good, stride, len, buffer)
    -timeit(ref_good, stride, len, buffer)
  ;

  float avg_memtime_bad =
    +timeit(bad, stride, len, buffer)
    -timeit(ref_bad, stride, len, buffer)
  ;

  avg_memtime_good *= 1000000000/CLOCKS_PER_SEC; // convert to ns
  avg_memtime_bad *= 1000000000/CLOCKS_PER_SEC; // convert to ns

  printf("avg mem access time good:\t%.3f ns\n", avg_memtime_good/bufsize);
  printf("avg mem access time bad:\t%.3f ns\n", avg_memtime_bad/bufsize);
}
