/**
 * @file   gen.h
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 16:23:30 2017
 * 
 * @brief  general include
 * 
 * 
 */
#ifndef GEN_H
#define GEN_H	1
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#define HAVE_STDARG_H 1
#define P(Prototype)            Prototype
#define RAND(Number)            (rand() % (Number))
#define ABS(Number)          (((Number) < 0)?-(Number):(Number))
#define MY_MAX(X,Y)             (((X) > (Y))?(X):(Y))
#define MY_MIN(X,Y)             (((X) < (Y))?(X):(Y))
#define TRUE 1
#define FALSE 0

typedef int     t_boolean;

char	*xstrdup();
void    *xmalloc(size_t size);

typedef void	(*t_interf_input_proc) P((void *data));

typedef struct		s_interf_input_proc_data
{
  t_interf_input_proc	proc;
  void			*data;
}			t_interf_input_proc_data;

typedef int	(*t_interf_work_proc) P((void *data));

typedef struct		s_interf_work_proc_data
{
  t_interf_work_proc	proc;
  void			*data;
}			t_interf_work_proc_data;

#include "neural.h"
#include "nnsave.h"
#include "ppm_load.h"
#include "mnist_load.h"

#endif
