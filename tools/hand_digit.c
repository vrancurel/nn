/**
 * @file   hand_digit.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:41:26 2017
 * 
 * @brief  perceptron with MNIST digit database
 * 
 * 
 */

#include "gen.h"

#define DATAFILE	"hand_digit.data"	

extern int      optind;
extern char     *optarg;

int		vflag = 0;	/* verbose mode			*/
int		sflag = 0;	/* save automatically when learned */
int		iflag = 0;	/* interactive mode		*/
int		lflag = 0;	/* learn			*/
int		every = 1000;	/* save every 	*/
int		nlearnings = 1;	/* number of learnings		*/
char		*images_fname = NULL;
char		*labels_fname = NULL;

t_mnist         *images;
char            *labels;
int             n_labels, n_images;
t_neural_network *nn;
int             n_total = 0;
int             n_success = 0;

#define N_INPUT 784
#define N_HIDDEN 15
#define N_OUTPUT 10

void	usage()
{
  fprintf(stderr,
	  "usage: test [-vxsil][-n nlearnings][-e save_every] -I images_file -L labels_file\n");
  exit(1);
}

void	save()
{
  FILE	*f;
  
  printf("Saving data to %s\n", DATAFILE);
  if ((f = fopen(DATAFILE, "w+")) == NULL)
    myerror(TRUE, DATAFILE);
  neural_network_save(f, nn);
  fclose(f);
}

void	load()
{
  FILE	*f;
  
  if ((f = fopen(DATAFILE, "r")) == NULL)
    myerror(TRUE, DATAFILE);
  printf("Loading %s\n", DATAFILE);
  neural_network_load(f, nn);
  fclose(f);
  printf("ok\n");
}

void    learn_image(int nb)
{
  static float		data[N_INPUT], results[N_OUTPUT];
  int i, j;

  if (vflag)
    printf("%d\n", nb);

  for (i = 0;i < images[nb].n_rows;i++)
    {
      for (j = 0;j < images[nb].n_cols;j++)
        {
          data[j * images[nb].n_cols + i] = 
            (float)images[nb].data[i * images[nb].n_cols + j] / 255.0;
        }
    }

  for (i = 0;i < N_HIDDEN;i++) 
    {
      if (labels[nb] == i)
        results[i] = 1.0;
      else
        results[i] = 0;
    }

  neural_network_train(nn, data, results, 0.01, 0.2);
}

void	learn()
{
  int i;

  for (i = 0; i < n_images;i++)
    {
      learn_image(i);

      if ((i % every) == 0 && i != 0)
        save();
    }
}

void    run_image(int nb)
{
  static float		data[N_INPUT], results[N_OUTPUT];
  int i, j;

  for (i = 0;i < images[nb].n_rows;i++)
    {
      for (j = 0;j < images[nb].n_cols;j++)
        {
          data[j * images[nb].n_cols + i] = 
            (float)images[nb].data[i * images[nb].n_cols + j] / 255.0;
        }
    }

  neural_network_run(nn, data, results);

  float max_result = 0;
  int max = -1;
  for (i = 0;i < N_OUTPUT;i++) 
    {
      if (results[i] > max_result)
        {
          max_result = results[i];
          max = i;
        }
    }
  n_total++;
  if (labels[nb] == max) 
    {
      n_success++;
      if (vflag)
        fprintf(stderr, "%d: %d %d\n", nb, labels[nb], max);
    }
  else
    {
      if (vflag)
        fprintf(stderr, "%d: not found\n", nb);
    }
}

void	run()
{
  int i;

  for (i = 0; i < n_images;i++)
    run_image(i);

  printf("%f success ratio\n", (float) n_success/n_total * 100);
}

int	work(void *unused)
{
  static int	i = 0;
  
  if (i == nlearnings)
    return (1);
  if (vflag)
    printf("%d\n", i);
  learn();
  i++;
  return (0);
}

void	doit(int argc, char **argv)
{
  unsigned int	layer_defs[3];
  
  mnist_labels_load(labels_fname, &labels, &n_labels);
  mnist_images_load(images_fname, &images, &n_images);
  if (n_labels != n_images)
    myerror(FALSE, "n_labels %d and n_images %d mismatch", n_labels, n_images);
  assert(n_images>0);
  if (vflag)
    printf("n_images=%d %dx%d\n", n_labels, images[0].n_rows, images[0].n_cols);
  assert(N_INPUT == images[0].n_cols*images[0].n_rows);
  layer_defs[0] = N_INPUT;
  layer_defs[1] = N_HIDDEN;
  layer_defs[2] = N_OUTPUT;
  nn = neural_network_new(3, layer_defs);
  printf("nn ok\n");
  if (access(DATAFILE, R_OK) == 0)
    load();
  if (lflag)
    while (work(NULL) == 0);
  if (sflag)
    save();
  if (iflag)
    run();
  neural_network_delete(nn);
}

int	main(int argc, char **argv)
{
  char  opt;
  
  while ((opt = getopt(argc, argv, "vsile:I:L:n:")) != -1)
    switch (opt)
      {
      case 'v':
        vflag = 1;
        break ;
      case 's':
	sflag = 1;
	break ;
      case 'i':
	iflag = 1;
	break ;
      case 'l':
	lflag = 1;
	break ;
      case 'e':
	every = atoi(optarg);
	break ;
      case 'I':
	images_fname = xstrdup(optarg);
	break ;
      case 'L':
	labels_fname = xstrdup(optarg);
	break ;
      case 'n':
	nlearnings = atoi(optarg);
	break ;
      case '?':
      default:
        usage();
      }
  argc -= optind;
  argv += optind;
  if (!sflag && !iflag)
    fprintf(stderr, "Warning, I would not save data\n");
  if (!(images_fname && labels_fname))
    usage();
  doit(argc, argv);
}
