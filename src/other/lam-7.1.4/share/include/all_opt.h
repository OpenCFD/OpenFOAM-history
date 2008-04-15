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
 * $Id: all_opt.h,v 6.10 2003/03/22 15:31:55 jsquyres Exp $
 *
 *	Function:	- constants & templates for all_opt
 */

#ifndef LAM_ALL_OPT
#define LAM_ALL_OPT

#include <lam_config.h>

/*
 * constants
 */
#define AOVARNUM	-1			/* variable # arguments */

/* 
 * structures
 */
struct ao_desc {
	int		ao_ntaken;		/* # options taken */
	struct ao_opt	*ao_opttop;		/* options list top */
	struct ao_opt	*ao_optend;		/* options list end */
	struct ao_opt	*ao_optcur;		/* current option */

        int		ao_flags;		/* flags describing actions */
#define AOPRESERVE_ARGV 0x001
#define AOIGN_UNRECOG   0x002

        char            *ao_argv0;              /* copy of argv[0] */

	int		ao_unusedc;		/* unused argc */
	char		**ao_unusedv;		/* unused argv */

	char		**ao_argv_dup;		/* memory to be freed */
	char		**ao_string_dups;	/* memory to be freed */

	int		ao_tailc;		/* tail argc */
	char		**ao_tailv;		/* tail argv */
};

typedef struct ao_desc	OPT;

struct ao_opt {
	char		*aoo_string;		/* option string */
	int		aoo_flags;		/* option flags */
#define AOINT		0x001			/* integer parameters */
#define AOMUST		0x002			/* must be taken */
#define AODOARGS	0x004			/* do_args back. comp. */
#define AOTAKEN		0x100			/* option taken */
	int		aoo_nmaxparam;		/* max. # parameters */
	int		aoo_ninst;		/* # instances taken */
	struct ao_inst	*aoo_insts;		/* instances array */
	struct ao_opt	*aoo_mutex;		/* mutex options list */
	struct ao_opt	*aoo_next;		/* next option */
};

struct ao_inst {
	int		aoi_nparam;		/* # parameters */
	void		*aoi_params;		/* parameters array */
};

#ifdef __cplusplus
extern "C" {
#endif

  extern char *ao_argv0(OPT *aod);
  extern char *ao_chosen(OPT *aod, const char *opt);
  extern OPT *ao_init(void);
  extern int ao_exam_isopt(OPT *aod, const char *opt);
  extern int ao_exam_nparams(OPT *aod, const char *opt, int *nparams);
  extern void ao_free(OPT *aod);
  extern int ao_intparam(OPT *aod, const char *opt, int inst, 
			 int idx, int *iparam);
  extern int ao_ninsts(OPT *aod, const char *opt);
  extern int ao_nparams(OPT *aod, const char *opt, int inst);
  extern int ao_ntaken(OPT *aod);
  extern char *ao_param(OPT *aod, const char *opt, int inst, int idx);
  extern int ao_parse(OPT *aod, int *argc, char **argv);
  extern int ao_setflags(OPT *aod, int flags);
  extern int ao_setopt(OPT *aod, const char *opt, const char *mutex, 
		       int np, int flags);
  extern int ao_setopt1(OPT *aod, const char *opt, const char *mutex, 
			int np, int flags);
  extern int ao_tail(OPT *aod, int *tailc, char ***tailv);
  extern int ao_taken(OPT *aod, const char *opt);
  extern int ao_unused(OPT *aod, int *unusedc, char ***unusedv);

#ifdef __cplusplus
}
#endif

#endif	/* LAM_ALL_OPT */
