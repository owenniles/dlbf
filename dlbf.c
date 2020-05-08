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

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "dlbf.h"

#define META (3 * sizeof (unsigned))

/* Macros that can be used to extract a particular filter's metadata. */
#define __M(filt) (((unsigned *) filt)[0])
#define __K(filt) (((unsigned *) filt)[1])
#define __R(filt) (((unsigned *) filt)[2])

#define getbit(filt,i) \
  (((filt)[(i) / 8 + META] & (1 << (i) % 8)) == 1 << (i) % 8)

/* Monotonically set the specified bit to the specified value by increasing its
   value if possible. */
#define incbit(filt,i,b) ((filt)[(i) / 8 + META] |= (b) << (i) % 8)

/* Monotonically set the specified bit to the specified value by decreasing its
   value if possible. */
#define decbit(filt,i,b) ((filt)[(i) / 8 + META] &= ~((!b) << (i) % 8))

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

/* Allocates a new deletable Bloom filter that is m bits long, applies k
   discrete hash functions to each key, and tracks collision-freeness for r
   regions. */
uint8_t *
dlbf_alloc (unsigned m, unsigned k, unsigned r)
{
  unsigned *filt;

  if (m < 1 || k < 1 || r < 1 || r > m)
    return NULL;

  filt = calloc (META + (m + r - 1) / 8 + 1, 1);

  if (filt == NULL)
    return NULL;

  filt[0] = m;
  filt[1] = k;
  filt[2] = r;

  return (uint8_t *) filt;
}

void
dlbf_free (uint8_t *filt)
{
  free (filt);
}

/* Inserts the specified key into the Bloom filter. Returns the number of hash
   functions that will output a bit offset that falls within a non collision-
   free zone when applied to the specified key following its insertion. */
int
dlbf_insert (uint8_t *filt, int x)
{
  if (filt == NULL)
    return -1;

  unsigned m = __M (filt);
  unsigned k = __K (filt);
  unsigned r = __R (filt);
  int collisions = 0;
  
  /* The bits that are used to represent this particular key. */
  uint32_t offsets[k];

  for (int i = 0; i < (int) k; ++i)
    {
      uint8_t collision;
      uint32_t h = hash (i, x) % m;

      offsets[i] = h + r;
      collision = getbit (filt, offsets[i]) | getbit (filt, h * r / m);
      incbit (filt, h * r / m, collision);
      collisions += collision;
    }

  /* We have to do this in two steps because if there exists any pair i, j such
     that H_i (x) == H_j (x), we will detect a false collision. Therefore, in
     the most naive implementation, we calculate collisions before inserting
     the key. */
  for (int i = 0; i < (int) k; ++i)
    incbit (filt, offsets[i], 1);
  
  return collisions;
}

/* Queries the Bloom filter for the specified key. Returns 1 if the the key is
   found and 0 if not. */
int
dlbf_query (uint8_t *filt, int x)
{
  if (filt == NULL)
    return -1;
  
  for (int i = 0; i < (int) __K (filt); ++i)
    {
      if (getbit (filt, hash (i, x) % __M (filt) + __R (filt)) == 0)
	return 0;
    }

  return 1;
}

/* Removes the specified key from the Bloom filter. Returns the number of hash
   functions that output a bit offset that lies within a non collision free
   zone when applied to the specified key. */
int
dlbf_remove (uint8_t *filt, int x)
{
  if (filt == NULL || dlbf_query (filt, x) == 0)
    return -1;

  unsigned m = __M (filt);
  unsigned k = __K (filt);
  unsigned r = __R (filt);
  int collisions = 0;

  for (int i = 0; i < (int) k; ++i)
    {
      uint32_t h = hash (i, x) % m;
      uint8_t collision = getbit (filt, h * r / m);

      collisions += collision;
      decbit (filt, h + r, collision);
    }

  assert (collisions <= (int) k);

  return collisions;
}
