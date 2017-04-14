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
 * Copyright (c) 1996 by Sun Microsystems, Inc.
 */

#pragma ident  "@(#)ibm_thai_api.h 1.1      99/02/22 SMI"

#ifndef	_EUC2UTF_API_H_
#define	_EUC2UTF_API_H_

#include "common_thai.h"

extern hcode_type _eucTH_to_utf8(hcode_type);
extern hcode_type _874_to_838(hcode_type);
extern hcode_type _838_to_874(hcode_type);

#endif	/* _EUC2UTF_API_H_ */
