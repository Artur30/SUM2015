/* FILENAME: CAMERA.H
 * PROGRAMMER: AG6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 12.06.2014
 */

#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "vec.h"

/* Camera properties */
typedef struct
{
  VEC
    Loc,             /* Location */
    Dir, Up, Right,  /* Orient vectors */
    At;              /* Point to view */
} ag6CAMERA;

/***
 * Camera functions
 ***/

/* Getting the camera's matrix */
MATR AG6_RndCameraGetMatrix( ag6CAMERA *Cam );

/* Setting cameras properties */
VOID AG6_RndCameraSet( ag6CAMERA *Cam, VEC Loc, VEC At, VEC Up );

/* Setting cameras properties */
VOID AG6_RndCameraMoveDir( ag6CAMERA *Cam, DBL Coef );

/* Camera move by Right-Axes function. */
VOID AG6_RndCameraMoveRight( ag6CAMERA *Cam, DBL Coef );

/* Camera move by Up-Axes function. */
VOID AG6_RndCameraMoveUp( ag6CAMERA *Cam, DBL Coef );

/* Camera rotation by axes Y function. */
VOID AG6_RndCameraRotateUp( ag6CAMERA *Cam, DBL Angle );

/* Camera rotation by camera-axes-right function. */
VOID AG6_RndCameraRotateRight( ag6CAMERA *Cam, DBL Angle );

/* Camera rotation by camera-axes-dir function. */
VOID AG6_RndCameraRotateDir( ag6CAMERA *Cam, DBL Angle );

/* Cameras normalizing vectors function */
VOID AG6_RndCameraNormalize( ag6CAMERA *Cam );

/* Camera update information function. */
VOID AG6_RndCameraUpdateInfo( ag6CAMERA *Cam );

#endif /* __CAMERA_H_ */

/* END OF 'CAMERA.H' FILE */