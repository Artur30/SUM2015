/* FILENAME: RENDER.H
 * PROGRAMMER: AG6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ������� */
extern MATR
  AG6_RndMatrWorld,
  AG6_RndMatrView,
  AG6_RndMatrProj,
  AG6_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR AG6_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  AG6_RndWp, AG6_RndHp,    /* ������� ������� ������������� */
  AG6_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT AG6_RndProg;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT AG6_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID AG6_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagag6MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} ag6MATERIAL;

/* ���������� ������� ���������� */
#define AG6_MATERIAL_MAX 300
extern INT AG6_MtlLibSize;
extern ag6MATERIAL AG6_MtlLib[AG6_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       ag6MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT AG6_MtlAdd( ag6MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT AG6_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT AG6_MtlLoad( CHAR *FileName );

/***
 * ������ � �����������
 ***/

/* ���� ���������� */
typedef enum tagag6PRIM_TYPE
{
  AG6_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  AG6_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} ag6PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagag6PRIM
{
  ag6PRIM_TYPE Type; /* ��� ��������� (AG6_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI,      /* ���������� �������� ��� ������ ��������� */
    MtlNo;       /* ����� ��������� �� ���������� */
} ag6PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagag6UV
{
  FLT U, V; /* ���������� ���������� */
} ag6UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (ag6UV) ����������� ����������.
 */
__inline ag6UV UVSet( FLT U, FLT V )
{
  ag6UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagag6VERTEX
{
  VEC P;    /* ������� ������� */
  ag6UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} ag6VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ag6PRIM *Prim;
 *   - ��� ��������� (AG6_PRIM_***):
 *       ag6PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       ag6VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID AG6_PrimCreate( ag6PRIM *Prim, ag6PRIM_TYPE Type,
                     INT NoofV, INT NoofI, ag6VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ag6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AG6_PrimFree( ag6PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ag6PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AG6_PrimDraw( ag6PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ag6PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AG6_PrimCreatePlane( ag6PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       ag6PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AG6_PrimCreateSphere( ag6PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       ag6PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AG6_PrimLoad( ag6PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagag6GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  ag6PRIM *Prims;      /* ������ ���������� */
} ag6GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ag6GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       ag6PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT AG6_GeomAddPrim( ag6GEOM *G, ag6PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ag6GEOM *G;
 * ������������ ��������: ���.
 */
VOID AG6_GeomFree( ag6GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ag6GEOM *G;
 * ������������ ��������: ���.
 */
VOID AG6_GeomDraw( ag6GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ag6GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL AG6_GeomLoad( ag6GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT AG6_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
