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

LIBRARY=	libsunw_ssl.a
VERS=		.1

OBJECTS=	bio_ssl.o  \
		d1_both.o  \
		d1_clnt.o  \
		d1_enc.o   \
		d1_lib.o   \
		d1_meth.o  \
		d1_pkt.o   \
		d1_srtp.o  \
		d1_srvr.o  \
		kssl.o     \
		s23_clnt.o \
		s23_lib.o  \
		s23_meth.o \
		s23_pkt.o  \
		s23_srvr.o \
		s2_clnt.o  \
		s2_enc.o   \
		s2_lib.o   \
		s2_meth.o  \
		s2_pkt.o   \
		s2_srvr.o  \
		s3_both.o  \
		s3_cbc.o   \
		s3_clnt.o  \
		s3_enc.o   \
		s3_lib.o   \
		s3_meth.o  \
		s3_pkt.o   \
		s3_srvr.o  \
		ssl_algs.o \
		ssl_asn1.o \
		ssl_cert.o \
		ssl_ciph.o \
		ssl_err.o  \
		ssl_err2.o \
		ssl_lib.o  \
		ssl_rsa.o  \
		ssl_sess.o \
		ssl_stat.o \
		ssl_txt.o  \
		t1_clnt.o  \
		t1_enc.o   \
		t1_lib.o   \
		t1_meth.o  \
		t1_reneg.o \
		t1_srvr.o  \
		tls_srp.o

# include library definitions
include $(SRC)/lib/Makefile.lib

CLOBBERFILES += $(LIBLINKS)

LIBS =		$(DYNLIB)

LDLIBS += -lc -lsunw_crypto

LINTFLAGS =	-uxn
LINTFLAGS64 =	$(LINTFLAGS) -m64
LINTOUT=	lint.out
LINTSRC =       $(LINTLIB:%.ln=%)
ROOTLINTDIR =   $(ROOTLIBDIR)
ROOTLINT =      $(LINTSRC:%=$(ROOTLINTDIR)/%)

CPPFLAGS +=	-I.. \
		-I$(SRC)/lib/openssl/include

CPPFLAGS += 	-D_REENTRANT
CPPFLAGS += 	-DOPENSSL_THREADS
CPPFLAGS += 	-DDSO_DLFCN
CPPFLAGS += 	-DHAVE_DLFCN_H
CPPFLAGS += 	-DSOLARIS_OPENSSL
CPPFLAGS += 	-DNO_WINDOWS_BRAINDEATH
CPPFLAGS += 	-DOPENSSL_BN_ASM_GF2m
CPPFLAGS += 	-DSHA1_ASM
CPPFLAGS += 	-DSHA256_ASM
CPPFLAGS += 	-DSHA512_ASM
CPPFLAGS += 	-DMD5_ASM
CPPFLAGS += 	-DAES_ASM
CPPFLAGS += 	-DVPAES_ASM
CPPFLAGS += 	-DGHASH_ASM
CPPFLAGS += 	-DVPAES_ASM
CPPFLAGS +=	-DOPENSSL_BN_ASM_MONT

CFLAGS +=	$(CCVERBOSE)

CERRWARN +=	-erroff=E_END_OF_LOOP_CODE_NOT_REACHED
CERRWARN +=	-erroff=E_CONST_PROMOTED_UNSIGNED_LONG
CERRWARN +=	-erroff=E_INIT_DOES_NOT_FIT

$(LINTLIB) :=	LINTFLAGS = -nvx -I..
$(LINTLIB) :=	LINTFLAGS64 = -nvx -m64 -I..

.KEEP_STATE:

all : $(LIBS)

lint :	lintcheck

# include library targets
include $(SRC)/lib/Makefile.targ

pics/%.o:       ../%.c
	$(COMPILE.c) -o $@ $<

$(ROOTLINTDIR)/%: ../%
	$(INS.file)
