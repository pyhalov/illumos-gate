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
# Copyright 2014 Alexander Pyhalov
# Use is subject to license terms.
#
# lib/openssl/Makefile
#

include	$(SRC)/lib/Makefile.lib

SUBDIRS = libsunw_crypto .WAIT libsunw_ssl

# conditional assignments
all :=      	TARGET= all
install :=      TARGET= install
clean :=        TARGET= clean
clobber :=      TARGET= clobber
lint :=         TARGET= lint
_msg :=         TARGET= _msg

.KEEP_STATE:

HDRS=    aes.h \
         asn1.h \
         asn1_mac.h \
         asn1t.h \
         bio.h \
         blowfish.h \
         bn.h \
         buffer.h \
         camellia.h \
         cast.h \
         cmac.h \
         cms.h \
         comp.h \
         conf.h \
         conf_api.h \
         crypto.h \
         des.h \
         des_old.h \
         dh.h \
         dsa.h \
         dso.h \
         dtls1.h \
         e_os2.h \
         ebcdic.h \
         engine.h \
         err.h \
         evp.h \
         hmac.h \
         krb5_asn.h \
         kssl.h \
         lhash.h \
         md2.h \
         md4.h \
         md5.h \
         modes.h \
         obj_mac.h \
         objects.h \
         ocsp.h \
         opensslconf.h \
         opensslv.h \
         ossl_typ.h \
         pem.h \
         pem2.h \
         pkcs12.h \
         pkcs7.h \
         pqueue.h \
         rand.h \
         rc2.h \
         rc4.h \
         ripemd.h \
         rsa.h \
         safestack.h \
         sha.h \
         srp.h \
         srtp.h \
         ssl.h \
         ssl2.h \
         ssl23.h \
         ssl3.h \
         stack.h \
         sunw_prefix.h \
         symhacks.h \
         tls1.h \
         ts.h \
         txt_db.h \
         ui.h \
         ui_compat.h \
         x509.h \
         x509_vfy.h \
         x509v3.h

HDRDIR=		include/openssl
ROOTHDRDIR=     $(ROOT)/usr/include/openssl
ROOTHDRS=       $(HDRS:%=$(ROOTHDRDIR)/%)


all install clean clobber lint _msg: $(SUBDIRS)

$(ROOTHDRS):	$(ROOTHDRDIR)

$(ROOTHDRDIR):
	$(INS.dir)

$(ROOTHDRDIR)/%: $(HDRDIR)/%
	$(INS.file)

install install_h:	$(ROOTHDRS)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

check:
	@cd libsunw_ssl; pwd; $(MAKE) $@
	@cd libsunw_crypto; pwd; $(MAKE) $@

FRC:
