/* dlbf.h

   DlBF interface and tunable declarations.

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

#ifndef _DLBF_H_
#define _DLBF_H_

#ifdef TEST
#include <stdint.h>

extern uint8_t dlbf[];
#endif

/* The number of different hash functions. */
#ifndef K
#define K 8
#endif

/* The number of bits in the Bloom filter. */
#ifndef M
#define M 1024
#endif

/* The number of regions for which we track collision-freeness. */
#ifndef R
#define R 16
#endif

int insert (int);
int query (int);
int remove (int);

#endif /* not _DLBF_H_ */
