/* FILE NAME: OBJ.H
 * PROGRAMMER: Gumirov Artur
 * DATE: 06.06.2015
 * PURPOSE: WinAPI windowed application sample
 */

#ifndef __OBJ3D_H_
#define __OBJ3D_H_

#include "vec.h"
#include "def.h"

/* Draw object functioln.
 * ARGUMENTS:
 *   - context of the window:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID ObjDraw( HDC hDC, INT X, INT Y, DWORD Color, ag6ANIM *Ani );

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName );

#endif /* __OBJ3D_H_ */
