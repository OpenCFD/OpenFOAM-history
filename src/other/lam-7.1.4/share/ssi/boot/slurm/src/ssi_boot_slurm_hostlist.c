/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 *	Function:	- slurm boot SSI module
 */

#include <lam_config.h>
#include <lam-ssi-boot-slurm-config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <all_array.h>
#include <lam-ssi-boot-slurm.h>


/*
 * Local functions
 */
static int parse_ranges(char *base, char *ranges, lam_array_t **a);
static int parse_range(char *base, char *range, lam_array_t **a);


/*
 * Parse a nodelist and return an array of strings of node names
 */
lam_array_t *
lam_ssi_boot_slurm_parse_hostlist(char *regexp)
{
  int i, j, len;
  lam_array_t *a;
  char *base;

  if (regexp == NULL)
    return NULL;
  base = strdup(regexp);
  if (base == NULL)
    return NULL;

  a = lam_arr_init(sizeof(char *), NULL);
  if (a == NULL) {
    free(base);
    return NULL;
  }
  
  /* Find the base */

  len = strlen(regexp);
  for (i = 0; i < len; ++i) {
    if (base[i] == '[') {
      base[i] = '\0';
      break;
    }
  }

  /* If we didn't find a range, then this is it */

  if (i >= len) {
    char *begin = base;
    for (i = 0; i < len; ++i) {
      if (base[i] == ',') {
        base[i] = '\0';
        lam_arr_append(a, &begin);
        begin = &base[i] + 1;
      }
    }
    lam_arr_append(a, &base);
  } else {

    /* If we did find a range, find the end of the range */

    for (j = i; j < len; ++j) {
      if (base[j] == ']') {
        base[j] = '\0';
        break;
      }
    }
    if (j >= len) {
      lam_arr_free(a);
      free(base);
      return NULL;
    }

    parse_ranges(base, base + i + 1, &a);
  }

  /* All done */

  return a;
}


/*
 * Parse one or more ranges in a set
 */
static int 
parse_ranges(char *base, char *ranges, lam_array_t **a)
{
  int i, len;
  char *start, *orig;

  /* Look for commas, the separator between ranges */

  len = strlen(ranges);
  for (orig = start = ranges, i = 0; i < len; ++i) {
    if (ranges[i] == ',') {
      ranges[i] = '\0';
      if (parse_range(base, start, a) != 0) {
        return LAMERROR;
      }
      start = ranges + i + 1;
    }
  }

  /* Pick up the last range, if it exists */

  if (start < orig + len) {
    if (parse_range(base, start, a) != 0) {
      return LAMERROR;
    }
  }

  /* All done */

  return 0;
}


/*
 * Parse a single range in a set
 */
static int 
parse_range(char *base, char *range, lam_array_t **a)
{
  char *str, temp1[BUFSIZ], temp2[BUFSIZ];
  int i, j, start, end;
  int base_len, len;
  int str_start, str_end;
  int num_str_len;

  len = strlen(range);
  base_len = strlen(base);

  /* Look for the beginning of the first number */

  for (str_start = start = -1, i = 0; i < len; ++i) {
    if (isdigit((int) range[i])) {
      if (str_start == -1) {
        str_start = i;
        start = atoi(range + i);
        break;
      }
    }
  }
  if (start == -1) {
    return LAMERROR;
  }

  /* Look for the end of the first number */

  for (num_str_len = 0, end = -1; i < len; ++i, ++num_str_len)
    if (!isdigit((int) range[i]))
      break;

  /* Was there no range, just a single number? */

  if (i >= len) {
    str_end = len;
    end = start;
  }

  /* Nope, there was a range.  Look for the beginning of the second
     number */

  else {
    str_end = i - 1;
    for (; i < len; ++i) {
      if (isdigit((int) range[i])) {
        end = atoi(range + i);
        break;
      }
    }
  }
  if (end == -1)
    return LAMERROR;

  /* Make strings for all values in the range */

  for (i = start; i <= end; ++i) {
    len = base_len + 32;
    str = malloc(len);
    if (str == NULL) {
      return LAMERROR;
    }

    str[0] = '\0';
    snprintf(temp1, BUFSIZ - 1, "%s", base);
    snprintf(temp2, BUFSIZ - 1, "%d", i);
    temp1[BUFSIZ - 1] = temp2[BUFSIZ - 1] = '\0';

    /* Do we need zero pading? */

    if (strlen(temp2) < num_str_len) {
        for (j = 0; j < num_str_len - strlen(temp2); ++j) {
          strcat(temp1, "0");
      }
    }
    snprintf(str, len - 1, "%s%s", temp1, temp2);
    str[len - 1] = '\0';
    lam_arr_append(*a, &str);
  }

  /* All done */

  return 0;
}
