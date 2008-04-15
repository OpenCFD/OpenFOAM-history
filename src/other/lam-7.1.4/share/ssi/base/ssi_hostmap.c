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
 * $Id: ssi_hostmap.c,v 1.3 2003/11/26 23:58:33 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <typical.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi.h>
#include <lamnet.h>
#include <all_list.h>
#include <boot.h>
#include <lam_network.h>


/*
 * External variables
 */
extern int lam_ssi_base_hostmap_param_index;


/*
 * Private variables
 */
static int read_hostmap = 0;
static struct lamnode *hostmap = NULL;
static int hostmap_len = 0;


/*
 * Private functions
 */
static int resolve_hosts(struct lamnode *hostmap, int hostmap_len);


/*
 * Take an IP address as input and see if it exists in the
 * SSI-parameter-specified hostmap (default to
 * $sysconfdir/lam-hostmap.txt).  If it does, see if it has a
 * corresponding key name.  If it does, return that IP address.
 * Otherwise, return the original IP address.
 */
struct in_addr
lam_ssi_base_hostmap(struct in_addr *addr, char *keyname)
{
  int i;
  int happy;
  ELEM search;
  ELEM *result;
  char addr_ntoa[16];
  char *new_hostname;
  char *filename = 
    lam_ssi_base_param_lookup_string(lam_ssi_base_hostmap_param_index);
  struct in_addr ret = *addr;
  struct hostent *h;
  FILE *fp;

  /* Check to see if the filename exists */

  if (filename == NULL || strlen(filename) == 0)
    return ret;
 
  /* If we haven't read in the hostmap file, do so */

  if (read_hostmap == 0) {
    read_hostmap = 1;

    /* Check to see if the hostmap exists -- avoid an error message
       from bhostparse */

    fp = fopen(filename, "r");
    if (fp == NULL) {
      lam_debug(lam_ssi_did, "mpi: could not read hostmap: %s", filename);
      return ret;
    }
    fclose(fp);

    /* Read the hostmap */

    happy = bhostparse(filename, &hostmap, &hostmap_len);
    if (lam_ssi_verbose > 5) {
      if (happy == 0)
        lam_debug(lam_ssi_did, "mpi: read hostmap: %s", filename);
      else {
        lam_debug(lam_ssi_did, "mpi: could not read hostmap: %s", filename);
        return ret;
      }
    }

    /* Resolve all the hosts found in it */

    happy = resolve_hosts(hostmap, hostmap_len);
    if (happy == 0) {
      if (lam_ssi_verbose > 5)
        lam_debug(lam_ssi_did, "mpi: resolved all hosts in hostmap");
    } else {
      if (lam_ssi_verbose > 5)
        lam_debug(lam_ssi_did, "mpi: unable to resolve all hosts in hostmap");
      lam_deallocate_nodes(&hostmap, &hostmap_len);
      hostmap = NULL;
      hostmap_len = 0;
    }
  }

  /* Now try to lookup the host in the hostmap */

  if (hostmap != NULL) {
    for (i = 0; i < hostmap_len; ++i) {
      if (memcmp(&(hostmap[i].lnd_addr.sin_addr.s_addr), &(addr->s_addr),
                 sizeof(hostmap[i].lnd_addr.sin_addr.s_addr)) == 0) {
        if (lam_ssi_verbose > 5) {

          /* Some inet_ntoa() implementations overwrite the same
             buffer every time they are invoked.  For lazy
             implementations of printf(), this can cause problems if
             you map multiple %s's to multiple calls to inet_ntoa().
             [sigh]  So save a copy of inet_ntoa(*addr) for use
             later. */

          snprintf(addr_ntoa, sizeof(addr_ntoa), "%s", inet_ntoa(*addr));
          lam_debug(lam_ssi_did, "mpi: found host in map: %s", addr_ntoa);
        }

        /* We found it in the map -- see if we have a matching key */

        search.key = keyname;
        result = al_find(hostmap[i].lnd_keyval, &search);
        if (result == NULL) {
          if (lam_ssi_verbose > 5)
            lam_debug(lam_ssi_did, "mpi: host does not have \"%s\" attribute",
                      keyname);
        } else {
          new_hostname = result->value;

          /* Translate to an IP address */

          if ((h = lam_gethostbyname(new_hostname)) != NULL) {
            memcpy((char *) &ret, h->h_addr_list[0], sizeof(ret));
            if (lam_ssi_verbose > 5)
              lam_debug(lam_ssi_did, "mpi: remapped %s -> %s (%s)", 
                        addr_ntoa, new_hostname, inet_ntoa(ret));
          }
        }
          
        /* Found and processed; done */

        break;
      }
    }
  }

  /* All done */

  return ret;
}


/*
 * Release any resources claimed by the above function
 */
void 
lam_ssi_base_hostmap_finalize(void)
{
  if (hostmap != NULL) {
    lam_deallocate_nodes(&hostmap, &hostmap_len);
    read_hostmap = 0;
    hostmap = NULL;
    hostmap_len = 0;
  }
}


/*
 * Helper function to resolve an array of hostnames
 */
static int 
resolve_hosts(struct lamnode *hostmap, int hostmap_len)
{
  int i;

  /* If we can't resolve the hostname, set its IP addr to 0.0.0.0, but
     don't error out */

  for (i = 0; i < hostmap_len; ++i) {
    if (getinetaddr(hostmap[i].lnd_hname, 
                    (unsigned char *) &hostmap[i].lnd_addr.sin_addr) != 0) {
      printf("WARNING: n%d<%d> failed to resolve hostmap name %s (ignored)\n", 
             getnodeid(), (int) lam_getpid(), hostmap[i].lnd_hname);
      memset(&hostmap[i].lnd_addr.sin_addr, 0, 
             sizeof(hostmap[i].lnd_addr.sin_addr));
    } else
      hostmap[i].lnd_addr.sin_family = AF_INET;
  }

  return 0;
}
