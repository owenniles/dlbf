/* dlbf.c

   Simple implementation of a deletable Bloom filter interface.

   Copyright 2020 Owen Niles <oniles@college.harvard.edu>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <stdint.h>

#include "dlbf.h"

#ifndef TEST
static
#endif
uint8_t dlbf[(M + R - 1) / 8 + 1];

#define getbit(i) ((dlbf[(i) / 8] & (1 << (i) % 8)) == 1 << (i) % 8)

/* Monotonically set the specified bit to the specified value by increasing its
   value if possible. */
#define incbit(i,b) (dlbf[(i) / 8] |= (b) << (i) % 8)

/* Monotonically set the specified bit to the specified value by decreasing its
   value if possible. */
#define decbit(i,b) (dlbf[(i) / 8] &= ~((!b) << (i) % 8))

/* Computes the the hash function H_i on input x. */
static uint32_t
hash (int i, int x)
{
  uint64_t concat = ((uint64_t) i << 32) | x;
  uint64_t product = concat * 2654435769;
  uint32_t lower = product;
  uint32_t upper = product >> 32;

  return lower + upper;
}

/* Inserts the specified key into the Bloom filter. Returns -1 if the key
   already exists and 0 if not. */
int
insert (int x)
{
  uint32_t cache[K];
  
  if (query (x) == 1)
    return -1;

  for (int i = 0; i < K; ++i)
    {
      cache[i] = R + hash (i, x) % M;

      incbit (cache[i] * R / M, getbit (cache[i]));
    }

  /* We have to do this in two steps because if there exists any pair i, j such
     that H_i (x) == H_j (x), we will detect a false collision. Therefore, in
     the most naive implementation, we calculate collisions before inserting
     the key. */
  for (int i = 0; i < K; ++i)
    incbit (cache[i], 1);
  
  return 0;
}

/* Queries the Bloom filter for the specified key. Returns 1 if the the key is
   found and 0 if not. */
int
query (int x)
{
  for (int i = 0; i < K; ++i)
    {
      uint32_t j = R + hash (i, x) % M;

      if (getbit (j) == 0)
	return 0;
    }

  return 1;
}

/* Removes the specified key from the Bloom filter. Returns 0 if the key
   actually may have existed and -1 if not. */
int
remove (int x)
{
  if (query (x) == 0)
    return -1;

  for (int i = 0; i < K; ++i)
    {
      uint32_t j = R + hash (i, x) % M;

      decbit (j, getbit (j * R / M));
    }

  return 0;
}
