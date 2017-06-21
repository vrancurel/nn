/**
 * @file   interf.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:42:21 2017
 * 
 * @brief  GUI
 * 
 * 
 */

#include "interf.h"

char	*fallback[] = 
{
  NULL
};

XtAppContext	app_context;
Widget		viewport;
Widget		toplevel;
Widget		neur_widget;

void	interf_init(int argc, char **argv, t_neural_network *nn)
{
  toplevel = XtVaAppInitialize(&app_context, "Neurapp", NULL, 0,
			       &argc, argv, fallback, NULL);
  viewport = XtVaCreateManagedWidget("viewport", viewportWidgetClass,
                                     toplevel,
                                     XtNallowVert, True,
                                     XtNallowHoriz, True,
                                     NULL);
  neur_widget = XtVaCreateManagedWidget("neur", neurWidgetClass,
                                        viewport,
                                        XtNneuralNetwork, nn,
                                        NULL);
  XtRealizeWidget(toplevel);
}

void	interf_refresh()
{
  Dimension	width, height;
  
  assert(neur_widget);
  assert(XtIsRealized(neur_widget));
  XtVaGetValues(neur_widget,
		XtNwidth,	&width,
		XtNheight,	&height,
		NULL);
  XClearArea(XtDisplay(neur_widget), XtWindow(neur_widget),
	     0, 0, width, height, True);
  XFlush(XtDisplay(toplevel));
}

void	xt_input_proc(XtPointer closure, int *fd, XtInputId *id)
{
  t_interf_input_proc_data	*iipd;

  iipd = (t_interf_input_proc_data *)closure;
  iipd->proc(iipd->data);
}

XtWorkProcId work_proc_id;

Boolean	xt_work_proc(XtPointer closure)
{
  t_interf_work_proc_data	*iwpd;

  iwpd = (t_interf_work_proc_data *)closure;
  if (iwpd->proc(iwpd->data))
    XtRemoveWorkProc(work_proc_id);
  return (False);
}

void	interf_loop(t_interf_input_proc	proc1,
		    void		*data1,
		    t_interf_work_proc	proc2,
		    void		*data2)
{
  t_interf_input_proc_data	iipd;
  t_interf_work_proc_data	iwpd;

  if (proc1)
    {
      iipd.proc = proc1;
      iipd.data = data1;
      XtAppAddInput(app_context, 0, (void *)XtInputReadMask,
		    xt_input_proc, &iipd);
    }
  if (proc2)
    {
      iwpd.proc = proc2;
      iwpd.data = data2;
      work_proc_id = XtAppAddWorkProc(app_context, xt_work_proc, &iwpd);
    }
  XtAppMainLoop(app_context);
}
