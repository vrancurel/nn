/**
 * @file   neural.h
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:41:02 2017
 * 
 * @brief  neural routines header
 * 
 * 
 */

/*
 * Based upon the work of Daniel Franklin libneural-1.0.2
 */
#ifndef NEURAL_H
#define NEURAL_H	1
#include "gen.h"

typedef struct	s_neuron
{
  float		*weights;
  float		output;
}		t_neuron;


typedef struct	s_neuron_layer
{
  float		*pre_neuron_weights;	/* pre-malloced for speed */
  t_neuron	*neurons;
  unsigned int	num_neurons;
  unsigned int	num_weights;
}		t_neuron_layer;

typedef struct	s_neural_network
{
  unsigned int		num_layers;
  unsigned int		*layer_defs;
  t_neuron_layer	**layers;		/* layers[0] = NULL */
  float			*pre_output;		/* pre-malloced for speed */
  float			**pre_weight_deltas;	/* idem */
}		t_neural_network;

/* PROTO neural.c */
/* neural.c */
t_neuron_layer *neuron_layer_new P((unsigned int num_neurons, unsigned int num_weights));
void neuron_layer_delete P((t_neuron_layer *nl));
float sigmoid P((float data));
t_neural_network *neural_network_new P((unsigned int num_layers, unsigned int *layer_defs));
void neural_network_delete P((t_neural_network *nn));
void neural_network_run P((t_neural_network *nn, float *data, float *result));
void neural_network_train P((t_neural_network *nn, float *data, float *desired, float max_mse, float eta));

#endif

