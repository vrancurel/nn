/**
 * @file   nnview.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:41:42 2017
 * 
 * @brief  View a NN as saved
 * 
 * 
 */

#include "gen.h"

extern int      optind;
extern char     *optarg;

int		vflag = 0;	/* verbose mode			*/
int		xflag = 0;	/* display X GUI		*/
char		*fname = NULL;
int		slw = 0;	/* significantLineWidth		*/

t_neural_network	*nn;

void	usage()
{
  fprintf(stderr, "usage: nnview [-vx] fname\n");
  exit(1);
}

void	doit(int argc, char **argv)
{
  unsigned int	*layer_defs;
  unsigned int	num_layers;
  unsigned int	i;
  FILE		*f;
  
  if ((f = fopen(fname, "r")) == NULL)
    myerror(TRUE, "%s", fname);
  printf("Loading %s\n", fname);
  neural_network_file_guess_structure(f, &num_layers, &layer_defs);
  if (vflag)
    {
      printf("%d {", num_layers);
      for (i = 0;i < num_layers;i++)
	{
	  printf("%d", layer_defs[i]);
	  if (i != (num_layers - 1))
	    printf(", ");
	}
      printf("}\n");
    }
  nn = neural_network_new(num_layers, layer_defs);
  free(layer_defs);
  rewind(f);
  neural_network_load(f, nn);
  fclose(f);
  printf("ok\n");
  if (xflag)
    interf_init(argc, argv, nn);
  if (xflag)
    interf_loop(NULL, NULL, NULL, NULL);
}

int	main(int argc, char **argv)
{
  char  opt;
  
  while ((opt = getopt(argc, argv, "vxs:")) != -1)
    switch (opt)
      {
      case 'v':
        vflag = 1;
        break ;
      case 'x':
	xflag = 1;
	break ;
      case 's':
	slw = atoi(optarg);
	break ;
      case '?':
      default:
        usage();
      }
  argc -= optind;
  argv += optind;
  if (argc != 1)
    usage();
  fname = argv[0];
  doit(argc, argv);
}
