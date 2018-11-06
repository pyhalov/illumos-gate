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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

LDFLAGS	=	$(DYNFLAGS) $(LDLIBS)

COMMON = ../common/

ICONV_COMMON  = ../../common/

SRCS		=	UTF-8%zh_CN.iso2022-7.c \
			UTF-8%zh_CN.euc.c \
			UTF-8%zh_CN.gbk.c \
			UTF-8%zh_CN.iso2022-CN.c \
			zh_CN.euc%UTF-8.c \
			zh_CN.euc%zh_CN.iso2022-CN.c \
			zh_CN.euc%zh_TW-big5.c \
			zh_CN.gbk%UTF-8.c \
			zh_CN.iso2022-7%UTF-8.c \
			zh_CN.iso2022-CN%zh_CN.euc.c \
			zh_CN.iso2022-CN%UTF-8.c \
			HZ-GB-2312%UTF-8.c \
			HZ-GB-2312%zh_CN.euc.c \
			UTF-8%HZ-GB-2312.c \
			zh_CN.euc%HZ-GB-2312.c \
			zh_CN.gbk%HZ-GB-2312.c \
			zh_TW-big5%zh_CN.euc.c \
			UTF-8%zh_CN-cp935.c \
			zh_CN-cp935%UTF-8.c \
			zh_CN.gbk%zh_CN.iso2022-CN.c \
			zh_CN.gbk%zh_TW-big5.c \
			zh_CN.gbk%zh_TW-big5p.c \
			zh_CN.iso2022-CN%zh_CN.gbk.c \
			zh_TW-big5%zh_CN.gbk.c \
			zh_TW-big5p%zh_CN.gbk.c \
			zh_HK.hkscs%zh_CN.gbk.c \
			zh_CN.gbk%zh_HK.hkscs.c \
			zh_TW-euc%UTF-8.c \
			UTF-8%zh_TW-euc.c \
			zh_TW-iso2022-7%UTF-8.c \
			UTF-8%zh_TW-iso2022-7.c \
			zh_TW-big5%UTF-8.c \
			zh_HK.hkscs%UTF-8.c \
			UTF-8%zh_HK.hkscs.c \
			zh_HK.hkscs%zh_TW-big5p.c \
			zh_TW-big5p%zh_HK.hkscs.c \
			UTF-8%zh_TW-big5.c \
			zh_TW-big5p%UTF-8.c \
			UTF-8%zh_TW-big5p.c \
			UTF-8%zh_TW-cp937.c zh_TW-cp937%UTF-8.c \
			zh_TW-euc%zh_TW-big5.c \
			zh_TW-euc%zh_TW-iso2022-7.c \
			zh_TW-euc%zh_TW-iso2022-CN-EXT.c \
			zh_TW-big5%zh_TW-euc.c \
			zh_TW-big5%zh_TW-iso2022-7.c \
			zh_TW-iso2022-7%zh_TW-euc.c \
			zh_TW-iso2022-CN-EXT%zh_TW-euc.c \
			zh_TW-iso2022-7%zh_TW-big5.c \
			zh_TW-big5%zh_TW-iso2022-CN-EXT.c \
			zh_TW-iso2022-CN-EXT%zh_TW-big5.c

dummy: all

gb2312%UCS-2LE.o: $(COMMON)zh_CN.euc%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

GBK%UCS-2LE.o: $(COMMON)zh_CN.gbk%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

GBK%UCS-2BE.o: $(COMMON)zh_CN.gbk%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_2BE -c -o $@ $^
	$(POST_PROCESS_O)

GBK%UCS-4LE.o: $(COMMON)zh_CN.gbk%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_4LE -c -o $@ $^
	$(POST_PROCESS_O)

GBK%UCS-4BE.o: $(COMMON)zh_CN.gbk%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_4BE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-2LE%gb2312.o: $(COMMON)UTF-8%zh_CN.euc.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-2LE%GBK.o: $(COMMON)UTF-8%zh_CN.gbk.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-2BE%GBK.o: $(COMMON)UTF-8%zh_CN.gbk.c
	$(CC) $(CFLAGS) -DUCS_2BE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-4LE%GBK.o: $(COMMON)UTF-8%zh_CN.gbk.c
	$(CC) $(CFLAGS) -DUCS_4LE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-4BE%GBK.o: $(COMMON)UTF-8%zh_CN.gbk.c
	$(CC) $(CFLAGS) -DUCS_4BE -c -o $@ $^
	$(POST_PROCESS_O)

cns11643%UCS-2LE.o: $(COMMON)zh_TW-euc%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

BIG5%UCS-2LE.o: $(COMMON)zh_TW-big5%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

Big5-HKSCS%UCS-2LE.o: $(COMMON)zh_HK.hkscs%UTF-8.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-2LE%cns11643.o: $(COMMON)UTF-8%zh_TW-euc.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-2LE%BIG5.o: $(COMMON)UTF-8%zh_TW-big5.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

UCS-2LE%Big5-HKSCS.o: $(COMMON)UTF-8%zh_HK.hkscs.c
	$(CC) $(CFLAGS) -DUCS_2LE -c -o $@ $^
	$(POST_PROCESS_O)

common_utf8.o: $(ICONV_COMMON)/common_utf8.c
	$(CC) $(CFLAGS) -c -o $@ $(ICONV_COMMON)/common_utf8.c
	$(POST_PROCESS_O)

UCS-2LE%cns11643.so: UCS-2LE%cns11643.o common_utf8.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UCS-2LE%cns11643.o common_utf8.o
	$(POST_PROCESS_O)

UCS-2LE%BIG5.so: UCS-2LE%BIG5.o common_utf8.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UCS-2LE%BIG5.o common_utf8.o
	$(POST_PROCESS_SO)

UCS-2LE%Big5-HKSCS.so: UCS-2LE%Big5-HKSCS.o common_utf8.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UCS-2LE%Big5-HKSCS.o common_utf8.o
	$(POST_PROCESS_SO)

UTF-8%zh_TW-euc.so: UTF-8%zh_TW-euc.o common_utf8.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UTF-8%zh_TW-euc.o common_utf8.o
	$(POST_PROCESS_SO)

UTF-8%zh_TW-big5.so: UTF-8%zh_TW-big5.o common_utf8.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UTF-8%zh_TW-big5.o common_utf8.o
	$(POST_PROCESS_SO)

UTF-8%zh_HK.hkscs.so: UTF-8%zh_HK.hkscs.o common_utf8.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UTF-8%zh_HK.hkscs.o common_utf8.o
	$(POST_PROCESS_SO)

zh_TW-iso2022-CN-EXT.o: $(COMMON)/zh_TW-iso2022-CN-EXT.c
	$(CC) $(CFLAGS) -c -o $@ $^
	$(POST_PROCESS_O)

zh_TW-iso2022-CN-EXT%zh_TW-big5.so: zh_TW-iso2022-CN-EXT%zh_TW-big5.o zh_TW-iso2022-CN-EXT.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ zh_TW-iso2022-CN-EXT%zh_TW-big5.o zh_TW-iso2022-CN-EXT.o
	$(POST_PROCESS_SO)

zh_TW-iso2022-CN-EXT%zh_TW-euc.so: zh_TW-iso2022-CN-EXT%zh_TW-euc.o zh_TW-iso2022-CN-EXT.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ zh_TW-iso2022-CN-EXT%zh_TW-euc.o zh_TW-iso2022-CN-EXT.o
	$(POST_PROCESS_SO)

utf8%ibm.o: $(ICONV_COMMON)/utf8%ibm.c
	$(CC) $(CFLAGS) -c -o $@ $(ICONV_COMMON)/utf8%ibm.c
	$(POST_PROCESS_O)

tab_lookup.o: $(ICONV_COMMON)/tab_lookup.c
	$(CC) $(CFLAGS) -c -o $@ $(ICONV_COMMON)/tab_lookup.c
	$(POST_PROCESS_O)

zh_TW-cp937%UTF-8.so: zh_TW-cp937%UTF-8.o utf8%ibm.o tab_lookup.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ zh_TW-cp937%UTF-8.o utf8%ibm.o tab_lookup.o
	$(POST_PROCESS_SO)

UTF-8%zh_CN-cp935.so: UTF-8%zh_CN-cp935.o utf8%ibm.o tab_lookup.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UTF-8%zh_CN-cp935.o utf8%ibm.o tab_lookup.o
	$(POST_PROCESS_SO)

zh_CN-cp935%UTF-8.so: zh_CN-cp935%UTF-8.o utf8%ibm.o tab_lookup.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ zh_CN-cp935%UTF-8.o utf8%ibm.o tab_lookup.o
	$(POST_PROCESS_SO)

UTF-8%zh_TW-cp937.so: UTF-8%zh_TW-cp937.o utf8%ibm.o tab_lookup.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ UTF-8%zh_TW-cp937.o utf8%ibm.o tab_lookup.o
	$(POST_PROCESS_SO)

include $(SRC)/data/iconv/Makefile.asian

ALL_SOS  = Big5-HKSCS%UCS-2LE.so          zh_CN.gbk%zh_CN.iso2022-CN.so
ALL_SOS += BIG5%UCS-2LE.so                zh_CN.gbk%zh_HK.hkscs.so
ALL_SOS += cns11643%UCS-2LE.so            zh_CN.gbk%zh_TW-big5p.so
ALL_SOS += gb2312%UCS-2LE.so              zh_CN.gbk%zh_TW-big5.so
ALL_SOS += GBK%UCS-2LE.so                 zh_CN.iso2022-7%UTF-8.so
ALL_SOS += HZ-GB-2312%UTF-8.so            zh_CN.iso2022-CN%UTF-8.so
ALL_SOS += HZ-GB-2312%zh_CN.euc.so        zh_CN.iso2022-CN%zh_CN.euc.so
ALL_SOS += UCS-2LE%Big5-HKSCS.so          zh_CN.iso2022-CN%zh_CN.gbk.so
ALL_SOS += UCS-2LE%BIG5.so                zh_HK.hkscs%UTF-8.so
ALL_SOS += UCS-2LE%cns11643.so            zh_HK.hkscs%zh_CN.gbk.so
ALL_SOS += UCS-2LE%gb2312.so              zh_HK.hkscs%zh_TW-big5p.so
ALL_SOS += UCS-2LE%GBK.so                 zh_TW-big5p%UTF-8.so
ALL_SOS += UTF-8%HZ-GB-2312.so            zh_TW-big5p%zh_CN.gbk.so
ALL_SOS += zh_TW-big5p%zh_HK.hkscs.so     zh_TW-big5%zh_TW-iso2022-7.so
ALL_SOS += UTF-8%zh_CN.euc.so             zh_TW-big5%UTF-8.so
ALL_SOS += UTF-8%zh_CN.gbk.so             zh_TW-big5%zh_CN.euc.so
ALL_SOS += UTF-8%zh_CN.iso2022-7.so       zh_TW-big5%zh_CN.gbk.so
ALL_SOS += UTF-8%zh_CN.iso2022-CN.so      zh_TW-big5%zh_TW-euc.so
ALL_SOS += UTF-8%zh_TW-big5p.so           zh_TW-big5%zh_TW-iso2022-CN-EXT.so
ALL_SOS += zh_TW-euc%UTF-8.so             zh_TW-euc%zh_TW-iso2022-7.so
ALL_SOS += UTF-8%zh_TW-iso2022-7.so       zh_TW-euc%zh_TW-big5.so
ALL_SOS += zh_CN.euc%HZ-GB-2312.so        zh_TW-euc%zh_TW-iso2022-CN-EXT.so
ALL_SOS += zh_CN.euc%UTF-8.so             zh_TW-iso2022-7%UTF-8.so
ALL_SOS += zh_CN.euc%zh_CN.iso2022-CN.so  zh_TW-iso2022-7%zh_TW-big5.so
ALL_SOS += zh_CN.euc%zh_TW-big5.so        zh_TW-iso2022-7%zh_TW-euc.so
ALL_SOS += zh_CN.gbk%HZ-GB-2312.so        zh_CN.gbk%UTF-8.so
ALL_SOS += zh_TW-cp937%UTF-8.so           UTF-8%zh_TW-cp937.so
ALL_SOS += zh_CN-cp935%UTF-8.so           UTF-8%zh_CN-cp935.so
ALL_SOS += gb2312%UCS-2LE.so              UCS-2LE%gb2312.so
ALL_SOS += cns11643%UCS-2LE.so BIG5%UCS-2LE.so Big5-HKSCS%UCS-2LE.so
ALL_SOS += UCS-2LE%cns11643.so UCS-2LE%BIG5.so UCS-2LE%Big5-HKSCS.so
ALL_SOS += UTF-8%zh_TW-euc.so UTF-8%zh_TW-big5.so UTF-8%zh_HK.hkscs.so
ALL_SOS += zh_TW-iso2022-CN-EXT%zh_TW-euc.so zh_TW-iso2022-CN-EXT%zh_TW-big5.so
ALL_SOS += GBK%UCS-2LE.so GBK%UCS-2BE.so GBK%UCS-4LE.so GBK%UCS-4BE.so
ALL_SOS += UCS-2LE%GBK.so UCS-2BE%GBK.so UCS-4LE%GBK.so UCS-4BE%GBK.so

install: all $(ICONV_LIBS)

all: $(ALL_SOS)
