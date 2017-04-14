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
 * Copyright(c) 2001 Sun Microsystems, Inc.
 * All rights reserved.
 */
#ident "@(#)ea-iscii.h       1.1 01/11/13 SMI"

#ifndef _EA_ISCII_H_
#define _EA_ISCII_H_

#define uchar                unsigned char

#define NUKTA                0xFD
#define MATRA                0xFE
#define COMBINED_MATRA_NUKTA 0xFF

#define LEADING_BYTE         'x'
#define NUKTA_VALUE          'x'
#define FIRST_VOWEL          0x43


typedef struct _entry {
    uchar ea_iscii;
    uchar iscii;
    int   count;
} Entry;

Entry  eaiscii_isc_tbl[] = {
    { 0x41, 0xA1, 2 },
    { 0x43, 0xA4, 1 },
    { 0x44, 0xB3, 4 },
    { 0x48, 0xB8, 4 },
    { 0x4C, 0xBD, 10 },
    { 0x56, 0xC8, 5 },
    { 0x61, 0xCD, 1 },
    { 0x62, 0xCF, 4 },
    { 0x66, 0xD4, 5 },
    { 0x6B, 0xDA, 9 },
    { 0x74, 0xE4, 3 },
    { 0x77, 0xE8, 3 },
    { 0x7A, 0xEF, 1 }
};

Entry isc_eaiscii_tbl[] = {
    { 0x41, 0xA1, 2 },
    { 0x42, 0xA3, NUKTA },
    { 0x43, 0xA4, 1 },
    { 0x6B, 0xA5, MATRA },
    { 0x6C, 0xA6, MATRA },
    { 0x6D, 0xA7, MATRA },
    { 0x6E, 0xA8, MATRA },
    { 0x6F, 0xA9, MATRA },
    { 0x70, 0xAA, MATRA },
    { 0x71, 0xAB, MATRA },
    { 0x72, 0xAC, MATRA },
    { 0x73, 0xAD, MATRA },
    { 0x73, 0xAE, COMBINED_MATRA_NUKTA },
    { 0x74, 0xAF, MATRA },
    { 0x75, 0xB0, MATRA },
    { 0x76, 0xB1, MATRA },
    { 0x76, 0xB2, COMBINED_MATRA_NUKTA },
    { 0x44, 0xB3, 4 },
    { 0x47, 0xB7, NUKTA },
    { 0x48, 0xB8, 4 },
    { 0x4B, 0xBC, NUKTA },
    { 0x4C, 0xBD, 10 },
    { 0x55, 0xC7, NUKTA },
    { 0x56, 0xC8, 5 },
    { 0x61, 0xCD, 1 },
    { 0x61, 0xCE, NUKTA },
    { 0x62, 0xCF, 4 },
    { 0x65, 0xD3, NUKTA },
    { 0x66, 0xD4, 5 },
    { 0x6A, 0xD9, NUKTA },
    { 0x6B, 0xDA, 9 },
    { 0x73, 0xE3, NUKTA },
    { 0x74, 0xE4, 3 },
    { 0x76, 0xE7, NUKTA },
    { 0x77, 0xE8, 3 },
    { 0x7A, 0xEF, 1 }
};

uchar EAISCII_nukta_type [] = {
    0x0, 0x0,  0xA3, 0x0,  0x0, 0x0,  0x0,  0xB7, /* 40 - 47 */
    0x0, 0x0,  0x0,  0xBC, 0x0, 0x0,  0x0,  0x0,  /* 48 - 4F */
    0x0, 0x0,  0x0,  0x0,  0x0, 0xC7, 0x0,  0x0,  /* 50 - 57 */
    0x0, 0x0,  0x0,  0x0,  0x0, 0x0,  0x0,  0x0,  /* 58 - 5F */
    0x0, 0xCE, 0x0,  0x0,  0x0, 0x0,  0x0,  0x0,  /* 60 - 67 */
    0x0, 0x0,  0xD9, 0x0,  0x0, 0x0,  0x0,  0x0,  /* 68 - 6F */
    0x0, 0x0,  0x0,  0xE3, 0x0, 0x0,  0xE7, 0x0,  /* 70 - 77 */
    0x0, 0x0,  0x0,  0x0,  0x0, 0x0,  0x0,  0x0   /* 78 - 7F */
};

uchar EAISCII_vowel_type [] = {
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, /* 40 - 47 */
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, /* 48 - 4F */
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, /* 50 - 57 */
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, /* 58 - 5F */
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, /* 60 - 67 */
    0x0,  0x0,  0x0,  0xA5, 0xA6, 0xA7, 0xA8, 0xA9,/* 68 - 6F */
    0xAA, 0xAB, 0xAC, 0xAD, 0xAF, 0xB0, 0xB1, 0x0, /* 70 - 77 */
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0  /* 78 - 7F */
};

#endif
