/**
 * @file   mnist_extract.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:42:08 2017
 * 
 * @brief  extract a PGM from MNIST database tool
 * 
 * 
 */

#include "gen.h"

extern int      optind;
extern char     *optarg;

int		vflag = 0;	/* verbose mode			*/
int             xflag = -1;
char		*mnist_fname = NULL;
char		*pgm_fname = NULL;

t_mnist         *images;
char            *labels;
int             n_labels, n_images;

void	usage()
{
  fprintf(stderr, "usage: mnist_extract [-v] [-x nb] fname.mnist output.png\n");
  exit(1);
}

void    extract(int nb)
{
  FILE	*f;
  int ret, i, j;
  char *data;
  
  if ((f = fopen(pgm_fname, "w")) == NULL)
    myerror(TRUE, "fopen %s", pgm_fname);
  fprintf(f, "P5\n");
  fprintf(f, "%d %d\n", images[nb].n_cols, images[nb].n_rows);
  fprintf(f, "255\n");

  data = xmalloc(images[nb].n_cols * images[nb].n_rows * sizeof (u_char));
  
  for (i = 0;i < images[nb].n_rows;i++)
    {
      for (j = 0;j < images[nb].n_cols;j++)
        {
          data[j * images[nb].n_cols + i] = 
            255-images[nb].data[j * images[nb].n_cols + i]; 
        }
    }

  ret = fwrite(data, images[nb].n_cols, images[nb].n_rows, f);
  if (ret != images[nb].n_rows)
    myerror(FALSE, "short write");
  fclose(f);
  free(data);
}

void	doit(int argc, char **argv)
{
  mnist_images_load(mnist_fname, &images, &n_images);
  assert(n_images>0);
  if (vflag)
    fprintf(stderr, "n_images=%d %dx%d\n", n_images, images[0].n_rows, images[0].n_cols);

  extract(xflag);
}

int	main(int argc, char **argv)
{
  char  opt;
  
  while ((opt = getopt(argc, argv, "vx:")) != -1)
    switch (opt)
      {
      case 'v':
        vflag = 1;
        break ;
      case 'x':
	xflag = atoi(optarg);
	break ;
      case '?':
      default:
        usage();
      }
  argc -= optind;
  argv += optind;
  if (argc != 2)
    usage();
  mnist_fname = argv[0];
  pgm_fname = argv[1];
  doit(argc, argv);
}
