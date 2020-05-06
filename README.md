# Deletable Bloom Filter

This is an implementation of the deletable Bloom filter data structure described in [this][0] paper by Rothenberg et al.

Compile an object file by running

    make

Optionally, tune the data structure by specifying `K` (the number discrete hash functions that are applied to each key), `M` (the length of the Bloom filter in bits) and `R` (the number of regions within which to track collisions) like so:

    make DEFS="-DM=4096 -DR=128 -DK=16"

Test the configuration by running

    make check

[0](https://dl.acm.org/doi/10.1109/LCOMM.2010.06.100344)