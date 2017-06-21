/**
 * @file   nnsave.h
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:38:46 2017
 * 
 * @brief  neural network save routines
 * 
 * 
 */
#ifndef NNSAVE_H
#define NNSAVE_H	1
#include "gen.h"

/* PROTO nnsave.c */
/* nnsave.c */
float my_htonf P((float h));
float my_ntohf P((float n));
void neuron_layer_save P((FILE *f, t_neuron_layer *nl));
void neural_network_save P((FILE *f, t_neural_network *nn));
void neuron_layer_load P((FILE *f, t_neuron_layer *nl));
void neural_network_file_guess_structure P((FILE *f, unsigned int *num_layers_ret, unsigned int **layer_defs_ret));
void neural_network_load P((FILE *f, t_neural_network *nn));
#endif
