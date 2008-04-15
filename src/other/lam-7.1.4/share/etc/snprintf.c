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
 * $Id: snprintf.c,v 6.3 2002/10/21 04:48:07 brbarret Exp $ 
 *
 * This file is for systems that do not provide snprintf().  #@%$@#%$#@!!!
 * It was originally copied from:
 *
 *   http://www.cu-online.com/~jwthomp/downloads/jeffos/lib/snprintf.c
 *
 * Minor modifications made for portability reasons.
 * Original copyright and license below.
 */

/* Copyright 1997, Brian J. Swetland <swetland@neog.com>                 
** Free for non-commercial use.  Share and enjoy                         
**
** Minimal snprintf() function.
** %s - string     %d - signed int    %x - 32bit hex number (0 padded)
** %c - character  %u - unsigned int  %X -  8bit hex number (0 padded) 
*/

#include <lam_config.h>

/*
 * We only need this file if snprintf isn't already present in the
 * underlying OS/compiler.
 */
#if !LAM_HAVE_SNPRINTF

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <etc_misc.h>


static char hexmap[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' 
};


int 
#if LAM_WANT_PROTOS
/* This used to be va_snprintf, but it didn't seem to want to work
   with the snprintf() wrapper on some arches (e.g., Solaris 2.5.1).
   Since we don't use va_snprintf(), the solution was simple
   enough...*/
lam_snprintf(char *b, int l, char *fmt, ...)
#else
lam_snprintf(b, l, fmt, va_alist)
char *b;
int l;
char *fmt;
va_dcl
#endif
{
  int n,i;
  int total = l;
  unsigned u;    
  char *t,d[10];    
  va_list pvar;
  
#if __STDC__
  va_start(pvar, fmt);
#else
  va_start(pvar);
#endif
  
  if(!fmt || !b || (l < 1)) return -1; 
  
  while(l && *fmt) {
    if(*fmt == '%'){
      fmt++;
      if(!(--l)) break;
      
      switch(*fmt){
      case 's': /* string */
	t = va_arg(pvar, char *);
	while(l && *t){
	  *b++ = *t++;
	  l--;
	}
	break;
	
      case 'c': /* single character */
	*b++ = (char) va_arg(pvar, int);
	l--;                
	break;
	
      case 'x': /* 8 digit, unsigned 32bit hex integer */
	if(l < 8) { l = 0; break; }
	u = (unsigned int) va_arg(pvar, unsigned int);
	for(i=7;i>=0;i--){
	  b[i] = hexmap[u & 0x0F];
	  u >>= 4;
	}
	b+=8;
	l-=8;                
	break;
	
      case 'd': /* signed integer */
	n = va_arg(pvar, int);
	if(n < 0) {
	  u = -n;
	  *b++ = '-';
	  if(!(--l)) break;                    
	} else {
	  u = n;
	}
	goto u2;                
	
      case 'u': /* unsigned integer */
	u = (unsigned int) va_arg(pvar, unsigned int);                
      u2:
	i = 9;
	do {
	  d[i] = (u % 10) + '0';
	  u /= 10;
	  i--;
	} while(u && i >= 0);
	while(++i < 10){
	  *b++ = d[i];
	  if(!(--l)) break;                    
	}
	break;
	
      case 'U':
	u = (unsigned int) va_arg(pvar, unsigned int);
	i = 9;
	d[8] = d[7] = d[6] = ' ';
	do {
	  d[i] = (u % 10) + '0';
	  u /= 10;
	  i--;
	} while(u && i >= 0);
	i = 5;
	while(++i < 10){
	  *b++ = d[i];
	  if(!(--l)) break;
	}
	break;
	
      case 'X': /* 2 digit, unsigned 8bit hex int */
	if(l < 2) { l = 0; break; }
	n = va_arg(pvar, int);
	*b++ = hexmap[(n & 0xF0) >> 4];
	*b++ = hexmap[n & 0x0F];
	l-=2;                
	break;
      default:
	*b++ = *fmt;                
      }
    } else {
      *b++ = *fmt;
      l--;            
    }
    fmt++;            
  }
  *b = 0;
  
  va_end(pvar);
  
  if (l <= 0)
    return -1;
  else
    return (total - l);
}

#else

/*
 * If the compiler/OS already has snprintf, this file will be empty.
 * However, we don't want an "empty translation unit" kind of warning
 * from the compiler, so put a bogus variable here.
 */

int lam_you_have_snprintf___you_rock = 1;

#endif
