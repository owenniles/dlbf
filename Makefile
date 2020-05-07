#
#  Makefile
#
#  A collection of scripts for building and evaluating this implementation of a
#  deletable Bloom filter.
#
#  Copyright 2020 Owen Niles <oniles@college.harvard.edu>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

SHELL = /bin/sh
CC = gcc

#
#  Optional DEFS:
#  K		The number of hash functions to apply to each key. Defaults to
#		8.
#  M		The length of the Bloom filter in bits. Defaults to 1024.
#  R		The number of regions of the Bloom filter for which we track
#  		collision-freeness. Defaults to 16.
#

ifndef NDEBUG
CFLAGS += -g3
endif

CFLAGS += -Wall -Wextra
CPPFLAGS += $(DEFS)

TESTS = $(basename $(wildcard tests/test*.c))
TESTOBJS = $(addsuffix .o,$(TESTS))
CHECKS = $(TESTS:tests/test%=check-%)
P = %

.PHONY: all check clean $(CHECKS)

all $(TESTS): dlbf.o

dlbf.o $(TESTOBJS): dlbf.h

$(TESTOBJS): CPPFLAGS += -iquote . -DTEST

check: $(CHECKS)

clean:
	rm -rf *.o $(TESTS) $(TESTOBJS)

.SECONDEXPANSION:
$(CHECKS): $$(patsubst check-$$P,tests/test$$P,$$@)
	./$<
