/**
 * @file   mnist_load.h
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:39:07 2017
 * 
 * @brief  MNIST database loader
 * 
 * 
 */

#ifndef MNIST_LOAD_H
#define MNIST_LOAD_H	1
#include "gen.h"

typedef struct	s_mnist
{
  uint32_t      n_rows;
  uint32_t      n_cols;
  u_char	*data;
}		t_mnist;

/* PROTO mnist_load.c */
/* mnist_load.c */
void mnist_labels_load P((char *fname, char **labelsp, int *n_labelsp));
void mnist_images_load P((char *fname, t_mnist **imagesp, int *n_imagesp));
#endif
