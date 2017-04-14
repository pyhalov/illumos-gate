/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 1991-2005 Unicode, Inc. All rights reserved. Distributed
 * under the Terms of Use in http://www.unicode.org/copyright.html.
 *
 * This file has been modified by Sun Microsystems, Inc.
 */
/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)japanese.h	1.20	07/05/25 SMI"

#define GET(c)		((c) = *ip, ip++, ileft--)
#define PUT(c)		(*op = (c), op++, oleft--)
#define UNGET()		(ip--, ileft++)

#if	defined(DEBUG)
#include <stdio.h>
static const char	*debugmsg = "<empty>";
#define	DEBUGMSG(msg)	debugmsg = (msg);
#else	/* ! DEBUG */
#define	DEBUGMSG(msg)	;
#endif	/* DEBUG */

#define	RETERROR(no, msg) {\
		errno = (no);\
		DEBUGMSG(msg);\
		rv = ((size_t)-1);\
		goto ret;\
	}

#if	defined(DEBUG)
#define	DEBUGPRINTERROR \
	if (rv == (size_t)-1) { \
		if (errno == EILSEQ) { \
			fprintf(stderr, "DEBUG: EILSEQ: %s\n", debugmsg); \
		} else if (errno == E2BIG) { \
			fprintf(stderr, "DEBUG: E2BIG: %s\n", debugmsg); \
		} else if (errno == EINVAL) { \
			fprintf(stderr, "DEBUG: EINVAL: %s\n", debugmsg); \
		} else { \
			fprintf(stderr, \
			"DEBUG: errno=%d: %s\n", errno, debugmsg); \
		} \
	}
#else	/* !DEBUG */
#define	DEBUGPRINTERROR	/* nop */
#endif	/* DEBUG */

#define NGET(c, msg) \
	if (ileft-- == 0) {\
		RETERROR(EINVAL, msg)\
	} else {\
		(c) = *ip++;\
	}

#define NPUT(c, msg) \
	if (oleft-- == 0) {\
		RETERROR(E2BIG, msg)\
	} else {\
		*op++ = (c);\
	}

/* to be obsoleted; migrate to PUTU() in jfp_iconv_unicode.h */
#if	defined(JFP_ICONV_TOCODE_UCS2)
#define	PUTUCS2(uni, msg)	\
	if (write_unicode((unsigned int)uni, &op, &oleft, little, msg) \
			== (size_t)-1) { \
		rv = ((size_t)-1);\
		goto ret; \
	}
#else	/* !JFP_ICONV_TOCODE_UCS2 */
#define	PUTUCS2(ucs2, msg)	\
	if (ucs2 <= 0x7f) {\
		NPUT((unsigned char)(ucs2), msg);\
	} else if (ucs2 <= 0x7ff) {\
		NPUT((unsigned char)((((ucs2)>>6) & 0x1f) | 0xc0), msg);\
		NPUT((unsigned char)((((ucs2)>>0) & 0x3f) | 0x80), msg);\
	} else if (ucs2 <= 0xffff) {\
		NPUT((unsigned char)((((ucs2)>>12) & 0x0f) | 0xe0), msg);\
		NPUT((unsigned char)((((ucs2)>>6) & 0x3f) | 0x80), msg);\
		NPUT((unsigned char)((((ucs2)>>0) & 0x3f) | 0x80), msg);\
	}
#endif	/* JFP_ICONV_TOCODE_UCS2 */

#if	defined(JFP_ICONV_STATELESS)

#include	<stdlib.h>

static void *
_icv_open_stateless(void)
{
	void	*cd;

	/*
	 * Do malloc() only to get address as unique conversion descriptor.
	 * Though iconv() specification doesn't describe the uniqueness,
	 * some existing applications may be assuming it.
	 * Unless call of malloc() here is raised as a problem, keep away
	 * from breaking the assumption.
	 */

	if ((cd = malloc((size_t)1)) == NULL) {
		errno = ENOMEM;
		return ((void *)-1);
	} else {
		return (cd);
	}
}

static void
_icv_close_stateless(void *cd)
{
	if (cd == NULL) {
		errno = EBADF;
	} else {
		free(cd);
	}
	return;
}

#endif	/* ! JFP_ICONV_STATELESS */

#define ERR_RETURN	(-1)		/* result code on error */

/*
 * ISXXXX(c) macros below assume (c) is of type unsigned char
 */
/* is a valid code as ascii? */
#define ISASC(c)		((c) <= 0x7f)

/* is a valid code as C1 control? */
#define ISC1CTRL(c)		(((c) >= 0x80) && ((c) <= 0x9f))

/* is a valid code as C1 control allowed in EUC? */
#define ISC1CTRLEUC(c)		((((c) >= 0x80) && ((c) <= 0x8d)) ||\
					(((c) >= 0x90) && ((c) <= 0x9f)))

/* is a valid 1st or 2nd byte of EUC codeset 1? */
#define ISCS1(c)		(((c) >= 0xa1) && ((c) <= 0xfe))

/* is a valid 1st byte of EUC codeset 1 and in range row 1 thru 84? */
#define ISCS1_0208(c)		(((c) >= 0xa1) && ((c) <= 0xf4))

/* is a valid 1st byte of UDC area in EUC codeset 1? */
#define ISCS1_UDC(c)		(((c) >= 0xf5) && ((c) <= 0xfe))

/* is a valid 2nd (1st byte is SS2) byte of EUC codeset 2? */
#define ISCS2(c)		(((c) >= 0xa1) && ((c) <= 0xdf))

/* is a valid 2nd or 3rd (1st byte is SS3) byte of EUC codeset 3? */
#define ISCS3(c)		(((c) >= 0xa1) && ((c) <= 0xfe))

/* is a valid 2nd (1st byte is SS3) byte of UDC area in EUC codeset 3? */
#define ISCS3_UDC(c)		(((c) >= 0xf5) && ((c) <= 0xfe))

/* is a valid hankaku_katakana for SJIS? */
#define ISSJKANA(c)		(((c) >= 0xa1) && ((c) <= 0xdf))

/* is a valid character for the first byte of SJIS multibyte? */
#define ISSJMB_1(c)	((((c) >= 0x81) && ((c) <= 0x9f)) ||\
				 (((c) >= 0xe0) && ((c) <= 0xfc)))

/* is a valid character for the first byte of SJIS kanji? */
#define ISSJKANJI1(c)	((((c) >= 0x81) && ((c) <= 0x9f)) ||\
				 (((c) >= 0xe0) && ((c) <= 0xea)) ||\
				 (((c) >= 0xf0) && ((c) <= 0xf4)))

/* is a valid character for the first byte of a part of Suppl. SJIS? */
#define ISSJSUPKANJI1(c)	(((c) >= 0xf5) && ((c) <= 0xf9))

/* is a valid character for the first byte of SJIS UDC? */
#define ISSJUDC_1(c)		(((c) >= 0xf0) && ((c) <= 0xf9))

/* is a valid character for the first byte of ibm character set */
#define	ISSJIBM(c)		(((c) >= 0xfa) && ((c) <= 0xfc))

/* is a valid character for the first byte of ibm character set */
#define	ISSJNECIBM(c)		(((c) >= 0xed) && ((c) <= 0xef))

/* is a valid character for the second byte of SJIS kanji? */
#define ISSJKANJI2(c)	((((c) >= 0x40) && ((c) <= 0x7e)) ||\
					 (((c) >= 0x80) && ((c) <= 0xfc)))

/* is a valid character for UTF8 UDC ? */
#define ISUTF8UDC(c)	(((c) >= 0xe000) && ((c) <= 0xf8ff))

#define CS_0			0		/* codeset 0 */
#define CS_1			1		/* codeset 1 */
#define CS_2			2		/* codeset 2 */
#define CS_3			3		/* codeset 3 */

#define ST_INIT			0		/* init */
#define ST_INCS1		1		/* in codeset 1 */
#define ST_INCS2		2		/* in codeset 2 */
#define ST_INCS3		3		/* in codeset 3 */
#define ST_ESC			4		/* in ESC */
#define ST_MBTOG0_1		5		/* in the designation of MB to G0 - 1 */
#define ST_MBTOG0_2		6		/* in the designation of MB to G0 - 2 */
#define ST_SBTOG0		7		/* in the designation of SB to G0 */
#define ST_208REV_1		8		/* in the designation of JISX208-1990 */
#define ST_208REV_2		9		/* in the designation of JISX208-1990 */
#define ST_REV_AFT_ESC		10		/* in the designation of JISX208-1990 */
#define ST_REV_AFT_MBTOG0_1	11		/* in the designation of JISX208-1990 */
#define ST_REV_AFT_MBTOG0_2	12		/* in the designation of JISX208-1990 */

/*
 * CODE SET 0
 * ESC ( B   			: To ASCII
 * ESC ( J			: To JIS X 0201 - 1976 ROMAN
 * ESC ( @			: TO ISO 646 IRV
 *
 * CODE SET 1
 * ESC & @ ESC $ ( B		: To JIS X 0208 - 1990
 * ESC $ ( B			: To JIS X 0208 - 1983/1990
 * ESC $ ( @			: To JIS X 0208 - 1978
 * ESC $ B			: To JIS X 0208 - 1983/1990
 * ESC $ @			: To JIS X 0208 - 1978
 * ESC & @ ESC $ B		: To JIS X 0208 - 1983/1990
 *
 * CODE SET 2
 * SO  					: G1 -> G
 * SI  					: G0 -> G
 * ESC ( I				: To JIS X 0201 - 1976 Katakana
 *
 * CODE SET 3
 * ESC $ ( D			: To JIS X 0212 - 1990
 * ESC $ D			: To JIS X 0212 - 1990
 *
 */

#define ESC				0x1b		/* Escape : 1/12 */
#define SO				0x0e		/* Shift Out : 0/14 */
#define SI				0x0f		/* Shift In  : 0/15 */

#define SBTOG0_1			0x28		/* ( : 2/8 */
#define F_ASCII				0x42		/* B : 4/2 */
#define F_X0201_RM			0x4a		/* J : 4/10 */
#define F_ISO646			0x40		/* @ : 4/0 */
#define F_X0201_KN			0x49		/* I : 4/9 */

#define MBTOG0_1			0x24		/* $ : 2/4 */
#define MBTOG0_2			0x28		/* ( : 2/8 */
#define F_X0208_83_90			0x42		/* B : 4/2 */
#define F_X0208_78			0x40		/* @ : 4/0 */
#define F_X0212_90			0x44		/* D : 4/4 */
#define	X208REV_1			0x26		/* & : 2/6 */
#define	X208REV_2			0x40		/* @ : 4/0 */

#define	CMASK				0x7f
#define	CMSB				0x80

/* the byte length of ESC sequences */
#define SEQ_SBTOG0			3			/* ESC + ( + F */
#define SEQ_MBTOG0			4			/* ESC + $ + ( + F */
#define SEQ_MBTOG0_O			3			/* ESC + $ + F */

/* the byte length of SO/SI */
#define SEQ_SOSI			1			/* SO or SI */

/* the byte length of SS2/SS3 */
#define SEQ_SS				1			/* SS2 or SS3 */

/* the byte length of JIS characters */
#define JISW0				1			/* ASCII */
#define JISW1				2			/* Kanji */
#define JISW2				1			/* Hankaku Katakana */
#define JISW3				2			/* Hojo Kanji */

/* the byte length of EUC characters */
#define EUCW0				1			/* ASCII */
#define EUCW1				2			/* Kanji */
#define EUCW2				1			/* Hankaku Katakana */
#define EUCW3				2			/* Hojo Kanji */
#define SS2W				1			/* SS2 */
#define SS3W				1			/* SS3 */

/* the byte length of SJIS characters */
#define SJISW0				1			/* ASCII */
#define SJISW1				2			/* Kanji/UDC */
#define SJISW2				1			/* Hankaku Katakana */

#define EBCDIC0				1
#define EBCDIC1				2
#define EBCDIC2				1
#define EBCDIC3				2

/* the byte length of unknown characters */
#define UNKNOWNW			1

#define	TRUE	(1)
#define	FALSE	(0)

/*
 * sjtojis1[]:
 * Directly convert 1st octet of Shift-JIS encoded 2bytes codes
 * to the 1st octet of 7bit-JIS encoded 2bytes codes.
 * Because the 8th bit is always ON, the size of this array
 * can be limited to 128. (Precisely, 1st octet can vary between
 * 0x81 and 0xfc, and correspondent 7bit-JIS value is between
 * 0x21 and 0x7e. So we can use "0xff" as indicating invalid value.)
 *
 * Principal:
 * 1: Subtract 0x80
 * 2: Index the array by using the value above as index number.
 * 3: Value "0xff" means the index number is invalid or, there's
 *    no 7bit-JIS value that corresponds to.
 *
 * Note:
 * 1: When the 2nd octet of Shift-JIS encoded 2bytes codes is
 *    more than 0x9f, indexed value should be augumented by 1.
 * 2: Some of Shift-JIS code points are mapped to JISX0208 plane
 *    and others are JISX0212 plane.
 *    Need to differentiate them before indexing and designate
 *    appropriate character set. This table converts only code points
 *    within each character set.
 *
 */
static const unsigned char sjtojis1[] = {
/* Values never be indexed */
0xff,
/* Index 1KU - 62KU in JISX0208 */
0x21, 0x23, 0x25, 0x27, 0x29, 0x2b, 0x2d, 0x2f,
0x31, 0x33, 0x35, 0x37, 0x39, 0x3b, 0x3d, 0x3f,
0x41, 0x43, 0x45, 0x47, 0x49, 0x4b, 0x4d, 0x4f,
0x51, 0x53, 0x55, 0x57, 0x59, 0x5b, 0x5d,
/* Values never be indexed */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
/* Index 63KU - 84KU in JISX0208 */
                                          0x5f,
0x61, 0x63, 0x65, 0x67, 0x69, 0x6b, 0x6d, 0x6f,
0x71, 0x73,
/* Values never be indexed */
0xff, 0xff,
/* Followings will be mapped to 79KU - 84KU in JISX0212 (NEC/IBM) */
/* 0x6f, 0x71, 0x73,						*/
0xff, 0xff, 0xff,
/* Followings will be mapped to 85KU - 94KU in JISX0208 (UDC) */
0x75, 0x77, 0x79, 0x7b, 0x7d,
/* Followings will be mapped to 85KU - 94KU in JISX0212 (UDC) */
0x75, 0x77, 0x79, 0x7b, 0x7d,
/* Followings will be mapped to 79KU - 84KU in JISX0212 (IBM) */
/* 0x6f, 0x71, 0x73,						*/
0xff, 0xff, 0xff,
/* Values never be indexed */
0xff, 0xff, 0xff
};

/*
 * sjtojis2[]:
 * Directly convert 2nd octet of Shift-JIS encoded 2bytes codes
 * to the 2nd octet of 7bit-JIS encoded 2bytes codes.
 * 2nd octet in Shift-JIS can vary between 0x40 and 0xfc(except 0x7f)
 * and the correspondent 7bit-JIS octet is between 0x21 and 0x7e.
 * (So we can use '0xff' as indicating invalid value.)
 *
 * Principal:
 * 1: Index the array by using the value above as index number.
 * 2: Value "0xff" means the index number is invalid or, there's
 *    no 7bit-JIS value that corresponds to.
 *
 * Note:
 * 1: Some of Shift-JIS code points are mapped to JISX0208 plane
 *    and others to JISX0212 plane.
 *    Need to differentiate them before indexing and designate
 *    appropriate character set. This table converts only code points
 *    within each character set.
 *
 */
static const unsigned char sjtojis2[] = {
/* Values never be indexed */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
/* Followings are 2nd octet for 'odd' KU */
0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
/* Values never be indexed */
0xff,
/* Followings are 2nd octet for 'odd' KU */
                                          0x60,
0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
/* Followings are 2nd octet for 'even' KU */
0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
/* Values never be indexed */
0xff, 0xff, 0xff
};

/*
 * sjtoibmext[]:
 * Directly convert Shift-JIS encoded 2bytes codes
 * to the EUC encoded 2bytes codes.
 * This table is used for "IBM Extended Character Set" only.
 *
 * Principal:
 * 1: Subtract 0xfa40.
 * 2: Index the array by using the value above as index number.
 * 3: Value "0xffff" means the index number is invalid or, there's
 *    no EUC value that corresponds to.
 *
 * Note: The SJIS code points, 0xfa54 and 0xfa5b are mapped to JIS208
 *       hence they are NOT mapped with tables below.
 *       (They are mapped to 0xa2cc and 0xa2e8 in JIS208 respectively.)
 */
static const unsigned short sjtoibmext[] = {
0xf3f3, 0xf3f4, 0xf3f5, 0xf3f6, 0xf3f7, 0xf3f8, 0xf3f9, 0xf3fa, /* 0xfa47 */
0xf3fb, 0xf3fc, 0xf3fd, 0xf3fe, 0xf4a1, 0xf4a2, 0xf4a3, 0xf4a4,
0xf4a5, 0xf4a6, 0xf4a7, 0xf4a8, 0xffff, 0xa2c3, 0xf4a9, 0xf4aa,
0xf4ab, 0xf4ac, 0xf4ad, 0xffff, 0xd4e3, 0xdcdf, 0xe4e9, 0xe3f8,
0xd9a1, 0xb1bb, 0xf4ae, 0xc2ad, 0xc3fc, 0xe4d0, 0xc2bf, 0xbcf4,
0xb0a9, 0xb0c8, 0xf4af, 0xb0d2, 0xb0d4, 0xb0e3, 0xb0ee, 0xb1a7,
0xb1a3, 0xb1ac, 0xb1a9, 0xb1be, 0xb1df, 0xb1d8, 0xb1c8, 0xb1d7,
0xb1e3, 0xb1f4, 0xb1e1, 0xb2a3, 0xf4b0, 0xb2bb, 0xb2e6, 0xffff,
0xb2ed, 0xb2f5, 0xb2fc, 0xf4b1, 0xb3b5, 0xb3d8, 0xb3db, 0xb3e5,
0xb3ee, 0xb3fb, 0xf4b2, 0xf4b3, 0xb4c0, 0xb4c7, 0xb4d0, 0xb4de,
0xf4b4, 0xb5aa, 0xf4b5, 0xb5af, 0xb5c4, 0xb5e8, 0xf4b6, 0xb7c2,
0xb7e4, 0xb7e8, 0xb7e7, 0xf4b7, 0xf4b8, 0xf4b9, 0xb8ce, 0xb8e1,
0xb8f5, 0xb8f7, 0xb8f8, 0xb8fc, 0xb9af, 0xb9b7, 0xbabe, 0xbadb,
0xcdaa, 0xbae1, 0xf4ba, 0xbaeb, 0xbbb3, 0xbbb8, 0xf4bb, 0xbbca,
0xf4bc, 0xf4bd, 0xbbd0, 0xbbde, 0xbbf4, 0xbbf5, 0xbbf9, 0xbce4,
0xbced, 0xbcfe, 0xf4be, 0xbdc2, 0xbde7, 0xf4bf, 0xbdf0, 0xbeb0,
0xbeac, 0xf4c0, 0xbeb3, 0xbebd, 0xbecd, 0xbec9, 0xbee4, 0xbfa8,
0xbfc9, 0xc0c4, 0xc0e4, 0xc0f4, 0xc1a6, 0xf4c1, 0xc1f5, 0xc1fc,
0xf4c2, 0xc1f8, 0xc2ab, 0xc2a1, 0xc2a5, 0xf4c3, 0xc2b8, 0xc2ba,
0xf4c4, 0xc2c4, 0xc2d2, 0xc2d7, 0xc2db, 0xc2de, 0xc2ed, 0xc2f0,
0xf4c5, 0xc3a1, 0xc3b5, 0xc3c9, 0xc3b9, 0xf4c6, 0xc3d8, 0xc3fe,
0xf4c7, 0xc4cc, 0xf4c8, 0xc4d9, 0xc4ea, 0xc4fd, 0xf4c9, 0xc5a7,
0xc5b5, 0xc5b6, 0xf4ca, 0xc5d5, 0xc6b8, 0xc6d7, 0xc6e0, 0xc6ea,
0xc6e3, 0xc7a1, 0xc7ab, 0xc7c7, 0xc7c3, 0xffff, 0xffff, 0xffff,
0xc7cb, 0xc7cf, 0xc7d9, 0xf4cb, 0xf4cc, 0xc7e6, 0xc7ee, 0xc7fc, /* 0xfb47 */
0xc7eb, 0xc7f0, 0xc8b1, 0xc8e5, 0xc8f8, 0xc9a6, 0xc9ab, 0xc9ad,
0xf4cd, 0xc9ca, 0xc9d3, 0xc9e9, 0xc9e3, 0xc9fc, 0xc9f4, 0xc9f5,
0xf4ce, 0xcab3, 0xcabd, 0xcaef, 0xcaf1, 0xcbae, 0xf4cf, 0xcbca,
0xcbe6, 0xcbea, 0xcbf0, 0xcbf4, 0xcbee, 0xcca5, 0xcbf9, 0xccab,
0xccae, 0xccad, 0xccb2, 0xccc2, 0xccd0, 0xccd9, 0xf4d0, 0xcdbb,
0xf4d1, 0xcebb, 0xf4d2, 0xceba, 0xcec3, 0xf4d3, 0xcef2, 0xb3dd,
0xcfd5, 0xcfe2, 0xcfe9, 0xcfed, 0xf4d4, 0xf4d5, 0xf4d6, 0xffff,
0xf4d7, 0xd0e5, 0xf4d8, 0xd0e9, 0xd1e8, 0xf4d9, 0xf4da, 0xd1ec,
0xd2bb, 0xf4db, 0xd3e1, 0xd3e8, 0xd4a7, 0xf4dc, 0xf4dd, 0xd4d4,
0xd4f2, 0xd5ae, 0xf4de, 0xd7de, 0xf4df, 0xd8a2, 0xd8b7, 0xd8c1,
0xd8d1, 0xd8f4, 0xd9c6, 0xd9c8, 0xd9d1, 0xf4e0, 0xf4e1, 0xf4e2,
0xf4e3, 0xf4e4, 0xdcd3, 0xddc8, 0xddd4, 0xddea, 0xddfa, 0xdea4,
0xdeb0, 0xf4e5, 0xdeb5, 0xdecb, 0xf4e6, 0xdfb9, 0xf4e7, 0xdfc3,
0xf4e8, 0xf4e9, 0xe0d9, 0xf4ea, 0xf4eb, 0xe1e2, 0xf4ec, 0xf4ed,
0xf4ee, 0xe2c7, 0xe3a8, 0xe3a6, 0xe3a9, 0xe3af, 0xe3b0, 0xe3aa,
0xe3ab, 0xe3bc, 0xe3c1, 0xe3bf, 0xe3d5, 0xe3d8, 0xe3d6, 0xe3df,
0xe3e3, 0xe3e1, 0xe3d4, 0xe3e9, 0xe4a6, 0xe3f1, 0xe3f2, 0xe4cb,
0xe4c1, 0xe4c3, 0xe4be, 0xf4ef, 0xe4c0, 0xe4c7, 0xe4bf, 0xe4e0,
0xe4de, 0xe4d1, 0xf4f0, 0xe4dc, 0xe4d2, 0xe4db, 0xe4d4, 0xe4fa,
0xe4ef, 0xe5b3, 0xe5bf, 0xe5c9, 0xe5d0, 0xe5e2, 0xe5ea, 0xe5eb,
0xf4f1, 0xf4f2, 0xf4f3, 0xe6e8, 0xe6ef, 0xe7ac, 0xf4f4, 0xe7ae,
0xf4f5, 0xe7b1, 0xf4f6, 0xe7b2, 0xe8b1, 0xe8b6, 0xf4f7, 0xf4f8,
0xe8dd, 0xf4f9, 0xf4fa, 0xe9d1, 0xf4fb, 0xffff, 0xffff, 0xffff,
0xe9ed, 0xeacd, 0xf4fc, 0xeadb, 0xeae6, 0xeaea, 0xeba5, 0xebfb, /* 0xfc47 */
0xebfa, 0xf4fd, 0xecd6, 0xf4fe, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
};
/*
 * jis208tosj1[]:
 * Directly convert 1st octet of JISX0208 encoded 2bytes codes
 * to the 1st octet of Shift-JIS encoded 2bytes codes.
 * Because the 8th bit is always OFF, the size of this array
 * can be limited to 128. (Precisely, 1st octet can vary between
 * 0x21 and 0x7e, and the correspondent Shift-JIS code is
 * between 0x81 and 0xfc. So we can use "0xff" as indicating invalid
 * value.)
 *
 * Principal:
 * 1: Index the array by using the value above as index number.
 * 2: Value "0xff" means the index number is invalid or, there's
 *    no Shift-JIS value that corresponds to.
 *
 * Note:
 * 1: Some of Shift-JIS code points are mapped to JISX0208 plane
 *    and others to JISX0212 plane.
 *    Need to differentiate them before indexing and designate
 *    appropriate character set. This table converts only code points
 *    within each character set.
 *
 */
static const unsigned char jis208tosj1[] = {
/* Values never be indexed */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff,
/* Index 1KU - 62KU in JISX0208 */
      0x81, 0x81, 0x82, 0x82, 0x83, 0x83, 0x84,
0x84, 0x85, 0x85, 0x86, 0x86, 0x87, 0x87, 0x88,
0x88, 0x89, 0x89, 0x8a, 0x8a, 0x8b, 0x8b, 0x8c,
0x8c, 0x8d, 0x8d, 0x8e, 0x8e, 0x8f, 0x8f, 0x90,
0x90, 0x91, 0x91, 0x92, 0x92, 0x93, 0x93, 0x94,
0x94, 0x95, 0x95, 0x96, 0x96, 0x97, 0x97, 0x98,
0x98, 0x99, 0x99, 0x9a, 0x9a, 0x9b, 0x9b, 0x9c,
0x9c, 0x9d, 0x9d, 0x9e, 0x9e, 0x9f, 0x9f,
/* Index 63KU - 84KU in JISX0208 */
                                          0xe0,
0xe0, 0xe1, 0xe1, 0xe2, 0xe2, 0xe3, 0xe3, 0xe4,
0xe4, 0xe5, 0xe5, 0xe6, 0xe6, 0xe7, 0xe7, 0xe8,
0xe8, 0xe9, 0xe9, 0xea, 0xea,
/* Followings are mapped to "UDC" area in Shift-JIS */
                              0xf0, 0xf0, 0xf1,
0xf1, 0xf2, 0xf2, 0xf3, 0xf3, 0xf4, 0xf4,
/* Values never be indexed */
0xff
};

/*
 * jis212tosj1[]:
 * Directly convert 1st octet of JISX0212 encoded 2bytes codes
 * to the 1st octet of Shift-JIS encoded 2bytes codes.
 * Because the 8th bit is always OFF, the size of this array
 * can be limited to 128. (Precisely, 1st octet can vary between
 * 0x21 and 0x7e, and the correspondent Shift-JIS code is
 * between 0x81 and 0xfc. So we can use "0xff" as indicating invalid
 * value.)
 *
 * Principal:
 * 1: Index the array by using the value above as index number.
 * 2: Value "0xff" means the index number is invalid or, there's
 *    no Shift-JIS value that corresponds to.
 *
 * Note:
 * 1: Most G3 characters cannot map to Shift-JIS coded space.
 *    (Such characters are mapped to unique code point. In this
 *    table, it is 0x222e in JISX0208.)
 * 2: Some of Shift-JIS code points are mapped to JISX0208 plane
 *    and others to JISX0212 plane.
 *    Need to differentiate them before indexing and designate
 *    appropriate character set. This table converts only code points
 *    within each character set.
 *
 */
static const unsigned char jis212tosj1[] = {
/* Values never be indexed */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
/* Followings are mapped to "NEC/IBM(?)" area in Shift-JIS */
/*                                          0xfa,	*/
/* 0xfa, 0xfb, 0xfb, 0xfc, 0xfc,			*/
                                          0xff,
0xff, 0xff, 0xff, 0xff, 0xff,
/* Followings are mapped to "UDC" area in Shift-JIS */
                              0xf5, 0xf5, 0xf6,
0xf6, 0xf7, 0xf7, 0xf8, 0xf8, 0xf9, 0xf9,
/* Values never be indexed */
0xff
};

/*
 * jistosj2[]:
 * Directly convert 2nd octet of 7bit-JIS encoded 2bytes codes
 * to the 2nd octet of Shift-JIS encoded 2bytes codes.
 * Although the source code may have the same value between
 * JISX0208 and JISX0212, the destination code point can vary with regard to
 * the eveness(oddness) of 'KU' number.
 *
 * Principal:
 * 1: Add 0x80 if 'KU' number is even.
 * 2: Index the array by using the value above as index number.
 * 3: Value "0xff" means the index number is invalid or, there's
 *    no Shift-JIS value that corresponds to.
 *
 * Note:
 * 1: Use this table to map the 2nd octet of both JISX0208
 *    and JISX0212.
 *
 */
static const unsigned char jistosj2[] = {
/* Values never be indexed */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff,
/* Index 1TEN - 63TEN in 'odd KU' */
      0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
/* Index 64TEN - 94TEN in 'odd KU' */
0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e,
/* Values never be indexed */
                                          0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff,
/* Index 1TEN - 63TEN in 'even KU' */
      0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad,
0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd,
0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5,
0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5,
0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd,
0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,
0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed,
0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc,
/* Values never be indexed */
0xff
};

#define	JGETA	(0x222e)
#define	EGETA	(0xa2ae)
#define	PGETA	(0x81ac)

/*
 * Remap NEC/IBM codes to IBM codes
 * if dest == 0xffff, that means the source
 * code point is illegal in the current spec.
 */

 #define REMAP_NEC(dest) \
	if ((0xed40 <= dest) && \
		(dest <= 0xed62)) { \
		dest += 0xd1c; \
	} else if ((0xed63 <= dest) && \
		(dest <= 0xed7e)) { \
		dest += 0xd1d; \
	} else if ((0xed80 <= dest) && \
		(dest <= 0xede0)) { \
		dest += 0xd1c; \
	} else if ((0xede1 <= dest) && \
		(dest <= 0xedfc)) { \
		dest += 0xd5f; \
	} else if ((0xee40 <= dest) && \
		(dest <= 0xee62)) { \
		dest += 0xd1c; \
	} else if ((0xee63 <= dest) && \
		(dest <= 0xee7e)) { \
		dest += 0xd1d; \
	} else if ((0xee80 <= dest) && \
		(dest <= 0xeee0)) { \
		dest += 0xd1c; \
	} else if ((0xeee1 <= dest) && \
		(dest <= 0xeeec)) { \
		dest += 0xd5f; \
	} else if ((0xeeef <= dest) && \
		(dest <= 0xeef8)) { \
		dest += 0xb51; \
	} else if ((0xeef9 <= dest) && \
		(dest <= 0xeefc)) { \
		dest += 0xb5b; \
	} else { \
		dest = 0xffff; \
	}

#define CHECK2BIG(width,time) \
	if (oleft < (width)) { \
		int i; \
		for (i = time; i > 0; i--) { \
			UNGET(); \
		} \
		errno = E2BIG; \
		retval = (size_t)ERR_RETURN; \
		goto ret; \
	}

/*
 * halfkana2zenkakuj[]:
 * Directly convert JIS X 0201 Kana (half width) to JIS X 0208
 * Kana (full width).
 * This table is for ISO-2022-JP.RFC1468 mode.
 *
 * Principal:
 * 1: Subtract 0x21 or 0x81.
 * 2: Index the array by using the value above as index number.
 *
 * Note:
 */
static const unsigned short halfkana2zenkakuj[] = {
0x2123, 0x2156, 0x2157, 0x2122, 0x2126, 0x2572, 0x2521, 0x2523,
0x2525, 0x2527, 0x2529, 0x2563, 0x2565, 0x2567, 0x2543, 0x213c,
0x2522, 0x2524, 0x2526, 0x2528, 0x252a, 0x252b, 0x252d, 0x252f,
0x2531, 0x2533, 0x2535, 0x2537, 0x2539, 0x253b, 0x253d, 0x253f,
0x2541, 0x2544, 0x2546, 0x2548, 0x254a, 0x254b, 0x254c, 0x254d,
0x254e, 0x254f, 0x2552, 0x2555, 0x2558, 0x255b, 0x255e, 0x255f,
0x2560, 0x2561, 0x2562, 0x2564, 0x2566, 0x2568, 0x2569, 0x256a,
0x256b, 0x256c, 0x256d, 0x256f, 0x2573, 0x212b, 0x212c};

/*
 * halfkana2zenkakue[]:
 * Directly convert JIS X 0201 Kana (half width) to JIS X 0208
 * (eucJP).
 * This table is for ISO-2022-JP.RFC1468 mode.
 *
 * Principal:
 * 1: Subtract 0x21 or 0x81.
 * 2: Index the array by using the value above as index number.
 *
 * Note:
 */
static const unsigned short halfkana2zenkakue[] = {
0xa1a3, 0xa1d6, 0xa1d7, 0xa1a2, 0xa1a6, 0xa5f2, 0xa5a1,
0xa5a3, 0xa5a5, 0xa5a7, 0xa5a9, 0xa5e3, 0xa5e5, 0xa5e7,
0xa5c3, 0xa1bc, 0xa5a2, 0xa5a4, 0xa5a6, 0xa5a8, 0xa5aa,
0xa5ab, 0xa5ad, 0xa5af, 0xa5b1, 0xa5b3, 0xa5b5, 0xa5b7,
0xa5b9, 0xa5bb, 0xa5bd, 0xa5bf, 0xa5c1, 0xa5c4, 0xa5c6,
0xa5c8, 0xa5ca, 0xa5cb, 0xa5cc, 0xa5cd, 0xa5ce, 0xa5cf,
0xa5d2, 0xa5d5, 0xa5d8, 0xa5db, 0xa5de, 0xa5df, 0xa5e0,
0xa5e1, 0xa5e2, 0xa5e4, 0xa5e6, 0xa5e8, 0xa5e9, 0xa5ea,
0xa5eb, 0xa5ec, 0xa5ed, 0xa5ef, 0xa5f3, 0xa1ab, 0xa1ac};

/*
 * halfkana2zenkakus[]:
 * Directly convert JIS X 0201 Kana (half width) to PCK Kana
 * (full width).
 * This table is for ISO-2022-JP.RFC1468 mode.
 *
 * Principal:
 * 1: Subtract 0x21 or 0x81.
 * 2: Index the array by using the value above as index number.
 *
 * Note:
 */
static const unsigned short halfkana2zenkakus[] = {
0x8142, 0x8175, 0x8176, 0x8141, 0x8145, 0x8392, 0x8340, 0x8342,
0x8344, 0x8346, 0x8348, 0x8383, 0x8385, 0x8387, 0x8362, 0x815b,
0x8341, 0x8343, 0x8345, 0x8347, 0x8349, 0x834a, 0x834c, 0x834e,
0x8350, 0x8352, 0x8354, 0x8356, 0x8358, 0x835a, 0x835c, 0x835e,
0x8360, 0x8363, 0x8365, 0x8367, 0x8369, 0x836a, 0x836b, 0x836c,
0x836d, 0x836e, 0x8371, 0x8374, 0x8377, 0x837a, 0x837d, 0x837e,
0x8380, 0x8381, 0x8382, 0x8384, 0x8386, 0x8388, 0x8389, 0x838a,
0x838b, 0x838c, 0x838d, 0x838f, 0x8393, 0x814a, 0x814b};
