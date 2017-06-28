/**
 * @file   neural.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:40:09 2017
 * 
 * @brief  neural network routines
 * 
 * 
 */

#include "gen.h"

extern int	vflag;

t_neuron_layer	*neuron_layer_new(unsigned int	num_neurons,
				  unsigned int	num_weights)
{
  t_neuron_layer	*nl;
  int			i, j;

  nl = xmalloc(sizeof (*nl));
  nl->neurons = xmalloc(num_neurons * sizeof (t_neuron));
  nl->num_neurons = num_neurons;
  nl->num_weights = num_weights;
  nl->pre_neuron_weights = xmalloc(num_neurons * num_weights * sizeof (float));
  for (j = 0;j < num_neurons;j++)
    {
      nl->neurons[j].weights = nl->pre_neuron_weights + j * num_weights;
      for (i = 0;i < num_weights;i++)
	{
	  nl->neurons[j].weights[i] = 0.5 - rand() / (float)RAND_MAX;
	}
    }
  return (nl);
}

void	neuron_layer_delete(t_neuron_layer *nl)
{
  free(nl->pre_neuron_weights);
  free(nl->neurons);
  free(nl);
}

float	sigmoid(float data)
{
  return (1.0 / (1.0 + exp(-data)));
}

t_neural_network	*neural_network_new(unsigned int	num_layers,
					    unsigned int	*layer_defs)
{
  t_neural_network	*nn;
  unsigned int		i;

  assert(num_layers > 2);
  nn = xmalloc(sizeof (*nn));
  nn->num_layers = num_layers;
  nn->layer_defs = xmalloc(nn->num_layers * sizeof (unsigned int));
  bcopy(layer_defs, nn->layer_defs,
	nn->num_layers * sizeof (unsigned int));
  nn->layers = xmalloc(nn->num_layers * sizeof (t_neuron_layer *));
  nn->layers[0] = NULL;
  for (i = 1;i < nn->num_layers;i++)
    nn->layers[i] = neuron_layer_new(nn->layer_defs[i], nn->layer_defs[i - 1]);
  nn->pre_output = xmalloc(nn->layer_defs[nn->num_layers - 1] * 
			   sizeof (float));
  nn->pre_weight_deltas = xmalloc(nn->num_layers * sizeof (float *));
  nn->pre_weight_deltas[0] = NULL;
  for (i = 1;i < nn->num_layers;i++)
    nn->pre_weight_deltas[i] = xmalloc(nn->layer_defs[i] * sizeof (float));
  return (nn);
}

void	neural_network_delete(t_neural_network	*nn)
{
  unsigned int	i;

  free(nn->pre_output);
  for (i = 1;i < nn->num_layers;i++)
    free(nn->pre_weight_deltas[i]);
  free(nn->pre_weight_deltas);  
  for (i = 1;i < nn->num_layers;i++)
    neuron_layer_delete(nn->layers[i]);
  free(nn->layers);
  free(nn->layer_defs);
  free(nn);
}

void	neural_network_run(t_neural_network	*nn,
			   float		*data,
			   float		*result)
{
  int		i, j, k;
  float		sum;

  for (j = 0;j < nn->layer_defs[1];j++)
    {
      sum = 0;
      for (i = 0;i < nn->layer_defs[0];i++)
	{
	  sum += nn->layers[1]->neurons[j].weights[i] * data[i];
	}
      nn->layers[1]->neurons[j].output = sigmoid(sum);
    }
  for (k = 2; k < (nn->num_layers - 1);k++)
    {
      for (j = 0;j < nn->layer_defs[k];j++)
	{
	  sum = 0;
	  for (i = 0;i < nn->layer_defs[k - 1];i++)
	    {
	      sum += nn->layers[k]->neurons[j].weights[i] * 
		nn->layers[k - 1]->neurons[i].output;
	    }
	  nn->layers[k]->neurons[j].output = sigmoid(sum);
	}
    }
  for (j = 0;j < nn->layer_defs[nn->num_layers - 1];j++)
    {
      sum = 0;
      for (i = 0;i < nn->layer_defs[nn->num_layers - 2];i++)
	{
	  sum += nn->layers[nn->num_layers - 1]->neurons[j].weights[i] *
	    nn->layers[nn->num_layers - 2]->neurons[i].output;
	}
      result[j] = sigmoid(sum);
    }
}

void	neural_network_train(t_neural_network	*nn,
			     float		*data, 
			     float		*desired,
			     float		max_mse,/* Mean Square Error */
			     float		eta)	/* learning rate */
{
  float		*output, **weight_deltas;
  float		mse, sum;
  int		i, j, k;

  output = nn->pre_output;
  weight_deltas = nn->pre_weight_deltas;
  while (1)
    {
      neural_network_run(nn, data, output);
      mse = 0;
      //delta on last layer and lse computation
      for (i = 0;i < nn->layer_defs[nn->num_layers - 1];i++)
	{
	  weight_deltas[nn->num_layers - 1][i] = desired[i] - output[i];
	  mse +=  weight_deltas[nn->num_layers - 1][i] *
	    weight_deltas[nn->num_layers - 1][i];
	  weight_deltas[nn->num_layers - 1][i] *= output[i] * (1 - output[i]);
	}
      //if (vflag)
      //printf("mse=%f max_mse=%f\n", mse, max_mse);
      if (mse < max_mse)
	break ;
      //delta on hidden layers
      for (k = nn->num_layers - 1;k > 1;k--)
	{
	  for (j = 0;j < nn->layer_defs[k - 1];j++)
	    {
	      sum = 0;
	      for (i = 0; i < nn->layer_defs[k];i++)
		{
		  sum += weight_deltas[k][i] *
		    nn->layers[k]->neurons[i].weights[j];
		}
	      weight_deltas[k - 1][j] = sum *
		nn->layers[k - 1]->neurons[j].output *
		(1 - nn->layers[k - 1]->neurons[j].output);
	    }
	}
      //update hidden layers weights
      for (k = nn->num_layers - 1;k > 1;k--)
	{
	  for (j = 0;j < nn->layer_defs[k];j++)
	    {
	      for (i = 0;i < nn->layer_defs[k - 1];i++)
		{
                  nn->layers[k]->neurons[j].weights[i] +=
                    eta * weight_deltas[k][j] *
                    nn->layers[k - 1]->neurons[i].output;
		}
	    }
	}
      //update input layer weights
      for (j = 0;j < nn->layer_defs[1];j++)
	{
	  for (i = 0;i < nn->layer_defs[0];i++)
	    {
              nn->layers[1]->neurons[j].weights[i] += 
                eta * weight_deltas[1][j] * data[i];
	    }
	}
    }
}
