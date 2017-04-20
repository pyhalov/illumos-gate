#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
#

include $(SRC)/Makefile.master


# let the .so compilation be driven by present recoding tables
TABLES:sh =	cd ../tbls/ && ls *tbl

ALL_SOS:sh =    (cd ../tbls/ && ls *tbl |sed -e s:_:%:g -e 's:\.tbl$:.so:g')

CFLAGS		+= $(INCLUDES) $(XREGSFLAG) -KPIC -G -dy -D_REENTRANT

LDFLAGS=       -G $(ZTEXT) $(ZDEFS) $(BDIRECT) \
		$(MAPFILES:%=-M%) $(MAPFILE.PGA:%=-M%) $(MAPFILE.NED:%=-M%)
LDLIBS		= -lc

CLEANFILES =	*.o *.so core ../common/tbl.h


ICONV_DIR		= $(ROOT)/usr/lib/iconv

.NO_PARALLEL:

.PARALLEL: $(ALL_SOS)


all: $(ALL_SOS)

include $(SRC)/data/iconv/Makefile.asian

$(ALL_SOS): ../common/euro.h ../common/euro.c ../common/tbl.h
	TABLE=`echo $@ | $(TR) -d "-" | sed -e s:%:_:g | /usr/bin/cut -d. -f1` ; \
	$(CC) $(CFLAGS) -DT_$$TABLE ../common/euro.c -c -o $@.o ; \
	$(LD) $(LDFLAGS) -o $@ $@.o $(LDLIBS)
	$(POST_PROCESS_SO)

../common/tbl.h: ../genincl $(TABLES:%=../tbls/%)
	cd ..; ./genincl > common/tbl.h

FRC:

