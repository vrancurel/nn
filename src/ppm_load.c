/**
 * @file   ppm_load.c
 * @author vr <vr@vr-VirtualBox>
 * @date   Mon Jun 19 18:39:22 2017
 * 
 * @brief  load ppm files
 * 
 * 
 */

#include "gen.h"

t_ppm	*ppm_load(char *fname)
{
  char	buf[16];
  FILE	*f;
  int	maxval;
  t_ppm	*ppm;

  if ((f = fopen(fname, "rb")) == NULL)
    myerror(TRUE, "fopen %s", fname);
  if (!fgets(buf, sizeof (buf), f))
    myerror(TRUE, "fopen");
  assert(strlen(buf) >= 2);
  if (buf[0] != 'P' || buf[1] != '6')
    myerror(FALSE, "image format nust be P6");
  ppm = xmalloc(sizeof (*ppm));
  if (fscanf(f, "%d %d", &ppm->width, &ppm->height) != 2)
    myerror(FALSE, "error loading");
  if (fscanf(f, "%d", &maxval) != 1)
    myerror(FALSE, "error loading");  
  while (fgetc(f) != '\n')
    ;
  ppm->data = xmalloc(3 * ppm->width * ppm->height);
  if (fread(ppm->data, 3 * ppm->width, ppm->height, f) != ppm->height)
    myerror(FALSE, "error loading");
  fclose(f);
  return (ppm);
}

t_pgm	*pgm_load(char *fname)
{
  char	buf[16];
  FILE	*f;
  int	maxval;
  t_pgm	*pgm;
  char	c;

  if ((f = fopen(fname, "rb")) == NULL)
    myerror(TRUE, "fopen %s", fname);
  if (!fgets(buf, sizeof (buf), f))
    myerror(TRUE, "fopen");
  assert(strlen(buf) >= 2);
  if (buf[0] != 'P' || buf[1] != '5')
    myerror(FALSE, "image format nust be P5");
  pgm = xmalloc(sizeof (*pgm));
  while (1)
    {
      if ((c = fgetc(f)) == '#')
	while (fgetc(f) != '\n')
	  ;
      else
	{
	  ungetc(c, f);
	  break ;
	}
    }
  if (fscanf(f, "%d %d", &pgm->width, &pgm->height) != 2)
    myerror(FALSE, "error loading");
  if (fscanf(f, "%d", &maxval) != 1)
    myerror(FALSE, "error loading");  
  while (fgetc(f) != '\n')
    ;
  pgm->data = xmalloc(pgm->width * pgm->height);
  if (fread(pgm->data, pgm->width, pgm->height, f) != pgm->height)
    myerror(FALSE, "error loading");
  /*bufdump(stdout, pgm->data, pgm->width * pgm->height);*/
  fclose(f);
  return (pgm);
}
