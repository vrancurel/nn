/**
 * @file   mnist_load.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:40:21 2017
 * 
 * @brief  MNIST database load
 * 
 * 
 */

#include "gen.h"

void mnist_labels_load(char *fname, char **labelsp, int *n_labelsp)
{
  FILE	*f;
  uint32_t magic;
  char *labels;
  uint32_t n_labels;
  int ret, i;

  if ((f = fopen(fname, "rb")) == NULL)
    myerror(TRUE, "fopen %s", fname);

  ret = fread(&magic, 4, 1, f);
  if (1 != ret)
    myerror(FALSE, "short read label magic");
  magic = ntohl(magic);
  if (magic != 2049)
    myerror(FALSE, "invalid labels magic %d", magic);
  ret = fread(&n_labels, 4, 1, f);
  if (1 != ret)
    myerror(FALSE, "short read n_labels");
  n_labels = ntohl(n_labels);
  labels = xmalloc(n_labels * sizeof (u_char));
  for (i = 0;i < n_labels;i++)
    {
      u_char byte;
      ret = fread(&byte, 1, 1, f);
      if (1 != ret)
        myerror(FALSE, "short read label");
      labels[i] = byte;
    }
  
  fclose(f);

  if (labelsp)
    *labelsp = labels;

  if (n_labelsp)
    *n_labelsp = n_labels;
}

void mnist_images_load(char *fname, t_mnist **imagesp, int *n_imagesp)
{
  FILE	*f;
  uint32_t magic;
  t_mnist *images;
  uint32_t n_images, n_rows, n_cols, sz;
  int ret, i;

  if ((f = fopen(fname, "rb")) == NULL)
    myerror(TRUE, "fopen %s", fname);

  ret = fread(&magic, 4, 1, f);
  if (1 != ret)
    myerror(FALSE, "short read images magic");
  magic = ntohl(magic);
  if (magic != 2051)
    myerror(FALSE, "invalid images magic %d", magic);
  ret = fread(&n_images, 4, 1, f);
  if (1 != ret)
    myerror(FALSE, "short read n_images");
  n_images = ntohl(n_images);
  ret = fread(&n_rows, 4, 1, f);
  if (1 != ret)
    myerror(FALSE, "short read n_rows");
  n_rows = ntohl(n_rows);
  ret = fread(&n_cols, 4, 1, f);
  if (1 != ret)
    myerror(FALSE, "short read n_cols");
  n_cols = ntohl(n_cols);
  sz = n_rows * n_cols;
  images = xmalloc(n_images * sizeof (t_mnist));
  for (i = 0;i < n_images;i++)
    {
      u_char *data;
      data = xmalloc(sz * sizeof (u_char));
      ret = fread(data, 1, sz, f);
      if (sz != ret)
        myerror(FALSE, "short read pixel");
      images[i].n_rows = n_rows;
      images[i].n_cols = n_cols;
      images[i].data = data;
    }
  
  fclose(f);

  if (imagesp)
    *imagesp = images;

  if (n_imagesp)
    *n_imagesp = n_images;
}
