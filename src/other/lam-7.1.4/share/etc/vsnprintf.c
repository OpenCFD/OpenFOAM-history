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
 *
 *	$Id: vsnprintf.c,v 6.3 2003/05/21 20:28:17 brbarret Exp $
 * 
 *	Function: - this is not exactly vsprintf(), but it's in the
 *	same spirit, so we gave it the same name.
 */

#include <lam_config.h>

#include <stdio.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <stdlib.h>
#include <syslog.h>
#include <string.h>

#include <etc_misc.h>


/* 
 * lam_vsnprintf
 *
 * Make a good guess about how long a printf-style varags formatted
 * string will be once all the % escapes are filled in.  We don't
 * handle every % escape here, but we handle enough, and then add a
 * fudge factor in at the end.  When we have that, alloc out a buffer
 * and snprintf into it.  The whole reason for this routine is because
 * we can't count on vsnprintf to be on every system.  :-( Ok, it's
 * not exactly the same as vsnprintf, but it's in the same spirit, so
 * it's ok to use a derrivative of the name...  
 */
char *
lam_vsnprintf(char *format, va_list arglist)
{
  int i, len;
  char *sarg;
  int iarg;
  long larg;
  double darg;
  float farg;

  /* Important: keep a copy of the original arglist because when we
     traverse through the original arglist, it has internal state that
     knows where it is in the list of args.  At the end of this
     routine, we'll need the whole list in order to call vsprintf(),
     and there doesn't appear to be a way to "rewind" a va_alist. 

     Copy order taken from Autoconf docs
  */
  va_list arglist2;
#if LAM_HAVE_VA_COPY
  va_copy(arglist2, arglist);
#elif LAM_HAVE_UNDERSCORE_VA_COPY
  __va_copy(arglist2, arglist);
#else
  memcpy (&arglist2, &arglist, sizeof(va_list));
#endif

  /* Start off with a fudge factor of 128 to handle the % escapes that
     we aren't calculating here */

  len = strlen(format) + 128;
  for (i = 0; i < strlen(format); ++i) {
    if (format[i] == '%' && i + 1 < strlen(format) && format[i + 1] != '%') {
      ++i;
      switch(format[i]) {
      case 's':
	sarg = va_arg(arglist, char*);
	/* If there's an arg, get the strlen, otherwise we'll use (null) */
	if (sarg != NULL)
	  len += strlen(sarg);
	else
	  len += 5;
	break;
	
      case 'd':
      case 'i':
	iarg = va_arg(arglist, int);
	/* Alloc for minus sign */
	if (iarg < 0)
	  ++len;
	/* Now get the log10 */
	do {
	  ++len;
	  iarg /= 10;
	} while (iarg != 0);
	break;
	
      case 'x':
      case 'X':
	iarg = va_arg(arglist, int);
	/* Now get the log16 */
	do {
	  ++len;
	  iarg /= 16;
	} while (iarg != 0);
	break;
	
      case 'f':
	farg = va_arg(arglist, int);
	/* Alloc for minus sign */
	if (farg < 0) {
	  ++len;
	  farg = -farg;
	}
	/* Alloc for 3 decimal places + '.' */
	len += 4;
	/* Now get the log10 */
	do {
	  ++len;
	  farg /= 10.0;
	} while (farg != 0);
	break;
	
      case 'g':
	darg = va_arg(arglist, int);
	/* Alloc for minus sign */
	if (darg < 0) {
	  ++len;
	  darg = -darg;
	}
	/* Alloc for 3 decimal places + '.' */
	len += 4;
	/* Now get the log10 */
	do {
	    ++len;
	    darg /= 10.0;
	} while (darg != 0);
	break;
	
      case 'l':
	/* Get %ld %lx %lX %lf */
	if (i + 1 < strlen(format)) {
	  ++i;
	  switch(format[i]) {
	  case 'x':
	  case 'X':
	    larg = va_arg(arglist, int);
	    /* Now get the log16 */
	    do {
	      ++len;
	      larg /= 16;
	    } while (larg != 0);
	    break;
	    
	  case 'f':
	    darg = va_arg(arglist, int);
	    /* Alloc for minus sign */
	    if (darg < 0) {
	      ++len;
	      darg = -darg;
	    }
	    /* Alloc for 3 decimal places + '.' */
	    len += 4;
	    /* Now get the log10 */
	    do {
	      ++len;
	      darg /= 10.0;
	    } while (darg != 0);
	    break;
	    
	  case 'd':
	  default:
	    larg = va_arg(arglist, int);
	    /* Now get the log10 */
	    do {
	      ++len;
	      larg /= 10;
	    } while (larg != 0);
	    break;
	  }
	}
	
      default:
	break;
      }
    }
  }

  /* Wasn't that simple?  Now malloc out a string and do the final
     formatting into that string. */
  
  sarg = malloc(len);
  vsprintf(sarg, format, arglist2);

  /* Return the new string */

  return sarg;
}
