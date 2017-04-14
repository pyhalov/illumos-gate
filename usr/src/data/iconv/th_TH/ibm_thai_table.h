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
 *  ICU License - ICU 1.8.1 and later
 *
 *  COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 1995-2005 International Business Machines Corporation and others
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * of the copyright holder.
 *
 * --------------------------------------------------------------------------
 * All trademarks and registered trademarks mentioned herein are the property
 * of their respective owners.
 */
/*
 * Copyright (c) 1999 by Sun Microsystems, Inc.
 */

#pragma   ident       "@(#)ibm_thai_table.h 1.2     99/02/20 SMI"

#ifndef _IBM_THAI_TABLE_H_
#define _IBM_THAI_TABLE_H_


#include "common_thai.h"
#include "common_def.h"

hcode_table ibm874_838_tbl[] = {
	{ 0x00, 0x00 },
	{ 0x01, 0x01 },
	{ 0x02, 0x02 },
	{ 0x03, 0x03 },
	{ 0x04, 0x37 },
	{ 0x05, 0x2d },
	{ 0x06, 0x2e },
	{ 0x07, 0x2f },
	{ 0x08, 0x16 },
	{ 0x09, 0x05 },
	{ 0x0a, 0x25 },
	{ 0x0b, 0x0b },
	{ 0x0c, 0x0c },
	{ 0x0d, 0x0d },
	{ 0x0e, 0x0e },
	{ 0x0f, 0x0f },
	{ 0x10, 0x10 },
	{ 0x11, 0x11 },
	{ 0x12, 0x12 },
	{ 0x13, 0x13 },
	{ 0x14, 0x3c },
	{ 0x15, 0x3d },
	{ 0x16, 0x32 },
	{ 0x17, 0x26 },
	{ 0x18, 0x18 },
	{ 0x19, 0x19 },
	{ 0x1a, 0x3f },
	{ 0x1b, 0x27 },
	{ 0x1c, 0x1c },
	{ 0x1d, 0x1d },
	{ 0x1e, 0x1e },
	{ 0x1f, 0x1f },
	{ 0x20, 0x40 },
	{ 0x21, 0x5a },
	{ 0x22, 0x7f },
	{ 0x23, 0x7b },
	{ 0x24, 0x5b },
	{ 0x25, 0x6c },
	{ 0x26, 0x50 },
	{ 0x27, 0x7d },
	{ 0x28, 0x4d },
	{ 0x29, 0x5d },
	{ 0x2a, 0x5c },
	{ 0x2b, 0x4e },
	{ 0x2c, 0x6b },
	{ 0x2d, 0x60 },
	{ 0x2e, 0x4b },
	{ 0x2f, 0x61 },
	{ 0x30, 0xf0 },
	{ 0x31, 0xf1 },
	{ 0x32, 0xf2 },
	{ 0x33, 0xf3 },
	{ 0x34, 0xf4 },
	{ 0x35, 0xf5 },
	{ 0x36, 0xf6 },
	{ 0x37, 0xf7 },
	{ 0x38, 0xf8 },
	{ 0x39, 0xf9 },
	{ 0x3a, 0x7a },
	{ 0x3b, 0x5e },
	{ 0x3c, 0x4c },
	{ 0x3d, 0x7e },
	{ 0x3e, 0x6e },
	{ 0x3f, 0x6f },
	{ 0x40, 0x7c },
	{ 0x41, 0xc1 },
	{ 0x42, 0xc2 },
	{ 0x43, 0xc3 },
	{ 0x44, 0xc4 },
	{ 0x45, 0xc5 },
	{ 0x46, 0xc6 },
	{ 0x47, 0xc7 },
	{ 0x48, 0xc8 },
	{ 0x49, 0xc9 },
	{ 0x4a, 0xd1 },
	{ 0x4b, 0xd2 },
	{ 0x4c, 0xd3 },
	{ 0x4d, 0xd4 },
	{ 0x4e, 0xd5 },
	{ 0x4f, 0xd6 },
	{ 0x50, 0xd7 },
	{ 0x51, 0xd8 },
	{ 0x52, 0xd9 },
	{ 0x53, 0xe2 },
	{ 0x54, 0xe3 },
	{ 0x55, 0xe4 },
	{ 0x56, 0xe5 },
	{ 0x57, 0xe6 },
	{ 0x58, 0xe7 },
	{ 0x59, 0xe8 },
	{ 0x5a, 0xe9 },
	{ 0x5b, 0x49 },
	{ 0x5c, 0xe0 },
	{ 0x5d, 0x59 },
	{ 0x5e, 0x69 },
	{ 0x5f, 0x6d },
	{ 0x60, 0x79 },
	{ 0x61, 0x81 },
	{ 0x62, 0x82 },
	{ 0x63, 0x83 },
	{ 0x64, 0x84 },
	{ 0x65, 0x85 },
	{ 0x66, 0x86 },
	{ 0x67, 0x87 },
	{ 0x68, 0x88 },
	{ 0x69, 0x89 },
	{ 0x6a, 0x91 },
	{ 0x6b, 0x92 },
	{ 0x6c, 0x93 },
	{ 0x6d, 0x94 },
	{ 0x6e, 0x95 },
	{ 0x6f, 0x96 },
	{ 0x70, 0x97 },
	{ 0x71, 0x98 },
	{ 0x72, 0x99 },
	{ 0x73, 0xa2 },
	{ 0x74, 0xa3 },
	{ 0x75, 0xa4 },
	{ 0x76, 0xa5 },
	{ 0x77, 0xa6 },
	{ 0x78, 0xa7 },
	{ 0x79, 0xa8 },
	{ 0x7a, 0xa9 },
	{ 0x7b, 0xc0 },
	{ 0x7c, 0x4f },
	{ 0x7d, 0xd0 },
	{ 0x7e, 0xa1 },
	{ 0x7f, 0x07 },
	{ 0x80, 0x20 },
	{ 0x81, 0x21 },
	{ 0x82, 0x22 },
	{ 0x83, 0x23 },
	{ 0x84, 0x24 },
	{ 0x85, 0x15 },
	{ 0x86, 0x06 },
	{ 0x87, 0x17 },
	{ 0x88, 0x28 },
	{ 0x89, 0x29 },
	{ 0x8a, 0x2a },
	{ 0x8b, 0x2b },
	{ 0x8c, 0x2c },
	{ 0x8d, 0x09 },
	{ 0x8e, 0x0a },
	{ 0x8f, 0x1b },
	{ 0x90, 0x30 },
	{ 0x91, 0x31 },
	{ 0x92, 0x1a },
	{ 0x93, 0x33 },
	{ 0x94, 0x34 },
	{ 0x95, 0x35 },
	{ 0x96, 0x36 },
	{ 0x97, 0x08 },
	{ 0x98, 0x38 },
	{ 0x99, 0x39 },
	{ 0x9a, 0x3a },
	{ 0x9b, 0x3b },
	{ 0x9c, 0x04 },
	{ 0x9d, 0x14 },
	{ 0x9e, 0x3f },
	{ 0x9f, 0x41 },
	{ 0xa0, 0x51 },
	{ 0xa1, 0x42 },
	{ 0xa2, 0x43 },
	{ 0xa3, 0x44 },
	{ 0xa4, 0x45 },
	{ 0xa5, 0x46 },
	{ 0xa6, 0x47 },
	{ 0xa7, 0x48 },
	{ 0xa8, 0x52 },
	{ 0xa9, 0x53 },
	{ 0xaa, 0x54 },
	{ 0xab, 0x55 },
	{ 0xac, 0x56 },
	{ 0xad, 0x57 },
	{ 0xae, 0x58 },
	{ 0xaf, 0x62 },
	{ 0xb0, 0x63 },
	{ 0xb1, 0x64 },
	{ 0xb2, 0x65 },
	{ 0xb3, 0x66 },
	{ 0xb4, 0x67 },
	{ 0xb5, 0x68 },
	{ 0xb6, 0x72 },
	{ 0xb7, 0x73 },
	{ 0xb8, 0x74 },
	{ 0xb9, 0x75 },
	{ 0xba, 0x76 },
	{ 0xbb, 0x77 },
	{ 0xbc, 0x78 },
	{ 0xbd, 0x8a },
	{ 0xbe, 0x8b },
	{ 0xbf, 0x8c },
	{ 0xc0, 0x8d },
	{ 0xc1, 0x8e },
	{ 0xc2, 0x8f },
	{ 0xc3, 0x9a },
	{ 0xc4, 0x9b },
	{ 0xc5, 0x9c },
	{ 0xc6, 0x9d },
	{ 0xc7, 0x9e },
	{ 0xc8, 0x9f },
	{ 0xc9, 0xaa },
	{ 0xca, 0xab },
	{ 0xcb, 0xac },
	{ 0xcc, 0xad },
	{ 0xcd, 0xae },
	{ 0xce, 0xaf },
	{ 0xcf, 0xba },
	{ 0xd0, 0xbb },
	{ 0xd1, 0xbc },
	{ 0xd2, 0xbd },
	{ 0xd3, 0xbe },
	{ 0xd4, 0xbf },
	{ 0xd5, 0xcb },
	{ 0xd6, 0xcc },
	{ 0xd7, 0xcd },
	{ 0xd8, 0xce },
	{ 0xd9, 0xcf },
	{ 0xda, 0xda },
	{ 0xdb, 0xca },
	{ 0xdc, 0xe1 },
	{ 0xdd, 0xfd },
	{ 0xde, 0xfe },
	{ 0xdf, 0x70 },
	{ 0xe0, 0xdb },
	{ 0xe1, 0xdc },
	{ 0xe2, 0xdd },
	{ 0xe3, 0xde },
	{ 0xe4, 0xdf },
	{ 0xe5, 0xea },
	{ 0xe6, 0xeb },
	{ 0xe7, 0xec },
	{ 0xe8, 0xed },
	{ 0xe9, 0xee },
	{ 0xea, 0xef },
	{ 0xeb, 0xfa },
	{ 0xec, 0xfb },
	{ 0xed, 0xfc },
	{ 0xee, 0x71 },
	{ 0xef, 0x80 },
	{ 0xf0, 0xb0 },
	{ 0xf1, 0xb1 },
	{ 0xf2, 0xb2 },
	{ 0xf3, 0xb3 },
	{ 0xf4, 0xb4 },
	{ 0xf5, 0xb5 },
	{ 0xf6, 0xb6 },
	{ 0xf7, 0xb7 },
	{ 0xf8, 0xb8 },
	{ 0xf9, 0xb9 },
	{ 0xfa, 0x90 },
	{ 0xfb, 0xa0 },
	{ 0xfc, 0x4a },
	{ 0xfd, 0x5f },
	{ 0xfe, 0x6a },
	{ 0xff, 0xff }
};

hcode_table ibm838_874_tbl[] = {
	{ 0x00, 0x00 },
	{ 0x01, 0x01 },
	{ 0x02, 0x02 },
	{ 0x03, 0x03 },
	{ 0x04, 0x9c },
	{ 0x05, 0x09 },
	{ 0x06, 0x86 },
	{ 0x07, 0x7f },
	{ 0x08, 0x97 },
	{ 0x09, 0x8d },
	{ 0x0a, 0x8e },
	{ 0x0b, 0x0b },
	{ 0x0c, 0x0c },
	{ 0x0d, 0x0d },
	{ 0x0e, 0x0e },
	{ 0x0f, 0x0f },
	{ 0x10, 0x10 },
	{ 0x11, 0x11 },
	{ 0x12, 0x12 },
	{ 0x13, 0x13 },
	{ 0x14, 0x9d },
	{ 0x15, 0x85 },
	{ 0x16, 0x08 },
	{ 0x17, 0x87 },
	{ 0x18, 0x18 },
	{ 0x19, 0x19 },
	{ 0x1a, 0x92 },
	{ 0x1b, 0x8f },
	{ 0x1c, 0x1c },
	{ 0x1d, 0x1d },
	{ 0x1e, 0x1e },
	{ 0x1f, 0x1f },
	{ 0x20, 0x80 },
	{ 0x21, 0x81 },
	{ 0x22, 0x82 },
	{ 0x23, 0x83 },
	{ 0x24, 0x84 },
	{ 0x25, 0x0a },
	{ 0x26, 0x17 },
	{ 0x27, 0x1b },
	{ 0x28, 0x88 },
	{ 0x29, 0x89 },
	{ 0x2a, 0x8a },
	{ 0x2b, 0x8b },
	{ 0x2c, 0x8c },
	{ 0x2d, 0x05 },
	{ 0x2e, 0x06 },
	{ 0x2f, 0x07 },
	{ 0x30, 0x90 },
	{ 0x31, 0x91 },
	{ 0x32, 0x16 },
	{ 0x33, 0x93 },
	{ 0x34, 0x94 },
	{ 0x35, 0x95 },
	{ 0x36, 0x96 },
	{ 0x37, 0x04 },
	{ 0x38, 0x98 },
	{ 0x39, 0x99 },
	{ 0x3a, 0x9a },
	{ 0x3b, 0x9b },
	{ 0x3c, 0x14 },
	{ 0x3d, 0x15 },
	{ 0x3e, 0x1a },
	{ 0x3f, 0x9e },
	{ 0x40, 0x20 },
	{ 0x41, 0x9f },
	{ 0x42, 0xa1 },
	{ 0x43, 0xa2 },
	{ 0x44, 0xa3 },
	{ 0x45, 0xa4 },
	{ 0x46, 0xa5 },
	{ 0x47, 0xa6 },
	{ 0x48, 0xa7 },
	{ 0x49, 0x5b },
	{ 0x4a, 0xfc },
	{ 0x4b, 0x2e },
	{ 0x4c, 0x3c },
	{ 0x4d, 0x28 },
	{ 0x4e, 0x2b },
	{ 0x4f, 0x7c },
	{ 0x50, 0x26 },
	{ 0x51, 0xa0 },
	{ 0x52, 0xa8 },
	{ 0x53, 0xa9 },
	{ 0x54, 0xaa },
	{ 0x55, 0xab },
	{ 0x56, 0xac },
	{ 0x57, 0xad },
	{ 0x58, 0xae },
	{ 0x59, 0x5d },
	{ 0x5a, 0x21 },
	{ 0x5b, 0x24 },
	{ 0x5c, 0x2a },
	{ 0x5d, 0x29 },
	{ 0x5e, 0x3b },
	{ 0x5f, 0xfd },
	{ 0x60, 0x2d },
	{ 0x61, 0x2f },
	{ 0x62, 0xaf },
	{ 0x63, 0xb0 },
	{ 0x64, 0xb1 },
	{ 0x65, 0xb2 },
	{ 0x66, 0xb3 },
	{ 0x67, 0xb4 },
	{ 0x68, 0xb5 },
	{ 0x69, 0x5e },
	{ 0x6a, 0xfe },
	{ 0x6b, 0x2c },
	{ 0x6c, 0x25 },
	{ 0x6d, 0x5f },
	{ 0x6e, 0x3e },
	{ 0x6f, 0x3f },
	{ 0x70, 0xdf },
	{ 0x71, 0xee },
	{ 0x72, 0xb6 },
	{ 0x73, 0xb7 },
	{ 0x74, 0xb8 },
	{ 0x75, 0xb9 },
	{ 0x76, 0xba },
	{ 0x77, 0xbb },
	{ 0x78, 0xbc },
	{ 0x79, 0x60 },
	{ 0x7a, 0x3a },
	{ 0x7b, 0x23 },
	{ 0x7c, 0x40 },
	{ 0x7d, 0x27 },
	{ 0x7e, 0x3d },
	{ 0x7f, 0x22 },
	{ 0x80, 0xef },
	{ 0x81, 0x61 },
	{ 0x82, 0x62 },
	{ 0x83, 0x63 },
	{ 0x84, 0x64 },
	{ 0x85, 0x65 },
	{ 0x86, 0x66 },
	{ 0x87, 0x67 },
	{ 0x88, 0x68 },
	{ 0x89, 0x69 },
	{ 0x8a, 0xbd },
	{ 0x8b, 0xbe },
	{ 0x8c, 0xbf },
	{ 0x8d, 0xc0 },
	{ 0x8e, 0xc1 },
	{ 0x8f, 0xc2 },
	{ 0x90, 0xfa },
	{ 0x91, 0x6a },
	{ 0x92, 0x6b },
	{ 0x93, 0x6c },
	{ 0x94, 0x6d },
	{ 0x95, 0x6e },
	{ 0x96, 0x6f },
	{ 0x97, 0x70 },
	{ 0x98, 0x71 },
	{ 0x99, 0x72 },
	{ 0x9a, 0xc3 },
	{ 0x9b, 0xc4 },
	{ 0x9c, 0xc5 },
	{ 0x9d, 0xc6 },
	{ 0x9e, 0xc7 },
	{ 0x9f, 0xc8 },
	{ 0xa0, 0xfb },
	{ 0xa1, 0x7e },
	{ 0xa2, 0x73 },
	{ 0xa3, 0x74 },
	{ 0xa4, 0x75 },
	{ 0xa5, 0x76 },
	{ 0xa6, 0x77 },
	{ 0xa7, 0x78 },
	{ 0xa8, 0x79 },
	{ 0xa9, 0x7a },
	{ 0xaa, 0xc9 },
	{ 0xab, 0xca },
	{ 0xac, 0xcb },
	{ 0xad, 0xcc },
	{ 0xae, 0xcd },
	{ 0xaf, 0xce },
	{ 0xb0, 0xf0 },
	{ 0xb1, 0xf1 },
	{ 0xb2, 0xf2 },
	{ 0xb3, 0xf3 },
	{ 0xb4, 0xf4 },
	{ 0xb5, 0xf5 },
	{ 0xb6, 0xf6 },
	{ 0xb7, 0xf7 },
	{ 0xb8, 0xf8 },
	{ 0xb9, 0xf9 },
	{ 0xba, 0xcf },
	{ 0xbb, 0xd0 },
	{ 0xbc, 0xd1 },
	{ 0xbd, 0xd2 },
	{ 0xbe, 0xd3 },
	{ 0xbf, 0xd4 },
	{ 0xc0, 0x7b },
	{ 0xc1, 0x41 },
	{ 0xc2, 0x42 },
	{ 0xc3, 0x43 },
	{ 0xc4, 0x44 },
	{ 0xc5, 0x45 },
	{ 0xc6, 0x46 },
	{ 0xc7, 0x47 },
	{ 0xc8, 0x48 },
	{ 0xc9, 0x49 },
	{ 0xca, 0xdb },
	{ 0xcb, 0xd5 },
	{ 0xcc, 0xd6 },
	{ 0xcd, 0xd7 },
	{ 0xce, 0xd8 },
	{ 0xcf, 0xd9 },
	{ 0xd0, 0x7d },
	{ 0xd1, 0x4a },
	{ 0xd2, 0x4b },
	{ 0xd3, 0x4c },
	{ 0xd4, 0x4d },
	{ 0xd5, 0x4e },
	{ 0xd6, 0x4f },
	{ 0xd7, 0x50 },
	{ 0xd8, 0x51 },
	{ 0xd9, 0x52 },
	{ 0xda, 0xda },
	{ 0xdb, 0xe0 },
	{ 0xdc, 0xe1 },
	{ 0xdd, 0xe2 },
	{ 0xde, 0xe3 },
	{ 0xdf, 0xe4 },
	{ 0xe0, 0x5c },
	{ 0xe1, 0xdc },
	{ 0xe2, 0x53 },
	{ 0xe3, 0x54 },
	{ 0xe4, 0x55 },
	{ 0xe5, 0x56 },
	{ 0xe6, 0x57 },
	{ 0xe7, 0x58 },
	{ 0xe8, 0x59 },
	{ 0xe9, 0x5a },
	{ 0xea, 0xe5 },
	{ 0xeb, 0xe6 },
	{ 0xec, 0xe7 },
	{ 0xed, 0xe8 },
	{ 0xee, 0xe9 },
	{ 0xef, 0xea },
	{ 0xf0, 0x30 },
	{ 0xf1, 0x31 },
	{ 0xf2, 0x32 },
	{ 0xf3, 0x33 },
	{ 0xf4, 0x34 },
	{ 0xf5, 0x35 },
	{ 0xf6, 0x36 },
	{ 0xf7, 0x37 },
	{ 0xf8, 0x38 },
	{ 0xf9, 0x39 },
	{ 0xfa, 0xeb },
	{ 0xfb, 0xec },
	{ 0xfc, 0xed },
	{ 0xfd, 0xdd },
	{ 0xfe, 0xde },
	{ 0xff, 0xff }
};

#endif
