/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: filedr.c,v 6.11 2003/01/13 19:15:06 jsquyres Exp $
 *
 *	Function:	- file system diagnostic tool (file doctor)
 *			- tests the remote file system functions
 *
 *	Examples:	  filedr -c -t -rw -g ps -l 30
 *			  filedr -ro -f junk -m 664
 *			  filedr -v -wr -f junk -l 1000
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <freq.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 * local functions
 */
static void		help();		/* print help */
static void		fdr_close();	/* close file */
static void		fdr_init();	/* init flags based on options */
static void 		fdr_open();	/* open file */
static void 		fdr_read();	/* read data from file */
static void		fdr_write();	/* write data into file */
static int		otod();		/* converts octal string to dec */

/*
 * local variables
 */
static char		*usage =
"filedr [-chtv] -ro/wo/rw/wr [-f filename] [-g flags] [-m modes] [-l length]\n";

static int		fl_close;	/* close file */
static int		fl_rdwr;	/* do read then write */
static int		fl_read;	/* file read flag */
static int		fl_stdfile;	/* stdin, stdout, stderr */
static int		fl_time;	/* timing results */
static int		fl_verbose;	/* verbose flag */
static int		fl_write;	/* file write flag */
static int		fl_wrrd;	/* do write then read */

static int		flags;		/* open file with flags	*/
static int		modes;		/* open file with modes */
static int		numbytes;	/* num of bytes to rd/wr */
static int		handle;		/* file handle/udesc */
static char		*filename;	/* file name */
static char		*buffer;	/* read-write buffer */

/*
 *	main
 */
int
main(argc, argv)

int			argc;
char			*argv[];

{
/*
 * Parse command line.
 */
	validopts("chtvN");
	followed("fgml");
	twoletter("row");
	twoletter("wor");

	if (do_args(&argc, argv) || (errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, usage);
		exit(errno);
	}
/*
 * Check for help request.
 */
	if (opt_taken('h')) {
		help();
		kexit(0);
	}
/*
 * Attach to kernel.
 */
	if (kinit(0)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "filedr", hostname, NULL);
	  exit(0);
	}
/*
 * Initialize flags based on selected options.
 */
	fdr_init();
/*
 * Perform specified file operations.
 */
	if (!fl_stdfile) {
		fdr_open();
	}

	if (numbytes > 0) {

		if (fl_read || fl_rdwr) {
			fdr_read();
/*
 * Close and open file again.
 */
			if (fl_rdwr) {

				if ((!fl_stdfile) && fl_close) {
	    				fdr_close();
					flags &= ~LAM_O_CREAT;
					fdr_open();
            			}

				fdr_write();
			}
		} 

		if (fl_write || fl_wrrd) {
			fdr_write();

			if (fl_wrrd) {
/* 
 * Close and open file again.
 */
				if ((!fl_stdfile) && fl_close) {
					fdr_close();
					flags &= ~LAM_O_CREAT;
					fdr_open();
				}

				fdr_read();
			}
		}
	}

	if ((!fl_stdfile) && fl_close) {
		fdr_close();
	}

	return(0);
}

/*
 *	fdr_open
 *
 *	Function:	- opens non-stdio file
 */
static void
fdr_open()

{
	double		timeval;

	if (fl_time) timeval = ttime();

	VERBOSE("opening %s ...\n", filename);
	if ((handle = lam_rfopen(filename, flags, modes)) < 0)
			lamfail("filedr (lam_rfopen)"); 

	if (fl_time) timeval = ttime() - timeval;

	if (fl_time) {
		printf("opened %s in %f seconds\n", filename, timeval);
	}
} 

/*
 *	fdr_close
 *
 *	Function:	- closes file
 */
static void
fdr_close()

{
	double		timeval;

	if (fl_time) timeval = ttime();

	VERBOSE("closing %s ...\n", filename);
	if (lam_rfclose(handle)) lamfail("filedr (lam_rfclose)");

	if (fl_time) timeval = ttime() - timeval;

	if (fl_time) {
                printf("closed %s in %f seconds\n", filename, timeval);
        }
}

/*
 *	fdr_read()
 *
 *	Function:	- reads data from file
 */
static void
fdr_read()

{
	double		timeval;
	int		i;
	int		ret_bytes;
/*
 * Read numbytes amount of data.
 */
	buffer = malloc ((unsigned) numbytes);
	if (buffer == 0) lamfail("filedr (malloc)");

	for (i = 0; i < numbytes; i++) {
		buffer[i] = ' ';
	}

	VERBOSE("reading %d bytes from %s ...\n", numbytes, filename);
	if (fl_time) timeval = ttime();
	ret_bytes = lam_rfread(handle, buffer, numbytes);
	if (ret_bytes < 0) lamfail("filedr (lam_rfread)");
	if (fl_time) timeval = ttime() - timeval;

        if (fl_time) {
		printf("read %d bytes in %f seconds\n", ret_bytes, timeval);
        }

	free(buffer);
}

/*
 *	fdr_write
 *
 *	Function:	- writes data to a file
 */
static void
fdr_write()

{
	double		timeval;
	int 		i;
	int		ret_bytes;

        buffer = malloc ((unsigned) numbytes);
        if (buffer == NULL) lamfail("filedr (malloc)");

	for (i = 0; i < numbytes; i++) {
		buffer[i] = (char) (i % 256);
	}

	VERBOSE("writing %d bytes to %s ...\n", numbytes, filename);
	if (fl_time) timeval = ttime();
	ret_bytes = lam_rfwrite(handle, buffer, numbytes);
	if (ret_bytes < 0) lamfail("filedr (lam_rfwrite)");
	if (fl_time) timeval = ttime() - timeval;

	if (fl_time) {
        	printf("wrote %d bytes in %f seconds\n", ret_bytes, timeval);
        }

	free(buffer);
}
	
/*
 *	help
 *
 *	Function:	- prints useful information on this command
 */
static void
help()

{
	printf("\nSynopsis:\tfiledr <options>\n");
	printf("\nDescription:\tTest and time remote file system access.\n");
	printf("\nOptions:\t-h\t\tPrint this message.\n");
	printf("\t\t-c\t\tDo not close the file.\n");
	printf("\t\t-t\t\tTime file operations.\n");
	printf("\t\t-v\t\tBe verbose.\n");
	printf("\t\t-ro\t\tRead-only the file. (default)\n");
	printf("\t\t-wo\t\tWrite-only the file.\n");
	printf("\t\t-rw\t\tRead then write the file.\n");
	printf("\t\t-wr\t\tWrite then read the file.\n");
	printf("\t\t-f <filename>\tWork on this file.\n");
	printf("\t\t-g <flags>\tOpen file with flags.\n");
	printf("\t\t-m <modes>\tCreate with permission modes (octal).\n");
	printf("\t\t-l <#bytes>\tSet transfer length.\n");
	printf("\nOpen Flags:\tstring of characters from the set:\n");
	printf("\t\t\tp,c,t,x,e\n");
	printf("\t\tfor:\n");
	printf("\t\t\tappend, create, trunc, excl, reuse\n");
}

/*
 *	fdr_init
 *
 *	Function:	- sets option flags and parameters
 */
static void
fdr_init()

{
	char 	*str_flags;
	char 	*str_modes;
	int 	other_chr;
/*
 * Check for switches.
 */
	fl_close = !opt_taken('c');
	fl_time = opt_taken('t');

	fl_verbose = opt_taken('v');
/*
 * Check for parameters.
 */
	fl_stdfile = FALSE;
	fl_read = FALSE;
	fl_write = FALSE;
	fl_rdwr = FALSE;
	fl_wrrd = FALSE;
	flags = 0;

	other_chr = get_2nd_let('r');

	if (other_chr == 'o') {
		fl_read = TRUE;
		flags = flags | LAM_O_RDONLY;
	} else if (other_chr == 'w') {
		fl_rdwr = TRUE;
		flags = flags | LAM_O_RDWR;
	} else {
		other_chr = get_2nd_let('w');

		if (other_chr == 'o') {
			fl_write = TRUE;
			flags = flags | LAM_O_WRONLY;
		} else if (other_chr == 'r') {
			fl_wrrd = TRUE;
			flags = flags | LAM_O_RDWR;
		} else {
			fprintf(stderr, usage);
			kexit(EUSAGE);
		}
	}

	if (opt_taken('f')) {
		filename = getparam('f');

		if (strcmp(filename, "stdin") == 0) {
			handle = 0;
			fl_stdfile = TRUE;
		} else if (strcmp(filename, "stdout") == 0) {
			handle = 1;
			fl_stdfile = TRUE;
		} else if (strcmp(filename, "stderr") == 0) {
			handle = 2;
			fl_stdfile = TRUE;
		} else {
			handle = -1;
		}
	}

	else {
		filename = "testfile";
		VERBOSE("operating on default file 'testfile'\n");
	}

	if (opt_taken('g')) {
		str_flags = getparam('g');
		if (strchr(str_flags,'p') != NULL)
				flags = flags | LAM_O_APPEND;
		else if (strchr(str_flags,'c') != NULL)
				flags = flags | LAM_O_CREAT;
		else if (strchr(str_flags,'t') != NULL)
				flags = flags | LAM_O_TRUNC;
		else if (strchr(str_flags,'x') != NULL)
				flags = flags | LAM_O_EXCL;
		else if (strchr(str_flags,'e') != NULL)
				flags = flags | LAM_O_REUSE;
		else {
			fprintf(stderr, usage);
			kexit(EUSAGE);
		}
	}
	
	if (opt_taken('m')) {
		str_modes = getparam('m');

		if ((modes = otod(str_modes)) == -1) {
			fprintf(stderr, "filedr: modes must be in octal\n");
			kexit(EUSAGE);
		}
	} else {
		modes = 0644;
	}

	if (opt_taken('l')) {
		if (intparam('l', &numbytes)) lamfail("filedr (intparam)");
	} else {
		numbytes = 1;
	}
}

/*
 *	otod
 *
 *	Function:	- converts a string of octal digits into decimal number
 *			- input string must be unsigned and may begin
 *			  with a zero
 */
static int 
otod(s)

char			*s;

{
	int		dec;
	int		mult;
	int		n;
	char		c;
	char		*p;

	if (s[0] == '0') {
		s++;
	}

	n = strlen(s);

	if (n < 1) {
		return(0);
	}

	p = s + n - 1;
	dec = 0;
	mult = 1;

	while (p + 1 != s) {
		c = *p;

		if ((c >= '0') && (c <= '7')) {
			n = c - '0';
		} else {
			return(-1);
		}

		dec = dec + (n * mult);
		mult *= 8;
		p = p-1;
	}

	return(dec);
}
