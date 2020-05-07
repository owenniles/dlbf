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

#include <stdint.h>

#ifdef TEST
#define M 1024
#define K 8
#define R 16
#endif

uint8_t *dlbf_alloc (unsigned, unsigned, unsigned);
void dlbf_free (uint8_t *);
int dlbf_insert (uint8_t *, int);
int dlbf_query (uint8_t *, int);
int dlbf_remove (uint8_t *, int);

#endif /* not _DLBF_H_ */
