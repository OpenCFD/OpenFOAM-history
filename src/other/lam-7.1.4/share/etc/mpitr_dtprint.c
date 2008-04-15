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
 *	Copyright 1996 The Ohio State University
 *	RBD/NJN/GDB
 *
 *	$Id: mpitr_dtprint.c,v 6.5 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- MPI datatype trace access functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpitrace.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>

/*
 * constants and macros
 */
#define DISP(f, x)	((f) ? ((x) & 0xFFFF) : (x))


/*
 * local functions
 */
static void print_datatype(struct trdtype **dtrbuf, int level);
static void print_basic(int dtype, struct trdtype **dtrace, int nlev);
static void print_contig(struct trdtype **dtrace, int nlev);
static void print_hvector(int dtype, struct trdtype **dtrace, int nlev, 
                          int fl_vec);
static void print_hindexed(int dtype, struct trdtype **dtrace, int nlev, 
                           int fl_idx);
static void print_struct(struct trdtype **dtrace, int nlev);
static void indent(int lev);


/*
 * local variables
 */
static int		colcount;		/* print column count */
static int		nlifconst;		/* newline if contructor */
static char		fmtbuf[80];		/* formatting buffer */
static char		*prefix;		/* prefix for each line */
static char		*obuf;			/* output buffer or NULL */
static int		identchar;		/* # chars per indent level */

/*
 * basic datatypes
 */
static char		*dtbasic[TRDTMAX + 1] = {
				"MPI_CONTIG", "MPI_VECTOR", "MPI_HVECTOR",
				"MPI_INDEX", "MPI_HINDEX", "MPI_STRUCT",
				"MPI_BYTE", "MPI_CHAR", "MPI_UNSIGNED_CHAR", 
				"MPI_SHORT", "MPI_UNSIGNED_SHORT", "MPI_INT",
				"MPI_UNSIGNED", "MPI_LOGICAL", "MPI_LONG",
				"MPI_UNSIGNED_LONG", "MPI_FLOAT","MPI_DOUBLE",
				"MPI_COMPLEX", "MPI_UB", "MPI_LB",
				"MPI_PACKED", "MPI_2INT", "", "",
				"MPI_FLOAT_INT",
				"MPI_DOUBLE_INT", "MPI_LONG_INT",
				"MPI_SHORT_INT", "MPI_LONG_DOUBLE", 
				"MPI_LONG_DOUBLE_INT", "MPI_INTEGER",
				"MPI_CHARACTER", "MPI_REAL", 
				"MPI_DOUBLE_PRECISION",  "MPI_DOUBLE_COMPLEX",
				"MPI_2INTEGER", "MPI_2REAL",
				"MPI_2DOUBLE_PRECISION", "MPI_LONG_LONG_INT",
				"MPI_UNSIGNED_LONG_LONG", "MPI_WCHAR",
			};

/*
 *	mpitr_dtypeprint
 *
 *	Function:	- print datatype trace
 *	Accepts:	- datatype index
 *			- trace datatype buffer (may be NULL if basic datatype)
 *			- number of chars to indent by
 *			- string prefix for each output line
 *			- output buffer (NULL means output to stdout)
 */
void
mpitr_dtypeprint(int4 dtype, char *dtrbuf, int4 indent, char *pfix, 
                 char *outbuf)
{
	struct trdtype	*p;			/* ptr trace datatype */

	identchar = indent;
	prefix = pfix;
	obuf = outbuf;
	
	if (dtype < 0) {
		if (obuf) {
			sprintf(obuf, "%sinvalid datatype: %d", prefix, dtype);
		} else {
			printf("%sinvalid datatype: %d\n", prefix, dtype);
		}
	}
/*
 * Handle basic datatypes.
 */
	else if (dtype <= TRDTMAX) {
		if (obuf) {
			strcpy(obuf, prefix);
			strcat(obuf, dtbasic[dtype]);
		} else {
			printf("%s%s\n", prefix, dtbasic[dtype]);
		}
	}
	else if (dtrbuf == 0) {
		if (obuf) {
			sprintf(obuf, "%sunknown datatype: %d", prefix, dtype);
		} else {
			printf("%sunknown datatype: %d\n", prefix, dtype);
		}
	}
/*
 * Recursively (depth-first) print datatype.
 */
	else {
		p = (struct trdtype *) dtrbuf;
		p++;
		colcount = 0;
		nlifconst = 0;
		if (obuf) *obuf = '\0';
		
		print_datatype(&p, 0);
	}
}

/*
 *	print_datatype
 *
 *	Function:	- print current datatype
 *			- recursive function
 *	Accepts:	- trace record (inout)
 *			- nesting level
 */
static void
print_datatype(struct trdtype **dtrbuf, int level)
{
	int		format;			/* datatype format */

	format = (*dtrbuf)->trd_dtype;

	switch(format) {

	case TRDTBYTE:
	case TRDTPACKED:
	case TRDTUB:
	case TRDTLB:
		
	case TRDTUCHAR:
	case TRDTCHAR:
	case TRDTUINT:
	case TRDTINT:
	case TRDTLONG:
	case TRDTULONG:
	case TRDTSHORT:
	case TRDTUSHORT:
	case TRDTFLOAT:
	case TRDTDOUBLE:
	case TRDTLONGDOUBLE:
	case TRDT2INT:
	case TRDTFLOATINT:
	case TRDTDOUBLEINT:
	case TRDTLONGDBLINT:
	case TRDTLONGINT:
	case TRDTSHORTINT:
	case TRDTLONGLONG:
	case TRDTULONGLONG:
	case TRDTWCHAR:

	case TRDTFLOGICAL:
	case TRDTFINTEGER:
	case TRDTFCHARACTER:
	case TRDTFREAL:
	case TRDTFDBLPREC:
	case TRDTFCOMPLEX:
	case TRDTFDBLCOMPLEX:
	case TRDTF2INTEGER:
	case TRDTF2REAL:
	case TRDTF2DBLPREC:

		print_basic(format, dtrbuf, level);
		break;

	case TRDTCONTIG:
		print_contig(dtrbuf, level);
		break;

	case TRDTVECTOR:
		print_hvector(format, dtrbuf, level, 1);
		break;

	case TRDTHVECTOR:
		print_hvector(format, dtrbuf, level, 0);
		break;

	case TRDTINDEXED:
		print_hindexed(format, dtrbuf, level, 1);
		break;

	case TRDTHINDEXED:
		print_hindexed(format, dtrbuf, level, 0);
		break;

	case TRDTSTRUCT:
		print_struct(dtrbuf, level);
		break;

	default:
		break;
	}
}

/*
 *	print_basic
 *
 *	Function:	- print basic datatype record
 *	Accepts:	- datatype label
 *			- trace record (inout)
 *			- nesting level
 */
static void
print_basic(int dtype, struct trdtype **dtrace, int nlev)
{
	nlifconst = 0;
	indent(nlev);
	if (obuf) {
		sprintf(fmtbuf, "%s\n", dtbasic[dtype]);
		strcat(obuf, fmtbuf);
	}
	else {
		printf("%s\n", dtbasic[dtype]);
	}
	colcount = 0;
	++(*dtrace);
}

/*
 *	print_contig
 *
 *	Function:	- print contiguous datatype record
 *	Accepts:	- trace record (inout)
 *			- nesting level
 */
static void
print_contig(struct trdtype **dtrace, int nlev)
{
	indent(nlev);

	sprintf(fmtbuf, "%s (%d)", dtbasic[TRDTCONTIG], (*dtrace)->trd_count);

	if (obuf) strcat(obuf, fmtbuf);
	else printf("%s", fmtbuf);

	colcount += strlen(fmtbuf);
	nlifconst = 1;

	++(*dtrace);

	print_datatype(dtrace, nlev + 1);
}

/*
 *	print_hvector
 *
 *	Function:	- print vector or hvector datatype record
 *	Accepts:	- datatype label
 *			- trace record (inout)
 *			- nesting level
 *			- vector flag
 */
static void
print_hvector(int dtype, struct trdtype **dtrace, int nlev, int fl_vec)
{
	indent(nlev);

	sprintf(fmtbuf, "%s (%d x %d, %d)", dtbasic[dtype],
			(*dtrace)->trd_count, (*dtrace + 1)->trd_length,
			DISP(fl_vec, (*dtrace + 1)->trd_stride));

	if (obuf) strcat(obuf, fmtbuf);
	else printf("%s", fmtbuf);

	colcount += strlen(fmtbuf);
	nlifconst = 1;

	*dtrace += 2;

	print_datatype(dtrace, nlev + 1);
}

/*
 *	print_hindexed
 *
 *	Function:	- print indexed or hindexed datatype record
 *	Accepts:	- datatype label
 *			- trace record (inout)
 *			- nesting level
 *			- indexed flag
 */
static void
print_hindexed(int dtype, struct trdtype **dtrace, int nlev, int fl_idx)
{
	int		i;			/* favourite index */
	int		count;			/* datatype count */

	indent(nlev);

	count = (*dtrace)->trd_count;
	++(*dtrace);

	sprintf(fmtbuf, "%s (%d)", dtbasic[dtype], count);
	
	if (obuf) strcat(obuf, fmtbuf);
	else printf("%s", fmtbuf);

	colcount += strlen(fmtbuf);

	for (i = 0; i < count; ++i) {
		sprintf(fmtbuf, " (%d, %d)", (*dtrace)->trd_length,
				DISP(fl_idx, (*dtrace)->trd_disp));

		if (obuf) strcat(obuf, fmtbuf);
		else printf("%s", fmtbuf);

		colcount += strlen(fmtbuf);
		++(*dtrace);
	}

	nlifconst = 1;

	print_datatype(dtrace, nlev + 1);
}

/*
 *	print_struct
 *
 *	Function:	- print struct datatype record
 *	Accepts:	- trace record (inout)
 *			- nesting level
 */
static void
print_struct(struct trdtype **dtrace, int nlev)
{
	int		i;			/* favourite index */
	int		count;			/* datatype count */
 
	indent(nlev);

	count = (*dtrace)->trd_count;
	++(*dtrace);

	sprintf(fmtbuf, "%s (%d)\n", dtbasic[TRDTSTRUCT], count);

	if (obuf) strcat(obuf, fmtbuf);
	else printf("%s", fmtbuf);

	colcount = 0;
	nlifconst = 0;

	for (i = 0; i < count; ++i) {

		indent(nlev + 1);

		sprintf(fmtbuf, "(%d, %d)",
				(*dtrace)->trd_length, (*dtrace)->trd_disp);

		if (obuf) strcat(obuf, fmtbuf);
		else printf("%s", fmtbuf);

		colcount += strlen(fmtbuf);
		++(*dtrace);

		print_datatype(dtrace, nlev + 2);
	}
}

/*
 *	indent
 *
 *	Function:	- indent line according to level
 *	Accepts:	- indentation level
 */
static void
indent(int lev)
{
	lev *= identchar;

	if (nlifconst) {
		if (obuf) strcat(obuf, "\n");
		else printf("\n");

		nlifconst = 0;
		colcount = 0;
	}

	if (obuf) {
		if (colcount == 0) strcat(obuf, prefix);
		if (colcount < lev) {
			for (; colcount < lev; ++colcount) strcat(obuf, " ");
		} else if (colcount > 0) {
			strcat(obuf, " ");
		}
	} else {
		if (colcount == 0) printf("%s", prefix);
		if (colcount < lev) {
			for (; colcount < lev; ++colcount) printf(" ");
		} else if (colcount > 0) {
			printf(" ");
		}
	}
}
