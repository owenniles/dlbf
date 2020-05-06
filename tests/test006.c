/* test006.c

   Make sure that the remove function actually removes an existing key from the
   Bloom filter.

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
#include <stdlib.h>

#include "dlbf.h"

int
main (void) {
  int r = rand ();
  
  insert (r);
  remove (r);
  assert (query (r) == 0);
}