/**
 * @file   nnsave.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:39:51 2017
 * 
 * @brief  neural network save routines
 * 
 * 
 */

#include "gen.h"

float	my_htonf(float h)
{
  unsigned int	*ph, *pn;
  float		n;

  ph = (unsigned int *)&h;
  pn = (unsigned int *)&n;
  *pn = htonl(*ph);
  return (n);
}

float	my_ntohf(float n)
{
  unsigned int	*ph, *pn;
  float		h;
  
  ph = (unsigned int *)&h;
  pn = (unsigned int *)&n;
  *ph = ntohl(*pn);
  return (h);
}

void	neuron_layer_save(FILE			*f,
			  t_neuron_layer	*nl)
{
  int		i, j;
  unsigned int	nnum_neurons, nnum_weights;
  float		nweight;

  nnum_neurons = htonl(nl->num_neurons);
  if (fwrite(&nnum_neurons, sizeof (nnum_neurons), 1, f) != 1)
    myerror(FALSE, "Saving: Can't write num_neurons");
  nnum_weights = htonl(nl->num_weights);
  if (fwrite(&nnum_weights, sizeof (nnum_weights), 1, f) != 1)
    myerror(FALSE, "Saving: Can't write num_weights");
  for (i = 0;i < nl->num_neurons;i++)
    for (j = 0;j < nl->num_weights;j++)
      {
	nweight = my_htonf(nl->neurons[i].weights[j]);
	if (fwrite(&nweight, sizeof (nweight), 1, f) != 1)
	  myerror(FALSE, "Saving: Can't write weights[%d]", j);
      }
}

void	neural_network_save(FILE		*f,
			    t_neural_network	*nn)
{
  unsigned int	nnum_layers, nnum_inputs;
  unsigned int	i;

  nnum_layers = htonl(nn->num_layers);
  if (fwrite(&nnum_layers, sizeof (nnum_layers), 1, f) != 1)
    myerror(FALSE, "Saving: Can't write num_layers");
  nnum_inputs = htonl(nn->layer_defs[0]);
  if (fwrite(&nnum_inputs, sizeof (nnum_inputs), 1, f) != 1)
    myerror(FALSE, "Saving: Can't write num_inputs");
  for (i = 1;i < nn->num_layers;i++)
    neuron_layer_save(f, nn->layers[i]);
}

void	neuron_layer_load(FILE			*f,
			  t_neuron_layer	*nl)
{
  int		i, j;
  unsigned int	num_neurons, num_weights;

  if (fread(&num_neurons, sizeof (num_neurons), 1, f) != 1)
    myerror(FALSE, "Loading: Can't read num_neurons");
  num_neurons = ntohl(num_neurons);
  if (fread(&num_weights, sizeof (num_weights), 1, f) != 1)
    myerror(FALSE, "Loading: Can't read num_weights");
  num_weights = ntohl(num_weights);
  if (num_neurons != nl->num_neurons || num_weights != nl->num_weights)
    myerror(FALSE, "Loading: Neuron layer size mismatch");
  for (i = 0;i < nl->num_neurons;i++)
    for (j = 0;j < nl->num_weights;j++)
      {
	if (fread(&nl->neurons[i].weights[j], sizeof (float), 1, f) != 1)
	  myerror(FALSE, "Loading: Can't read weights[%d]", j);
	nl->neurons[i].weights[j] = my_ntohf(nl->neurons[i].weights[j]);
      }
}

void	neural_network_file_guess_structure(FILE	 *f,
					    unsigned int *num_layers_ret,
					    unsigned int **layer_defs_ret)
{
  unsigned int	i, j;
  unsigned int	num_layers, num_inputs;
  unsigned int	num_neurons, num_weights;
 
  if (fread(&num_layers, sizeof (num_layers), 1, f) != 1)
    myerror(FALSE, "Guessing: Can't read num_layers");
  num_layers = ntohl(num_layers);
  (*num_layers_ret) = num_layers;
  if (fread(&num_inputs, sizeof (num_inputs), 1, f) != 1)
    myerror(FALSE, "Loading: Can't read num_inputs");
  num_inputs = ntohl(num_inputs);
  (*layer_defs_ret) = xmalloc(num_layers * sizeof (unsigned int));
  (*layer_defs_ret)[0] = num_inputs;
  for (i = 1;i < num_layers;i++)
    {
      if (fread(&num_neurons, sizeof (num_neurons), 1, f) != 1)
	myerror(FALSE, "Guessing: Can't read num_neurons");
      num_neurons = ntohl(num_neurons);
      (*layer_defs_ret)[i] = num_neurons;
      if (fread(&num_weights, sizeof (num_weights), 1, f) != 1)
	myerror(FALSE, "Guessing: Can't read num_weights");
      num_weights = ntohl(num_weights);
      for (j = 0;j < num_neurons;j++)
	if (fseek(f, num_weights * sizeof (float), SEEK_CUR) == -1) 
	  myerror(TRUE, "Guessing: Can't seek weights");
    }
}

void	neural_network_load(FILE		*f,
			    t_neural_network	*nn)
{
  unsigned int	i;
  unsigned int	num_inputs;

  if (fread(&nn->num_layers, sizeof (nn->num_layers), 1, f) != 1)
    myerror(FALSE, "Loading: Can't read num_layers");
  nn->num_layers = ntohl(nn->num_layers);
  if (fread(&num_inputs, sizeof (num_inputs), 1, f) != 1)
    myerror(FALSE, "Loading: Can't read num_inputs");
  num_inputs = ntohl(num_inputs);
  if (num_inputs != nn->layer_defs[0])
    myerror(FALSE, "Loading: Num_inputs size mismatch");
  for (i = 1;i < nn->num_layers;i++)
    {
      neuron_layer_load(f, nn->layers[i]);
      nn->layer_defs[i] = nn->layers[i]->num_neurons;
    }
}
