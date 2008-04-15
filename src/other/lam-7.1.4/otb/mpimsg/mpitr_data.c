/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	NJN/RBD
 *
 *	$Id: mpitr_data.c,v 6.7 2002/10/09 20:56:25 brbarret Exp $
 *
 *	Function:	- print packed MPI message contents
 */

#include <lam_config.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <mpitrace.h>
#include <mpi_types.h>
#include <portable.h>
#include <t_types.h>

/*
 * local definitions
 */
#define VCHAR		0
#define VSHORT		1
#define VINT		2
#define VLONG		3
#define VFLOAT		4
#define	VDOUBLE		5
#define	VLONGDOUBLE	6
#define	VLONGDBLINT	7
#define VCOMPLEX        8
#define	V2INT		9
#define V2FLOAT		10
#define V2DOUBLE	11
#define VFLOATINT	12
#define VDOUBLEINT	13
#define VLONGINT	14
#define VSHORTINT	15
#define VLOGIC		16
#define VDBLCOMPLEX	17
#define VLONGLONG	18

#define HDISP(x)	(((x) & 0xFFFF) * (((x) >> 16) & 0xFFFF))

#if LAM_SIZEOF_LONG_DOUBLE
typedef	long double		longdouble;
#else
typedef	double			longdouble;
#endif

#if LAM_SIZEOF_LONG_LONG
typedef	long long		longlong;
typedef	unsigned long long	ulonglong;
#else
typedef	long			longlong;
typedef	unsigned long		ulonglong;
#endif

/*
 * global functions
 */
void			mpitr_dataprint();

/*
 * local functions
 */
static void		output();
static void		print_bound();
static void		print_char();
static void		print_wchar();
static void		print_complex();
static void		print_dblcomplex();
static void		print_double();
static void		print_long_double();
static void		print_float();
static void		print_int();
static void		print_long();
static void		print_long_long();
static void		print_ulong_long();
static void		print_logic();
static void		print_short();
static void		print_uchar();
static void		print_uint();
static void		print_ulong();
static void		print_ushort();
static void		print_2int();
static void		print_2float();
static void		print_2double();
static void		print_floatint();
static void		print_doubleint();
static void		print_longdblint();
static void		print_longint();
static void		print_shortint();
static void		unroll();
static void		unroll_contig();
static void		unroll_hvector();
static void		unroll_hindexed();
static void		unroll_struct();

/*
 * local variables
 */
static int		fl_homog;	/* homogeneous LAM flag */
static int		address;	/* line starting address */
static unsigned int	eltcount;	/* output line column count */
static int		fl_first;	/* first line flag */
static char		fmtbuf[80];	/* formatting buffer */
/*
 * Maximum number of elements of each type that can be printed on a line.
 * This is for the usual case where sizeof(long) == sizeof(int) == 4.
 */
static unsigned int	eltsperline[] = { 8, 8, 4, 4, 4, 4, 2, 1, 2, 2, 2, 2, 2,
					  2, 2, 2, 32, 2, 4};
/*
 *	mpitr_dataprint
 *
 *	Function:	- print MPI message data
 *	Accepts:	- datatype trace
 *			- message data
 *			- length message data 
 *			- number of leaf elements to print (-1 means all)
 *			- homogeneous LAM?
 */
void
mpitr_dataprint(dtype, data, datalen, nelts, homog)

char			*dtype;
char			*data;
int			datalen;
int			nelts;
int			homog;

{
	struct trdtype	*trace;			/* trace datatype */
	struct trdtype	*p;
/*
 * Set elements per line table.
 */
	eltsperline[VSHORT] = 16 / sizeof(short);
	eltsperline[VINT] = 16 / sizeof(int);
	eltsperline[VLONG] = 16 / sizeof(long);
	eltsperline[VFLOAT] = 16 / sizeof(float);
	eltsperline[VDOUBLE] = 16 / sizeof(double);
	eltsperline[VLONGDOUBLE] = 16 / sizeof(longdouble);
	eltsperline[VLONGLONG] = 16 / sizeof(longlong);
/*
 * Recursively (depth-first) print datatype.
 */
	eltcount = 0;
	address = 0;
	fl_first = 1;
	fl_homog = homog;

	trace = ((struct trdtype *) dtype) + 1;
	
	while ((datalen > 0) && (nelts != 0)) {
		p = trace;
		unroll(&p, &data, &datalen, &nelts);
	}

	printf("\n");
}

/*
 *	unroll
 *
 *	Function:	- unroll datatype & data
 *			- recursive function
 *	Accepts:	- dtype trace (inout)
 *			- data buffer (inout)
 *			- length of data buffer (inout)
 *			- number elements (inout)
 */
static void
unroll(dtype, data, len, elts)

struct trdtype		**dtype;
char			**data;
int			*len;
int			*elts;

{
	if ((*len <= 0) || (*elts == 0)) return;

	switch((*dtype)->trd_dtype) {

	case TRDTUCHAR:
	case TRDTBYTE:
	case TRDTPACKED:
		print_uchar(dtype, data, len);
		--(*elts);
		break;

	case TRDTCHAR:
	case TRDTFCHARACTER:
		print_char(dtype, data, len);
		--(*elts);
		break;

	case TRDTWCHAR:
		print_wchar(dtype, data, len);
		--(*elts);

	case TRDTUINT:
		print_uint(dtype, data, len);
		--(*elts);
		break;

	case TRDTULONG:
		print_ulong(dtype, data, len);
		--(*elts);
		break;

	case TRDTINT:
	case TRDTFINTEGER:
		print_int(dtype, data, len);
		--(*elts);
		break;

	case TRDTLONG:
		print_long(dtype, data, len);
		--(*elts);
		break;

	case TRDTLONGLONG:
		print_long_long(dtype, data, len);
		--(*elts);
		break;

	case TRDTULONGLONG:
		print_ulong_long(dtype, data, len);
		--(*elts);
		break;

	case TRDTFLOGICAL:
		print_logic(dtype, data, len);
		--(*elts);
		break;

	case TRDTSHORT:
		print_short(dtype, data, len);
		--(*elts);
		break;

	case TRDTUSHORT:
		print_ushort(dtype, data, len);
		--(*elts);
		break;

	case TRDTFLOAT:
	case TRDTFREAL:
		print_float(dtype, data, len);
		--(*elts);
		break;

	case TRDTDOUBLE:
	case TRDTFDBLPREC:
		print_double(dtype, data, len);
		--(*elts);
		break;

	case TRDTLONGDOUBLE:
		print_long_double(dtype, data, len);
		--(*elts);
		break;

	case TRDTFCOMPLEX:
		print_complex(dtype, data, len);
		--(*elts);
		break;

	case TRDTFDBLCOMPLEX:
		print_dblcomplex(dtype, data, len);
		--(*elts);
		break;

	case TRDTUB:
	case TRDTLB:
		print_bound(dtype, data, len);
		break;

	case TRDT2INT:
	case TRDTF2INTEGER:
		print_2int(dtype, data, len);
		--(*elts);
		break;

	case TRDTF2REAL:
		print_2float(dtype, data, len);
		--(*elts);
		break;

	case TRDTF2DBLPREC:
		print_2double(dtype, data, len);
		--(*elts);
		break;

	case TRDTFLOATINT:
		print_floatint(dtype, data, len);
		--(*elts);
		break;

	case TRDTDOUBLEINT:
		print_doubleint(dtype, data, len);
		--(*elts);
		break;

	case TRDTLONGDBLINT:
		print_longdblint(dtype, data, len);
		--(*elts);
		break;

	case TRDTSHORTINT:
		print_shortint(dtype, data, len);
		--(*elts);
		break;

	case TRDTLONGINT:
		print_longint(dtype, data, len);
		--(*elts);
		break;

	case TRDTCONTIG:
		unroll_contig(dtype, data, len, elts);
		break;

	case TRDTVECTOR:
		unroll_hvector(dtype, data, 1, len, elts);
		break;

	case TRDTHVECTOR:
		unroll_hvector(dtype, data, 0, len, elts);
		break;

	case TRDTINDEXED:
		unroll_hindexed(dtype, data, 1, len, elts);
		break;

	case TRDTHINDEXED:
		unroll_hindexed(dtype, data, 0, len, elts);
		break;

	case TRDTSTRUCT:
		unroll_struct(dtype, data, len, elts);
		break;

	default:
		break;
	}
}

/*
 *	unroll_contig
 *
 *	Function:	- unroll contiguous datatype & data
 */
static void
unroll_contig(dtype, data, len, elts)

struct trdtype		**dtype;
char			**data;
int			*len;
int			*elts;

{
	int		count;
	struct trdtype	*subtype;

	count = (*dtype)->trd_count;
	subtype = ++(*dtype);

	for (; (count > 0) && (*len > 0) && (*elts != 0); --count) {
		*dtype = subtype;
		unroll(dtype, data, len, elts);
	}

	eltcount = 0;
}

/*
 *	unroll_hvector
 *
 *	Function:	- unroll hvector datatype & data
 */
static void
unroll_hvector(dtype, data, fl_vec, len, elts)

struct trdtype		**dtype;
char			**data;
int			fl_vec;
int			*len;
int			*elts;

{
	int		i, j;
	int		count;
	int		length;
	int		stride;
	int		savaddr;
	struct trdtype	*subtype;

	count = (*dtype)->trd_count;
	++(*dtype);
	length = (*dtype)->trd_length;
	stride = (*dtype)->trd_stride;

	if (fl_vec) stride = HDISP(stride);

	subtype = ++(*dtype);
	savaddr = address;

	for (i = 0; (i < count) && (*len > 0) && (*elts != 0); ++i) {

		address = savaddr;
		savaddr += stride;

		for (j = 0; (j < length) && (*len > 0) && (*elts != 0); ++j) {
			*dtype = subtype;
			unroll(dtype, data, len, elts);
		}

		eltcount = 0;
	}
}

/*
 *	unroll_hindexed
 *
 *	Function:	- unroll hindexed datatype & data
 */
static void
unroll_hindexed(dtype, data, fl_idx, len, elts)

struct trdtype		**dtype;
char			**data;
int			fl_idx;
int			*len;
int			*elts;

{
	int		i, j;
	int		count;
	int		savaddr;
	int		disp;
	struct trdtype	*savbuff;
	struct trdtype	*idxbuff;

	count = (*dtype)->trd_count;
	idxbuff = ++(*dtype);
	*dtype += count;
	savbuff = *dtype;
	savaddr = address;

	for (i = 0;
	     (i < count) && (*len > 0) && (*elts != 0);
	     ++i, ++idxbuff) {

		disp = idxbuff->trd_disp;
		if (fl_idx) disp = HDISP(disp);

		address = savaddr + idxbuff->trd_disp;

		for (j = 0;
		     (j < idxbuff->trd_length) && (*len > 0) && (*elts != 0);
		     ++j) {
			*dtype = savbuff;
			unroll(dtype, data, len, elts);
		}

		eltcount = 0;
	}
}

/*
 *	unroll_struct
 *
 *	Function:	- unroll struct datatype & data
 */
static void
unroll_struct(dtype, data, len, elts)

struct trdtype		**dtype;
char			**data;
int			*len;
int			*elts;

{
	int		i, j;
	int		count;
	int		length;
	int		savaddr;
	struct trdtype	*savbuff;

	count = (*dtype)->trd_count;
	++(*dtype);
	savaddr = address;
 
	for (i = 0; (i < count) && (*len > 0) && (*elts != 0); ++i) {

		length = (*dtype)->trd_length;
		address = savaddr + (*dtype)->trd_disp;

		savbuff = ++(*dtype);

		for (j = 0; (j < length) && (*len > 0) && (*elts != 0); ++j) {
			*dtype = savbuff;
			unroll(dtype, data, len, elts);
		}

		eltcount = 0;
	}
}

/*
 *	print_uchar
 *
 *	Function:	- print unsigned character
 */
static void
print_uchar(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	unsigned char	c;

	if (*len >= 1) {

		++(*dtype);
		c = *((unsigned char *) *data);
		++(*data);

		sprintf(fmtbuf, isprint(c) ? "%4c" : "%4u", c & 0xFF);
		output(fmtbuf, eltsperline[VCHAR]);

		++address;
	}

	--(*len);
}

/*
 *	print_char
 *
 *	Function:	- print character
 */
static void
print_char(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	SIGNED char	c;

	if (*len >= 1) {

		++(*dtype);
		c = **data;
		++(*data);

		if (c >= 0) {
			sprintf(fmtbuf, isprint(c) ? "%4c" : "%4u", c & 0xFF);
		} else {
			sprintf(fmtbuf, "%4d", c);
		}

		output(fmtbuf, eltsperline[VCHAR]);

		++address;
	}

	--(*len);
}

/*
 *	print_wchar
 *
 *	Function:	- print wide char
 */
static void
print_wchar(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	wchar_t		w;

	if (*len >= sizeof(wchar_t)) {

		++(*dtype);
		memcpy((char *) &w, *data, sizeof(wchar_t));
		*data += sizeof(wchar_t);

		if (!fl_homog) ttol_wchar(&w, &w);
		sprintf(fmtbuf, "%15lX", (long) w);
		output(fmtbuf, eltsperline[VINT]);

		address += sizeof(wchar_t);
	}

	*len -= sizeof(wchar_t);
}

/*
 *	print_int
 *
 *	Function:	- print signed integer
 */
static void
print_int(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	int		i;

	if (*len >= sizeof(int)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) ttol_int(&i, &i);
		sprintf(fmtbuf, "%15d", i);
		output(fmtbuf, eltsperline[VINT]);

		address += sizeof(int);
	}

	*len -= sizeof(int);
}

/*
 *	print_uint
 *
 *	Function:	- print unsigned integer
 */
static void
print_uint(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	unsigned int	i;

	if (*len >= sizeof(unsigned int)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(unsigned int));
		*data += sizeof(unsigned int);

		if (!fl_homog) ttol_int(&i, &i);
		sprintf(fmtbuf, "%15u", i);
		output(fmtbuf, eltsperline[VINT]);

		address += sizeof(unsigned int);
	}

	*len -= sizeof(unsigned int);
}

/*
 *	print_long
 *
 *	Function:	- print long integer
 */
static void
print_long(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	long int	i;

	if (*len >= sizeof(long int)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(long int));
		*data += sizeof(long int);

		if (!fl_homog) ttol_long(&i, &i);
		sprintf(fmtbuf, "%15ld", i);
		output(fmtbuf, eltsperline[VLONG]);

		address += sizeof(long int);
	}

	*len -= sizeof(long int);
}

/*
 *	print_ulong
 *
 *	Function:	- print unsigned long integer
 */
static void
print_ulong(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	unsigned long	i;

	if (*len >= sizeof(unsigned long)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(unsigned long));
		*data += sizeof(unsigned long);

		if (!fl_homog) ttol_long(&i, &i);
		sprintf(fmtbuf, "%15lu", i);
		output(fmtbuf, eltsperline[VLONG]);

		address += sizeof(unsigned long);
	}

	*len -= sizeof(unsigned long);
}

/*
 *	print_logic
 *
 *	Function:	- print boolean
 */
static void
print_logic(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	unsigned int	i;

	if (*len >= sizeof(unsigned int)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(unsigned int));
		*data += sizeof(unsigned int);

		if (!fl_homog) ttol_int(&i, &i);
		sprintf(fmtbuf, "%1s", (i) ? "F" : "T");
		output(fmtbuf, eltsperline[VLOGIC]);

		address += sizeof(unsigned int);
	}

	*len -= sizeof(unsigned int);
}

/*
 *	print_short
 *
 *	Function:	- print short integer
 */
static void
print_short(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	short		i;

	if (*len >= sizeof(short)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(short));
		*data += sizeof(short);

		if (!fl_homog) ttol_short(&i, &i);
		sprintf(fmtbuf, "%7d", (int) i);
		output(fmtbuf, eltsperline[VSHORT]);

		address += sizeof(short);
	}

	*len -= sizeof(short);
}

/*
 *	print_ushort
 *
 *	Function:	- print unsigned short integer
 */
static void
print_ushort(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	unsigned short	i;

	if (*len >= sizeof(unsigned short)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(unsigned short));
		*data += sizeof(unsigned short);

		if (!fl_homog) ttol_short(&i, &i);
		sprintf(fmtbuf, "%7u", (unsigned int) i);
		output(fmtbuf, eltsperline[VSHORT]);

		address += sizeof(unsigned short);
	}

	*len -= sizeof(unsigned short);
}

/*
 *	print_float
 *
 *	Function:	- print single-precision floating point
 */
static void
print_float(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	float		f;

	if (*len >= sizeof(float)) {

		++(*dtype);
		memcpy((char *) &f, *data, sizeof(float));
		*data += sizeof(float);

		if (!fl_homog) ttol_float(&f, &f);
		sprintf(fmtbuf, "%15g", f);
		output(fmtbuf, eltsperline[VFLOAT]);

		address += sizeof(float);
	}

	*len -= sizeof(float);
}

/*
 *	print_double
 *
 *	Function:	- print double-precision floating point
 */
static void
print_double(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	double		f;

	if (*len >= sizeof(double)) {

		++(*dtype);
		memcpy((char *) &f, *data, sizeof(double));
		*data += sizeof(double);

		if (!fl_homog) ttol_double(&f, &f);
		sprintf(fmtbuf, "%#15g", f);
		output(fmtbuf, eltsperline[VDOUBLE]);

		address += sizeof(double);
	}

	*len -= sizeof(double);
}

/*
 *	print_long_long
 *
 *	Function:	- print long long integer
 */
static void
print_long_long(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	longlong	i;

	if (*len >= sizeof(longlong)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(longlong));
		*data += sizeof(longlong);

#if LAM_SIZEOF_LONG_LONG
		if (!fl_homog) {
			ttol_longlong(&i, &i);
		}
		sprintf(fmtbuf, "%15lld", i);
#else
		if (!fl_homog) {
			ttol_long(&i, &i);
		}
		sprintf(fmtbuf, "%15ld", i);
#endif
		output(fmtbuf, eltsperline[VLONGLONG]);

		address += sizeof(longlong);
	}

	*len -= sizeof(longlong);
}

/*
 *	print_ulong_long
 *
 *	Function:	- print unsigned long long integer
 */
static void
print_ulong_long(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	ulonglong	i;

	if (*len >= sizeof(ulonglong)) {

		++(*dtype);
		memcpy((char *) &i, *data, sizeof(ulonglong));
		*data += sizeof(ulonglong);

#if LAM_SIZEOF_LONG_LONG
		if (!fl_homog) {
			ttol_longlong(&i, &i);
		}
		sprintf(fmtbuf, "%15llu", i);
#else
		if (!fl_homog) {
			ttol_long(&i, &i);
		}
		sprintf(fmtbuf, "%15lu", i);
#endif
		output(fmtbuf, eltsperline[VLONGLONG]);

		address += sizeof(ulonglong);
	}

	*len -= sizeof(ulonglong);
}

/*
 *	print_long_double
 *
 *	Function:	- print quad-precision floating point
 */
static void
print_long_double(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	longdouble	d;

	if (*len >= sizeof(longdouble)) {

		++(*dtype);
		memcpy((char *) &d, *data, sizeof(longdouble));
		*data += sizeof(longdouble);

#if LAM_SIZEOF_LONG_DOUBLE
		if (!fl_homog) {
			ttol_longdbl(&d, &d);
		}
		sprintf(fmtbuf, "%#15Lg", d);
#else
		if (!fl_homog) {
			ttol_double(&d, &d);
		}
		sprintf(fmtbuf, "%#15g", d);
#endif
		output(fmtbuf, eltsperline[VLONGDOUBLE]);

		address += sizeof(longdouble);
	}

	*len -= sizeof(longdouble);
}

/*
 *	print_complex
 *
 *	Function:	- print complex
 */
static void
print_complex(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	float		fr, fi;

	if (*len >= (2 * sizeof(float))) {

		++(*dtype);
		memcpy((char *) &fr, *data, sizeof(float));
		*data += sizeof(float);
		memcpy((char *) &fi, *data, sizeof(float));
		*data += sizeof(float);

		if (!fl_homog) {
			ttol_float(&fr, &fr);
			ttol_float(&fi, &fi);
		}

		sprintf(fmtbuf, "%15g %15g", fr, fi);
		output(fmtbuf, eltsperline[VCOMPLEX]);

		address += 2 * sizeof(float);
	}

	*len -= 2 * sizeof(float);
}

/*
 *	print_dblcomplex
 *
 *	Function:	- print double complex
 */
static void
print_dblcomplex(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	double		f1, f2;

	if (*len >= (2 * sizeof(double))) {

		++(*dtype);
		memcpy((char *) &f1, *data, sizeof(double));
		*data += sizeof(double);
		memcpy((char *) &f2, *data, sizeof(double));
		*data += sizeof(double);

		if (!fl_homog) {
			ttol_double(&f1, &f1);
			ttol_double(&f2, &f2);
		}

		sprintf(fmtbuf, "%15g %15g", f1, f2);
		output(fmtbuf, eltsperline[VDBLCOMPLEX]);

		address += 2 * sizeof(double);
	}

	*len -= 2 * sizeof(double);
}

/*
 *	print_bound
 *
 *	Function:	- print upper/lower bound
 */
static void
print_bound(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	if (*len >= 0) {
		++(*dtype);
		*data += 0;	/* keep it orthogonal & use all vars */
	}

	*len -= 0;
}

/*
 *	print_2int
 *
 *	Function:	- print 2 integers
 */
static void
print_2int(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	int		i1, i2;

	if (*len >= (2 * sizeof(int))) {

		++(*dtype);
		memcpy((char *) &i1, *data, sizeof(int));
		*data += sizeof(int);
		memcpy((char *) &i2, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) {
			ttol_int(&i1, &i1);
			ttol_int(&i2, &i2);
		}

		sprintf(fmtbuf, "%15d %15d", i1, i2);
		output(fmtbuf, eltsperline[V2INT]);

		address += 2 * sizeof(int);
	}

	*len -= 2 * sizeof(int);
}

/*
 *	print_2float
 *
 *	Function:	- print 2 floats
 */
static void
print_2float(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	float		f1, f2;

	if (*len >= (2 * sizeof(float))) {

		++(*dtype);
		memcpy((char *) &f1, *data, sizeof(float));
		*data += sizeof(float);
		memcpy((char *) &f2, *data, sizeof(float));
		*data += sizeof(float);

		if (!fl_homog) {
			ttol_float(&f1, &f1);
			ttol_float(&f2, &f2);
		}

		sprintf(fmtbuf, "%15g %15g", f1, f2);
		output(fmtbuf, eltsperline[V2FLOAT]);

		address += 2 * sizeof(float);
	}

	*len -= 2 * sizeof(float);
}

/*
 *	print_2double
 *
 *	Function:	- print 2 doubles
 */
static void
print_2double(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	double		f1, f2;

	if (*len >= (2 * sizeof(double))) {

		++(*dtype);
		memcpy((char *) &f1, *data, sizeof(double));
		*data += sizeof(double);
		memcpy((char *) &f2, *data, sizeof(double));
		*data += sizeof(double);

		if (!fl_homog) {
			ttol_double(&f1, &f1);
			ttol_double(&f2, &f2);
		}

		sprintf(fmtbuf, "%15g %15g", f1, f2);
		output(fmtbuf, eltsperline[V2DOUBLE]);

		address += 2 * sizeof(double);
	}

	*len -= 2 * sizeof(double);
}

/*
 *	print_floatint
 *
 *	Function:	- print a float and an integer
 */
static void
print_floatint(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	float		f;
	int		i;

	if (*len >= (sizeof(float) + sizeof(int))) {

		++(*dtype);
		memcpy((char *) &f, *data, sizeof(float));
		*data += sizeof(float);
		memcpy((char *) &i, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) {
			ttol_float(&f, &f);
			ttol_int(&i, &i);
		}

		sprintf(fmtbuf, "%15g %15d", f, i);
		output(fmtbuf, eltsperline[VFLOATINT]);

		address += sizeof(float) + sizeof(int);
	}

	*len -= sizeof(float) + sizeof(int);
}

/*
 *	print_doubleint
 *
 *	Function:	- print a long and an integer
 */
static void
print_longint(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	long		l;
	int		i;

	if (*len >= (sizeof(long) + sizeof(int))) {

		++(*dtype);
		memcpy((char *) &l, *data, sizeof(long));
		*data += sizeof(long);
		memcpy((char *) &i, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) {
			ttol_long(&l, &l);
			ttol_int(&i, &i);
		}

		sprintf(fmtbuf, "%15ld %15d", l, i);
		output(fmtbuf, eltsperline[VLONGINT]);

		address += sizeof(long) + sizeof(int);
	}

	*len -= sizeof(long) + sizeof(int);
}

/*
 *	print_doubleint
 *
 *	Function:	- print a double and an integer
 */
static void
print_doubleint(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	double		f;
	int		i;

	if (*len >= (sizeof(double) + sizeof(int))) {

		++(*dtype);
		memcpy((char *) &f, *data, sizeof(double));
		*data += sizeof(double);
		memcpy((char *) &i, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) {
			ttol_double(&f, &f);
			ttol_int(&i, &i);
		}

		sprintf(fmtbuf, "%15g %15d", f, i);
		output(fmtbuf, eltsperline[VDOUBLEINT]);

		address += sizeof(double) + sizeof(int);
	}

	*len -= sizeof(double) + sizeof(int);
}

/*
 *	print_longdblint
 *
 *	Function:	- print a long double and an integer
 */
static void
print_longdblint(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	longdouble	d;
	int		i;

	if (*len >= (sizeof(longdouble) + sizeof(int))) {

		++(*dtype);
		memcpy((char *) &d, *data, sizeof(longdouble));
		*data += sizeof(longdouble);
		memcpy((char *) &i, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) {
#if LAM_SIZEOF_LONG_DOUBLE
			ttol_longdbl(&d, &d);
#else
			ttol_double(&d, &d);
#endif			
			ttol_int(&i, &i);
		}
#if LAM_SIZEOF_LONG_DOUBLE
		sprintf(fmtbuf, "%15Lg %15d", d, i);
#else
		sprintf(fmtbuf, "%15g %15d", d, i);
#endif		
		output(fmtbuf, eltsperline[VDOUBLEINT]);

		address += sizeof(longdouble) + sizeof(int);
	}

	*len -= sizeof(longdouble) + sizeof(int);
}

/*
 *	print_shortint
 *
 *	Function:	- print a short and an integer
 */
static void
print_shortint(dtype, data, len)

struct trdtype		**dtype;
char			**data;
int			*len;

{
	short		s;
	int		i;

	if (*len >= (sizeof(short) + sizeof(int))) {

		++(*dtype);
		memcpy((char *) &s, *data, sizeof(short));
		*data += sizeof(short);
		memcpy((char *) &i, *data, sizeof(int));
		*data += sizeof(int);

		if (!fl_homog) {
			ttol_short(&s, &s);
			ttol_int(&i, &i);
		}

		sprintf(fmtbuf, "%7d %15d", (int) s, i);
		output(fmtbuf, eltsperline[VSHORTINT]);

		address += sizeof(short) + sizeof(int);
	}

	*len -= sizeof(short) + sizeof(int);
}

/*
 *	output
 *
 *	Function:	- print string (representing a basic element) out
 *			- track # elements per line
 *			- print address at start of line
 *			- handle continuation lines
 *	Accepts:	- string representation of element
 *			- max number of elements per line
 */
static void
output(string, maxelts)

char			*string;
unsigned int		maxelts;

{
/*
 * Break long lines and print address at the beginning of new lines.
 */
	if ((eltcount == 0) || (eltcount >= maxelts)) {

		if (fl_first) {
			fl_first = 0;
		} else {
			printf("\n");
		}

		printf("%8x:", address);
		eltcount = 0;
	}
/*
 * Print the string.
 */
	printf(" %s", string);
	eltcount++;
}
