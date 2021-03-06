/* FILENAME: UMODEL.C
 * PROGRAMMER: AG6
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"
#include "render.h"

typedef struct tagag6UNIT_MODEL
{
  AG6_UNIT_BASE_FIELDS;

  ag6GEOM Model;
  ag6GEOM Geom;
  ag6GEOM GeomHouse;
  ag6PRIM MAP;
  ag6PRIM SKY;
  INT TextId;
} ag6UNIT_MODEL;

static VOID AG6_AnimUnitInit( ag6UNIT_MODEL *Uni, ag6ANIM *Ani )
{
  ag6MATERIAL Mtl = AG6_MtlLib[0];



  AG6_PrimCreateHeightField(&Uni->MAP, VecSet(500, 0, 0), VecSet(-500, 0, 0), VecSet(0, 0, 500), 100, "MAP3.BMP");
  Mtl.TexId = AG6_TextureLoad("texture.BMP");
  Uni->MAP.MtlNo = Uni->TextId = AG6_MtlAdd(&Mtl);
} /* END 'AG6_AnimUnitInit' FUNCTION */



static VOID AG6_AnimUnitClose( ag6UNIT_MODEL *Uni, ag6ANIM *Ani )
{ 
  AG6_PrimFree(&Uni->MAP);
  AG6_GeomFree(&Uni->Model);
  AG6_PrimFree(&Uni->SKY);
} /* End of 'AG6_AnimUnitClose' function */

static VOID AG6_AnimUnitRender( ag6UNIT_MODEL *Uni, ag6ANIM *Ani )
{ 
  static DBL MovX = 0, MovY = 0;
  INT i, j;

  AG6_RndMatrView = MatrView(Ani->RndCamera.Loc,
                             Ani->RndCamera.At,
                             Ani->RndCamera.Up);

  MovX += Ani->Keys['J'] * 0.2;
  MovX -= Ani->Keys['H'] * 0.2;
  AG6_RndMatrView = MatrMulMatr(MatrMulMatr(
                    AG6_RndMatrView, MatrRotateY(MovY)), MatrRotateX(MovX));


  if (Ani->KeysClick['X'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['C'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      AG6_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.1, 0.1, 0.1)),
          MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
          MatrTranslate(j * 1.30, 0, i * 1.30 + 100 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
    }

  AG6_RndMatrWorld = MatrIdentity();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AG6_PrimDraw(&Uni->MAP);
  AG6_PrimDraw(&Uni->SKY);
} /* End of 'AG6_AnimUnitRender' function */

ag6UNIT * AG6_UnitModelCreate( VOID )
{
  ag6UNIT_MODEL *Uni;

  if ((Uni = (VOID *)AG6_AnimUnitCreate(sizeof(ag6UNIT_MODEL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)AG6_AnimUnitInit;
  Uni->Close = (VOID *)AG6_AnimUnitClose;
  Uni->Render = (VOID *)AG6_AnimUnitRender;
  return (ag6UNIT *)Uni;
} /* End of 'AG6_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */