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
 *	LAM
 *	SFH
 *
 *	$Id: tstdio.h,v 6.5 2002/10/09 20:57:26 brbarret Exp $
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * tstdio.h - Trollius standard I/O include file
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#ifndef _TSTDIO
#define	_TSTDIO

#include <lam_config.h>
/*
 * constants
 */
#ifndef NULL
#define NULL		(0)
#endif

#define _TNFILE		(16)		/* maximum number of open streams */
#define TOPEN_MAX	_TNFILE		/* equivalent to _TNFILE */
#define TBUFSIZ		(1024)		/* default buffer size */

#ifndef EOF
#define EOF		(-1)		/* end-of-file indicator */
#endif

#ifndef EOS
#define EOS		('\0')		/* end-of-string indicator */
#endif

/*
 * lseek() origins
 */
#ifndef SEEK_SET
#define	SEEK_SET	0		/* from beginning of file */
#define	SEEK_CUR	1		/* from current location */
#define	SEEK_END	2		/* from end of file */
#endif

/*
 * FILE structure flags
 */
#define	_TIOREAD	0x0001		/* file may be read from */
#define	_TIOWRT		0x0002		/* file may be written to */
#define	_TIOBIN		0x0004		/* file is in "binary" mode */
#define	_TIODEV		0x0008		/* file is a character device */
#define _TIOREMOTE	0x0010		/* File resides on remote node */
#define	_TIORW		0x0080		/* last i/o was 0:read/1:write */
#define	_TIOFBF		0x0100		/* i/o is fully buffered */
#define	_TIOLBF		0x0100		/* i/o is line buffered */
#define	_TIONBF		0x0400		/* i/o is not buffered */
#define	_TIOMYBUF	0x0800		/* standard buffer */
#define	_TIOEOF		0x1000		/* EOF has been reached */
#define	_TIOERR		0x4000		/* an error has occured */

/*
 * FILE structure
 */
typedef	struct
{
	int		_cnt;		/* # of bytes in buffer */
	unsigned char	*_ptr;		/* current buffer pointer */
	unsigned char	*_base;		/* base of file buffer */
	unsigned int	_flag;		/* file status flags */
	int		_file;		/* file handle */
	int		_bsiz;		/* buffer size */
	unsigned char	_ch;		/* tiny buffer, for "unbuffered" i/o */
} TFILE;

#ifdef __cplusplus
extern "C" {
#endif

extern TFILE		*tfdopen __ARGS((int fd, const char *mode));
extern TFILE		*tfopen __ARGS((const char *fname, const char *mode));
extern TFILE		*tfreopen __ARGS((const char *fname,
					const char *mode, TFILE *fp));
extern char		*tfgets __ARGS((char *buf, int len, TFILE *fp));
extern char		*tgets __ARGS((char *buf));
extern char		*tsprintf __ARGS((char *buf, const char *fmt, ...));
extern char		*tvsprintf __ARGS((char *buf, const char *fmt, ...));
extern int		tfclose __ARGS((TFILE *fp));
extern int		tfflush __ARGS((TFILE *fp));
extern int		tfgetc __ARGS((TFILE *fp));
extern int		tfprintf __ARGS((TFILE *fp, const char *fmt, ...));
extern int		tfputc __ARGS((int c, TFILE *fp));
extern int		tfputs __ARGS((const char *buf, TFILE *fp));
extern int		tfread __ARGS((char *buf, int esize,
					int nitem, TFILE *fp));
extern int		tfscanf __ARGS((TFILE *fp, const char *fmt, ...));
extern int		tfseek __ARGS((TFILE *fp, long offset, int whence));
extern int		tfungetc __ARGS((int c, TFILE *fp));
extern int		tfwrite __ARGS((const char *buf, int esize,
					int nitem, TFILE *fp));
extern int		tprintf __ARGS((const char *fmt, ...));
extern int		tscanf __ARGS((const char *fmt, ...));
extern int		tsetlinebuf __ARGS((TFILE *fp));
extern int		tsetvbuf __ARGS((TFILE *fp, unsigned char *buf,
					int type, int size));
extern int		tsscanf __ARGS((char *buf, const char *fmt, ...));
extern int		tvfprintf __ARGS((TFILE *fp, const char *fmt, ...));
extern int		tvprintf __ARGS((const char *fmt, ...));
extern long		tftell __ARGS((TFILE *fp));
extern void		tfcloseall __ARGS((void));
extern void		trewind __ARGS((TFILE *fp));
extern void		tsetbuf __ARGS((TFILE *fp, unsigned char *buf));
extern void		tsetbuffer __ARGS((TFILE *fp,
					unsigned char *buf, int size));

  /*
   * 24 April 2001 JMS
   *
   * Glen Maynard noticed that tputs is a termcap/ncurses function.  A
   * [uncommon] case where this name clash can cause problems is when
   * linking LAM/MPI user programs with readline, which requires
   * either -ltermcap or -lncurses (I think those are two different
   * implementations of the same interface).  Particularly if the
   * readline library has a termcap/ncurses dependency automatically
   * linked in, and therefore the user doesn't supply it (e.g., Debian
   * does this).  Hence, the user's command line goes like this:
   *
   *     mpicc foo.c -lreadline
   *
   * which effectively expands to this (since the linker automatically
   * adds -lncurses at the end):
   *
   *     gcc foo.c -lreadline ...LAM libraries... -lncurses
   *
   * which puts ncurses at the back of the list, so readline calls
   * tputs() in LAM.  Needless to say, LAM's tputs() and ncurses'
   * tputs() do not do the same thing at all.  Oops.
   *
   * The easiest way to fix this is to make LAM's tputs() be a
   * #define, and have the real function name be lam_tputs().  This
   * won't fix *every* case (e.g., if the user includes <tstdio.h> and
   * tries to call the termcap/ncurses tputs(), but a) it will fix the
   * readline issue, and b) I don't think anyone will do that.  :-)
   */
#define tputs lam_tputs
extern int		lam_tputs __ARGS((const char *buf));
#ifdef __cplusplus
}
#endif

/*
 * standard streams
 */
#define tstdin	(&_tiob[0])
#define tstdout	(&_tiob[1])
#define tstderr	(&_tiob[2])
#define tstdprn	(&_tiob[3])
#define tstdaux	(&_tiob[4])

/*
 * external variables
 */
extern TFILE	_tiob[];

/*
 * stream macros
 */
#define tclearerr(fp)	((void) ((fp)->_flag &= ~(_TIOERR|_TIOEOF)))
#define tfeof(fp)	((fp)->_flag & _TIOEOF)
#define tferror(fp)	((fp)->_flag & _TIOERR)
#define tfileno(fp)	((fp)->_file)

/*
 * aliases
 */
#define	tgetc			tfgetc
#define	tputc			tfputc
#define	tgetchar()		tfgetc(tstdin)
#define	tputchar(c)		tfputc((c),tstdout)
#define	tungetc			tfungetc
#define	tungetchar(c)		tfungetc((c),tstdin)

#endif /* _TSTDIO */
