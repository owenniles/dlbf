# Deletable Bloom Filter

This is an implementation of the deletable Bloom filter data structure described in [this][0] paper by Rothenberg et al.

Compile an object file by running

    make

Test a particular configuration of the data structure by running a command such as

    make DEFS="-DM=4096 -DR=128 -DK=16"

where M is the length of the Bloom filter in bits, K is the number of discrete hash functions that are applied to each key, and R is the number of regions in which collision-freeness is tracked.

[0]: https://dl.acm.org/doi/10.1109/LCOMM.2010.06.100344