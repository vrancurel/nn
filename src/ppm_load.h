/**
 * @file   ppm_load.h
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:40:52 2017
 * 
 * @brief  load a ppm
 * 
 * 
 */

#ifndef PPM_LOAD_H
#define PPM_LOAD_H	1
#include "gen.h"

typedef struct	s_ppm
{
  unsigned int	width;
  unsigned int	height;
  unsigned char	*data;
}		t_ppm;

typedef struct	s_pgm
{
  unsigned int	width;
  unsigned int	height;
  unsigned char	*data;
}		t_pgm;

/* PROTO ppm_load.c */
/* ppm_load.c */
t_ppm *ppm_load P((char *fname));
t_pgm *pgm_load P((char *fname));
#endif
