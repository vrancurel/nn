/**
 * @file   myerror.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:41:54 2017
 * 
 * @brief  error routine
 * 
 * 
 */

#include "gen.h"

#ifdef HAVE_STDARG_H
void	myerror(int sys, char *fmt, ...)
#else
void	myerror(sys, fmt, va_alist)
int	sys;
char    *fmt;
va_dcl
#endif
{
  va_list	ap;
  char		buf[BUFSIZ];
  
#ifdef HAVE_STDARG_H
  va_start(ap, fmt);
#else
  va_start(ap);
  va_arg(ap, int);
  va_arg(ap, char *);
#endif
  vsnprintf(buf, sizeof (buf), fmt, ap);
  fprintf(stderr, "%s", buf);
  if (sys)
    fprintf(stderr, ": %s\n", strerror(errno));
  else
    fprintf(stderr, "\n");
  va_end(ap);
  abort();
}
