/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 * 
 *	Function:	- tstdio output formatting rountines
 */
 
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <string.h>			/* for size_t strlen() */
#include <stddef.h>			/* for size_t */
#include <ctype.h>			/* for isdigit */

#include <portable.h>			/* for isdigit */
#include <tstdio.h>			/* for int tputc() */

/*
 * useful defines
 */
#define MAXLENGTH	15
#define RESET_FLAGS	width		= 0; \
			precision	= 0; \
			dotf		= 0; \
			percentf	= 0; \
			dashf		= 0; \
			plusf		= 0; \
			spacef		= 0; \
			zerof		= 0; \
			hashf		= 0; \
			wstarf		= 0; \
			pstarf		= 0; \
			dispmod		= '\0' ;
/*
 * global variables
 */
char			converted_arg[MAXLENGTH];

/*
 * static functions
 */
static int		output();
static int		getnum();
static int		charformat();
static void		round_up();
static void		norm();
static int		fegformat();
static int		nextdigit();
static int		trailzscan();
static int
intconv(unsigned long		arg,
        char			type,
        int			*minusfp);
static int
intformat(unsigned long		ularg,
          int			width,
          int			precision,
          int			zerof,
          int			plusf,
          int			spacef,
          int			dashf,
          int			hashf,
          char			type,
          TFILE			*fp,
          char			**storepp);

/*
 *	_fmt()
 *
 *	Function:	- performs stdio output formatting
 *			- outputs to either a file or a buffer
 *			- differs from ANSI printf() in two respects:
 *				- hash flag has no effect on types f,e,E,g,G
 *				- pointers are printed in a min 8-character
 *				  field with filling zeros
 *			- called by printf, fprintf, sprintf, vprintf,
 *			  vfprintf and vsprintf
 *	Accepts:	- file pointer to destination file
 *			- a character pointer to destination buffer
 *			- the format string
 *			- va_list of items to print
 *	Returns:	- number of printed characters, or EOF if error
 */
int
_fmt(fp, storep, format, arglist)

TFILE			*fp;
char			*storep;
CONST char		*format;
va_list			arglist;

{
	char		c;
	char		dispmod;	/* h,l,L */
	CONST char	*index;		/* to format string */
	char		*stringp;	/* to string argument */
	double		darg;
	int		count 	= 0;	/* number of printed characters */
	int		dashf;		/* left-justification */
	int		dotf;		/* explicit precision expression */
	int		errorf	= 0;
	int		hashf;		/* hex and octal prefixes */
/*
 *	hashf HAS NO EFFECT ON f,e,E,g,G TYPES.
 */
	int		iarg;
	int		percentf;	/* inside format descriptor */	
	int		plusf;		/* sign display */
	int		precision;
	int		pstarf;		/* precision star flag */
	int		returnval;	/* return value of format functions */
	int		spacef;		/* -/<space> sign display */
	int		width;
	int		wstarf;		/* width star flag */
	int		zerof;		/* zero-fill */
	long		larg;
	short		sarg;
	unsigned	uarg;
	unsigned long	ularg;
	unsigned short	usarg;
	void		*parg;

	if ((fp == 0) && (storep == 0)) {
		return(EOF);
	}

	index = format;
	RESET_FLAGS;
/*
 *	Main loop through format string.
 */
	while ((c = *index) != '\0') {
		if (!percentf) {
			if (c != '%') {
				if (output(c, fp, &storep) == EOF) {
					errorf = 1;
					break;
				}
				else {
					++ count;
				}
			}
			else {
				percentf = 1;	/* start of format descrip */
			}
			++ index;
		}
		else {
/*
 * Handle flags
 */
			while ((c == '-') || (c == '+') || (c == ' ')
					|| (c == '0') || (c == '#')) {
				switch (c = *index)
				{
				case '-':
					dashf = 1;
					break;
				case '+':
					plusf = 1;
					break;
				case ' ':
					spacef = 1;
					break;
				case '0':
					zerof = 1;
					break;
				case '#':
					hashf = 1;
					break;
				default:
					-- index;
					break;
				}
				++ index;
			}
/*
 *	Read fields (width.precision) and size display-mode (h,l,L).
 */
			width = getnum(&index, &wstarf);
			if (wstarf) {
				width = va_arg(arglist, int);
			}
			c = *index;
			if (c == '.') {
				dotf = 1;
				++ index;
				precision = getnum(&index, &pstarf);
				if (pstarf) {
					precision = va_arg(arglist, int);
				}
				c =* index;
			}
			if ((c == 'h') || (c == 'l') || (c == 'L')) {
				dispmod = c;
				++ index;
				c = *index;
			}
			switch (c)
			{
			case '%':
				if (output(c, fp, &storep) == EOF) {
					errorf = 1;
					break;
				}
				else {
					++ count;
				}
				break;
			case 'i':
			case 'd':
				if (dispmod == 'h') {
					sarg = (short int) va_arg(arglist, int);
					ularg = sarg;
				}
				else if (dispmod == 'l') {
					larg = (long int) va_arg(arglist, long int);
					ularg = larg;
				}
				else {
					iarg = va_arg(arglist, int);
					ularg = iarg;
				}
				returnval = intformat(ularg, width, precision,
						      zerof, plusf, spacef,
						      dashf, hashf, c, fp,
						      &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			case 'u':
			case 'o':
			case 'x':
			case 'X':
				if (dispmod == 'h') {
					usarg = (unsigned short) va_arg(arglist,unsigned int);
					ularg = usarg;
				}
				else if (dispmod == 'l') {
					ularg = (unsigned long) va_arg(arglist, unsigned long);
				}
				else {
					uarg = (unsigned) va_arg(arglist, unsigned);
					ularg = uarg;
				}
				returnval = intformat(ularg, width, precision,
						      zerof, plusf, spacef,
						      dashf, hashf, c, fp,
						      &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			case 'f':
			case 'e':
			case 'E':
			case 'g':
			case 'G':
				darg = (double) va_arg(arglist, double);
				returnval = fegformat(darg, width, precision,
						      dotf, zerof, plusf,
						      spacef, dashf, c, fp,
						      &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			case 'c':
				converted_arg[ 0 ] = (char) va_arg(arglist, int);
				converted_arg[ 1 ] = '\0';
				returnval = charformat(converted_arg, width,
						       precision, dashf, dotf,
						       c, fp, &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			case 's':
				stringp = va_arg(arglist, char *);
				returnval = charformat(stringp, width,
						       precision, dashf, dotf,
						       c, fp, &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			case 'p':
				parg = va_arg(arglist, void *);
				returnval = intformat((unsigned long) parg,
						      width, 8, 0, 0, 0, dashf,
						      hashf, 'x', fp, 
						      &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			case 'n':
				*va_arg(arglist, int *) = count;
				break;
			default:
				converted_arg[ 0 ] = c;
				converted_arg[ 1 ] = '\0';
				returnval = charformat(converted_arg, width,
						       precision, dashf, dotf,
						       'c', fp, &storep);
				if (returnval == EOF) {
					errorf = 1;
					break;
				}
				count += returnval;
				break;
			}
			if (errorf) {
				break;
			}
			++ index;
			RESET_FLAGS;
		}
	}
	if (errorf) {
		count = EOF;
	}

/*
 * Don't forget to null-terminate.
 */
	if ((fp == 0) && !(storep == 0)) {
		*storep = 0;
	}

	return(count);
}

/*
 *	output
 *
 *	Functions:	- send one char to the output, either string or file
 *	Accepts:	- character to be printed
 *			- file pointer for destination
 *			- char * to the destination
 *	Returns:	- the printed character, or EOF in case of an error
 */
static int
output(c, fp, storepp)

char			c;
TFILE			*fp;
char			**storepp;
	
{
	int		returnval;

/* 
 * If the file pointer is null, the character is stored 
 * in the momery pointed to by (*storepp).
 * If not, the character is stored in the file pointed to
 * by fp.
 */
	if (fp == 0) {
		**storepp = c;
		++ (*storepp);
		returnval = c;
	}
	else {
		returnval = tputc(c, fp);
	}
	return(returnval);
}


/*
 *	getnum
 *
 *	Function:	- read and evaluate numeric item from a given
 *			  position in a string
 *	Accepts:	- pointer to flag
 *			- pointer to pointer to string
 *	Returns:	- integer value of read string
 */
static int
getnum(stringpp, flagp)

CONST char			**stringpp;
int			*flagp;		/* width and precision star flags */

{
	char		c;
	int		val	= 0;	/* int value of read numeric string */

/*
 * If given position is an asterisk, * sets flag to 1. Non-digit
 * characters other than *, * cause zero as a return. At return,
 * the index points to next character to be read.
 */
	c = *(*stringpp);
	if (c == '*') { 
		*flagp = 1;
		++ (*stringpp);
	}
	else if (c == '-') {	/* negative fields ignored */
		val = 0;
	}
	else if (isdigit((int) c)) {
		while(isdigit((int) c)) {
			val = val * 10 + ((int) c - (int) '0');
			++ (*stringpp);
			c = *(*stringpp);
		}
	}
	return(val);
}


/*
 *	intconv
 *
 *	Function:	- convert absolute value of integer to a string
 *			- handle types i,d,u,o,x,X
 *			- set flag for	negative integers
 *	Accepts:	- int to convert
 *			- type
 *			- pointer to flag
 *	Returns:	- position in extern character array where the first
 *			  character is stored (position < MAXLENGTH)
 */
static int
intconv(unsigned long		arg,
        char			type,
        int			*minusfp)
{
	int		base = 10;			/* for conversions */
	int 		index = MAXLENGTH - 1;		/* first character */

	*minusfp = 0;
	switch(type)
	{
	case 'i':
	case 'd':
	case 'u':
		base = 10;
		break;
	case 'o':
		base = 8;
		break;
	case 'x':
	case 'X':
		base = 16;
		break;
	default:
		break;
	}
	if ((type == 'd') || (type == 'i')) {	/* signed integers */
		if ((long int) arg < 0) {
			arg =- arg;
			*minusfp = 1;
		}
	}
	if (arg == 0) {
		converted_arg[ index ] = '0';
	}
	else {
		while (arg != 0) {
							/* hex a-f, A-F */
			if ((arg % base) > 9) {
				converted_arg[ index-- ] =
					((type == 'x') ? 'a' : 'A') +
						((int) (arg % base)) - 10;
			}
							/* decimal digits */
			else {
				converted_arg[ index-- ] = '0' +
					((int) (arg % base));
			}
			arg /= base;
		}
		++ index;
	}
	return(index);
}


/*
 *	intformat
 *
 *	Function:	- print integer stored in external character array
 *	Accepts:	- the int to convert
 *			- width and precision fields
 *			- many various flags
 *			- type
 *			- fp to write to
 *			- string to write to
 *	Returns:	- number of "printed" characters
 */
static int
intformat(unsigned long		ularg,
          int			width,
          int			precision,
          int			zerof,
          int			plusf,
          int			spacef,
          int			dashf,
          int			hashf,
          char			type,
          TFILE			*fp,
          char			**storepp)
{
	int		avail;			/* available space in field */
	int		chrcount	= 0;	/* printed characters */
	int		errorf		= 0;
	int		field;			/* max(width,precision) */
	int		hexchr		= 0;	/* 2 for 0x hex prefix */
	int		i;			/* indice */
	int		length;			/* of converted integer */
	int		minusf;			/* sign flag */
	int		point;			/* index ret'd by intconv() */
	int		signchrs	= 0;	/* 1 for +,-,<space> */
	
	point = intconv(ularg, type, &minusf);
	length = MAXLENGTH - point;
	precision = ((precision < length) ? length : precision);
	if (type == 'o') {
		if (hashf) {				/* oct 0 prefix */
			if (precision < length + 1) {
				++ precision;
			}
		}
	}
	field = ((width > precision) ? width : precision);
	avail = field - precision;
	
	if (zerof) {
		dashf = 0;
	}
	if ((spacef) || (minusf) || (plusf)) {
		if ((type == 'i') || (type == 'd')) {
			signchrs = 1;
		}
	}
	if ((type == 'x') || (type == 'X')) {
		if (hashf) {
			hexchr = 2;
		}
	}
	if ((!zerof) && (!dashf)) {		/* right-justified */
		for(i=0; i<avail-signchrs-hexchr; ++i) {
			if (output(' ', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}

	switch (type)
	{
	case 'x':
	case 'X':
		if (hashf) {
			if (output('0', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
			if (output(type, fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		break;
	case 'i':
	case 'd':
		if (minusf) {
			if (output('-', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		else if (plusf) {
			if (output('+', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		else if (spacef) {
			if (output(' ', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		break;

	default:
		break;

	} /* end of switch */

	if (errorf) {
		return(EOF);
	}

	if (zerof) {					/* zero-fill due to */
		for(i=0; i<avail-signchrs; ++i) {	/* ...zero flag */
			if (output('0', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
	for(i=0; i<precision-length; ++i) {		/* ...precision */
		if (output('0', fp, storepp) == EOF) {
			errorf = 1;
			break;
		}
		++ chrcount;
	}
	for(i=point; i<MAXLENGTH; ++i) {		/* argument */
		if (output(converted_arg[ i ], fp, storepp) == EOF) {
			errorf = 1;
			break;
		}
		++ chrcount;
	}
	if (errorf) {
		return(EOF);
	}
	if (dashf) {					/* left-justified */
		for(i=0; i<avail-signchrs-hexchr; ++i) {
			if (output(' ', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
	return(chrcount);
}


/*
 *	charformat
 *
 *	Function:	- print character or string from external char array
 *	Accepts:	- the string to print
 *			- width and precisino fields
 *			- various flags
 *			- file to write to
 *			- string to write to
 *	Returns:	- number of "printed" characters
 */ 
static int
charformat(stringp, width, precision, dashf, dotf, type, fp, storepp)

char			*stringp;
int			width;
int			precision;
int			dashf;
int			dotf;
char			type;
TFILE			*fp;
char			**storepp;

{
	int		chrcount = 0;		/* printed characters */
	int		errorf   = 0;
	int		i;			/* index */

	size_t length = strlen(stringp);
	if (dotf) {
		if (precision == 0) {
			if (type == 'c') {
				precision = 1;
			}
		}
	}
	if ((precision > length) || (!dotf)) {
		precision = length;
	}
	if (dashf) {				/* left-justified */
		for(i=0; i<precision; ++i) {
			if (output(*(stringp + i), fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
	for(i=0; i<width-precision; ++i) {
		if (output(' ', fp, storepp) == EOF) {
			errorf = 1;
			break;
		}
		++ chrcount;
	}
	if (errorf) {
		return(EOF);
	}
	if (!dashf) {
		for(i=0; i<precision; ++i) {
			if (output(*(stringp + i), fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
	return(chrcount);
}


/*
 *	round_up
 *
 *	Function:	- round up argument to given precision
 *	Accepts:	- ptr to argument to round
 *			- precision
 */
static void
round_up(dargp, precision)
	double	*dargp;
	int	precision;
{
	double power = (double) 1;	/* 10 to the (-precision) */
	int i;

	for(i=0; i<precision; ++i) {
		power /= 10;
	}
	*dargp += (.5 * power);
}


/*
 *	norm
 *
 *	Function:	- write argument in exponential notation
 * 	Accepts:	- argument
 *			- ptr to mantissa ((mantissa >= 0) && (mantissa < 1))
 *			- ptr to exponent
 */
static void
norm(darg, mantissap, exponentp)

double			darg;
double			*mantissap;
int			*exponentp;

{
	int		sign;		/* 1 for pos., -1 for neg. */

	sign = (darg < 0) ? -1 : 1;
	if (darg < 0) {
		darg = -darg;
	}

	*exponentp = 0;

	while (((darg < .1) || (darg >= 1.0)) && (darg != 0)) {

		if (darg < .1) {
			darg *= 10;
			(*exponentp)--;
		}
		else if (darg >= 1) {
			darg /= 10;
			(*exponentp)++;
		}
	}
	*mantissap = darg * sign;
}


/*
 *	fegformat
 *
 *	Function:	- print argument in floating point notation
 *			- IGNORES L format for long doubles
 *	Accepts:	- argument to format
 *			- width and precision fields
 *			- various and sundry format flags
 *			- file to print into
 *			- string to print into
 *	Returns:	- number of "printed" characters
 */
static int
fegformat(darg, width, precision, dotf, zerof, plusf,
	  spacef, dashf, type, fp, storepp)

double			darg;
int			width;
int			precision;
int			dotf;
int			zerof;
int			plusf;
int			spacef;
int			dashf;
char			type;
TFILE			*fp;
char			**storepp;

{
	double		mantissa;
	int		avail;			/* available space in field */
	int		chrcount	= 0;	/* printed characters */
	int		decpointf	= 1;
	int		errorf		= 0;
	int		exponent;
	int		i;			/* indice */
	int		msdigit		= 0;	/* 1st ~0 digit in mantissa */
	int		minusf		= 0;
	int		nulargf		= 0;
	int		signchrs	= 0;	/* 1 for +,-,<space> */
	int		tempexp;		/* temporary exponent */
	
	precision = ((dotf) ? precision : 6);
	decpointf = (((dotf) && (precision == 0)) ? 0 : 1);
	if (darg < 0) {
		darg =- darg;
		minusf = 1;
	}
	norm(darg, &mantissa, &exponent);
/*
 *	Round place according to type.
 */

	tempexp = precision;
	if (type == 'f') {
		tempexp += exponent;
	}
	else if ((type == 'e') || (type == 'E')) {
		++ tempexp;
	}
	else {
		if (precision == 0) {
			++ tempexp;
		}
	}
	if (mantissa == 0) {
		nulargf = 1;
	}
	round_up(&mantissa, tempexp);

/*
 * JRV: HEY! rounding might have taken this out of
 *	the normal form we just put it in.  If so, fix it now.
 */
	if (mantissa >= 1.0) {
		mantissa /= 10;
		exponent++;
	}

/*
 *	g,G formats call either f or e,E, depending on size of argument and
 *	precision. In f format, argument are printed until first non-zero
 *	significant digit.
 */
	if ((type == 'g') || (type == 'G')) {
		if (exponent < 2) {
			msdigit = 1 - exponent;
		} 
		if ((exponent < -3) || (exponent > precision)) {
			type -= ('G' - 'E');
			precision -= ((precision > 0) ? 1 : 0);
		}
		else {
			type = 'f';
			precision -= exponent;
			if (precision < msdigit) {
				precision = msdigit;
				decpointf = 1;
			} 
		}
		tempexp = precision;
		if ((type == 'e') || (type == 'E')) {
			++ tempexp;
		}
		else {
			tempexp += exponent;
		}
		precision -= trailzscan(mantissa, tempexp);
		if (precision <= 0) {
			precision = 0;
			decpointf = 0;
		}
	}

/*
 * Set width and available space for printing mantissa.
 */	
	if ((type == 'e') || (type == 'E')) {
		width -= (3 + ((exponent > 99) ? 3 : 2));
	}
	if ((spacef) || (minusf) || (plusf)) {
		signchrs = 1;
	}
	avail = width - precision - decpointf - signchrs;
	if (type == 'f') {
		avail -= ((exponent <= 0) ? 1 : exponent);
	}
	if (dashf) {
		zerof = 0;
	}
						/* right-justified */
	if ((!zerof) && (!dashf)) {
		for(i=0; i<avail; ++i) {
			if (output(' ', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
/*
 * Handle sign characters.
 */
	if (minusf) {
		if (output('-', fp, storepp) == EOF) {
			return(EOF);
		}
		++ chrcount;
	}
	else if (plusf) {
		if (output('+', fp, storepp) == EOF) {
			return(EOF);
		}
		++ chrcount;
	}
	else if (spacef) {
		if (output(' ', fp, storepp) == EOF) {
			return(EOF);
		}
		++ chrcount;
	}
/*
 * Handle zero-fill.
 */
	if (zerof) {
		for(i=0; i<avail; ++i) {
			if (output('0', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}

	tempexp = exponent;
	if ((type == 'e') || (type == 'E')) {
		if (!nulargf) {
			if (mantissa) {
				tempexp = 1;
				-- exponent;
			}
		}
	}
/*
 *	Float argument is printed here.
 */
	if (tempexp <= 0) {			/* integer part */
		if (output('0', fp, storepp) == EOF) {
			return (EOF);
		}
		++ chrcount;
	}
	else {
		for(i=0; i < tempexp; ++i) {
			if (output('0' + nextdigit(&mantissa),
				   fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
	if (decpointf) {			/* decimal part */
		if (output('.', fp, storepp) == EOF) {
			return(EOF);
		}
		++ chrcount;
		if (tempexp < 0) {
			i = 0;
			while ((i < precision) && (i < -exponent)) {
				if (output('0', fp, storepp) == EOF) {
					errorf = 1;
					break;
				}
				++ chrcount;
				++ i;
			}
			if (errorf) {
				return(EOF);
			}
			precision += exponent;
		}
		for(i=0; i<precision; ++i) {
			if (output('0' + nextdigit(&mantissa), fp,
				   storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}

/*
 *	Exponential notation with: e(E)+(-)XX(X).
 */
	if ((type == 'e') || (type == 'E')) {
		width -= i;
		if (output(type, fp, storepp) == EOF) {
			return(EOF);
		}
		++ chrcount;
		precision = ((exponent > 99) ? 3 : 2);
		i = intformat((unsigned long) exponent, 0,
			      precision, 0, 1, 0, 0, 0, 'd', fp, storepp);
		if (i == EOF) {
			return(EOF);
		}
		chrcount += i;
	}
						/* left-justified */
	if (dashf) {
		for(i=0; i<avail; ++i) {
			if (output(' ', fp, storepp) == EOF) {
				errorf = 1;
				break;
			}
			++ chrcount;
		}
		if (errorf) {
			return(EOF);
		}
	}
	return(chrcount);
}


/*
 *	nextdigit
 *
 *	Function:	- extract from mantissa the next digit
 *	Accepts:	- pointer to mantissa.
 *	Returns:	- integer value of extracted digit
 */
static int
nextdigit(mantissap)

double			*mantissap;

{
	int		digit;

	*mantissap *= 10;
	digit = (int) (*mantissap);
	*mantissap -= (double) digit;
	return(digit);
}


/*
 *	trailzscan
 *
 *	Function:	- scan mantissa to precision place for consecutive
 *			  zeros
 *	Accepts:	- mantissa
 *			- precision
 *	Returns:	- number of consecutive zeros, the last being at the
 *			  precision place.
 */
static int
trailzscan(mantissa, precision)

double			mantissa;
int			precision;

{
	int	counter	= 0;
	int	countf	= 0;
/*
 *	countf: flag that starts the counter when zero encountered after a
 *	non-zero digit.
 */
	int	digit;
	int	i;				/* indice */

	for(i=0; i<precision; ++i) {
		digit = nextdigit(&mantissa);
		if (digit == 0) {
			if (!countf) {	/* first zero, counter set */
				countf = 1;
			}
			++ counter;
		}
		else {
			counter = 0;		/* counter and flag reset */
			countf = 0;
		}
	}
	return(counter);
}
