#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NBRUNS 1000

void good(size_t stride, size_t len, char const* buffer);
void bad(size_t stride, size_t len, char const* buffer);

int main(int argc, char* argv[]) {
  if (argc != 3) exit(1);
  const size_t stride = atoi(argv[1]);
  const size_t len = atoi(argv[2]);
  char* buffer = malloc(stride*len);

  clock_t totgood = 0;
  clock_t totbad = 0;

  for (int i=0; i<NBRUNS; i++) {
    totbad -= clock();
    bad(stride, len, buffer);
    totbad += clock();
  }
  for (int i=0; i<NBRUNS; i++) {
    totgood -= clock();
    good(stride, len, buffer);
    totgood += clock();
  }

  free(buffer);

  totgood *= 1000000000/CLOCKS_PER_SEC;
  totbad *= 1000000000/CLOCKS_PER_SEC;

  float avggood = totgood/NBRUNS;
  float avgbad = totbad/NBRUNS;
  printf("avg mem access time good:\t%.3f ns\n", avggood/(stride*len));
  printf("avg mem access time bad:\t%.3f ns\n", avgbad/(stride*len));
}
