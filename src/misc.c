/**
 * @file   misc.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:40:32 2017
 * 
 * @brief  misc routines
 * 
 * 
 */

#include "gen.h"

char    *xstrdup(char *s)
{
  char  *ns;

  if (ns = strdup(s))
    return (ns);
  myerror(TRUE, "strdup");
  return (NULL); /* make compiler happy */
}

void    *xmalloc(size_t size)
{
  void  *ptr;

  if (ptr = malloc(size))
    return (ptr);
  myerror(TRUE, "malloc");
  return (NULL); /* make compiler happy */
}
