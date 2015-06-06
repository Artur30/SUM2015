/* FILE NAME: GLOBE.H
 * PROGRAMMER: Gumirov Artur
 * DATE: 05.06.2015
 * PURPOSE: WinAPI windowed application sample
 */

#ifndef __GLOBE_H_
#define __GLOBE_H_

typedef struct
{
  DOUBLE X, Y, Z;
} VEC;

VOID GlobeBuild( VOID );
VOID GlobeDraw( HDC hDC, INT w, INT h );

#define PI 3.141592653589793238462643383278502884197169399375105820

#endif