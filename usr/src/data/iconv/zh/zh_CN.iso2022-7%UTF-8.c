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
 * Copyright(c) 1995 Sun Microsystems, Inc.
 * All rights reserved.
 */
#pragma ident "@(#)zh_CN.iso2022-7_to_UTF-8.c	1.2 99/06/04"


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#ifdef DEBUG
#include <sys/fcntl.h>
#include <sys/stat.h>
#endif
#include <public_struc.h>
#include <gb2312_unicode.h>
#include <cns11643_unicode_CN.h>

#define MSB 	0x80	/* most significant bit */
#define MBYTE	0x8e	/* multi-byte (4 byte character) */
#define PMASK	0xa0	/* plane number mask */
#define ONEBYTE 0xff	/* right most byte */
#define MSB_OFF 0x7f	/* mask off MBS */

#define SI	0x0f		/* shift in */
#define SO	0x0e		/* shift out */
#define ESC 0x1b		/* escape */
#define SS2	0x4e		/* SS2 shift out */
#define SS3 0x4f		/* SS3 shift out */
#define UTF8_NON_ID_CHAR1	0xEF	/* non-identified character */
#define UTF8_NON_ID_CHAR2	0xBF	/* non-identified character */
#define UTF8_NON_ID_CHAR3	0xBD	/* non-identified character */

typedef struct _icv_state {
	char	_buf[10];
	size_t	_bufcont;
	char	_keepc[4];	/* maximum # byte of CNS11643 code */
	short	_gstate;		/* state machine id */
	short	_istate;		/* state for shift in/out */
	int		_plane;		/* plane number for Chinese character */
	int		_last_plane;	/* last charactor's plane # */
	int 	_errno;		/* internal errno */
} _iconv_st;

enum _GSTATE    { G0, G1, G2, G3, G4, G5, G6, G7, G8, G9, \
				  G10,G11,G12,G13,G14,G15,G16,G17,G18,G19, \
				  G20,G21,G22,G23,G24,G25,G26,G27,G28,G29 };

enum _ISTATE	{ IN, OUT };


int iso_to_utf8(_iconv_st * st, char* buf, size_t buflen);
int binsearch(unsigned long x, table_t v[], int n);
int flush_buf(_iconv_st * st, char ** outbuf, size_t * outbytesleft);

int flush_buf(_iconv_st * st, char ** outbuf, size_t * outbytesleft) {

	if (!st->_bufcont)
		return 0;
	if (st->_bufcont > *outbytesleft) {
		st->_errno = E2BIG;
		return -1;
	}
	strncpy(st->_buf, *outbuf, st->_bufcont);
	(*outbuf)+=(st->_bufcont);
	(*outbytesleft)-=(st->_bufcont);
	st->_bufcont = 0;
	return st->_bufcont;
}

/*
 * Open; called from iconv_open()
 */
void *
_icv_open()
{
	_iconv_st *st;

	if ((st = (_iconv_st *)malloc(sizeof(_iconv_st))) == NULL) {
		errno = ENOMEM;
		return ((void *) -1);
	}

	st->_gstate = G0;
	st->_istate = IN;
	st->_last_plane = st->_plane = -1;
	st->_errno = 0;
	st->_bufcont = 0;

	return ((void *) st);
}

/*
 * Close; called from iconv_close()
 */
void
_icv_close(_iconv_st *st)
{
	if (st == NULL)
		errno = EBADF;
	else
		free(st);
}

/*
 * Actual conversion; called from iconv()
 */
size_t _icv_iconv(_iconv_st *st, \
					char **inbuf, size_t *inbytesleft, \
					char **outbuf, size_t *outbytesleft) {
	int		n;
	char	c;

	if (st == NULL) {
		errno = EBADF;
		return ((size_t) -1);
	}

	if (inbuf == NULL || *inbuf == NULL) { /* Reset request. */
		st->_errno = 0;
		return ((size_t) 0);
	}

	errno = st->_errno = 0;	/* reset internal and external errno */

	/* a state machine for interpreting ISO 2022-7 code */
	while (*inbytesleft > 0 && *outbytesleft > 0) {
		switch (st->_gstate) {
			case G0:		/* assuming ASCII in the beginning */
				if (**inbuf == ESC) {
					st->_gstate = G1;
					st->_buf[st->_bufcont++] = ESC;
				} else {	/* real ASCII */
					**outbuf = **inbuf;
					(*outbuf)++;
					(*outbytesleft)--;
				}
				break;
			case G1:		/* got ESC, expecting $ */
				if (**inbuf == '$') {
					st->_gstate = G2;
					st->_buf[st->_bufcont++] = '$';
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_errno = 0;
					st->_istate = IN;
					continue;	/* don't advance inbuf */
				}
				break;
			case G2:		/* got $, expecting ) * or + */
				if (**inbuf == ')') {
					st->_gstate = G3;
				} else if (**inbuf == '*') {
					st->_gstate = G12;
					st->_plane = 2;
				} else if (**inbuf == '+') {
					st->_gstate = G19;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_errno = 0;
					st->_istate = IN;
					continue;	/* don't advance inbuf */
				}
				st->_buf[st->_bufcont++] = **inbuf;
				break;
			case G3:	/* got ) expecting A,G,H */
						/* H is for the bug of and zh_TW.BIG5 */
				if (**inbuf == 'A') {
					st->_plane = 0;
					st->_gstate = G4;
				} else if (**inbuf == 'G') {
					st->_plane = 1;
					st->_gstate = G8;
				} else if (**inbuf == 'H') {
					st->_plane = 2;
					st->_gstate = G8;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_errno = 0;
					st->_istate = IN;
					continue;
				}
				st->_buf[st->_bufcont++] = **inbuf;
				break;
		case G4:	/* ESC $ ) A got, and SO is expected */
				if (**inbuf == SO) {
					st->_gstate = G5;
					st->_istate = OUT;
					st->_bufcont = 0;
					st->_last_plane = st->_plane;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_errno = 0;
					st->_istate = IN;
					st->_plane = st->_last_plane;
					continue;
				}
				break;
		case G5:	/* SO (Shift Out) */
				if (**inbuf == SI) {
					st->_istate = IN;
				st->_gstate = G7;
					st->_last_plane = st->_plane;
				} else if (**inbuf == ESC) {
					st->_bufcont = 0;
					st->_gstate = G0;
					continue;
				} else {	/* Chinese Charactors */
					st->_keepc[0] = **inbuf;
					st->_gstate = G6;
				}
				break;
		case G6:	/* GB2312: 2nd Chinese character */
				st->_keepc[1] = **inbuf;
				n = iso_to_utf8(st, *outbuf, *outbytesleft);
				if (n > 0) {
					(*outbuf) += n;
					(*outbytesleft) -= n;
				} else {
					errno = st->_errno;
					return (size_t)-1;
				}
				st->_gstate = G5;
				break;
			case G7:	/* Shift in */
				if (**inbuf == SO) {
					st->_gstate = G5;
					st->_istate = OUT;
					st->_last_plane = st->_plane;
					st->_bufcont = 0;
				} else if (**inbuf == ESC) {
					st->_gstate = G0;
					continue;
				} else {
					**outbuf = **inbuf;
					(*outbuf)++;
					(*outbytesleft) --;
				}
				break;
		case G8:	/* BIG5: Chinese character */
				if (**inbuf == SO) {
					st->_istate = OUT;
					st->_gstate = G9;
					st->_bufcont = 0;
					st->_last_plane = st->_plane;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_errno = 0;
					st->_plane = st->_last_plane;
					st->_istate = IN;
					continue;
				}
				break;
		case G9:
				if (**inbuf == SI) {
					st->_istate = IN;
					st->_gstate = G11;
					st->_last_plane = st->_plane;
				} else if (**inbuf == ESC) {
					if (flush_buf(st, outbuf, outbytesleft) == -1) {
						errno = st->_errno;
						return (size_t)-1;
					}
					st->_gstate = G0;
					continue;
				} else {	/* Chinese Charactor */
					st->_keepc[0] = **inbuf;
					st->_gstate = G10;
				}
				break;
			case G10:
				st->_keepc[1] = **inbuf;
				n = iso_to_utf8(st, *outbuf, *outbytesleft);
				if (n > 0) {
					(*outbuf) += n;
					(*outbytesleft) -= n;
				} else {
					errno = st->_errno;
					return (size_t)-1;
				}
				st->_gstate = G9;
				break;
			case G11:
				st->_bufcont = 0;
				if (**inbuf == SO) {
					st->_istate = OUT;
					st->_gstate = G9;
				} else if (**inbuf == ESC) {
					st->_gstate = G0;
					continue;
				} else {
					**outbuf = **inbuf;
					(*outbuf)++;
					(*outbytesleft)--;
				}
				break;
			case G12:
				if (**inbuf == 'H') {
					st->_buf[st->_bufcont++] = 'H';
					st->_gstate = G13;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_istate = IN;
					st->_plane = st->_last_plane;
					st->_gstate = G0;
					continue;
				}
				break;
			case G13:
				if (**inbuf == ESC) {
					st->_buf[st->_bufcont++] = **inbuf;
					st->_gstate = G14;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_istate = IN;
					st->_plane = st->_last_plane;
					continue;
				}
				break;
			case G14:
				if (**inbuf == SS2) {
					st->_istate = OUT;
					st->_gstate = G15;
					st->_bufcont = 0;
					st->_last_plane = st->_plane = 2;
				} else if (**inbuf == '$') {
					st->_bufcont --;
					if (flush_buf(st, outbuf, outbytesleft) == -1) {
						errno = st->_errno;
						return (size_t)-1;
					} else {
						st->_gstate = G1;
						st->_plane = st->_last_plane;
						st->_istate = IN;
						continue;
					}
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_istate = IN;
					st->_plane = st->_last_plane;
					continue;
				}
				break;
			case G15:
				if (**inbuf == SI) {
					st->_gstate = G16;
					st->_istate = IN;
					st->_last_plane = st->_plane;
				} else if (**inbuf == ESC) {
					st->_bufcont = 0;
					st->_gstate = G0;
					continue;
				} else {
					st->_keepc[0] = **inbuf;
					st->_gstate = G18;
				}
				break;
			case G16:
				if (**inbuf == ESC) {
					st->_gstate = G17;
					st->_buf[st->_bufcont++] = ESC;
				} else {
					**outbuf = **inbuf;
					(*outbuf) ++;
					(*outbytesleft) --;
					st->_bufcont = 0;
				}
				break;
			case G17:
				if (**inbuf == '$') {
					st->_gstate = G1;
					st->_buf[st->_bufcont++] = '$';
					continue;
				} else if (**inbuf == SS2) {
					st->_bufcont = 0;
					st->_gstate = G15;
					st->_istate = OUT;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G16;
					st->_istate = IN;
				}
				break;
			case G18:
				st->_keepc[1] = **inbuf;
				st->_gstate = G15;
				if ((n = iso_to_utf8(st, \
									*outbuf, \
									*outbytesleft)) > 0) {
					(*outbuf)+=n;
					(*outbytesleft)-=n;
				} else {
					errno = st->_errno;
					return (size_t)-1;
				}
				break;
			case G19:	/* Plane #: 3 - 16 */
				c = **inbuf;
				if				(c == 'I' || \
								c == 'J' || \
								c == 'K' || \
								c == 'L' || \
								c == 'M' || \
								c == 'N' || \
								c == 'O' || \
								c == 'P' || \
								c == 'Q' || \
								c == 'R' || \
								c == 'S' || \
								c == 'T' || \
								c == 'U' || \
								c == 'V') {
					st->_plane = c - 'I' + 3;
					st->_gstate = G20;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_errno = 0;
					st->_istate = IN;
					st->_plane = st->_last_plane;
					continue;
				}
				st->_buf[st->_bufcont++] = c;
				break;
			case G20:
				if (**inbuf == ESC) {
					st->_buf[st->_bufcont++] = **inbuf;
					st->_gstate = G21;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_istate = IN;
					st->_last_plane = st->_plane;
					continue;
				}
				break;
			case G21:
				if (**inbuf == SS3) {
					st->_istate = OUT;
					st->_gstate = G22;
					st->_bufcont = 0;
				} else if (**inbuf == '$') {
					st->_bufcont --;
					if (flush_buf(st, outbuf, outbytesleft) == -1) {
						errno = st->_errno;
						return (size_t)-1;
					} else {
						st->_istate = IN;
						st->_last_plane = st->_plane;
						st->_gstate = G1;
						continue;
					}
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G0;
					st->_istate = IN;
					st->_last_plane = st->_plane;
					continue;
				}
				break;
			case G22:
				if (**inbuf == SI) {
					st->_istate = IN;
					st->_gstate = G24;
					st->_last_plane = st->_plane;
				} else {
					st->_keepc[0] = (char)MBYTE;
					st->_keepc[1] = (char)(PMASK + st->_plane);
					st->_keepc[2] = **inbuf;
					st->_gstate = G23;
				}
				break;
			case G23:
				st->_keepc[3] = **inbuf;
				if ((n = iso_to_utf8(st, \
									*outbuf, \
									*outbytesleft)) > 0) {
					(*outbuf)+=n;
					(*outbytesleft-=n);
				} else {
					errno = st->_errno;
					return (size_t)-1;
				}
				st->_gstate = G22;
				break;
			case G24:
				if (**inbuf == ESC) {
					st->_gstate = G25;
					st->_buf[st->_bufcont++] = ESC;
				} else {
					**outbuf = **inbuf;
					(*outbuf)++;
					(*outbytesleft)--;
					st->_bufcont = 0;
				}
				break;
			case G25:
				if (**inbuf == '$') {
					st->_gstate = G1;
					continue;
				} else if (**inbuf == SS3) {
					st->_gstate = G22;
					st->_bufcont = 0;
					st->_istate = OUT;
				} else if (flush_buf(st, outbuf, outbytesleft) == -1) {
					errno = st->_errno;
					return (size_t)-1;
				} else {
					st->_gstate = G24;
					st->_istate = IN;
				}
				break;
			default:			/* should never come here */
				st->_errno = errno = EILSEQ;
				st->_gstate = G0;	/* reset state */
				break;
		}	/* end of switch */

		(*inbuf)++;
		(*inbytesleft)--;

		if (st->_errno) {
			break;
		}
		if (errno)
			return((size_t)(-1));
	}

	if (*inbytesleft > 0 && *outbytesleft == 0) {
		errno = E2BIG;
		return((size_t)(-1));
	}
	return (size_t)((*inbytesleft));
}

/*
 * ISO 2022-7 code --> UTF-8 code
 * Return: > 0 - converted with enough space in output buffer
 *         = 0 - no space in outbuf
 */
int iso_to_utf8(_iconv_st * st, char* buf, size_t buflen) {
	unsigned long iso_val;
	int unidx;
	unsigned long unicode;
	switch (st->_plane) {
		case 0:
			unidx = (((st->_keepc[0] & 0xff) - 0x21) * 94) + \
					(st->_keepc[1] & 0xff) - 0x21;
			if (unidx < 0 || unidx >= GBMAX) {
				st->_errno = EILSEQ;
				return 0;
			}
			unicode = Unicode[unidx];
			break;

		case 1:
			iso_val = ((st->_keepc[0] & MSB_OFF) << 8) + \
					(st->_keepc[1] & MSB_OFF);
			unidx = binsearch(iso_val, cns1_utf_tab, MAX_CNS1_NUM);
			if (unidx >= 0) {
				unicode = cns1_utf_tab[unidx].value;
				break;
			}
		case 2:
			iso_val = ((st->_keepc[0] & MSB_OFF) << 8) + \
					(st->_keepc[1] & MSB_OFF);
			unidx = binsearch(iso_val, cns2_utf_tab, MAX_CNS2_NUM);
			if (unidx >= 0) {
				unicode = cns2_utf_tab[unidx].value;
				break;
			}
		default:
			st->_errno = EILSEQ;
			return 0;
	}
	if (unidx >=0) {
		if (unicode > 0x0080 && unicode <= 0x07ff) {
			if (buflen < 2) {
				st->_errno = E2BIG;
				return 0;
			}
			*buf = (char)((unicode >> 6) & 0x1f) | 0xc0;
			*(buf+1) = (char)(unicode & 0x3f) | 0x80;
			return 2;
		}
		if (unicode > 0x0800 && unicode <= 0xffff) {
			if (buflen < 3) {
				st->_errno = E2BIG;
				return(0);
			}
			*buf = (char)((unicode >> 12) & 0xf) | 0xe0;
			*(buf+1) = (char)((unicode >>6) & 0x3f) | 0x80;
			*(buf+2) = (char)(unicode & 0x3f) | 0x80;
			return 3;
		}
	}
	if (buflen < 3) {
		st->_errno = E2BIG;
		return 0;
	}

	*buf = (char)UTF8_NON_ID_CHAR1;
	*(buf + 2) = (char)UTF8_NON_ID_CHAR2;
	*(buf + 2) = (char)UTF8_NON_ID_CHAR3;
	return 3;
}

/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */
int binsearch(unsigned long x, table_t v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if (x < v[mid].key)
			high = mid - 1;
		else if (x > v[mid].key)
			low = mid + 1;
		else	/* found match */
			return mid;
	}
	return (-1);	/* no match */
}

#ifdef DEBUG
main(int argc, char ** argv) {
	char *inbuf, *outbuf, *in_tmp, *out_tmp;
	size_t inbytesleft, outbytesleft;
	int fd;
	int i;
	struct stat s;
	_iconv_st * st;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s input\n", argv[0]);
		exit(-1);
	}
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("open");
		exit(-2);
	}
	if (fstat(fd, &s) == -1) {
		perror("stat");
		exit(-3);
	}
	inbytesleft = outbytesleft = s.st_size;
	in_tmp = inbuf = (char *)malloc(inbytesleft);
	out_tmp = outbuf = (char *)malloc(outbytesleft);
	if (!inbuf || !outbuf) {
		perror("malloc");
		exit(-1);
	}
	if (read(fd, inbuf, inbytesleft) != inbytesleft) {
		perror("read");
		exit(-4);
	}
	for (i = 0; i < inbytesleft; i++)
		fprintf(stderr, "%x\t", *(inbuf+i));
	fprintf(stderr, "\n");
	st = (_iconv_st *)_icv_open();
	if (st == (_iconv_st *) -1) {
		perror("_icv_open");
		exit(-1);
	}
	if (_icv_iconv(st, \
			&inbuf, &inbytesleft, \
			&outbuf, &outbytesleft) == -1) {
		perror("icv_iconv");
		fprintf(stderr, "\ninbytesleft = %d\n", inbytesleft);
		exit(-2);
	}
	if (write(1, out_tmp, s.st_size - outbytesleft) == -1) {
		perror("write");
		exit(-1);
	}
	free(in_tmp);
	free(out_tmp);
	close(fd);
	_icv_close(st);
}
#endif
