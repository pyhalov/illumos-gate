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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
#ident	"@(#)Makefile	1.9	07/01/11 SMI"
#
#

include $(SRC)/Makefile.master

include $(SRC)/data/iconv/Makefile.asian

include $(SRC)/data/iconv/ja/Makefile.iconv

LIB = $(MODULES:%.c=%.so)
LIB64 = $(MODULES:%.c=$(MACH64)/%.so)

DYNFLAGS=	-G $(ZTEXT) $(ZDEFS) $(BDIRECT) \
		$(MAPFILES:%=-M%) $(MAPFILE.PGA:%=-M%) $(MAPFILE.NED:%=-M%)
LDLIBS=	-lc

install		:=	TARGET = install
all		:=	TARGET = all
lint		:=	TARGET = lint
clean		:=	TARGET = clean
clobber		:=	TARGET = clobber

LINKS= \
	eucJP_TO_ISO-2022-JP.RFC1468.c \
	PCK_TO_ISO-2022-JP.RFC1468.c \
	UTF-8_TO_ISO-2022-JP.RFC1468.c \
	eucJP_TO_UTF-8-Java.c \
	UTF-8-Java_TO_eucJP.c \
	PCK_TO_UTF-8-Java.c \
	UTF-8-Java_TO_PCK.c \
	eucJP_TO_UTF-8.c \
	eucJP_TO_UTF-16.c \
	eucJP_TO_UTF-16BE.c \
	eucJP_TO_UTF-16LE.c \
	eucJP_TO_UCS-2.c \
	eucJP_TO_UCS-2BE.c \
	eucJP_TO_UCS-2LE.c \
	eucJP_TO_UTF-32.c \
	eucJP_TO_UTF-32BE.c \
	eucJP_TO_UTF-32LE.c \
	UTF-8_TO_eucJP.c \
	UTF-16_TO_eucJP.c \
	UTF-16BE_TO_eucJP.c \
	UTF-16LE_TO_eucJP.c \
	UCS-2_TO_eucJP.c \
	UCS-2BE_TO_eucJP.c \
	UCS-2LE_TO_eucJP.c \
	UTF-32_TO_eucJP.c \
	UTF-32BE_TO_eucJP.c \
	UTF-32LE_TO_eucJP.c \
	PCK_TO_UTF-8.c \
	PCK_TO_UTF-16.c \
	PCK_TO_UTF-16BE.c \
	PCK_TO_UTF-16LE.c \
	PCK_TO_UCS-2.c \
	PCK_TO_UCS-2BE.c \
	PCK_TO_UCS-2LE.c \
	PCK_TO_UTF-32.c \
	PCK_TO_UTF-32BE.c \
	PCK_TO_UTF-32LE.c \
	UTF-8_TO_PCK.c \
	UTF-16_TO_PCK.c \
	UTF-16BE_TO_PCK.c \
	UTF-16LE_TO_PCK.c \
	UCS-2_TO_PCK.c \
	UCS-2BE_TO_PCK.c \
	UCS-2LE_TO_PCK.c \
	UTF-32_TO_PCK.c \
	UTF-32BE_TO_PCK.c \
	UTF-32LE_TO_PCK.c \
	eucJP-ms_TO_UTF-8.c \
	eucJP-ms_TO_UTF-16.c \
	eucJP-ms_TO_UTF-16BE.c \
	eucJP-ms_TO_UTF-16LE.c \
	eucJP-ms_TO_UCS-2.c \
	eucJP-ms_TO_UCS-2BE.c \
	eucJP-ms_TO_UCS-2LE.c \
	eucJP-ms_TO_UTF-32.c \
	eucJP-ms_TO_UTF-32BE.c \
	eucJP-ms_TO_UTF-32LE.c \
	UTF-8_TO_eucJP-ms.c \
	UTF-16_TO_eucJP-ms.c \
	UTF-16BE_TO_eucJP-ms.c \
	UTF-16LE_TO_eucJP-ms.c \
	UCS-2_TO_eucJP-ms.c \
	UCS-2BE_TO_eucJP-ms.c \
	UCS-2LE_TO_eucJP-ms.c \
	UTF-32_TO_eucJP-ms.c \
	UTF-32BE_TO_eucJP-ms.c \
	UTF-32LE_TO_eucJP-ms.c \
	ms932_TO_UTF-8.c \
	ms932_TO_UTF-16.c \
	ms932_TO_UTF-16BE.c \
	ms932_TO_UTF-16LE.c \
	ms932_TO_UCS-2.c \
	ms932_TO_UCS-2BE.c \
	ms932_TO_UCS-2LE.c \
	ms932_TO_UTF-32.c \
	ms932_TO_UTF-32BE.c \
	ms932_TO_UTF-32LE.c \
	UTF-8_TO_ms932.c \
	UTF-16_TO_ms932.c \
	UTF-16BE_TO_ms932.c \
	UTF-16LE_TO_ms932.c \
	UCS-2_TO_ms932.c \
	UCS-2BE_TO_ms932.c \
	UCS-2LE_TO_ms932.c \
	UTF-32_TO_ms932.c \
	UTF-32BE_TO_ms932.c \
	UTF-32LE_TO_ms932.c \
	EUC-JIS-2004_TO_UTF-8.c \
	EUC-JIS-2004_TO_UTF-16.c \
	EUC-JIS-2004_TO_UTF-16BE.c \
	EUC-JIS-2004_TO_UTF-16LE.c \
	EUC-JIS-2004_TO_UCS-2.c \
	EUC-JIS-2004_TO_UCS-2BE.c \
	EUC-JIS-2004_TO_UCS-2LE.c \
	EUC-JIS-2004_TO_UTF-32.c \
	EUC-JIS-2004_TO_UTF-32BE.c \
	EUC-JIS-2004_TO_UTF-32LE.c \
	UTF-8_TO_EUC-JIS-2004.c \
	UTF-16_TO_EUC-JIS-2004.c \
	UTF-16BE_TO_EUC-JIS-2004.c \
	UTF-16LE_TO_EUC-JIS-2004.c \
	UCS-2_TO_EUC-JIS-2004.c \
	UCS-2BE_TO_EUC-JIS-2004.c \
	UCS-2LE_TO_EUC-JIS-2004.c \
	UTF-32_TO_EUC-JIS-2004.c \
	UTF-32BE_TO_EUC-JIS-2004.c \
	UTF-32LE_TO_EUC-JIS-2004.c \
	Shift_JIS-2004_TO_UTF-8.c \
	Shift_JIS-2004_TO_UTF-16.c \
	Shift_JIS-2004_TO_UTF-16BE.c \
	Shift_JIS-2004_TO_UTF-16LE.c \
	Shift_JIS-2004_TO_UCS-2.c \
	Shift_JIS-2004_TO_UCS-2BE.c \
	Shift_JIS-2004_TO_UCS-2LE.c \
	Shift_JIS-2004_TO_UTF-32.c \
	Shift_JIS-2004_TO_UTF-32BE.c \
	Shift_JIS-2004_TO_UTF-32LE.c \
	UTF-8_TO_Shift_JIS-2004.c \
	UTF-16_TO_Shift_JIS-2004.c \
	UTF-16BE_TO_Shift_JIS-2004.c \
	UTF-16LE_TO_Shift_JIS-2004.c \
	UCS-2_TO_Shift_JIS-2004.c \
	UCS-2BE_TO_Shift_JIS-2004.c \
	UCS-2LE_TO_Shift_JIS-2004.c \
	UTF-32_TO_Shift_JIS-2004.c \
	UTF-32BE_TO_Shift_JIS-2004.c \
	UTF-32LE_TO_Shift_JIS-2004.c \
	ISO-2022-JP-2004_TO_UTF-8.c \
	ISO-2022-JP-2004_TO_UTF-16.c \
	ISO-2022-JP-2004_TO_UTF-16BE.c \
	ISO-2022-JP-2004_TO_UTF-16LE.c \
	ISO-2022-JP-2004_TO_UCS-2.c \
	ISO-2022-JP-2004_TO_UCS-2BE.c \
	ISO-2022-JP-2004_TO_UCS-2LE.c \
	ISO-2022-JP-2004_TO_UTF-32.c \
	ISO-2022-JP-2004_TO_UTF-32BE.c \
	ISO-2022-JP-2004_TO_UTF-32LE.c \
	UTF-8_TO_ISO-2022-JP-2004.c \
	UTF-16_TO_ISO-2022-JP-2004.c \
	UTF-16BE_TO_ISO-2022-JP-2004.c \
	UTF-16LE_TO_ISO-2022-JP-2004.c \
	UCS-2_TO_ISO-2022-JP-2004.c \
	UCS-2BE_TO_ISO-2022-JP-2004.c \
	UCS-2LE_TO_ISO-2022-JP-2004.c \
	UTF-32_TO_ISO-2022-JP-2004.c \
	UTF-32BE_TO_ISO-2022-JP-2004.c \
	UTF-32LE_TO_ISO-2022-JP-2004.c

dummy: all

# symlink rules

# ISO-2022-JP.RFC1468 -> ISO-2022-JP
eucJP_TO_ISO-2022-JP.RFC1468.c:
	$(RM) $@; $(SYMLINK) ../common/eucJP_TO_ISO-2022-JP.c $@
PCK_TO_ISO-2022-JP.RFC1468.c:
	$(RM) $@; $(SYMLINK) ../common/PCK_TO_ISO-2022-JP.c $@
UTF-8_TO_ISO-2022-JP.RFC1468.c:
	$(RM) $@; $(SYMLINK) ../common/UTF-8_TO_ISO-2022-JP.c $@

# UTF-8-Java -> Unicode
eucJP_TO_UTF-8-Java.c: ../common/eucJP_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/eucJP_TO_Unicode.c $@
UTF-8-Java_TO_eucJP.c: ../common/Unicode_TO_eucJP.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_eucJP.c $@
PCK_TO_UTF-8-Java.c: ../common/PCK_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/PCK_TO_Unicode.c $@
UTF-8-Java_TO_PCK.c: ../common/Unicode_TO_PCK.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_PCK.c $@

eucJP_TO_UTF-8.c \
eucJP_TO_UTF-16.c eucJP_TO_UTF-16BE.c eucJP_TO_UTF-16LE.c \
eucJP_TO_UCS-2.c eucJP_TO_UCS-2BE.c eucJP_TO_UCS-2LE.c \
eucJP_TO_UTF-32.c eucJP_TO_UTF-32BE.c eucJP_TO_UTF-32LE.c \
eucJP-ms_TO_UTF-8.c \
eucJP-ms_TO_UTF-16.c eucJP-ms_TO_UTF-16BE.c eucJP-ms_TO_UTF-16LE.c \
eucJP-ms_TO_UCS-2.c eucJP-ms_TO_UCS-2BE.c eucJP-ms_TO_UCS-2LE.c \
eucJP-ms_TO_UTF-32.c eucJP-ms_TO_UTF-32BE.c eucJP-ms_TO_UTF-32LE.c: ../common/eucJP_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/eucJP_TO_Unicode.c $@

PCK_TO_UTF-8.c \
PCK_TO_UTF-16.c PCK_TO_UTF-16BE.c PCK_TO_UTF-16LE.c \
PCK_TO_UCS-2.c PCK_TO_UCS-2BE.c PCK_TO_UCS-2LE.c \
PCK_TO_UTF-32.c PCK_TO_UTF-32BE.c PCK_TO_UTF-32LE.c \
ms932_TO_UTF-8.c \
ms932_TO_UTF-16.c ms932_TO_UTF-16BE.c ms932_TO_UTF-16LE.c \
ms932_TO_UCS-2.c ms932_TO_UCS-2BE.c ms932_TO_UCS-2LE.c \
ms932_TO_UTF-32.c ms932_TO_UTF-32BE.c ms932_TO_UTF-32LE.c: ../common/PCK_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/PCK_TO_Unicode.c $@

UTF-8_TO_eucJP.c \
UTF-16_TO_eucJP.c UTF-16BE_TO_eucJP.c UTF-16LE_TO_eucJP.c \
UCS-2_TO_eucJP.c UCS-2BE_TO_eucJP.c UCS-2LE_TO_eucJP.c \
UTF-32_TO_eucJP.c UTF-32BE_TO_eucJP.c UTF-32LE_TO_eucJP.c \
UTF-8_TO_eucJP-ms.c \
UTF-16_TO_eucJP-ms.c UTF-16BE_TO_eucJP-ms.c UTF-16LE_TO_eucJP-ms.c \
UCS-2_TO_eucJP-ms.c UCS-2BE_TO_eucJP-ms.c UCS-2LE_TO_eucJP-ms.c \
UTF-32_TO_eucJP-ms.c UTF-32BE_TO_eucJP-ms.c UTF-32LE_TO_eucJP-ms.c: ../common/Unicode_TO_eucJP.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_eucJP.c $@

UTF-8_TO_PCK.c \
UTF-16_TO_PCK.c UTF-16BE_TO_PCK.c UTF-16LE_TO_PCK.c \
UCS-2_TO_PCK.c UCS-2BE_TO_PCK.c UCS-2LE_TO_PCK.c \
UTF-32_TO_PCK.c UTF-32BE_TO_PCK.c UTF-32LE_TO_PCK.c \
UTF-8_TO_ms932.c \
UTF-16_TO_ms932.c UTF-16BE_TO_ms932.c UTF-16LE_TO_ms932.c \
UCS-2_TO_ms932.c UCS-2BE_TO_ms932.c UCS-2LE_TO_ms932.c \
UTF-32_TO_ms932.c UTF-32BE_TO_ms932.c UTF-32LE_TO_ms932.c: ../common/Unicode_TO_PCK.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_PCK.c $@

EUC-JIS-2004_TO_UTF-8.c \
EUC-JIS-2004_TO_UTF-16.c EUC-JIS-2004_TO_UTF-16BE.c EUC-JIS-2004_TO_UTF-16LE.c \
EUC-JIS-2004_TO_UCS-2.c EUC-JIS-2004_TO_UCS-2BE.c EUC-JIS-2004_TO_UCS-2LE.c \
EUC-JIS-2004_TO_UTF-32.c EUC-JIS-2004_TO_UTF-32BE.c EUC-JIS-2004_TO_UTF-32LE.c: ../common/EUC-JIS-2004_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/EUC-JIS-2004_TO_Unicode.c $@

UTF-8_TO_EUC-JIS-2004.c \
UTF-16_TO_EUC-JIS-2004.c UTF-16BE_TO_EUC-JIS-2004.c UTF-16LE_TO_EUC-JIS-2004.c \
UCS-2_TO_EUC-JIS-2004.c UCS-2BE_TO_EUC-JIS-2004.c UCS-2LE_TO_EUC-JIS-2004.c \
UTF-32_TO_EUC-JIS-2004.c UTF-32BE_TO_EUC-JIS-2004.c UTF-32LE_TO_EUC-JIS-2004.c: ../common/Unicode_TO_EUC-JIS-2004.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_EUC-JIS-2004.c $@

Shift_JIS-2004_TO_UTF-8.c \
Shift_JIS-2004_TO_UTF-16.c \
Shift_JIS-2004_TO_UTF-16BE.c Shift_JIS-2004_TO_UTF-16LE.c \
Shift_JIS-2004_TO_UCS-2.c \
Shift_JIS-2004_TO_UCS-2BE.c Shift_JIS-2004_TO_UCS-2LE.c \
Shift_JIS-2004_TO_UTF-32.c \
Shift_JIS-2004_TO_UTF-32BE.c Shift_JIS-2004_TO_UTF-32LE.c: ../common/Shift_JIS-2004_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/Shift_JIS-2004_TO_Unicode.c $@

UTF-8_TO_Shift_JIS-2004.c \
UTF-16_TO_Shift_JIS-2004.c \
UTF-16BE_TO_Shift_JIS-2004.c UTF-16LE_TO_Shift_JIS-2004.c \
UCS-2_TO_Shift_JIS-2004.c \
UCS-2BE_TO_Shift_JIS-2004.c UCS-2LE_TO_Shift_JIS-2004.c \
UTF-32_TO_Shift_JIS-2004.c \
UTF-32BE_TO_Shift_JIS-2004.c UTF-32LE_TO_Shift_JIS-2004.c: ../common/Unicode_TO_Shift_JIS-2004.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_Shift_JIS-2004.c $@

ISO-2022-JP-2004_TO_UTF-8.c \
ISO-2022-JP-2004_TO_UTF-16.c \
ISO-2022-JP-2004_TO_UTF-16BE.c ISO-2022-JP-2004_TO_UTF-16LE.c \
ISO-2022-JP-2004_TO_UCS-2.c \
ISO-2022-JP-2004_TO_UCS-2BE.c ISO-2022-JP-2004_TO_UCS-2LE.c \
ISO-2022-JP-2004_TO_UTF-32.c \
ISO-2022-JP-2004_TO_UTF-32BE.c ISO-2022-JP-2004_TO_UTF-32LE.c: ../common/ISO-2022-JP-2004_TO_Unicode.c
	$(RM) $@; $(SYMLINK) ../common/ISO-2022-JP-2004_TO_Unicode.c $@

UTF-8_TO_ISO-2022-JP-2004.c \
UTF-16_TO_ISO-2022-JP-2004.c \
UTF-16BE_TO_ISO-2022-JP-2004.c UTF-16LE_TO_ISO-2022-JP-2004.c \
UCS-2_TO_ISO-2022-JP-2004.c \
UCS-2BE_TO_ISO-2022-JP-2004.c UCS-2LE_TO_ISO-2022-JP-2004.c \
UTF-32_TO_ISO-2022-JP-2004.c \
UTF-32BE_TO_ISO-2022-JP-2004.c UTF-32LE_TO_ISO-2022-JP-2004.c: ../common/Unicode_TO_ISO-2022-JP-2004.c
	$(RM) $@; $(SYMLINK) ../common/Unicode_TO_ISO-2022-JP-2004.c $@

#ALL_SOS.cmd= print $(DYNOBJS) | sed -e s:_TO_:%:
#ALL_SOS= $(ALL_SOS.cmd:sh)

all: $(LINKS) $(DYNOBJS)

CLEANFILES += $(LINKS)  $(DYNOBJS) $(ALL_SOS)

clobber: clean

install: $(ICONV_DIR) all
	for f in $(DYNOBJS); do \
		fp=`echo $$f | $(SED) -e 's/_TO_/%/'`; \
		echo "installing $$f as $(ICONV_DIR)/$$fp ..." ; \
		$(RM) $(ICONV_DIR)/$$fp ; \
		$(CP) $$f $(ICONV_DIR)/$$fp ; \
		$(CHMOD) 755 $(ICONV_DIR)/$$fp ; \
	done

lint:	$(LINTOUTS)

%.lint: %.c
	$(LINT) $(LINTFLAGS) $<
	$(LINT) $(LINTFLAGS64) $<

FRC:
