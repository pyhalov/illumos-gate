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

HDRS.cmd=	ls include/openssl/
HDRS=		$(HDRS.cmd:sh)

HDRDIR=		include/openssl
ROOTHDRDIR=     $(ROOT)/usr/include/openssl
ROOTHDRS=       $(HDRS:%=$(ROOTHDRDIR)/%)
CHECKHDRS=      $(HDRS:%.h=%.check)


all install clean clobber lint: $(SUBDIRS)

$(ROOTHDRS):	$(ROOTHDRDIR)

$(ROOTHDRDIR):
	$(INS.dir)

$(ROOTHDRDIR)/%: $(HDRDIR)/%
	$(INS.file)

install install_h:	$(ROOTHDRS)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

check: $(CHECKHDRS)
	@cd libsunw_ssl; pwd; $(MAKE) $@
	@cd libsunw_crypto; pwd; $(MAKE) $@

FRC:
