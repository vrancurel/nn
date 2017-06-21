@class Neur(Core) @file=Neur @nodoc

@imports
@incl "gen.h"

@public
@var t_neural_network			*neuralNetwork = NULL
@var Dimension				neuronRadius = 3
@var Dimension				margin = 10
@var Dimension				hspace = 300
@var Dimension				vspace = 100
@var Pixel				foreground = <String> "black"
@var Pixel				neuronBackground = <String> "gold"
@var Pixel				positiveLineColor = <String> "blue";
@var Pixel				negativeLineColor = <String> "red";
@var Pixel				lineFactor = 5
@var Boolean				displayLabels = True
@var int				significantLinesWidth = 0

@private
@var GC					gc
@var GC					neuron_background_gc
@var GC					positive_line_gc
@var GC					negative_line_gc
@var XPoint				**points
@var int				max_neurons

@methods
@proc		destroy
{
  XtWarning("Not yet implemented");
}

@proc		initialize
{
  XGCValues	xgcv;
  int		i;

  if (!$neuralNetwork)
  {
    XtWarning("Need a neural network as a resource");
    exit(1);
  }
  $max_neurons = 0;
  for (i = 0;i < $neuralNetwork->num_layers;i++)
    $max_neurons = MY_MAX($neuralNetwork->layer_defs[i], $max_neurons);
  $height = ($neuronRadius * 2 + 2) * $max_neurons + 2 * $margin +
    $vspace * ($max_neurons - 1); 
  $width = ($neuronRadius * 2 + 2) * ($neuralNetwork->num_layers) + 
    ($margin + $hspace) * ($neuralNetwork->num_layers - 1);
  xgcv.foreground = $foreground;
  $gc = XCreateGC(XtDisplay($), DefaultRootWindow(XtDisplay($)),
		  GCForeground, &xgcv);
  xgcv.foreground = $positiveLineColor;
  $positive_line_gc = XCreateGC(XtDisplay($), DefaultRootWindow(XtDisplay($)),
				GCForeground, &xgcv);
  xgcv.foreground = $negativeLineColor;
  $negative_line_gc = XCreateGC(XtDisplay($), DefaultRootWindow(XtDisplay($)),
				GCForeground, &xgcv);
  xgcv.foreground = $neuronBackground;
  $neuron_background_gc = XCreateGC(XtDisplay($), 
				    DefaultRootWindow(XtDisplay($)),
				    GCForeground, &xgcv);
  $points = xmalloc($neuralNetwork->num_layers * sizeof (XPoint *));
  for (i = 0;i < $neuralNetwork->num_layers;i++)
    $points[i] = xmalloc($neuralNetwork->layer_defs[i] * sizeof (XPoint));
}

@proc resize
{
  $vspace = ($height - ($neuronRadius * 2 + 2) * $max_neurons + 2 * $margin) / 
    ($max_neurons - 1); 
  $hspace =
    ($width - ($neuronRadius * 2 + 2) * ($neuralNetwork->num_layers)) / 
    ($neuralNetwork->num_layers - 1) - $margin;
}

@proc void	draw_neuron($, Position x, Position y)
{
  XFillArc(XtDisplay($), XtWindow($), $neuron_background_gc,
	   x, y, $neuronRadius * 2, $neuronRadius * 2, 360 * 64, 360 * 64);
  XDrawArc(XtDisplay($), XtWindow($), $gc,
	   x, y, $neuronRadius * 2, $neuronRadius * 2, 360 * 64, 360 * 64);
}

@proc void	display_label($, 
			      unsigned int i,
			      unsigned int j, 
			      unsigned int k)
{
  char	buf[256];
  
  snprintf(buf, sizeof (buf), "%f", 
	   $neuralNetwork->layers[k]->neurons[i].weights[j]);
  buf[sizeof (buf) - 1] = 0;
  XDrawString(XtDisplay($), XtWindow($), $gc,
	      $points[k][i].x +
	      ($points[k - 1][j].x + 
	       2 * $neuronRadius + 2 -
	       $points[k][i].x) / 2,
	      $points[k][i].y + 1 + $neuronRadius +
	      ($points[k - 1][j].y + 1 + $neuronRadius -
	       $points[k][i].y + 1 + $neuronRadius) / 2,
	      buf, strlen(buf));
}    

@proc void	display_line($,
			     Region region, 
			     unsigned int i,
			     unsigned int j,
			     unsigned int k)
{
  XGCValues	xgcv;
  GC		gc;
  Position	x1, y1, x2, y2;
  Dimension	width, height;
  
  /*printf("%d %d %d\n", i, j, k);*/
  if ($neuralNetwork->layers[k]->neurons[i].weights[j] >= 0.0)
    gc = $positive_line_gc;
  else
    gc = $negative_line_gc;
  xgcv.line_width = 
    ABS($neuralNetwork->layers[k]->neurons[i].weights[j]) * 
    $lineFactor;
  if (xgcv.line_width >= $significantLinesWidth)
    {
      XChangeGC(XtDisplay($), gc, GCLineWidth, &xgcv);
      x1 = MY_MIN($points[k][i].x, 
		  $points[k - 1][j].x + 2 * $neuronRadius + 2);
      y1 = MY_MIN($points[k][i].y + 1 + $neuronRadius,
		  $points[k - 1][j].y + 1 + $neuronRadius);
      x2 = MY_MAX($points[k][i].x,
		  $points[k - 1][j].x + 2 * $neuronRadius + 2);
      y2 = MY_MAX($points[k][i].y + 1 + $neuronRadius,
		  $points[k - 1][j].y + 1 + $neuronRadius);
      if (XRectInRegion(region, x1, y1, x2 - x1, y2 - y1))
	{
	  XDrawLine(XtDisplay($), XtWindow($), gc,
		    $points[k][i].x,
		    $points[k][i].y + 1 + $neuronRadius,
		    $points[k - 1][j].x + 2 * $neuronRadius + 2,
		    $points[k - 1][j].y + 1 + $neuronRadius);
	  if ($displayLabels)
	    display_label($, i, j, k);
	}
    }
}

@proc expose
{
  Position	xoff, yoff;
  Dimension	layer_height;
  unsigned int	i, j, k;

  region = XCreateRegion();
  XtAddExposureToRegion(event, region);
  for (j = 0;j < $neuralNetwork->num_layers;j++)
    {
      layer_height = $neuralNetwork->layer_defs[j] * (2 * $neuronRadius + 2) + 
	($neuralNetwork->layer_defs[j] - 1) * $vspace;
      yoff = ($height - 2 * $margin - layer_height) / 2 + $margin;
      xoff = $margin + ($neuronRadius * 2 + 2 + $hspace) * j;;
      for (i = 0;i < $neuralNetwork->layer_defs[j];i++)
	{
	  if (XRectInRegion(region, xoff,
			    yoff + i * (($neuronRadius * 2 + 2) + $vspace),
			    $neuronRadius * 2, $neuronRadius * 2))
	    draw_neuron($, 
			xoff, yoff + i * (($neuronRadius * 2 + 2) + $vspace));
	  $points[j][i].x = xoff;
	  $points[j][i].y = yoff + i * (($neuronRadius * 2 + 2) + $vspace);
	}
    }
  for (k = 1;k < $neuralNetwork->num_layers;k++)
    {
      for (i = 0;i < $neuralNetwork->layer_defs[k];i++)
	{
	  for (j = 0;j < $neuralNetwork->layers[k]->num_weights;j++)
	    {
	      display_line($, region, i, j, k);
	    }
	}
    }
  XDestroyRegion(region);   
}		
