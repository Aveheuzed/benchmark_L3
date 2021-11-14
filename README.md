## Building:

  `make`.

  (You can change the compiler / compiler settings in the `makefile`, although
  it probably won't change anything, since the computation core is written in assembly.)

## Usage:

  `bin/benchmark {length of a line} {number of lines}`

  The "good" core runs over the table in order, i.e. runs over the first line,
  then the second, then the third…

  The "bad" core runs over the table out-of order: it fetches all the first indices,
  then all the second indices, then the third indices…
  (It still processes the lines in increasing order on each loop.)

  The two cores use the same instructions, in the same order, the same amount of times.
  The only difference is the names of the registers they operate on.


## Experiment plan:

  - fix `nblines` to st greater than the L3 entry size(e.g. 1024 B),

    increase `stride` until `tbad >> tgood`. `nblines*stride` at that point is the __L3 cache size__.

  - fix `(nblines*stride)` to st. greater than the L3 cache size (e.g. 16MiB),

    decrease `stride` (and adjust `nblines`) until `tbad≈tgood`. stride at that point is the __L3 entry size__.
