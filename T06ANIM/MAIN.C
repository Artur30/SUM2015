/* FILE NAME: MAIN.C
 * PROGRAMMER: Gumirov Artur
 * DATE: 06.06.2015
 * PURPOSE: WinAPI windowed application sample
 */

#include <stdio.h>

#include <math.h>

#include <windows.h>

#include "vec.h"

VOID main( VOID )
{
  MATR M = 
  {
    {
      {1, 1, 1, 0},
      {1, 1, 2, 0},
      {1, 2, 1, 1},
      {2, 1, 1, 1}
    }
  }, M1, M2;

  M1 = MatrInverse(M);
  M2 = MatrMulMatr(M, M1);
  M2 = MatrMulMatr(M1, M);

  M = MatrRotate(90, 0, 1, 0);
  M1 = MatrInverse(M);

  getch()
}
