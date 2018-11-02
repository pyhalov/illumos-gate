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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

SRCS		=	tcvn%UCS-2.c \
            tcvn%UTF-8.c \
            tcvn%viscii.c \
            UCS-2%tcvn.c  \
            UCS-2%viscii.c  \
            UTF-8%tcvn.c  \
            UTF-8%viscii.c  \
            viscii%tcvn.c  \
            viscii%UCS-2.c  \
            viscii%UTF-8.c
COMMON = ../common/

dummy: all

tcvn%UCS-2LE.o: $(COMMON)tcvn%UCS-2.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^

tcvn%UCS-2BE.o: $(COMMON)tcvn%UCS-2.c
	$(CC) $(CFLAGS) -c -o $@ $^

viscii%UCS-2LE.o: $(COMMON)viscii%UCS-2.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^

viscii%UCS-2BE.o: $(COMMON)viscii%UCS-2.c
	$(CC) $(CFLAGS) -c -o $@ $^

UCS-2LE%tcvn.o: $(COMMON)UCS-2%tcvn.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^

UCS-2BE%tcvn.o: $(COMMON)UCS-2%tcvn.c
	$(CC) $(CFLAGS) -c -o $@ $^

UCS-2LE%viscii.o: $(COMMON)UCS-2%viscii.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^

UCS-2BE%viscii.o: $(COMMON)UCS-2%viscii.c
	$(CC) $(CFLAGS) -c -o $@ $^

include $(SRC)/data/iconv/Makefile.asian

ALL_SOS  = tcvn%UCS-2LE.so		tcvn%UCS-2BE.so
ALL_SOS += viscii%UCS-2LE.so		viscii%UCS-2BE.so
ALL_SOS += UCS-2LE%tcvn.so		UCS-2BE%tcvn.so
ALL_SOS += UCS-2LE%viscii.so		UCS-2BE%viscii.so
ALL_SOS += UTF-8%tcvn.so			tcvn%UTF-8.so
ALL_SOS += UTF-8%viscii.so		viscii%UTF-8.so
ALL_SOS += tcvn%viscii.so			viscii%tcvn.so


install: all $(ICONV_LIBS)

all: $(ALL_SOS)
