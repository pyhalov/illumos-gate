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
 *  under the Terms of Use in http://www.unicode.org/copyright.html.
 *
 * This file has been modified by Sun Microsystems, Inc.
 *
*/
/*
 * Copyright(c) 2008 Sun Microsystems, Inc.
 * All rights reserved.
 */
#ident "@(#)unicode_viscii.h	1.1 08/07/21"
#ifndef _UNICODE_VISCII_H_
#define _UNICODE_VISCII_H_

#include <public_struc.h>


static const unsigned long viscii2uni_1[32] = {
    /*
    * 0x00 - Ox1f
    */
    0x0000,  0x0001,  0x1EB2,  0x0003,  0x0004,  0x1EB4,  0x1EAA,  0x0007,
    0x0008,  0x0009,  0x000A,  0x000B,  0x000C,  0x000D,  0x000E,  0x000F,
    0x0010,  0x0011,  0x0012,  0x0013,  0x1EF6,  0x0015,  0x0016,  0x0017,
    0x0018,  0x1EF8,  0x001A,  0x001B,  0x001C,  0x001D,  0x1EF4,  0x001F,
};

static const unsigned long viscii2uni_2[128] = {
    /*
    * 0x80 - Oxff
    */
    0x1EA0,  0x1EAE,  0x1EB0,  0x1EB6,  0x1EA4,  0x1EA6,  0x1EA8,  0x1EAC,
    0x1EBC,  0x1EB8,  0x1EBE,  0x1EC0,  0x1EC2,  0x1EC4,  0x1EC6,  0x1ED0,
    0x1ED2,  0x1ED4,  0x1ED6,  0x1ED8,  0x1EE2,  0x1EDA,  0x1EDC,  0x1EDE,
    0x1ECA,  0x1ECE,  0x1ECC,  0x1EC8,  0x1EE6,  0x0168,  0x1EE4,  0x1EF2,
    0x00D5,  0x1EAF,  0x1EB1,  0x1EB7,  0x1EA5,  0x1EA7,  0x1EA9,  0x1EAD,
    0x1EBD,  0x1EB9,  0x1EBF,  0x1EC1,  0x1EC3,  0x1EC5,  0x1EC7,  0x1ED1,
    0x1ED3,  0x1ED5,  0x1ED7,  0x1EE0,  0x01A0,  0x1ED9,  0x1EDD,  0x1EDF,
    0x1ECB,  0x1EF0,  0x1EE8,  0x1EEA,  0x1EEC,  0x01A1,  0x1EDB,  0x01AF,
    0x00C0,  0x00C1,  0x00C2,  0x00C3,  0x1EA2,  0x0102,  0x1EB3,  0x1EB5,
    0x00C8,  0x00C9,  0x00CA,  0x1EBA,  0x00CC,  0x00CD,  0x0128,  0x1EF3,
    0x0110,  0x1EE9,  0x00D2,  0x00D3,  0x00D4,  0x1EA1,  0x1EF7,  0x1EEB,
    0x1EED,  0x00D9,  0x00DA,  0x1EF9,  0x1EF5,  0x00DD,  0x1EE1,  0x01B0,
    0x00E0,  0x00E1,  0x00E2,  0x00E3,  0x1EA3,  0x0103,  0x1EEF,  0x1EAB,
    0x00E8,  0x00E9,  0x00EA,  0x1EBB,  0x00EC,  0x00ED,  0x0129,  0x1EC9,
    0x0111,  0x1EF1,  0x00F2,  0x00F3,  0x00F4,  0x00F5,  0x1ECF,  0x1ECD,
    0x1EE5,  0x00F9,  0x00FA,  0x0169,  0x1EE7,  0x00FD,  0x1EE3,  0x1EEE,
};

static int viscii_2_uni(const unsigned char *in, unsigned long *out )
{
    unsigned char c = *in;
    if (c < 0x20)
        *out = (unsigned long) viscii2uni_1[c];
    else if (c < 0x80)
        *out = (unsigned long) c;
    else
        *out = (unsigned long) viscii2uni_2[c-0x80];
    return 1;
}

static const unsigned char uni2viscii_1[248] = {
    0xc0, 0xc1, 0xc2, 0xc3, 0x00, 0x00, 0x00, 0x00,
    0xc8, 0xc9, 0xca, 0x00, 0xcc, 0xcd, 0x00, 0x00,
    0x00, 0x00, 0xd2, 0xd3, 0xd4, 0xa0, 0x00, 0x00,
    0x00, 0xd9, 0xda, 0x00, 0x00, 0xdd, 0x00, 0x00,
    0xe0, 0xe1, 0xe2, 0xe3, 0x00, 0x00, 0x00, 0x00,
    0xe8, 0xe9, 0xea, 0x00, 0xec, 0xed, 0x00, 0x00,
    0x00, 0x00, 0xf2, 0xf3, 0xf4, 0xf5, 0x00, 0x00,
    0x00, 0xf9, 0xfa, 0x00, 0x00, 0xfd, 0x00, 0x00,
    0x00, 0x00, 0xc5, 0xe5, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xd0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xce, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9d, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xb4, 0xbd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf,
    0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char uni2viscii_2[96] = {
    /*
    * From 0x1EA0 to 0x1EF9.
    */
    0x80, 0xd5, 0xc4, 0xe4, 0x84, 0xa4, 0x85, 0xa5,
    0x86, 0xa6, 0x06, 0xe7, 0x87, 0xa7, 0x81, 0xa1,
    0x82, 0xa2, 0x02, 0xc6, 0x05, 0xc7, 0x83, 0xa3,
    0x89, 0xa9, 0xcb, 0xeb, 0x88, 0xa8, 0x8a, 0xaa,
    0x8b, 0xab, 0x8c, 0xac, 0x8d, 0xad, 0x8e, 0xae,
    0x9b, 0xef, 0x98, 0xb8, 0x9a, 0xf7, 0x99, 0xf6,
    0x8f, 0xaf, 0x90, 0xb0, 0x91, 0xb1, 0x92, 0xb2,
    0x93, 0xb5, 0x95, 0xbe, 0x96, 0xb6, 0x97, 0xb7,
    0xb3, 0xde, 0x94, 0xfe, 0x9e, 0xf8, 0x9c, 0xfc,
    0xba, 0xd1, 0xbb, 0xd7, 0xbc, 0xd8, 0xff, 0xe6,
    0xb9, 0xf1, 0x9f, 0xcf, 0x1e, 0xdc, 0x14, 0xd6,
    0x19, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int uni_2_viscii (unsigned long in,  unsigned char *out)
{
    unsigned char c = 0;

    if (in < 0x0080 && (in >= 0x0020 || (0xBDEFFF9B & (1 << in)) ) ) {
        /* Bit mask from right to left, 0 means no code point:
         * 1011 1101 1110 1111 1111 1111 1001 1011
         */
        *out = in;
        return 1;
    } else if (in >= 0x00c0 && in < 0x01b8)
        c = uni2viscii_1[in-0x00c0];
    else if (in >= 0x1ea0 && in < 0x1f00)
        c = uni2viscii_2[in-0x1ea0];

    if (c != 0) {
        *out = c;
        return 1;
    }

    return 0;
}

#endif
