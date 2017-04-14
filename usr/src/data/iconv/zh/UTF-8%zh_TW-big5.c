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
#pragma ident   "@(#)UTF2_to_big5.c	1.16 01/08/16 SMI; ALE"

/*
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/isa_defs.h>
#include <errno.h>
#include "unicode_big5.h"	/* UTF8 to Big-5 mapping table */
#include "common_defs.h"

#define	MSB	0x80	/* most significant bit */
#define ONEBYTE	0xff	/* right most byte */

#define NON_ID_CHAR   '?' /* non-identified character */

typedef struct _icv_state {
	char	keepc[6];	/* maximum # byte of UTF8 code */
	short	ustate;
	int	_errno;		/* internal errno */
        boolean little_endian;
        boolean bom_written;
} _iconv_st;

enum _USTATE	{ U0, U1, U2, U3, U4, U5, U6, U7 };

static int get_big5_by_utf(uint_t, int *, unsigned long *);
static int utf8_to_big5(int, unsigned long, char *, size_t, int *);
static int binsearch(unsigned long, utf_big5[], int);


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

	st->ustate = U0;
	st->_errno = 0;
        st->little_endian = false;
        st->bom_written = false;
#if defined(UCS_2LE)
        st->little_endian = true;
        st->bom_written = true;
#endif
	return ((void *) st);
}


/*
 * Close; called from iconv_close()
 */
void
_icv_close(_iconv_st *st)
{
	if (!st)
		errno = EBADF;
	else
		free(st);
}


/*
 * Actual conversion; called from iconv()
 */
/*=========================================================
 *
 *       State Machine for interpreting UTF8 code
 *
 *=========================================================
 *                        2nd byte  3rd byte  4th byte
 *          +----->------->------>U5----->U6------------>U7
 *          |                                            |
 *          |     3 byte unicode                         |
 *          +----->------->-------+                      |
 *          |                     |                      |
 *          ^                     v                      |
 *          |  2 byte             U2 ---> U3             |
 *          |  unicode                    v              |
 * +------> U0 -------> U1                +-------->U4---+
 * ^  ascii |           |                           ^    |
 * |        |           +-------->--------->--------+    |
 * |        v                                            v
 * +----<---+-----<------------<------------<------------+
 *
 *=========================================================*/
size_t
_icv_iconv(_iconv_st *st, char **inbuf, size_t *inbytesleft,
				char **outbuf, size_t *outbytesleft)
{
	int		n, unidx;
	unsigned long	big5code;
	int		uconv_num = 0;
	int		utf8_len;
	uint_t          ucs;

#ifdef DEBUG
    fprintf(stderr, "==========     iconv(): UTF2 --> Big-5     ==========\n");
#endif
	if (st == NULL) {
		errno = EBADF;
		return ((size_t) -1);
	}

	if (inbuf == NULL || *inbuf == NULL) { /* Reset request. */
		st->ustate = U0;
		st->_errno = 0;
		return ((size_t) 0);
	}

	st->_errno = 0;		/* reset internal errno */
	errno = 0;		/* reset external errno */

	/* a state machine for interpreting UTF8 code */
	while (*inbytesleft > 0 && *outbytesleft > 0) {

	        uchar_t  first_byte;
		int	 uconv_num_internal = 0;

		switch (st->ustate) {
		case U0:		/* assuming ASCII in the beginning */
                       /*
                        * Code converion for UCS-2LE to support Samba
                        */
                        if (st->little_endian) {
                          st->ustate = U1;
                          st->keepc[0] = **inbuf;
                        }
			else if ((**inbuf & MSB) == 0) {	/* ASCII */
				**outbuf = **inbuf;
				(*outbuf)++;
				(*outbytesleft)--;
			} else {	/* Chinese character */
				if ((**inbuf & 0xe0) == 0xc0) {	/* 2 byte unicode 0xc2..0xdf */

				        /* invalid sequence if the first char is either 0xc0 or 0xc1 */
				        if ( number_of_bytes_in_utf8_char[((uchar_t)**inbuf)] == ICV_TYPE_ILLEGAL_CHAR )
				            st->_errno = errno = EILSEQ;
				        else {
					    st->ustate = U1;
					    st->keepc[0] = **inbuf;
					}
				} else if ((**inbuf & 0xf0) == 0xe0) {	/* 3 byte 0xe0..0xef */
					st->ustate = U2;
					st->keepc[0] = **inbuf;
				} else {
				        /* four bytes of UTF-8 sequences */
				        if ( number_of_bytes_in_utf8_char[((uchar_t)**inbuf)] == ICV_TYPE_ILLEGAL_CHAR )
					   st->_errno = errno = EILSEQ;
				        else {
					   st->ustate = U5;
					   st->keepc[0] = **inbuf;
					}
				}
			}
			break;
		case U1:		/* 2 byte unicode */
			if ((**inbuf & 0xc0) == MSB || st->little_endian) {
				utf8_len = 2;
				st->keepc[1] = **inbuf;

                                /*
                                 * Code conversion for UCS-2LE to support Samba
                                 */
                                if  (st->little_endian) {
                                  /*
                                   * It's ASCII
                                   */
                                  if (st->keepc[1] == 0 && (st->keepc[0] & 0x80) == 0) {
                                    *(*outbuf)++ = st->keepc[0];
				    (*outbytesleft)--;
                                    st->ustate = U0;
                                    break;
                                  }

                                  ucs = ((st->keepc[1] & 0xff) << 8) | (st->keepc[0] & 0xff);

                                } else
				  convert_utf8_to_ucs4(&st->keepc[0], utf8_len, &ucs);

				st->ustate = U4;
#ifdef DEBUG
    fprintf(stderr, "UTF8: %02x%02x   --> ",
	st->keepc[0]&ONEBYTE, st->keepc[1]&ONEBYTE);
#endif
				continue;	/* should not advance *inbuf */
			} else {
				st->_errno = errno = EILSEQ;
			}
			break;
		case U2:		/* 3 byte unicode - 2nd byte */

		        first_byte = st->keepc[0];

		        /* if the first byte is 0xed, it is illegal sequence if the second
			 * one is between 0xa0 and 0xbf because surrogate section is ill-formed
			 */
		        if (((uchar_t)**inbuf) < valid_min_2nd_byte[first_byte] ||
			    ((uchar_t)**inbuf) > valid_max_2nd_byte[first_byte] )
		            st->_errno = errno = EILSEQ;
			else {
				st->ustate = U3;
				st->keepc[1] = **inbuf;
			}
			break;
		case U3:		/* 3 byte unicode - 3rd byte */
			if ((**inbuf & 0xc0) == MSB) {
				st->ustate = U4;
				utf8_len = 3;
				st->keepc[2] = **inbuf;

				convert_utf8_to_ucs4(&st->keepc[0], utf8_len, &ucs);
#ifdef DEBUG
    fprintf(stderr, "UTF8: %02x%02x%02x --> ", st->keepc[0]&ONEBYTE,
		st->keepc[1]&ONEBYTE, **inbuf&ONEBYTE);
#endif
				continue;	/* should not advance *inbuf */
			} else {
				st->_errno = errno = EILSEQ;
			}
			break;
		case U4:

			n = get_big5_by_utf(ucs, &unidx, &big5code);

		        if ( n == -1 )
		         {  /* unicode is either 0xfffe or 0xffff */
			    st->_errno = errno = EILSEQ;
			    break;
			 }

/* comment the following lines out to ignore the non-Big5 characters
			if (n != 0) {	* legal unicode;illegal Big5 *
				st->_errno = errno = EILSEQ;
				break;
			}
*/

			n = utf8_to_big5(unidx, big5code,
					*outbuf, *outbytesleft, &uconv_num_internal);
			if (n > 0) {
				(*outbuf) += n;
				(*outbytesleft) -= n;

				uconv_num += uconv_num_internal;

				st->ustate = U0;
			} else {
				st->_errno = errno = E2BIG;
			}
			break;
		case U5:

		        first_byte = st->keepc[0];

		        /* if the first byte is 0xf0, it is illegal sequence if
			 * the second one is between 0x80 and 0x8f
			 * for Four-Byte UTF: U+10000..U+10FFFF
			 */
		        if (((uchar_t)**inbuf) < valid_min_2nd_byte[first_byte] ||
			    ((uchar_t)**inbuf) > valid_max_2nd_byte[first_byte] )
		            st->_errno = errno = EILSEQ;
		        else
		           {
			      st->ustate = U6;
			      st->keepc[1] = **inbuf;
		           }
		        break;
		case U6:
		        if ((**inbuf & 0xc0) == MSB) /* 0x80..0xbf */
		          {
			     st->ustate = U7;
			     st->keepc[2] = **inbuf;
			  }
		        else
		          st->_errno = errno = EILSEQ;
		        break;
		case U7:
		        if ((**inbuf & 0xc0) == MSB) /* 0x80..0xbf */
		          {  /* replace with double NON_ID_CHARs */

			     utf8_len = 4;
			     st->keepc[3] = **inbuf;

			     convert_utf8_to_ucs4(&st->keepc[0], utf8_len, &ucs);

			     st->ustate = U4;
			     continue;

#if 0
			     if ( *outbytesleft < 2 )
			        st->_errno = errno = E2BIG;
			     else
			       {
				  **outbuf = NON_ID_CHAR;
				  *(*outbuf+1) = NON_ID_CHAR;
				  (*outbytesleft) -= 2;

				  uconv_num++;

				  st->ustate = U0;
			       }
#endif
			  }
		        else
		          st->_errno = errno = EILSEQ;
		        break;
		default:			/* should never come here */
			st->_errno = errno = EILSEQ;
			st->ustate = U0;	/* reset state */
			break;
		}

		if (st->_errno) {
#ifdef DEBUG
    fprintf(stderr, "!!!!!\tst->_errno = %d\tst->ustate = %d\n",
		st->_errno, st->ustate);
#endif
			break;
		}

		(*inbuf)++;
		(*inbytesleft)--;

	}

        if (*inbytesleft == 0 && st->ustate != U0)
                errno = EINVAL;

	if (*inbytesleft > 0 && *outbytesleft == 0)
		errno = E2BIG;

	if (errno) {
		int num_reversed_bytes = 0;

		switch (st->ustate)
	        {
		 case U1:
		   num_reversed_bytes = 1;
		   break;
		 case U2:
		   num_reversed_bytes = 1;
		   break;
		 case U3:
		   num_reversed_bytes = 2;
		   break;
		 case U4:
		   num_reversed_bytes = utf8_len - 1;
		   break;
		 case U5:
		   num_reversed_bytes = 1;
		   break;
		 case U6:
		   num_reversed_bytes = 2;
		   break;
		 case U7:
		   num_reversed_bytes = 3;
		   break;
	        }

		/*
		 * if error, *inbuf points to the byte following the last byte
		 * successfully used in the conversion.
		 */
		*inbuf -= num_reversed_bytes;
		*inbytesleft += num_reversed_bytes;
		st->ustate = U0;
		return ((size_t) -1);
	}

	return uconv_num;
}

#ifdef UDC_SUPPORT
typedef struct _udc_sect {
  unsigned int start, end, count;
} UDC;

UDC udc[] = {
  { 0xFA40, 0xFEFE, 0x311 }
};

#define UDC_START_UNICODE 0xF0000

int
ifUDC(uint_t ucs, UDC *udc, unsigned long *big5code)
{
   int i;
   if ( ucs < UDC_START_UNICODE ) goto bail;

   ucs -= UDC_START_UNICODE;
   for (i=0; i < 1; ++i)
     {
       if ( ucs > udc[i].count - 1 )
	 ucs -= udc[i].count;
       else {
	 unsigned char c1, c2;

	 c1 = (uchar_t)(udc[i].start >> 8) + ucs/157;
	 c2 = ucs % 157 + 0x40;
	 if ( c2 > 0x7E ) c2 += (0xA1 - 0x7F);

	 *big5code = (c1 << 8) | c2;
	 return 2;
       }
     }

bail:
   return -1;
}
#endif
/*
 * Match Big-5 code by UTF8 code;
 * Return: = 0 - match from Unicode to Big-5 found
 *         = 1 - match from Unicode to Big-5 NOT found
 *         =-1 - illegal sequence
 *
 * Since binary search of the UTF8 to Big-5 table is necessary, might as well
 * return index and Big-5 code matching to the unicode.
 */
static int get_big5_by_utf(uint_t ucs, int *unidx, unsigned long *big5code)
{
	int ret;
        /* 0xfffe and 0xffff should not be allowed */
        if ( ucs == 0xFFFE || ucs == 0xFFFF ) return -1;

#ifdef UDC_SUPPORT
	if ((ret = ifUDC(ucs, udc, big5code)) != -1) {
	  *unidx = 1;
	  return ret;
	}
#endif

	*unidx = binsearch(ucs, utf_big5_tab, MAX_BIG5_NUM);
	if ((*unidx) >= 0)
		*big5code = utf_big5_tab[*unidx].big5code;
	else
		return(1);	/* match from UTF8 to Big-5 not found */
#ifdef DEBUG
    fprintf(stderr, "Unicode=%04x, idx=%5d, Big-5=%x ", ucs, *unidx, *big5code);
#endif

	return(0);
}


/*
 * ISO/IEC 10646 (Unicode) --> Big-5
 * Unicode --> UTF8 (FSS-UTF)
 *             (File System Safe Universal Character Set Transformation Format)
 * Return: > 0 - converted with enough space in output buffer
 *         = 0 - no space in outbuf
 */
static int utf8_to_big5(int unidx, unsigned long big5code, char *buf, size_t buflen, int *uconv_num)
{
	unsigned long	val;		/* Big-5 value */
	char		c1, c2, big5_str[3];

	if (buflen < 2) {
		errno = E2BIG;
		return(0);
	}

	if (unidx < 0) {	/* no match from UTF8 to Big-5 */
		*buf = *(buf+1) = NON_ID_CHAR;

		/* non-identical conversion */
		*uconv_num = 1;

	} else {
		val = big5code & 0xffff;
		c1 = (char) ((val & 0xff00) >> 8);
		c2 = (char) (val & 0xff);

	*buf = big5_str[0] = c1;
	*(buf+1) = big5_str[1] = c2;
	big5_str[2] = NULL;
	}

#ifdef DEBUG
    fprintf(stderr, "\t->%x %x<-\n", *buf, *(buf+1));
#endif

	return(2);
}


/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */
static int binsearch(unsigned long x, utf_big5 v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if (x < v[mid].unicode)
			high = mid - 1;
		else if (x > v[mid].unicode)
			low = mid + 1;
		else	/* found match */
			return mid;
	}
	return (-1);	/* no match */
}
