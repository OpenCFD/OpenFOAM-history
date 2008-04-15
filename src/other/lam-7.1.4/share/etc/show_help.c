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
 * $Id: show_help.c,v 6.13 2003/10/02 18:24:52 vsahay Exp $
 * 
 *	Function:	- Read the LAM help file for the given program & topic
 *			- output the help message
 *			- substitute for variables and parameters
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <errno.h>

#include <lam_config.h>
#include <portable.h>
#include <args.h>
#include <etc_misc.h>


/* Hoping we wont have more than 100 Help files including all SSI helpfiles */
#define NUMHELPFILES 100

/*
 * private variables
 */
static char *nonver_name = "lam-helpfile";
static char *ver_name = NULL;
static char *helpfile_name = NULL;
static char *line = "-----------------------------------------------------------------------------\n";
static char *vars[] = { 
  "$HOME", 
  "$HOME/etc",
  "$LAMHELPDIR",
  "$LAMHOME/etc",
  "$SYSCONFDIR",
  NULL
};
static int found_helpfile = 0;
static int errno_save = 0;
char *help_list[NUMHELPFILES];
  

/*
 * private functions
 */
static char *expand_var(int i);
static FILE *try_dir(const char *filename, char *dirname);
static FILE *open_helpfile(const char *filename);
static char **read_help(const char *filename, const char *program, 
			const char *topic);
static void print_subst(char *line, char **args, int nargs);
static void show_help_file_low(const char *filename, const char *program, 
                               const char *topic, va_list arglist);


/*
 * expand vars[which] and fill in environment variable values
 * returns pointer to a static space with the filled in result
 * will be overwritten during the next call
 */
static char *
expand_var(int which)
{
  static char ret[LAM_PATH_MAX + 1];
  int i, j, out, len = strlen(vars[which]);
  char *env, temp[LAM_PATH_MAX + 1];

  memset(ret, 0, LAM_PATH_MAX + 1);

  for (out = i = 0; i < len; i++) {
    if (vars[which][i] != '$') {
      ret[out++] = vars[which][i];
      continue;
    } else {
      for (j = i + 1; j < len && vars[which][j] != '/'; j++)
	continue;
      
      j--;
      memset(temp, 0, LAM_PATH_MAX + 1);
      lam_strncpy(temp, &vars[which][i + 1], j);
      if (strcmp(temp, "PREFIX") == 0)
	env = LAM_PREFIX;
      else if (strcmp(temp, "SYSCONFDIR") == 0)
	env = LAM_SYSCONFDIR;
      else
	env = getenv(temp);
      if (env == NULL)
	return NULL;

      strcat(ret, env);
      out = strlen(ret);
      i = j;
    } 
  }

  return ret;
}


static FILE *
check_file(const char *filename, char *dirname)
{
  FILE *fp;
  char buffer[LAM_PATH_MAX + 1];

  snprintf(buffer, LAM_PATH_MAX, "%s/%s", dirname, filename);
  fp = fopen(buffer, "r");
  if (fp != NULL) {
    helpfile_name = strdup(buffer);
    return fp;
  } else if (errno != ENOENT)
    found_helpfile = 1;

  return fp;
}


static FILE *
try_dir(const char *filename, char *dirname)
{
  int len;
  FILE *fp = NULL;

  /* Allocate out the version filename, if it hasn't been done yet */

  if (ver_name == NULL) {
    len =  64 + strlen(LAM_VERSION);
    ver_name = malloc(len);
    snprintf(ver_name, len - 1, "lam-%s-helpfile", LAM_VERSION);
  }

  /* Now try the requested dirname */

  if (dirname != NULL) {
    if (filename != NULL && fp == NULL)
      fp = check_file(filename, dirname);
    if (fp == NULL)
      fp = check_file(nonver_name, dirname);
    if (fp == NULL)
      fp = check_file(ver_name, dirname);
  }

  return fp;
}


static FILE *
open_helpfile(const char *filename)
{
  FILE *fp = NULL;
  char *env;
  int i;

  /* Try several directories */

  for (i = 0; vars[i] != NULL; i++) {
    env = expand_var(i);
    if (env != NULL) {
      fp = try_dir(filename, env);
      if (fp != NULL)
	break;
    }
  }

  return fp;
}



/*
 * Open up the help file, find the help lines for a given
 * program/topic, and return it in an array of char*'s, or NULL if an
 * error occurs.  
 */
static char**
read_help(const char *filename, const char *program, const char *topic)
{
  FILE *fp;
  char *str;
  char **ret;
  int len, i, j, k, found = 0, count = 0;
  long pos;
  char buffer[BUFSIZ];

  /* Try to open the help file */

  fp = open_helpfile(filename);
  if (fp == NULL) {
    fprintf(stderr, line);
    fprintf(stderr, "*** Oops -- I cannot open the LAM help file.\n");
    if (found_helpfile) {
      fprintf(stderr, "*** I found at least one helpfile, but some error prevented me from\n");
      fprintf(stderr, "*** opening it.\n\n");
    }
    
    fprintf(stderr, "*** I tried looking for it in the following places:\n");
    fprintf(stderr, "***\n");

    for (i = 0; vars[i] != NULL; i++) {
      if (strstr(vars[i], "$PREFIX") != NULL) {
	fprintf(stderr, "***   %s/%s\n", expand_var(i), nonver_name);
	fprintf(stderr, "***   %s/%s\n", expand_var(i), ver_name);
      } else {
	fprintf(stderr, "***   %s/%s\n", vars[i], nonver_name);
	fprintf(stderr, "***   %s/%s\n", vars[i], ver_name);
      }
    }
    fprintf(stderr, "***\n");

    fprintf(stderr, 
	    "*** You were supposed to get help on the program \"%s\"\n",
	    program);
    fprintf(stderr, "*** about the topic \"%s\"\n", topic);
    fprintf(stderr, "***\n");
    fprintf(stderr, "*** Sorry!\n");
    fprintf(stderr, line);

    return NULL;
  }

  /* Try to find the program/topic */

  if (program == NULL)
    program = "ALL";
  if (topic == NULL)
    topic = "ALL";
  str = malloc(strlen(program) + strlen(topic) + 16);
  sprintf(str, "-*-%s:%s-*-", program, topic);
  len = strlen(str);
  while (fgets(buffer, BUFSIZ, fp) != NULL) {
    if (strncmp(str, buffer, len) == 0) {
      found = 1;
      break;
    }
  }
  free(str);

  /* Did we find it? */

  if (!found) {
    fprintf(stderr, line);
    fprintf(stderr, "*** Oops -- cannot find the help that you're supposed to get.\n");
    fprintf(stderr, "*** Using the following help file:\n");
    fprintf(stderr, "***\n");
    fprintf(stderr, "***    %s\n", helpfile_name);
    fprintf(stderr, "***\n");
    fprintf(stderr, 
	    "*** You were supposed to get help on the program \"%s\"\n",
	    program);
    fprintf(stderr, "*** about the topic \"%s\"\n", topic);
    fprintf(stderr, "*** But it doesn't seem to be in that file.\n");
    fprintf(stderr, "***\n");
    fprintf(stderr, "*** Sorry!\n");
    fprintf(stderr, line);

    return NULL;
  }

  /* Read in the lines of help */
  /* First count the max of how many there are */

  pos = ftell(fp);
  while (fgets(buffer, BUFSIZ, fp) != NULL) {
    if (strncmp(buffer, "-*-", 3) == 0)
      break;
    count++;
  }

  /* Now alloc space and read in all the lines */
  /* Ignore lines that begin with # or [whitespace]+# */

  fseek(fp, pos, SEEK_SET);
  ret = malloc(sizeof(char*) * (count + 1));
  for (k = i = 0; i < count; i++) {
    fgets(buffer, BUFSIZ - 1, fp);
    buffer[BUFSIZ - 1] = '\0';
    for (j = 0; j < strlen(buffer); j++)
      if (!isspace((int) buffer[j]))
	break;
    if (buffer[j] == '#')
      continue;
    ret[k++] = strdup(buffer);
  }
  ret[k] = NULL;

  /* We're all done */

  fclose(fp);

  return ret;
}


/*
 * Perform substitutions; print out the resulting line.
 *
 * Substitute args[x - 1] for %x in the line (i.e., indexed from 1)
 * Print the output of perror() for %perror
 * Print the output of terror() for %terror
 * Print the errno for %errno
 * Substitute % for %%
 * Leave all other %'s alone
 */
static void
print_subst(char *line, char **args, int nargs)
{
  int num, i, last = 0, len = strlen(line);
  
  for (i = 0; i < len; i++)
    if (line[i] != '%')
      continue;
    else {
      if (i + 1 >= len)
	continue;
      else {
	if (strncmp(line + i + 1, "perror", 6) == 0) {
	  line[i] = '\0';
	  fprintf(stderr, "%s", line + last);
	  fflush(stderr);
	  errno = errno_save;
	  perror("");
	  last = i + 7;
	  i = last - 1;
	  continue;
	} else if (strncmp(line + i + 1, "terror", 6) == 0) {
	  line[i] = '\0';
	  fprintf(stderr, "%s", line + last);
	  fflush(stderr);
	  errno = errno_save;
	  terror("");
	  last = i + 7;
	  i = last - 1;
	  continue;
	} else if (strncmp(line + i + 1, "errno", 5) == 0) {
	  line[i] = '\0';
	  fprintf(stderr, "%s%d", line + last, errno_save);
	  fflush(stderr);
	  last = i + 6;
	  i = last - 1;
	  continue;
	}
	num = atoi(line + i + 1);
	if (num > 0) {
	  line[i] = '\0';

	  /* If we have a valid arg, print it */

	  if (num <= nargs && args[num - 1] != NULL)
	    fprintf(stderr, "%s%s", line + last, args[num - 1]);
	  else
	    fprintf(stderr, "%s", line + last);

	  /* Advance last past the number */

	  last = i + 1;
	  while (isdigit((int) line[last]) && last < len)
	    last++;
	  i = last - 1;
	}
      }
    }
  if (last < len)
    fprintf(stderr, "%s", line + last);
}


/*
 * Turn around and invoke show_help_file_low() with "lam-helpfile"
 * filename
 */
void
#if LAM_WANT_PROTOS
show_help(const char *program, const char *topic, ...)
#else
show_help(program, topic, va_alist)
const char *program;
const char *topic;
va_dcl
#endif
{
  va_list arglist;
#if __STDC__
  va_start(arglist, topic);
#else
  va_start(arglist);
#endif
  show_help_file_low("lam-helpfile", program, topic, arglist);
}


/*
 * Turn around and invoke show_help_file_low() with the specified
 * filename
 */
void
#if LAM_WANT_PROTOS
show_help_file(const char *filename, const char *program, 
               const char *topic, ...)
#else
show_help_file(filename, program, topic, va_alist)
const char *filename;
const char *program;
const char *topic;
va_dcl
#endif
{
  va_list arglist;
#if __STDC__
  va_start(arglist, topic);
#else
  va_start(arglist);
#endif
  show_help_file_low(filename, program, topic, arglist);
}


/*
 * Show help for a particular program and topic.  Accept a variable
 * length list of args to be substituted in the output when the output
 * contains %x, where x is the number of the arugment from the
 * variable length argument list.
 */
static void
show_help_file_low(const char *filename, const char *program, 
                   const char *topic, va_list arglist)
{
  char **lines;
  char **args, *arg;
  int i, count = 0, max_args = 0;

  static int is_list_initialized = 0;
  char **curr_file;

  /* Inorder to prevent cascading help messages, we have a list here which 
     stores the names of the help-files as you call this function. The 
     filename is stored only the first time we try to print help message
     for the file. If the filename is already present in the list, we know
     that we have been here before, and we return without doing anything 
     This functionality can be turned off by setting the 
     LAM_MPI_ALLOW_CASCADE_HELP environment variable */

  if (getenv("LAM_MPI_ALLOW_CASCADE_HELP") == NULL) {

    /* I am entering for the first time ever. So initialize the list */

    if (!is_list_initialized) {
      for (i = 0; i < NUMHELPFILES; ++i)
	help_list[i] = NULL;
      is_list_initialized = 1;
    }

    curr_file = help_list;

    /* Go and search through the list */
    while (*curr_file) {
      if (strcmp(filename, *curr_file) == 0)
	return;
      ++curr_file;
    }
     
    /* Not found in the list, so insert here */

    /* Cannot use show_help in case of error during strdup, 
       so just a fprintf */
    
    if ((*curr_file = strdup(filename)) == NULL)
      fprintf(stderr, "Error: Malloc failed for filename in show_help \n");
  }
  
  errno_save = errno;

  /* Get the help strings */

  if ((lines = read_help(filename, program, topic)) == NULL)
    return;

  /* We got the strings -- do the variable replacement */
  /* Read in all the var args to an array of char*'s */

  args = malloc(sizeof(char*) * 10);
  max_args = 10;
  count = 0;
  while ((arg = va_arg(arglist, char*)) != NULL) {
    if (count >= max_args) {
      max_args += 10;
      args = realloc(args, sizeof(char*) * max_args);
    } 
    args[count] = arg;
    count++;
  }

  /* Make the array larger if necessary */
  
  if (count >= max_args) {
    args = realloc(args, sizeof(char*) * count);
    max_args = count;
  }
  args[count] = NULL;
  va_end(arglist);

  /* Now do the substititions */

  fprintf(stderr, "-----------------------------------------------------------------------------\n");
  for (i = 0; lines[i] != NULL; i++)
    print_subst(lines[i], args, count);
  fprintf(stderr, "-----------------------------------------------------------------------------\n");

  /* We're all done -- free up everything */

  for (i = 0; lines[i] != NULL; i++)
    free(lines[i]);
  free(lines);
  free(args);

  /* Ensure that we restore errno, just like we were never here... */

  errno = errno_save;
}
