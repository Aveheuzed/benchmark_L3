#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIGNIFICANT_FACTOR 6
#define NBRUNS 100
#define MAX_SAME_CHANGES 3

#define FLOOR 512L
#define CEIL 4096L

void ref_good(size_t stride, size_t len, char const* buffer);
void ref_bad(size_t stride, size_t len, char const* buffer);
void good(size_t stride, size_t len, char const* buffer);
void bad(size_t stride, size_t len, char const* buffer);

static inline int significative_at(size_t param) {
  char* buffer = malloc(param*param);

  clock_t goodtime = 0;
  goodtime += clock();
  for (size_t i=0; i<200; i++) ref_good(param, param, buffer);
  goodtime -= clock();

  goodtime -= clock();
  for (size_t i=0; i<200; i++) good(param, param, buffer);
  goodtime += clock();


  clock_t badtime = 0;
  badtime += clock();
  for (size_t i=0; i<200; i++) ref_bad(param, param, buffer);
  badtime -= clock();

  badtime -= clock();
  for (size_t i=0; i<200; i++) bad(param, param, buffer);
  badtime += clock();


  free(buffer);
  return badtime > SIGNIFICANT_FACTOR * goodtime;
}

static inline size_t dichotomy(size_t low, size_t high) {
  // almost a plain dichotomy; we monitor the change tendency in addition.
  // when the tendency stays, we accelerate things up by pushing the other bound

  // this is implemented in order to become noise resistant!

  int changes_tendency = 0; // ++ when updating low, -- when updating high
  while (high-low > 2) {
    size_t mid = (high+low)/2;
    printf("Trying at %lu\n", mid);
    if (significative_at(mid)) {
      high = mid;
      if (changes_tendency<=0) changes_tendency--;
      else changes_tendency = -1;
    }
    else {
      low = mid;
      if (changes_tendency>=0) changes_tendency++;
      else changes_tendency = +1;
    }

    if (abs(changes_tendency) > MAX_SAME_CHANGES) {
      puts("Adjustment triggers!");
      if (changes_tendency < 0) {
        // push `low` away
        low -= (high-low)/2;
      }
      else {
        // push `high` away
        high += (high-low)/2;
      }
      changes_tendency = 0;
    }
  }
  return (low+high)/2;
}

int main(int argc, char* argv[]) {
  static const char* prefixes[] = {"B", "KiB", "MiB", "GiB", "ZiB", "YiB"};


  float approx = dichotomy(FLOOR, CEIL);
  approx *= approx;

  int i = 0;
  while (approx > 1024) {
    approx /= 1024;
    i++;
  }

  printf("Estimated L3 buffer size: %.1f %s\n", approx, prefixes[i]);
}
