/* FILE NAME: GLOBEDRAW.C
 * PROGRAMMER: Gumirov Artur
 * DATE: 05.06.2015
 * PURPOSE: WinAPI windowed application sample
 */

#include <windows.h>

#include "globe.h"

#include "image.h"

#include <time.h>

#include <math.h>

#define N 30 * 5

#define M 30 * 10

#define PI 3.141592653589793238462643383278502884197169399375105820

static VEC Grid[N][M];

IMAGE GlobeImage;

/* Функция векторного произведения */
VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* Функция вычитания векторов */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;

  return r;
} /* End of 'RotateX' function */

VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  
  POINT pnts[4];

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;

  if ((pnts[0].x - pnts[1].x) * (pnts[0].y + pnts[1].y) +
      (pnts[1].x - pnts[2].x) * (pnts[1].y + pnts[2].y) +
      (pnts[2].x - pnts[3].x) * (pnts[2].y + pnts[3].y) +
      (pnts[3].x - pnts[0].x) * (pnts[3].y + pnts[0].y) < 0)
  return;


  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

VOID GlobeBuild( VOID )
{
  INT i, j;
  DOUBLE phi, theta, R = 288, t = clock() / (DOUBLE)CLOCKS_PER_SEC;

  if (GlobeImage.hBm == NULL)
    ImageLoad(&GlobeImage, "globe.bmp");

  srand(30);

  for (i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * PI; /* изменение угла тетта в цикле */
    for (j = 0; j < M; j++)
    {
       phi = j / (M - 1.0) * 2 * PI + t * 0.30;
       /* Проекции */
       Grid[i][j].X = R * sin(theta) * sin(phi);
       Grid[i][j].Y = R * cos(theta);
       Grid[i][j].Z = R * sin(theta) * cos(phi);

       Grid[i][j] = RotateX(Grid[i][j], sin(t * 3) * 30);
    }
  }

} /* End of 'GlobeBuild' function */

VOID GlobeDraw( HDC hDC, INT w, INT h )
{
  INT i, j;
  DOUBLE R = 288, t = clock() / (DOUBLE)CLOCKS_PER_SEC;

   for (i = 0; i < N - 1; i++)
    for (j = 0; j < M - 1; j++)
    {
      INT
        ix = j * (GlobeImage.W - 1) / (M - 1),
        iy = i * (GlobeImage.H - 1) / (N - 1),
        x = Grid[i][j].X + w / 2,
        y = Grid[i][j].Y + h / 2;


      SetDCBrushColor(hDC, ImageGetP(&GlobeImage, ix, iy));
      DrawQuad(hDC, Grid[i][j], Grid[i][j + 1], Grid[i + 1][j + 1], Grid[i + 1][j], w, h);
    }
} /* End of 'GlobeDraw' function */


