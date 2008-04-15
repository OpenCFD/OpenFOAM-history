/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * $HEADER$
 *
 *	Very confusing software.  See offeeecial copyrights below.
 *	Modified for LAM by JRV
 *
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 *
 *	Function	- tstdio internal routines for input formatting
 */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "lam_config.h"

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)vfscanf.c	5.7 (Berkeley) 12/14/92";
#endif /* LIBC_SCCS and not lint */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <portable.h>
#include <tstdio.h>

#define FLOATING_POINT

#define	BUF		513	/* Maximum length of numeric string. */

/*
 * JRV: Added these includes, defines and declarations.
 */
extern double		strtod();
extern long		strtol();
#define hardway
#if !__STDC__
#define const
#endif

/*
 * Flags used during conversion.
 */
#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double; unimplemented */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x08	/* suppress assignment */
#define	POINTER		0x10	/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x20	/* do not skip blanks */

/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */
#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */

#define	DPTOK		0x100	/* (float) decimal point is still legal */
#define	EXPOK		0x200	/* (float) exponent (e+3, etc) still legal */

#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */

/*
 * Conversion types.
 */
#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* integer, i.e., strtol */
#define	CT_FLOAT	4	/* floating, i.e., strtod */

#define u_char unsigned char
#define u_long unsigned long

static u_char *__sccl();

/*
 * vfscanf
 */
int
lam_svfscanf(fp, fmt0, ap)
	register TFILE *fp;
	char const *fmt0;
	va_list ap;
{
	register u_char *fmt = (u_char *)fmt0;
	register int c;		/* character from format, or conversion */
	register size_t width;	/* field width, or 0 */
	register char *p;	/* points into all kinds of strings */
	register int n;		/* handy integer */
	register int flags;	/* flags as defined above */
	register char *p0;	/* saves original value of p when necessary */
	int nassigned;		/* number of fields assigned */
	int nread;		/* number of characters consumed from fp */
	int base;		/* base argument to strtol */
	long (*ccfn)();		/* conversion function (strtol) */
	char ccltab[256];	/* character class table for %[...] */
	char buf[BUF];		/* buffer for numeric conversions */
	SIGNED char cin;	/* JRV: character to use for getc */
	int	i;		/* JRV: my favorite loop index */

	/* `basefix' is used to avoid `if' tests in the integer scanner */
	static short basefix[17] =
		{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	nassigned = 0;
	nread = 0;
	base = 0;		/* XXX just to keep gcc happy */
	ccfn = NULL;		/* XXX just to keep gcc happy */


/*
 * JRV:  Added priming read.
 */
	cin = tfgetc(fp);

	for (;;) {
		c = *fmt++;
		if (c == 0) {
/* JRV: added next line... */
			tungetc(cin, fp);
			return (nassigned);
		}
		if (isspace(c)) {
			for (;;) {

/* JRV: replaced this junk	if (fp->_r <= 0 && __srefill(fp))
 * with the lines immed.		return (nassigned);
 * following			if (!isspace(*fp->_p))
 *					break;
 *				nread++, fp->_r--, fp->_p++;
 */

/*
 * JRV: Begin replacement
 */
				if (cin == EOF) {
					return(nassigned);
				}
				if ( !isspace(cin) ) {
					break;
				}
				nread++;
				cin = tfgetc(fp);
/*
 * JRV: End replacement
 */
			}
			continue;
		}
		if (c != '%')
			goto literal;
		width = 0;
		flags = 0;
		/*
		 * switch on the format.  continue if done;
		 * break once format type is derived.
		 */
again:		c = *fmt++;
		switch (c) {
		case '%':
literal:
/* JRV			if (fp->_r <= 0 && __srefill(fp))
 *				goto input_failure;
 *			if (*fp->_p != c)
 *				goto match_failure;
 *			fp->_r--, fp->_p++;
 *			nread++;
 *			continue;
 */

/*
 * JRV: Begin replacement
 */
			if (cin == EOF) {
				goto input_failure;
			}
			if (cin != c) {
				goto match_failure;
			}
			cin = tfgetc(fp);
			nread++;
			continue;
/*
 * JRV: End replacement
 */
		case '*':
			flags |= SUPPRESS;
			goto again;
		case 'l':
			flags |= LONG;
			goto again;
		case 'L':
			flags |= LONGDBL;
			goto again;
		case 'h':
			flags |= SHORT;
			goto again;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			width = width * 10 + c - '0';
			goto again;

		/*
		 * Conversions.
		 * Those marked `compat' are for 4.[123]BSD compatibility.
		 *
		 * (According to ANSI, E and X formats are supposed
		 * to the same as e and x.  Sorry about that.)
		 */
		case 'D':	/* compat */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'd':
			c = CT_INT;
			ccfn = strtol;
			base = 10;
			break;

		case 'i':
			c = CT_INT;
			ccfn = strtol;
			base = 0;
			break;

		case 'O':	/* compat */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'o':
			c = CT_INT;
			ccfn = strtol;
			base = 8;
			break;

		case 'u':
			c = CT_INT;
			ccfn = strtol;
			base = 10;
			break;

		case 'X':	/* compat   XXX */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'x':
			flags |= PFXOK;	/* enable 0x prefixing */
			c = CT_INT;
			ccfn = strtol;
			base = 16;
			break;

#ifdef FLOATING_POINT
		case 'E':	/* compat   XXX */
		case 'F':	/* compat */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'e': case 'f': case 'g':
			c = CT_FLOAT;
			break;
#endif

		case 's':
			c = CT_STRING;
			break;

		case '[':
			fmt = __sccl(ccltab, fmt);
			flags |= NOSKIP;
			c = CT_CCL;
			break;

		case 'c':
			flags |= NOSKIP;
			c = CT_CHAR;
			break;

		case 'p':	/* pointer format is like hex */
			flags |= POINTER | PFXOK;
			c = CT_INT;
			ccfn = strtol;
			base = 16;
			break;

		case 'n':
			if (flags & SUPPRESS)	/* ??? */
				continue;
			if (flags & SHORT)
				*va_arg(ap, short *) = nread;
			else if (flags & LONG)
				*va_arg(ap, long *) = nread;
			else
				*va_arg(ap, int *) = nread;
			continue;

		/*
		 * Disgusting backwards compatibility hacks.	XXX
		 */
		case '\0':	/* compat */
			return (EOF);

		default:	/* compat */
			if (isupper(c))
				flags |= LONG;
			c = CT_INT;
			ccfn = strtol;
			base = 10;
			break;
		}

		/*
		 * We have a conversion that requires input.
		 */
/* JRV
 *
 *		if (fp->_r <= 0 && __srefill(fp))
 *			goto input_failure;
 */

/*
 * JRV: Begin replacement
 */
		if (cin == EOF) {
			goto input_failure;
		}
/*
 * JRV: End replacement
 */
		/*
		 * Consume leading white space, except for formats
		 * that suppress this.
		 */
		if ((flags & NOSKIP) == 0) {
/* JRV
 *			while (isspace(*fp->_p)) {
 *				nread++;
 *				if (--fp->_r > 0)
 *					fp->_p++;
 *				else if (__srefill(fp))
 *					goto input_failure;
 * 			}
 */			/*
			 * Note that there is at least one character in
			 * the buffer, so conversions that do not set NOSKIP
			 * ca no longer result in an input failure.
			 */
/*
 * JRV: Begin replacement
 */
			while (isspace(cin)) {
				nread++;
				cin = tfgetc(fp);
				if (cin == EOF) {
					goto input_failure;
				}
			}
/*
 * JRV: End replacement
 */
		}

		/*
		 * Do the conversion.
		 */
		switch (c) {

		case CT_CHAR:
			/* scan arbitrary characters (sets NOSKIP) */
			if (width == 0)
				width = 1;
			if (flags & SUPPRESS) {
/* JRV				size_t sum = 0;
 *				for (;;) {
 *					if ((n = fp->_r) < width) {
 *						sum += n;
 *						width -= n;
 *						fp->_p += n;
 *						if (__srefill(fp)) {
 *							if (sum == 0)
 *							    goto input_failure;
 *							break;
 *						}
 *					} else {
 *						sum += width;
 *						fp->_r -= width;
 *						fp->_p += width;
 *						break;
 *					}
 *				}
 *				nread += sum;
 */

/*
 * JRV: Begin replacement
 */
				for(i=0; i<width; i++) {
					if ((cin = tfgetc(fp)) == EOF) {
						goto input_failure;
					}
				}
				nread += width;
/*
 * JRV: End replacement
 */
			} else {
				size_t r = tfread((void *)va_arg(ap, char *), 1,
				    width, fp);

				if (r == 0)
					goto input_failure;
				nread += r;
				nassigned++;
			}
			break;

		case CT_CCL:
			/* scan a (nonempty) character class (sets NOSKIP) */
			if (width == 0)
				width = ~0;	/* `infinity' */
			/* take only those things in the class */
			if (flags & SUPPRESS) {
				n = 0;

/* JRV				while (ccltab[*fp->_p]) {
 *					n++, fp->_r--, fp->_p++;
 *					if (--width == 0)
 *						break;
 *					if (fp->_r <= 0 && __srefill(fp)) {
 *						if (n == 0)
 *							goto input_failure;
 *						break;
 *					}
 *				}
 */

/*
 * JRV: Begin replacement
 */
				while (ccltab[cin]) {

					n++;
					cin = tfgetc(fp);

					if (--width == 0) {
						break;
					}

					if (cin == EOF) {
						if (n == 0) {
							goto input_failure;
						}
						break;
					}
				}
/*
 * JRV: End replacement
 */

				if (n == 0)
					goto match_failure;
			} else {
				p0 = p = va_arg(ap, char *);

/* JRV				while (ccltab[*fp->_p]) {
 *					fp->_r--;
 *					*p++ = *fp->_p++;
 *					if (--width == 0)
 *						break;
 *					if (fp->_r <= 0 && __srefill(fp)) {
 *						if (p == p0)
 *							goto input_failure;
 *						break;
 *					}
 *				}
 */

/*
 * JRV: Begin replacement
 */

				while (ccltab[cin]) {
					*p++ = tfgetc(fp);
					if (--width == 0) {
						break;
					}
					if (((SIGNED char) *p) == EOF) {
						if (p == p0) {
							goto input_failure;
						}
						break;
					}
				}
/*
 * JRV: End replacement
 */
				n = p - p0;
				if (n == 0)
					goto match_failure;
				*p = 0;
				nassigned++;
			}
			nread += n;
			break;

		case CT_STRING:
			/* like CCL, but zero-length string OK, & no NOSKIP */
			if (width == 0)
				width = ~0;
			if (flags & SUPPRESS) {
				n = 0;

/* JRV				while (!isspace(*fp->_p)) {
 *					n++, fp->_r--, fp->_p++;
 *					if (--width == 0)
 *						break;
 *					if (fp->_r <= 0 && __srefill(fp))
 *						break;
 *				}
 */

/*
 * JRV: Begin replacement
 */

				while (!isspace(cin)) {
					n++;
					cin = tfgetc(fp);
					if (--width == 0) {
						break;
					}
					if (cin == EOF) {
						break;
					}
				}
/*
 * JRV: End replacement
 */

				nread += n;
			} else {

				p0 = p = va_arg(ap, char *);

/* JRV				while (!isspace(*fp->_p)) {
 *					fp->_r--;
 *					*p++ = *fp->_p++;
 *					if (--width == 0)
 *						break;
 *					if (fp->_r <= 0 && __srefill(fp))
 *						break;
 *				}
 */

/*
 * JRV: Begin replacement
 */
				while(!isspace(cin)) {
					*p++ = cin;
					cin = tfgetc(fp);
					if (--width == 0) {
						break;
					}
					if (cin == EOF) {
						break;
					}
				}
/*
 * JRV: End replacement
 */
				*p = 0;
				nread += p - p0;
				nassigned++;
			}
			continue;

		case CT_INT:
			/* scan an integer as if by strtol */
#ifdef hardway
			if (width == 0 || width > sizeof(buf) - 1)
				width = sizeof(buf) - 1;
#else
			/* size_t is unsigned, hence this optimisation */
			if (--width > sizeof(buf) - 2)
				width = sizeof(buf) - 2;
			width++;
#endif
			flags |= SIGNOK | NDIGITS | NZDIGITS;
			for (p = buf; width; width--) {

/* JRV 				c = *fp->_p;
 */

/*
 * JRV: Begin replacement
 */
				c = cin;
/*
 * JRV: End replacement
 */


				/*
				 * Switch on the character; `goto ok'
				 * if we accept it as a part of number.
				 */
				switch (c) {

				/*
				 * The digit 0 is always legal, but is
				 * special.  For %i conversions, if no
				 * digits (zero or nonzero) have been
				 * scanned (only signs), we will have
				 * base==0.  In that case, we should set
				 * it to 8 and enable 0x prefixing.
				 * Also, if we have not scanned zero digits
				 * before this, do not turn off prefixing
				 * (someone else will turn it off if we
				 * have scanned any nonzero digits).
				 */
				case '0':
					if (base == 0) {
						base = 8;
						flags |= PFXOK;
					}
					if (flags & NZDIGITS)
					    flags &= ~(SIGNOK|NZDIGITS|NDIGITS);
					else
					    flags &= ~(SIGNOK|PFXOK|NDIGITS);
					goto ok;

				/* 1 through 7 always legal */
				case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					base = basefix[base];
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

				/* digits 8 and 9 ok iff decimal or hex */
				case '8': case '9':
					base = basefix[base];
					if (base <= 8)
						break;	/* not legal here */
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

				/* letters ok iff hex */
				case 'A': case 'B': case 'C':
				case 'D': case 'E': case 'F':
				case 'a': case 'b': case 'c':
				case 'd': case 'e': case 'f':
					/* no need to fix base here */
					if (base <= 10)
						break;	/* not legal here */
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

				/* sign ok only as first character */
				case '+': case '-':
					if (flags & SIGNOK) {
						flags &= ~SIGNOK;
						goto ok;
					}
					break;

				/* x ok iff flag still set & 2nd char */
				case 'x': case 'X':
					if (flags & PFXOK && p == buf + 1) {
						base = 16;	/* if %i */
						flags &= ~PFXOK;
						goto ok;
					}
					break;
				}

				/*
				 * If we got here, c is not a legal character
				 * for a number.  Stop accumulating digits.
				 */
				break;
		ok:
				/*
				 * c is legal: store it and look at the next.
				 */
				*p++ = c;

/* JRV				if (--fp->_r > 0)
 *					fp->_p++;
 *				else if (__srefill(fp))
 *					break;		
 */ 							/* EOF */


/*
 * JRV: Begin replacement
 */
				cin = tfgetc(fp);
				if (cin == EOF) {
					break;
				}
/*
 * JRV: End replacement
 */

			}
			/*
			 * If we had only a sign, it is no good; push
			 * back the sign.  If the number ends in `x',
			 * it was [sign] '0' 'x', so push back the x
			 * and treat it as [sign] '0'.
			 */
			if (flags & NDIGITS) {
				if (p > buf)
					(void) tungetc(*(u_char *)--p, fp);
				goto match_failure;
			}
			c = ((u_char *)p)[-1];
			if (c == 'x' || c == 'X') {
				--p;
				(void) tungetc(c, fp);
			}
			if ((flags & SUPPRESS) == 0) {
				u_long res;

				*p = 0;
				res = (*ccfn)(buf, (char **)NULL, base);
				if (flags & POINTER)
					*va_arg(ap, void **) = (void *)res;
				else if (flags & SHORT)
					*va_arg(ap, short *) = res;
				else if (flags & LONG)
					*va_arg(ap, long *) = res;
				else
					*va_arg(ap, int *) = res;
				nassigned++;
			}
			nread += p - buf;
			break;

#ifdef FLOATING_POINT
		case CT_FLOAT:
			/* scan a floating point number as if by strtod */
#ifdef hardway
			if (width == 0 || width > sizeof(buf) - 1)
				width = sizeof(buf) - 1;
#else
			/* size_t is unsigned, hence this optimisation */
			if (--width > sizeof(buf) - 2)
				width = sizeof(buf) - 2;
			width++;
#endif
			flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
			for (p = buf; width; width--) {

/* JRV				c = *fp->_p;
 */

/*
 * JRV: Begin replacement
 */
				c = cin;
/*
 * JRV: End replacement
 */

				/*
				 * This code mimicks the integer conversion
				 * code, but is much simpler.
				 */
				switch (c) {

				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
				case '8': case '9':
					flags &= ~(SIGNOK | NDIGITS);
					goto fok;

				case '+': case '-':
					if (flags & SIGNOK) {
						flags &= ~SIGNOK;
						goto fok;
					}
					break;
				case '.':
					if (flags & DPTOK) {
						flags &= ~(SIGNOK | DPTOK);
						goto fok;
					}
					break;
				case 'e': case 'E':
					/* no exponent without some digits */
					if ((flags&(NDIGITS|EXPOK)) == EXPOK) {
						flags =
						    (flags & ~(EXPOK|DPTOK)) |
						    SIGNOK | NDIGITS;
						goto fok;
					}
					break;
				}
				break;
		fok:
				*p++ = c;

/* JRV				if (--fp->_r > 0)
 *					fp->_p++;
 *				else if (__srefill(fp))
 *					break;
 */						/* EOF */

/*
 * JRV: End replacement
 */
				cin = tfgetc(fp);
				if (cin == EOF) {
					break;
				}
/*
 * JRV: End replacement
 */

			}
			/*
			 * If no digits, might be missing exponent digits
			 * (just give back the exponent) or might be missing
			 * regular digits, but had sign and/or decimal point.
			 */
			if (flags & NDIGITS) {
				if (flags & EXPOK) {
					/* no digits at all */
					while (p > buf)
						tungetc(*(u_char *)--p, fp);
					goto match_failure;
				}
				/* just a bad exponent (e and maybe sign) */
				c = *(u_char *)--p;
				if (c != 'e' && c != 'E') {
					(void) tungetc(c, fp);/* sign */
					c = *(u_char *)--p;
				}
				(void) tungetc(c, fp);
			}
			if ((flags & SUPPRESS) == 0) {
				double res;

				*p = 0;
				res = strtod(buf,(char **) NULL);
				if (flags & LONG)
					*va_arg(ap, double *) = res;
				else
					*va_arg(ap, float *) = res;
				nassigned++;
			}
			nread += p - buf;
			break;
#endif /* FLOATING_POINT */
		}
	}
input_failure:
	return (nassigned ? nassigned : -1);
match_failure:
	return (nassigned);
}

/*
 * Fill in the given table from the scanset at the given format
 * (just after `[').  Return a pointer to the character past the
 * closing `]'.  The table has a 1 wherever characters should be
 * considered part of the scanset.
 */
static u_char *
__sccl(tab, fmt)
	register char *tab;
	register u_char *fmt;
{
	register int c, n, v;

	/* first `clear' the whole table */
	c = *fmt++;		/* first char hat => negated scanset */
	if (c == '^') {
		v = 1;		/* default => accept */
		c = *fmt++;	/* get new first char */
	} else
		v = 0;		/* default => reject */
	/* should probably use memset here */
	for (n = 0; n < 256; n++)
		tab[n] = v;
	if (c == 0)
		return (fmt - 1);/* format ended before closing ] */

	/*
	 * Now set the entries corresponding to the actual scanset
	 * to the opposite of the above.
	 *
	 * The first character may be ']' (or '-') without being special;
	 * the last character may be '-'.
	 */
	v = 1 - v;
	for (;;) {
		tab[c] = v;		/* take character c */
doswitch:
		n = *fmt++;		/* and examine the next */
		switch (n) {

		case 0:			/* format ended too soon */
			return (fmt - 1);

		case '-':
			/*
			 * A scanset of the form
			 *	[01+-]
			 * is defined as `the digit 0, the digit 1,
			 * the character +, the character -', but
			 * the effect of a scanset such as
			 *	[a-zA-Z0-9]
			 * is implementation defined.  The V7 Unix
			 * scanf treats `a-z' as `the letters a through
			 * z', but treats `a-a' as `the letter a, the
			 * character -, and the letter a'.
			 *
			 * For compatibility, the `-' is not considerd
			 * to define a range if the character following
			 * it is either a close bracket (required by ANSI)
			 * or is not numerically greater than the character
			 * we just stored in the table (c).
			 */
			n = *fmt;
			if (n == ']' || n < c) {
				c = '-';
				break;	/* resume the for(;;) */
			}
			fmt++;
			do {		/* fill in the range */
				tab[++c] = v;
			} while (c < n);
#if 1	/* XXX another disgusting compatibility hack */
			/*
			 * Alas, the V7 Unix scanf also treats formats
			 * such as [a-c-e] as `the letters a through e'.
			 * This too is permitted by the standard....
			 */
			goto doswitch;
#else
			c = *fmt++;
			if (c == 0)
				return (fmt - 1);
			if (c == ']')
				return (fmt);
			break;
#endif

		case ']':		/* end of scanset */
			return (fmt);

		default:		/* just another character */
			c = n;
			break;
		}
	}
	/* NOTREACHED */
}

#if 0
vscanf(fmt, ap)
	const char *fmt;
	_VA_LIST_ ap;
{

	return (lam_svfscanf(tstdin, fmt, ap));
}

static int
eofread(cookie, buf, len)
	void *cookie;
	char *buf;
	int len;
{

	return (0);
}

vsscanf(str, fmt, ap)
	const char *str;
	const char *fmt;
	_VA_LIST_ ap;
{
	int ret;
	TFILE f;

	f._flags = __SRD;
	f._bf._base = f._p = (unsigned char *)str;
	f._bf._size = f._r = strlen(str);
	f._read = eofread;
	f._ub._base = NULL;
	f._lb._base = NULL;
	return (lam_svfscanf(&f, fmt, ap));
}
#endif
