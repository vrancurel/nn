/**
 * @file   interf.h
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:41:16 2017
 * 
 * @brief  GUI
 * 
 * 
 */

#ifndef INTERF_H
#define INTERF_H	1
#include "gen.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/Shell.h>
#include <X11/Xaw/Viewport.h>
#include "X11/Neur.h"

/* PROTO interf.c */
/* interf.c */
void interf_init P((int argc, char **argv, t_neural_network *nn));
void interf_refresh P((void));
void xt_input_proc P((XtPointer closure, int *fd, XtInputId *id));
Boolean xt_work_proc P((XtPointer closure));
void interf_loop P((t_interf_input_proc proc1, void *data1, t_interf_work_proc proc2, void *data2));
#endif
