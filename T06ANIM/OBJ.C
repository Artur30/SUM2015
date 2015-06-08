/* FILE NAME: OBJ.H
 * PROGRAMMER: Gumirov Artur
 * DATE: 06.06.2015
 * PURPOSE: WinAPI windowed application sample
 */

#include <stdio.h>
#include <stdlib.h>

#include "obj.h"

VEC *ObjV; /* Vertex coordinates */
INT ObjNumOfV; /* Number of model vertices */

VOID ObjDraw( HDC hDC )
{
  INT i;

  for (i = 0; i < ObjNumOfV; i++)
  {
    ObjV[i].X = ObjV[i].X * 50;
    ObjV[i].Y = ObjV[i].Y * 50;
    ObjV[i].Z = ObjV[i].Z * 50;

    SelectObject(hDC, GetStockObject(BLACK_BRUSH));
    Ellipse(hDC, 300 - ObjV[i].X - 2, 300 - ObjV[i].Y + 2, 300 + ObjV[i].X + 2, 300 - ObjV[i].Y - 2);
  }

} /* End of 'ObjDraw' function */

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */

BOOL ObjLoad( CHAR *FileName )
{
  FILE *F;
  INT nv = 0;
  static CHAR Buf[10000];

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
  }

  /* Allocate memory for data */
  if ((ObjV = malloc(sizeof(VEC) * nv)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Read vertices */
  rewind(F);
  nv = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &ObjV[nv].X, &ObjV[nv].Y, &ObjV[nv].Z);
      nv++;
    }
  }

  fclose(F);

  ObjNumOfV = nv;
  return TRUE;
} /* End of 'ObjLoad' function */