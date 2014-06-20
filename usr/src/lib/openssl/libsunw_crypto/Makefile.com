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

LIBRARY=	libsunw_crypto.a
VERS=		.1

COMMON_OBJECTS =	cpt_err.o  \
			cryptlib.o \
			cversion.o \
			ebcdic.o   \
			ex_data.o  \
			fips_ers.o \
			mem_dbg.o  \
			mem.o	   \
			o_dir.o    \
			o_fips.o   \
			o_init.o   \
			o_str.o    \
			o_time.o   \
			uid.o
OBJECTS +=	$(COMMON_OBJECTS)

# aes/*
AES_OBJECTS =	aes_cfb.o  \
		aes_ctr.o  \
		aes_ecb.o  \
		aes_ige.o  \
		aes_misc.o \
		aes_ofb.o  \
		aes_wrap.o
OBJECTS +=	$(AES_OBJECTS)

# asn1/*
ASN1_OBJECTS = a_bitstr.o  \
		a_bool.o    \
		a_bytes.o   \
		a_d2i_fp.o  \
		a_digest.o  \
		a_dup.o     \
		a_enum.o    \
		a_gentm.o   \
		a_i2d_fp.o  \
		a_int.o     \
		a_mbstr.o   \
		a_object.o  \
		a_octet.o   \
		a_print.o   \
		a_set.o     \
		a_sign.o    \
		a_strex.o   \
		a_strnid.o  \
		a_time.o    \
		a_type.o    \
		a_utctm.o   \
		a_utf8.o    \
		a_verify.o  \
		ameth_lib.o \
		asn1_err.o  \
		asn1_gen.o  \
		asn1_lib.o  \
		asn1_par.o  \
		asn_mime.o  \
		asn_moid.o  \
		asn_pack.o  \
		bio_asn1.o  \
		bio_ndef.o  \
		d2i_pr.o    \
		d2i_pu.o    \
		evp_asn1.o  \
		f_enum.o    \
		f_int.o     \
		f_string.o  \
		i2d_pr.o    \
		i2d_pu.o    \
		n_pkey.o    \
		nsseq.o     \
		p5_pbe.o    \
		p5_pbev2.o  \
		p8_pkey.o   \
		t_bitst.o   \
		t_crl.o     \
		t_pkey.o    \
		t_req.o     \
		t_spki.o    \
		t_x509.o    \
		t_x509a.o   \
		tasn_dec.o  \
		tasn_enc.o  \
		tasn_fre.o  \
		tasn_new.o  \
		tasn_prn.o  \
		tasn_typ.o  \
		tasn_utl.o  \
		x_algor.o   \
		x_attrib.o  \
		x_bignum.o  \
		x_crl.o     \
		x_exten.o   \
		x_info.o    \
		x_long.o    \
		x_name.o    \
		x_nx509.o   \
		x_pkey.o    \
		x_pubkey.o  \
		x_req.o     \
		x_sig.o     \
		x_spki.o    \
		x_val.o     \
		x_x509.o    \
		x_x509a.o
OBJECTS += $(ASN1_OBJECTS)

# bf/*
BF_OBJECTS =	bf_cfb64.o \
		bf_ecb.o   \
		bf_ofb64.o \
		bf_skey.o
OBJECTS += $(BF_OBJECTS)

# bio/*
BIO_OBJECTS =	b_dump.o	\
		b_print.o	\
		b_sock.o	\
		bf_buff.o	\
		bf_nbio.o	\
		bf_null.o	\
		bio_cb.o	\
		bio_err.o	\
		bio_lib.o	\
		bss_acpt.o	\
		bss_bio.o	\
		bss_conn.o	\
		bss_dgram.o	\
		bss_fd.o	\
		bss_file.o	\
		bss_log.o	\
		bss_mem.o	\
		bss_null.o	\
		bss_sock.o
OBJECTS += $(BIO_OBJECTS)

# bn/*
BN_OBJECTS =	bn_add.o	\
		bn_blind.o	\
		bn_const.o	\
		bn_ctx.o	\
		bn_depr.o	\
		bn_div.o	\
		bn_err.o	\
		bn_exp.o	\
		bn_exp2.o	\
		bn_gcd.o	\
		bn_gf2m.o	\
		bn_kron.o	\
		bn_lib.o	\
		bn_mod.o	\
		bn_mont.o	\
		bn_mpi.o	\
		bn_mul.o	\
		bn_nist.o	\
		bn_prime.o	\
		bn_print.o	\
		bn_rand.o	\
		bn_recp.o	\
		bn_shift.o	\
		bn_sqr.o	\
		bn_sqrt.o	\
		bn_word.o	\
		bn_x931p.o
OBJECTS += $(BN_OBJECTS)

# buffer/*
BUFFER_OBJECTS =	buf_err.o	\
			buf_str.o	\
			buffer.o
OBJECTS += $(BUFFER_OBJECTS)

# camellia/*
CAMELLIA_OBJECTS =	cmll_cfb.o	\
			cmll_ctr.o	\
			cmll_ecb.o	\
			cmll_ofb.o 	\
			cmll_utl.o
OBJECTS += $(CAMELLIA_OBJECTS)

# cast/*
CAST_OBJECTS =	c_cfb64.o	\
		c_ecb.o		\
		c_enc.o		\
		c_ofb64.o	\
		c_skey.o
OBJECTS += $(CAST_OBJECTS)

# cmac/*
CMAC_OBJECTS =	cm_ameth.o	\
		cm_pmeth.o	\
		cmac.o
OBJECTS += $(CMAC_OBJECTS)

# cms/*
CMS_OBJECTS =	cms_asn1.o	\
		cms_att.o	\
		cms_cd.o	\
		cms_dd.o	\
		cms_enc.o	\
		cms_env.o	\
		cms_err.o	\
		cms_ess.o	\
		cms_io.o	\
		cms_lib.o	\
		cms_pwri.o	\
		cms_sd.o	\
		cms_smime.o
OBJECTS += $(CMS_OBJECTS)

# comp/*
COMP_OBJECTS =	c_rle.o		\
		c_zlib.o	\
		comp_err.o	\
		comp_lib.o
OBJECTS += $(COMP_OBJECTS)

# conf/*
CONF_OBJECTS = 	conf_api.o	\
		conf_def.o	\
		conf_err.o	\
		conf_lib.o	\
		conf_mall.o	\
		conf_mod.o	\
		conf_sap.o
OBJECTS += $(CONF_OBJECTS)

# des/*
DES_OBJECTS =	cbc_cksm.o	\
		cbc_enc.o       \
		cfb_enc.o       \
		cfb64ede.o      \
		cfb64enc.o      \
		des_old.o       \
		des_old2.o      \
		ecb_enc.o       \
		ecb3_enc.o      \
		ede_cbcm_enc.o  \
		enc_read.o      \
		enc_writ.o      \
		fcrypt.o        \
		ofb_enc.o       \
		ofb64ede.o      \
		ofb64enc.o      \
		pcbc_enc.o      \
		qud_cksm.o      \
		rand_key.o      \
		read2pwd.o      \
		rpc_enc.o       \
		set_key.o       \
		str2key.o       \
		xcbc_enc.o
OBJECTS += $(DES_OBJECTS)

# dh/*
DH_OBJECTS =	dh_ameth.o	\
		dh_asn1.o	\
		dh_check.o	\
		dh_depr.o	\
		dh_err.o 	\
		dh_gen.o 	\
		dh_key.o 	\
		dh_lib.o 	\
		dh_pmeth.o	\
		dh_prn.o
OBJECTS += $(DH_OBJECTS)

# dsa/*
DSA_OBJECTS =	dsa_ameth.o	\
		dsa_asn1.o      \
		dsa_depr.o      \
		dsa_err.o       \
		dsa_gen.o       \
		dsa_key.o       \
		dsa_lib.o       \
		dsa_ossl.o      \
		dsa_pmeth.o     \
		dsa_prn.o       \
		dsa_sign.o      \
		dsa_vrf.o
OBJECTS += $(DSA_OBJECTS)

# dso/*
DSO_OBJECTS =	dso_beos.o  	\
		dso_dl.o    	\
		dso_dlfcn.o 	\
		dso_err.o   	\
		dso_lib.o   	\
		dso_null.o  	\
		dso_openssl.o	\
		dso_vms.o   	\
		dso_win32.o
OBJECTS += $(DSO_OBJECTS)

# engine/*
ENGINE_OBJECTS =	eng_all.o	\
			eng_cnf.o	\
			eng_cryptodev.o	\
			eng_ctrl.o	\
			eng_dyn.o	\
			eng_err.o	\
			eng_fat.o	\
			eng_init.o	\
			eng_lib.o	\
			eng_list.o	\
			eng_openssl.o	\
			eng_pkey.o	\
			eng_rdrand.o	\
			eng_rsax.o	\
			eng_table.o	\
			hw_pk11.o	\
			hw_pk11_pub.o	\
			tb_asnmth.o	\
			tb_cipher.o	\
			tb_dh.o	\
			tb_digest.o	\
			tb_dsa.o	\
			tb_ecdh.o	\
			tb_ecdsa.o	\
			tb_pkmeth.o	\
			tb_rand.o	\
			tb_rsa.o	\
			tb_store.o
OBJECTS += $(ENGINE_OBJECTS)

# err/*
ERR_OBJECTS =	err_all.o	\
		err_prn.o	\
		err.o
OBJECTS += $(ERR_OBJECTS)

# evp/*
EVP_OBJECTS =	bio_b64.o		\
		bio_enc.o   		\
		bio_md.o    		\
		bio_ok.o    		\
		c_all.o     		\
		c_allc.o    		\
		c_alld.o    		\
		digest.o    		\
		e_aes.o     		\
		e_aes_cbc_hmac_sha1.o	\
		e_bf.o      		\
		e_camellia.o		\
		e_cast.o    		\
		e_des.o     		\
		e_des3.o    		\
		e_idea.o    		\
		e_null.o    		\
		e_old.o     		\
		e_rc2.o     		\
		e_rc4.o     		\
		e_rc4_hmac_md5.o	\
		e_rc5.o     		\
		e_seed.o    		\
		e_xcbc_d.o  		\
		encode.o    		\
		evp_acnf.o  		\
		evp_cnf.o   		\
		evp_enc.o   		\
		evp_err.o   		\
		evp_fips.o  		\
		evp_key.o   		\
		evp_lib.o   		\
		evp_pbe.o   		\
		evp_pkey.o  		\
		m_dss.o     		\
		m_dss1.o    		\
		m_ecdsa.o   		\
		m_md2.o     		\
		m_md4.o     		\
		m_md5.o     		\
		m_mdc2.o    		\
		m_null.o    		\
		m_ripemd.o  		\
		m_sha.o     		\
		m_sha1.o    		\
		m_sigver.o  		\
		m_wp.o      		\
		names.o     		\
		p5_crpt.o   		\
		p5_crpt2.o  		\
		p_dec.o     		\
		p_enc.o     		\
		p_lib.o     		\
		p_open.o    		\
		p_seal.o    		\
		p_sign.o    		\
		p_verify.o  		\
		pmeth_fn.o  		\
		pmeth_gn.o  		\
		pmeth_lib.o
OBJECTS += $(EVP_OBJECTS)

# hmac/*
HMAC_OBJECTS =	hm_ameth.o	\
		hm_pmeth.o	\
		hmac.o
OBJECTS += $(HMAC_OBJECTS)

# krb5/*
KRB5_OBJECTS =	krb5_asn.o
OBJECTS += $(KRB5_OBJECTS)

# lhash/*
LHASH_OBJECTS =	lh_stats.o	\
		lhash.o
OBJECTS += $(LHASH_OBJECTS)

# md2/*
MD2_OBJECTS =	md2_dgst.o	\
		md2_one.o
OBJECTS += $(MD2_OBJECTS)

# md4/*
MD4_OBJECTS =	md4_dgst.o	\
		md4_one.o
OBJECTS += $(MD4_OBJECTS)

# md5/*
MD5_OBJECTS =	md5_dgst.o	\
		md5_one.o
OBJECTS += $(MD5_OBJECTS)

# modes/*
MODES_OBJECTS =	cbc128.o	\
		ccm128.o	\
		cfb128.o	\
		ctr128.o	\
		cts128.o	\
		gcm128.o	\
		ofb128.o	\
		xts128.o
OBJECTS += $(MODES_OBJECTS)

# objects/*
OBJECTS_OBJECTS =	o_names.o	\
			obj_dat.o	\
			obj_err.o	\
			obj_lib.o	\
			obj_xref.o
OBJECTS += $(OBJECTS_OBJECTS)

# ocsp/*
OCSP_OBJECTS = 	ocsp_asn.o	\
		ocsp_cl.o 	\
		ocsp_err.o	\
		ocsp_ext.o	\
		ocsp_ht.o 	\
		ocsp_lib.o	\
		ocsp_prn.o	\
		ocsp_srv.o	\
		ocsp_vfy.o
OBJECTS += $(OCSP_OBJECTS)

# pem/*
PEM_OBJECTS = 	pem_all.o	\
		pem_err.o	\
		pem_info.o	\
		pem_lib.o	\
		pem_oth.o	\
		pem_pk8.o	\
		pem_pkey.o	\
		pem_seal.o	\
		pem_sign.o	\
		pem_x509.o	\
		pem_xaux.o	\
		pvkfmt.o
OBJECTS += $(PEM_OBJECTS)

# pkcs12/*
PKCS12_OBJECTS =	p12_add.o	\
			p12_asn.o	\
			p12_attr.o	\
			p12_crpt.o	\
			p12_crt.o	\
			p12_decr.o	\
			p12_init.o	\
			p12_key.o	\
			p12_kiss.o	\
			p12_mutl.o	\
			p12_npas.o	\
			p12_p8d.o	\
			p12_p8e.o	\
			p12_utl.o	\
			pk12err.o
OBJECTS += $(PKCS12_OBJECTS)

# pkcs7/*
PKCS7_OBJECTS = bio_pk7.o	\
		pk7_asn1.o	\
		pk7_attr.o	\
		pk7_doit.o	\
		pk7_lib.o	\
		pk7_mime.o	\
		pk7_smime.o	\
		pkcs7err.o
OBJECTS += $(PKCS7_OBJECTS)

# pqueue/*
PQUEUE_OBJECTS = pqueue.o
OBJECTS += $(PQUEUE_OBJECTS)

# rand/*
RAND_OBJECTS =	md_rand.o	\
		rand_egd.o	\
		rand_err.o	\
		rand_lib.o	\
		rand_nw.o	\
		rand_os2.o	\
		rand_unix.o	\
		rand_win.o	\
		randfile.o
OBJECTS += $(RAND_OBJECTS)

# rc2/*
RC2_OBJECTS =	rc2_cbc.o	\
		rc2_ecb.o	\
		rc2_skey.o 	\
		rc2cfb64.o 	\
		rc2ofb64.o
OBJECTS += $(RC2_OBJECTS)

# rc4/*
RC4_OBJECTS =	rc4_utl.o
OBJECTS += $(RC4_OBJECTS)

# ripemd/*
RIPEMD_OBJECTS =	rmd_dgst.o	\
			rmd_one.o
OBJECTS += $(RIPEMD_OBJECTS)

# rsa/*
RSA_OBJECTS =	rsa_ameth.o	\
		rsa_asn1.o	\
		rsa_chk.o	\
		rsa_crpt.o	\
		rsa_depr.o	\
		rsa_eay.o	\
		rsa_err.o	\
		rsa_gen.o	\
		rsa_lib.o	\
		rsa_none.o	\
		rsa_null.o	\
		rsa_oaep.o	\
		rsa_pk1.o	\
		rsa_pmeth.o	\
		rsa_prn.o	\
		rsa_pss.o	\
		rsa_saos.o	\
		rsa_sign.o	\
		rsa_ssl.o	\
		rsa_x931.o
OBJECTS += $(RSA_OBJECTS)

# sha/*
SHA_OBJECTS =	sha1_one.o	\
		sha1dgst.o	\
		sha256.o	\
		sha512.o	\
		sha_dgst.o	\
		sha_one.o
OBJECTS += $(SHA_OBJECTS)

# srp/*
SRP_OBJECTS = 	srp_lib.o	\
		srp_vfy.o
OBJECTS += $(SRP_OBJECTS)

# stack/*
STACK_OBJECTS =	stack.o
OBJECTS += $(STACK_OBJECTS)

# ts/*
TS_OBJECTS =	ts_asn1.o	\
		ts_conf.o	\
		ts_err.o	\
		ts_lib.o	\
		ts_req_print.o	\
		ts_req_utils.o	\
		ts_rsp_print.o	\
		ts_rsp_sign.o	\
		ts_rsp_utils.o	\
		ts_rsp_verify.o	\
		ts_verify_ctx.o
OBJECTS += $(TS_OBJECTS)

# txt_db/*
TXT_DB_OBJECTS =	txt_db.o
OBJECTS += $(TXT_DB_OBJECTS)

# ui/*
UI_OBJECTS =	ui_compat.o	\
		ui_err.o	\
		ui_lib.o	\
		ui_openssl.o	\
		ui_util.o
OBJECTS += $(UI_OBJECTS)

# x509/*
X509_OBJECTS =	by_dir.o	\
		by_file.o	\
		x_all.o		\
		x509_att.o	\
		x509_cmp.o	\
		x509_d2.o	\
		x509_def.o	\
		x509_err.o	\
		x509_ext.o	\
		x509_lu.o	\
		x509_obj.o	\
		x509_r2x.o	\
		x509_req.o	\
		x509_set.o	\
		x509_trs.o	\
		x509_txt.o	\
		x509_v3.o	\
		x509_vfy.o	\
		x509_vpm.o	\
		x509cset.o	\
		x509name.o	\
		x509rset.o	\
		x509spki.o	\
		x509type.o
OBJECTS += $(X509_OBJECTS)

# x509v3/*
X509V3_OBJECTS =	pcy_cache.o	\
			pcy_data.o	\
			pcy_lib.o	\
			pcy_map.o	\
			pcy_node.o	\
			pcy_tree.o	\
			v3_addr.o	\
			v3_akey.o	\
			v3_akeya.o	\
			v3_alt.o	\
			v3_asid.o	\
			v3_bcons.o	\
			v3_bitst.o	\
			v3_conf.o	\
			v3_cpols.o	\
			v3_crld.o	\
			v3_enum.o	\
			v3_extku.o	\
			v3_genn.o	\
			v3_ia5.o	\
			v3_info.o	\
			v3_int.o	\
			v3_lib.o	\
			v3_ncons.o	\
			v3_ocsp.o	\
			v3_pci.o	\
			v3_pcia.o	\
			v3_pcons.o	\
			v3_pku.o	\
			v3_pmaps.o	\
			v3_prn.o	\
			v3_purp.o	\
			v3_skey.o	\
			v3_sxnet.o	\
			v3_utl.o	\
			v3err.o
OBJECTS += $(X509V3_OBJECTS)

# include library definitions
include $(SRC)/lib/Makefile.lib

CLOBBERFILES += $(LIBLINKS)

LIBS =		$(DYNLIB)

LDLIBS += -lsocket -lnsl -lc

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
CPPFLAGS += 	-DOPENSSL_PIC
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

CERRWARN +=	-_gcc=-Wno-switch
CERRWARN +=	-erroff=E_CONST_PROMOTED_UNSIGNED_LONG
CERRWARN +=	-erroff=E_END_OF_LOOP_CODE_NOT_REACHED

$(LINTLIB) :=	LINTFLAGS = -nvx -I..
$(LINTLIB) :=	LINTFLAGS64 = -nvx -m64 -I..

BUILD.perl =	$(PERL)  $< elf $(PERL_CPPFLAGS) > $@

.KEEP_STATE:

all : $(LIBS)

check_symbols: $(LIBS)
	$(ELFDUMP) $(LIBS) | \
		$(NAWK) '$$4=="FUNC" && $$5=="LOCL" && $$6=="H" && $$9!="_init" && $$9!="_fini" \
			{ if (!match($$9,"sunw_*")) print $$9; }'

lint :	lintcheck

# include library targets
include $(SRC)/lib/Makefile.targ

pics/%.o:	../%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../aes/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../asn1/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../bf/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../bio/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../bn/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../buffer/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../camellia/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../cast/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../cmac/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../cms/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../comp/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../conf/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../des/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../dh/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../dsa/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../dso/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../engine/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../err/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../evp/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../hmac/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../krb5/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../lhash/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../md2/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../md4/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../md5/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../modes/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../objects/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../ocsp/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../pem/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../pkcs12/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../pkcs7/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../pqueue/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../rand/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../rc2/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../rc4/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../ripemd/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../rsa/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../sha/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../srp/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../stack/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../ts/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../txt_db/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../ui/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../x509/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	../x509v3/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o:	%.s
	$(COMPILE.c) -o $@ $<

%.s:	../pl/%.pl
	$(BUILD.perl)

$(ROOTLINTDIR)/%: ../%
	$(INS.file)
