#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NNOTICES 3
#define NSIGMA 5
#define PRESAMPLES 8
#define NBRUNS 8
#define TOOLONG(time) ((time) > CLOCKS_PER_SEC/10)

void ref_good(size_t stride, size_t len, char const* buffer);
void ref_bad(size_t stride, size_t len, char const* buffer);
void good(size_t stride, size_t len, char const* buffer);
void bad(size_t stride, size_t len, char const* buffer);

static clock_t timing_good(size_t stride, size_t len) {
  char* buffer = malloc(stride*len);
  clock_t goodtime = 0;

  ref_good(stride, len, buffer);
  goodtime += clock();
  for (size_t i=0; i<NBRUNS; i++) ref_good(stride, len, buffer);
  goodtime -= clock();

  good(stride, len, buffer);
  goodtime -= clock();
  for (size_t i=0; i<NBRUNS; i++) good(stride, len, buffer);
  goodtime += clock();

  free(buffer);

  return goodtime;
}

static clock_t timing_bad(size_t stride, size_t len) {
  char* buffer = malloc(stride*len);
  clock_t badtime = 0;

  ref_bad(stride, len, buffer);
  badtime += clock();
  for (size_t i=0; i<NBRUNS; i++) ref_bad(stride, len, buffer);
  badtime -= clock();

  bad(stride, len, buffer);
  badtime -= clock();
  for (size_t i=0; i<NBRUNS; i++) bad(stride, len, buffer);
  badtime += clock();

  free(buffer);

  return badtime;
}

static float indicator(float goodtime, float badtime) {
  return expf((badtime - goodtime) / (badtime + goodtime));
}

static void print_notice(float bufsize) {
  static const char* prefixes[] = {"B", "KiB", "MiB", "GiB", "ZiB", "YiB"};

  int i = 0;
  while (bufsize > 1024) {
    bufsize /= 1024;
    i++;
  }

  printf("Mark at %.1f %s\n", bufsize, prefixes[i]);
}

static void preheat(size_t inner, size_t outer, float *const avg, float *const stddev) {
  float sum = 0;
  float sumsq = 0;

  for (int n=0; n<PRESAMPLES; n++) {
    clock_t goodtime = timing_good(inner, outer);
    clock_t badtime = timing_bad(inner, outer);
    if (TOOLONG(goodtime) || TOOLONG(badtime)) {
      fprintf(stderr, "I think I've gone too far without finding anything. I'm sorry…\n");
      exit(1);
    }
    float new = indicator(goodtime, badtime);
    sum += new;
    sumsq += new*new;
  }

  sum /= PRESAMPLES; // average
  sumsq /= PRESAMPLES; // average of the squares
  *avg = sum;
  *stddev = 10*sqrtf((sumsq - sum*sum)/(PRESAMPLES-1));
}

static int swipe(size_t innerdim, size_t inner_inc, size_t outerdim) {
  fprintf(stderr, "Swipe with outer dim %lu\n", outerdim);

  float avg;
  float stddev;

  preheat(innerdim, outerdim, &avg, &stddev);

  fprintf(stderr, "Avg: %.3f // stddev: %.3f\n", avg, stddev);

  unsigned nnotices = 0;
  clock_t goodtime, badtime;
  do {
    innerdim += inner_inc;
    goodtime = timing_good(innerdim, outerdim);
    badtime = timing_bad(innerdim, outerdim);
    fprintf(stderr, "indicator for (%lu, %lu): %.3f ", innerdim, outerdim, indicator(goodtime, badtime));
    if (/*fabs*/(indicator(goodtime, badtime) - avg) > NSIGMA*stddev) {
      fprintf(stderr, "*\n");
      nnotices++;
    }
    else {
      fprintf(stderr, "\n");
      nnotices = 0;
    }
  } while (innerdim < outerdim*(2*2+1) && nnotices < NNOTICES);

  if (nnotices >= NNOTICES) {
    print_notice(innerdim*outerdim);
    return 0;
  }
  else {
    return 1;
  }
  // return nnotices < NNOTICES;
}

int main(int argc, char* argv[]) {
  for (size_t dim=512; swipe(dim, dim/4, dim); dim*=2);
}
