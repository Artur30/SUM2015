/* FILENAME: RENDER.C
 * PROGRAMMER: Gumirov Artur
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 09.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

/* Ìàòðèöû */
MATR
  PD6_RndMatrWorld = PD6_UNIT_MATR,
  PD6_RndMatrView = PD6_UNIT_MATR,
  PD6_RndMatrProj = PD6_UNIT_MATR,
  PD6_RndMatrWorldViewProj = PD6_UNIT_MATR;

/* Ïàðàìåòðû ïðîåöèðîâàíèÿ */
DBL
  PD6_RndWp = 3, PD6_RndHp = 3,     /* ðàçìåðû îáðàñòè ïðîåöèðîâàíèÿ */
  PD6_RndProjDist = 5;              /* ðàññòîÿíèå äî ïëîñêîñòè ïðîåêöèè */

/* Ôóíêöèÿ ïðåîáðàçîâàíèÿ èç ìèðîâîé ñèñòåìû êîîðäèíàò â êàäð.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - èñõîäíàÿ òî÷êà:
 *       VEC P;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ:
 *   (POINT) êîîðäèíàòû â êàäðå.
 */
POINT PD6_RndWorldToScreen( VEC P )
{
  POINT Ps;

  /* ïðåîáðàçîâàíèå ÑÊ */
  P = PointTransform(P, PD6_RndMatrWorldViewProj);

  Ps.x = (P.X / 2 + 0.5) * PD6_Anim.W + PD6_Anim.PosModel.X;
  Ps.y = (-P.Y / 2 + 0.5) * PD6_Anim.H + PD6_Anim.PosModel.Y;

  return Ps;
} /* End of 'PD6_RndWorldToScreen' function */


/* Ôóíêöèÿ çàãðóçêè ãåîìåòðè÷åñêîãî îáúåêòà.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - ñòðóêòóðà îáúåêòà äëÿ çàãðóçêè:
 *       pd6GOBJ *GObj;
 *   - èìÿ ôàéëà:
 *       CHAR *FileName;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ:
 *   (BOOL) TRUE ïðè óñïåõå, FALSE èíà÷å.
 */
BOOL PD6_RndGObjLoad( pd6GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(pd6GOBJ));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((GObj->V = malloc(sizeof(VEC) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  GObj->F = (INT (*)[3])(GObj->V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &GObj->V[nv].X, &GObj->V[nv].Y, &GObj->V[nv].Z);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }

  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;
  return TRUE;
} /* End of 'PD6_RndGObjLoad' function */

/* Ôóíêöèÿ îòðèñîâêè ãåîìåòðè÷åñêîãî îáúåêòà.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - ñòðóêòóðà îáúåêòà äëÿ çàãðóçêè:
 *       pd6GOBJ *GObj;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_RndGObjDraw( pd6GOBJ *GObj )
{
  INT i;
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* ïðîåöèðóåì âñå òî÷êè */
  PD6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(PD6_RndMatrWorld, PD6_RndMatrView), PD6_RndMatrProj);
  for (i = 0; i < GObj->NumOfV; i++)
    pnts[i] = PD6_RndWorldToScreen(GObj->V[i]);

  /* ðèñóåì òðåóãîëüíèêè */
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n1 = GObj->F[i][0],
      n2 = GObj->F[i][1],
      n3 = GObj->F[i][2];

    MoveToEx(PD6_Anim.hDC, pnts[n1].x, pnts[n1].y, NULL);
    LineTo(PD6_Anim.hDC, pnts[n2].x, pnts[n2].y);
    LineTo(PD6_Anim.hDC, pnts[n3].x, pnts[n3].y);
    LineTo(PD6_Anim.hDC, pnts[n1].x, pnts[n1].y);
  }

  free(pnts);
} /* End of 'PD6_RndGObjDraw' function */


VOID PD6_RndGObjFree( pd6GOBJ *GObj )
{
  free(GObj->V);
  memset(GObj, 0, sizeof(pd6GOBJ));
} /* End of 'PD6_RndGObjFree' function */

/* END OF 'RENDER.C' FILE */
