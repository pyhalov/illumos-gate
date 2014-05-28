#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

# aes/*.s
ASM_SOURCES = 	x86cpuid.s \
		aes-586.s \
		aesni-x86.s \
		vpaes-x86.s \
		bf-586.s \
		bn-586.s \
		co-586.s \
		x86-gf2m.s \
		x86-mont.s \
		cmll-x86.s \
		crypt586.s \
		des-586.s \
		md5-586.s \
		ghash-x86.s \
		rc4-586.s \
		rmd-586.s \
		sha1-586.s \
		sha256-586.s \
		sha512-586.s

OBJECTS +=	$(ASM_SOURCES:%.s=%.o)

CLEANFILES +=	$(ASM_SOURCES)

include ../Makefile.com

CPPFLAGS += -DL_ENDIAN
CPPFLAGS += -DOPENSSL_NO_INLINE_ASM
CPPFLAGS += -DOPENSSL_BN_ASM_PART_WORDS
CPPFLAGS += -DOPENSSL_IA32_SSE2
CPPFLAGS += -DRMD160_ASM
CPPFLAGS += -DAES_ASM
CPPFLAGS += -DPK11_LIB_LOCATION=\"/usr/lib/libpkcs11.so.1\"

PERL_CPPFLAGS += -DOPENSSL_IA32_SSE2
PERL_CPPFLAGS += -fPIC

# OpenSSL interface is a mess
MAPFILES =

.KEEP_STATE:

all:  $(ROOTLIBDIR) $(LIBS) $(LIBLINKS)

$(LIBLINKS):    FRC
	$(RM) $@; $(SYMLINK) $(DYNLIB) $@

$(ROOTLIBDIR):
	$(INS.dir)

install: all $(ROOTLIBS) $(ROOTLINKS)

FRC:
