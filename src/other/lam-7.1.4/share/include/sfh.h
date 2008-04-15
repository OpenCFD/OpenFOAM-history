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
 * $Id: sfh.h,v 6.15 2003/07/05 21:54:26 jsquyres Exp $
 *
 *	Function:	- constants, templates and prototypes
 */

#ifndef LAM_SFH_H
#define LAM_SFH_H

#include <lam_config.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

enum {
    SFH_INET,
    SFH_UNIX
};

#ifdef __cplusplus
extern "C" {
#endif

extern void	sfh_itoa __ARGS((int, char *));

extern int	sfh_argv_add __ARGS((int *, char ***, char *));
extern int	sfh_argv_count __ARGS((char **));
extern void	sfh_argv_free __ARGS((char **));
extern char	**sfh_argv_break __ARGS((char *, int));
extern char	**sfh_argv_dup __ARGS((char **));
extern char	*sfh_argv_glue __ARGS((char **, int, unsigned int));
extern unsigned int sfh_argv_len __ARGS((char **));
extern char 	**sfh_argv_break_quoted __ARGS((char *, int, char[]));
extern char	*sfh_argv_quote __ARGS((char *, char[]));

extern char	*sfh_path_find __ARGS((char *, char **, int));
extern char	*sfh_path_env_find __ARGS((char *, int));
extern char	*sfh_path_findv __ARGS((char *, char **, int, char **));
extern char	*sfh_path_env_findv __ARGS((char *, int, char **, char *));

extern int	sfh_sock_open_srv_inet_stm __ARGS((int *, int));
extern int	sfh_sock_open_clt_inet_stm __ARGS((unsigned char *, int));
extern int	sfh_sock_open_srv_unix_stm __ARGS((char *));
extern int	sfh_sock_open_clt_unix_stm __ARGS((char *));
extern int	sfh_sock_accept_tmout __ARGS((int, int));
extern int	sfh_sock_accept_peer_tmout __ARGS((int, int, 
						   struct sockaddr *, int *));
extern int	sfh_sock_open_srv_inet_dgm __ARGS((int *));
extern int	sfh_sock_open_clt_inet_dgm __ARGS((void));
extern void	sfh_sock_fill_inet_addr __ARGS((unsigned char *, int,
						struct sockaddr_in *));
extern int	sfh_sock_set_buf_size __ARGS((int , int, int, unsigned int));
extern int	sfh_send_fd __ARGS((int, int));
extern int	sfh_recv_fd __ARGS((int));

#ifdef __cplusplus
}
#endif

#endif	/* LAM_SFH_H */
