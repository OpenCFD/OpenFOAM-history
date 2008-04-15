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
 * $Id: bhostparse.c,v 6.22 2004/01/01 23:59:08 jsquyres Exp $
 * 
 *	Function:	- vanilla boot schema parser
 *			- host file syntax version
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boot.h>
#include <lamnet.h>
#include <net.h>
#include <typical.h>
#include <etc_misc.h>


/*
 * constants
 */

#define LNDBLOCK	10	       /* array allocation block */


/*
 * local variables
 */

static char hostname_key[] = "hostname";
static char cpu_key[] = "cpu";
static char user_key[] = "user";
static char schedule_key[] = "schedule";


/* 
 * Compare function for LIST
 */

static int comp(ELEM *listelem1, ELEM *listelem2);


/*
 *	parse_host
 *
 *	Function:	- parse a host file
 *			- fill the network description
 *			- node IDs are [0, N-1]
 *	Accepts:	- input file stream
 *			- node description array (value returned)
 *			- array size (value returned)
 */
int
bhostparse(char *filename, struct lamnode **plamnet, int *pnlamnet)
{
  char *host;			/* host machine name */
  char *user;			/* user name */
  char *linecopy;		/* copy of current line */
  int4 ncpus;			/* number of CPUs */
  int nlndalloc;		/* current allocation size */
  int node;			/* current node ID */
  int i;			/* favorite counter */
  int found;			/* if found duplicate */
  int lineno;			/* line number in host file */
  struct lamnode *lamnet;	/* current node desc. array */
  FILE *fp;			/* pointer to host file */
  char linebuf[LAM_MAX_PARSE_LINE];
  LIST **kvlist;		/* list of key-value pairs */
  ELEM *keyval;			/* entry in list */
  ELEM cpu_in, *cpu_out;	/* cpu entry in list */
  char *cpu_count;              /* cpu count in string form for list value */
  int cpu_count_len;            /* cpu_count string length */
  int listalloc;                /* current allocation size for list */
  int list_index;               /* list pointer index */
  ELEM *listelem;               /* element of the keyval list */
  ELEM search;                  /* search key for the list */

  /* Open the system file */

  fp = fopen(filename, "r");
  if (fp == 0) {
    show_help(NULL, "open-file", "The boot SSI module", filename, NULL);
    return LAMERROR;
  }

  /* Loop till EOF reading and parsing line-by-line */

  lamnet = NULL;
  nlndalloc = 0;
  node = 0;
  lineno = 0;
  kvlist = NULL;
  listalloc = 0;

  while (!feof(fp)) {
    if (fgets(linebuf, LAM_MAX_PARSE_LINE, fp) == 0) 
      break;

    /* Parse the line for hostname, num CPUs, username and key-value pairs */

    lineno++;
    linecopy = strdup(linebuf);
    if (linecopy == NULL) {
      show_help(NULL, "library-call-fail", "strdup", NULL);
      fclose(fp);
      return LAMERROR;
    }
    i = strlen(linecopy);
    if (linecopy[i - 1] == '\n')
      linecopy[i - 1] = '\0';

    /* Allocate initial node list array */

    if (kvlist == NULL) {
      kvlist = (LIST **) malloc(LNDBLOCK * sizeof(LIST *));

      if (kvlist == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	fclose(fp);
	return LAMERROR;
      }

      listalloc = LNDBLOCK;
      
      /* Initialize the list pointers to 0 */
      for (list_index = 0; list_index < listalloc; ++list_index) {
	kvlist[list_index] = NULL;
      }
    }

    /* Extend node list array if required */

    if (listalloc <= node) {
      listalloc += LNDBLOCK;
      kvlist = (LIST **) realloc((char *)kvlist, listalloc * sizeof (LIST *));

      if (kvlist == NULL) {
	show_help(NULL, "system-call-fail", "realloc", NULL);
	fclose(fp);
	return LAMERROR;
      }

      /* Initialize the list pointers to 0 */

      for (list_index = node; list_index < listalloc; ++list_index) {
	kvlist[list_index] = NULL;
      }
    }

    /* initialize the list if not already done */

    if (kvlist[node] == NULL) {
      kvlist[node] = al_init(sizeof(ELEM), comp);

      if (kvlist[node] == NULL) {
	show_help(NULL, "library-call-fail", "al_init", NULL);
	fclose(fp);
	return LAMERROR;
      }
    }

    bhostparseline(linebuf, lineno, linecopy, kvlist[node]);

    free(linecopy);

    ncpus = 0;
    host = NULL;
    user = NULL;

    /* get hostname */

    search.key = hostname_key;

    keyval = al_find(kvlist[node], &search);
    if (keyval != NULL) {
      host = strdup(keyval->value);
      if (host == NULL) {
	show_help(NULL, "library-call-fail", "strdup", NULL);
	fclose(fp);
	return LAMERROR;
      }
    }

    /* get cpu count */

    search.key = cpu_key;
    keyval = al_find(kvlist[node], &search);
    if (keyval != NULL) {
      ncpus = atoi(keyval->value);
    }

    /* get username */

    search.key = user_key;
    keyval = al_find(kvlist[node], &search);
    if (keyval != NULL) {
      user = strdup(keyval->value);
      if (user == NULL) {
	show_help(NULL, "library-call-fail", "strdup", NULL);
	fclose(fp);
	return LAMERROR;
      }
    }

    /* If a machine name is not found, must be a blank line or comment */

    if (host == NULL) 
      continue;

    /* if hostname is already used, accumulate cpu count and add newly got
       entries in the list */
 
    for (found = i = 0; i < node; i++) {
      if (strcasecmp(host, lamnet[i].lnd_hname) == 0) {
	
	/* accumulate cpu count, delete and insert is used instead of 
	   modifying the list element value, since malloc is internally 
	   done in the list and assigning a new value may require more
	   memory space for the char pointer value */

	lamnet[i].lnd_ncpus += ncpus;
	search.key = cpu_key;

	/* remove the previous cpu entry from list */

	cpu_out = al_find(lamnet[i].lnd_keyval, &search);
	free(cpu_out->key);
	free(cpu_out->value);
	al_delete(lamnet[i].lnd_keyval, cpu_out);
       
	/* insert new cpu entry into the list */
	
	cpu_in.key = strdup(cpu_key);
	if (cpu_in.key == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  fclose(fp);
	  return LAMERROR;
	}
	
	cpu_count_len = 8 * sizeof(int4);
	cpu_count = (char *) malloc (cpu_count_len);
	if (cpu_count == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  fclose(fp);
	  return LAMERROR;
	}
	snprintf(cpu_count, cpu_count_len, "%d", lamnet[i].lnd_ncpus);
	cpu_in.value = cpu_count;

	al_insert(lamnet[i].lnd_keyval, &cpu_in);

	/* append new entries in the list to the previous list 
	   Keep previous entries intact even if the new list has
	   modified values -- we are not updating entries, considering
	   that its an error on user part */
	
	for (listelem = al_top(kvlist[node]); listelem != NULL; 
	     listelem = al_next(kvlist[node], listelem)) {

	  if ((strcasecmp(listelem->key, hostname_key) == 0) || 
	      (strcasecmp(listelem->key, cpu_key) == 0)) {
	    free(listelem->key);
	    free(listelem->value);
	    continue;
	  }
	  
	  if (al_find(lamnet[i].lnd_keyval, listelem) == NULL){
	    al_insert(lamnet[i].lnd_keyval, listelem);
	  } else {
	    free(listelem->key);
	    free(listelem->value);
	  }
	}

	al_free(kvlist[node]);
	kvlist[node] = NULL;

	found = 1;
	break;
      }
    }
	
    if (found) {
      if (host != NULL)
	free(host);
      if (user!= NULL)
	free(user);
      continue;
    }
    
	  
    /* Allocate initial node description array */

    if (lamnet == NULL) {
      lamnet = (struct lamnode *) malloc((unsigned)
					 LNDBLOCK *
					 sizeof(struct lamnode));
      if (lamnet == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	fclose(fp);
	return LAMERROR;
      }
      nlndalloc = LNDBLOCK;
    }

    /* Extend node description array */

    if (nlndalloc <= node) {
      nlndalloc += LNDBLOCK;
      lamnet = (struct lamnode *) realloc((char *) lamnet,
					  (unsigned) nlndalloc *
					  sizeof(struct lamnode));
      if (lamnet == NULL) {
	show_help(NULL, "system-call-fail", "realloc", NULL);
	fclose(fp);
	return LAMERROR;
      }
    }

    lamnet[node].lnd_nodeid = node;
    lamnet[node].lnd_type = NT_BOOT;
    
    search.key = schedule_key;
    keyval = al_find(kvlist[node], &search);
    if (keyval != NULL) {
      if (strcasecmp(keyval->value, "no") == 0) {
	lamnet[node].lnd_type |= NT_WASTE;
      } else {
	if (strcasecmp(keyval->value, "yes") == 0) 
	  /* if NT_WASTE, then clear it */
	  lamnet[node].lnd_type &= ~NT_WASTE;
      }
    }
    
    lamnet[node].lnd_ncpus = ncpus;
    lamnet[node].lnd_bootport = 0;
    lamnet[node].lnd_ssi = NULL;

    /* Set host name */

    lamnet[node].lnd_hname = malloc((unsigned) strlen(host) + 1);
    if (lamnet[node].lnd_hname == NULL) {
      show_help(NULL, "system-call-fail", "malloc", NULL);
      fclose(fp);
      return LAMERROR;
    }
    strcpy(lamnet[node].lnd_hname, host);

    free(host);

    /* Set optional user name */

    lamnet[node].lnd_uname = NULL;

    if (user != NULL) {
      lamnet[node].lnd_uname = malloc((unsigned) strlen(user) + 1);
      if (lamnet[node].lnd_uname == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	fclose(fp);
	return LAMERROR;
      }
      strcpy(lamnet[node].lnd_uname, user);

      free(user);
    }

    /* Set the list */

    lamnet[node].lnd_keyval = kvlist[node];

    node++;
  }

  *pnlamnet = node;
  *plamnet = lamnet;

  free(kvlist);
  fclose(fp);

  return 0;
}



/*
 *	parseline
 *
 *	Function:	- parses the line buffer for hostname and key-value
 *			  pairs 
 *			- Format: hostname [key=value]*
 *			- skip comments: everything after a # character
 *			- returns a LIST containing key-value pairs along with 
 *			  hostname
 *	Accepts:	- line buffer (for parsing)
 *			- line number (for error messages)
 *			- line contents (for error messages)
 *			- LIST of key-value pairs (returned)
 */

void
bhostparseline(char *linebuf, int lineno, char *linedata, LIST *list)
{
  char *p;			/* favourite pointer */
  char *q;			/* end of current token */
  char lineno_str[20];		/* string for lineno */
  int hostname;                 /* did we get the hostname yet? */
  char *equals;			/* equal character in token */
  char *forslash;               /* forward slash in token */
  char *key;			/* key in key-value pair */
  char *value;			/* value in key-value pair */
  char *token;			/* current token */
  int quotes;			/* is token enclosed in quotes */
  int blank;                    /* check for blank line */
  ELEM host_keyvalue;           /* stores the host entry for key-val list */
  ELEM keyvalue;		/* key-value token */
  ELEM search;			/* search key */
  
  linebuf[LAM_MAX_PARSE_LINE - 1] = '\0';

  snprintf(lineno_str, 19, "%d", lineno);
  lineno_str[19] = '\0';

  hostname = 0;
  blank = 1;

  /*
   * Skip comments by replacing the first '#' with a NULL.
   */
  for (p = linebuf; *p; ++p) {
    if (*p == '#') {
      *p = '\0';
      continue;
    }
  }

  for (p = linebuf; *p; p = q) {
    quotes = 0;

    /* If *p is whitespace, just continue. */
    if (isspace((int) *p)) {
      q = p + 1;
      continue;
    }

    /* got the first non-whitespace character, hence not a blank line */
    blank = 0;

    /* Find end of token */
    if (*p == '"') 
      quotes = 1;

    if (quotes) {

      /* strip the quotes for token */
      token = p+1;

      for (q = p + 1; *q && (*q != '"'); ++q)
	continue;
      
      if (*q != '"') {
	show_help("bhost-parse", "imbalanced-quotes", lineno_str, linedata,
		  NULL);
	return;
      }
      
      *q = '\0';
      ++q;
    }
    else {
      token = p;
      for (q = p; *q && !isspace((int) *q); ++q)
	continue;

      if (*q != '\0') {
	*q = '\0';
	++q;
      }
    }

    /* first token should be hostname */
    equals = strchr(token, '=');
    forslash = strchr(token, '/');
	
    if (hostname == 0) {
      /* I am looking for valid hostname format -- checking for forward slash 
       here just to say that hostname format is valid if there is an equals
       in hostname along with forward slash before it -- in which case its a 
       globus hostname -- just for checking purpose, nothing else */
      if (((equals != NULL) && (forslash != NULL) && (equals < forslash)) ||
	  ((forslash == NULL) && (equals != NULL))) {
	show_help("bhost-parse", "hostname-must-be-first", lineno_str,
		  linedata, NULL);
	return;
      }

      host_keyvalue.key = strdup(hostname_key);
      if (host_keyvalue.key == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(errno);
      }

      host_keyvalue.value = strdup(token);
      if (host_keyvalue.value == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(errno);
      }
      
      /* Not inserting the hostname in the list now, so that if anything
	 is wrong with the rest of the line, the line can be ignored in
	 the calling function. Will insert hostname at the end, when 
	 everything is proper */

      hostname = 1;

    } else { /* key-value pair */
      
      if (equals == NULL) {
	/* not a valid key-value pair */
	show_help("bhost-parse", "invalid-key", lineno_str, linedata, NULL);
	return;
      } else if (isspace((int) *(equals - 1)) || isspace((int) *(equals + 1))) {
	/* not a valid key-value pair, cannot take whitespace before or after 
	   the equals sign -- this can happen only when enclosed in quotes */
	show_help("bhost-parse", "invalid-key", lineno_str, linedata, NULL);
	return;
      }
      *equals = '\0';
      key = token;
      value = equals + 1;
      
      /* add key value to the list */
      
      keyvalue.key = strdup(key);
      if (keyvalue.key == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(errno);
      }
      
      keyvalue.value = strdup(value);
      if (keyvalue.value == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(errno);
      }

      al_insert(list, &keyvalue);
    }
  }
  
  if (!blank) {

    /* fix the number of cpus if user has not set it already */

    search.key = cpu_key;
    if (al_find(list, &search) == NULL) {
      keyvalue.key = strdup(cpu_key);
      if (keyvalue.key == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(errno);
      }
    
      keyvalue.value = strdup("1");
      if (keyvalue.value == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(errno);
      }
      al_insert(list, &keyvalue);
    }

    /* Everything looks right, insert the hostname now */
  
    al_insert(list, &host_keyvalue);
  }
}


static int
comp(ELEM *listelem1, ELEM *listelem2)
{
  return strcasecmp(listelem1->key, listelem2->key);
}

