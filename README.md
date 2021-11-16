## Building:

  `make`.

  (You can change the compiler / compiler settings in the `makefile`, although
  it probably won't change anything, since the computation core is written in assembly.)

## Usage:

  `bin/benchmark` will try to find the L3 cache size using an enhanced dichotomic algorithm.

  The "good" core runs over the table in order, i.e. runs over the first line,
  then the second, then the third…

  The "bad" core runs over the table out-of order: it fetches all the first indices,
  then all the second indices, then the third indices…
  (It still processes the lines in increasing order on each loop.)

  The two cores use the same instructions, in the same order, the same amount of times.
  The only difference is the names of the registers they operate on.
