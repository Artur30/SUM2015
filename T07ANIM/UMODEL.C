/* FILENAME: UMODEL.C
 * PROGRAMMER: AG6
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

typedef struct tagag6UNIT_MODEL
{
  AG6_UNIT_BASE_FIELDS;

  ag6GEOM Model;
  ag6GEOM Geom;
  ag6PRIM Pr;
  INT TextId;
} ag6UNIT_MODEL;

static VOID AG6_AnimUnitInit( ag6UNIT_MODEL *Uni, ag6ANIM *Ani )
{
  ag6VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  Uni->TextId = AG6_TextureLoad("M.BMP");

  AG6_PrimCreate(&Uni->Pr, AG6_PRIM_TRIMESH, 4, 6, V, I);

  AG6_RndPrimMatrConvert = MatrMulMatr(MatrScale(0.5, 0.5, 0.5), MatrRotateX(-90));
  //AG6_GeomLoad(&Uni->Model, "NISPF.g3d");

  AG6_RndPrimMatrConvert = MatrMulMatr(MatrScale(0.5, 0.5, 0.5), MatrRotateX(-90));
  AG6_GeomLoad(&Uni->Geom, "Nissan_Pathfinder//NISPF.g3d");
} /* End of 'AG6_AnimUnitInit' function */

static VOID AG6_AnimUnitClose( ag6UNIT_MODEL *Uni, ag6ANIM *Ani )
{
  AG6_GeomFree(&Uni->Model);
  AG6_GeomFree(&Uni->Geom);
  AG6_PrimFree(&Uni->Pr);
} /* End of 'AG6_AnimUnitClose' function */

static VOID AG6_AnimUnitRender( ag6UNIT_MODEL *Uni, ag6ANIM *Ani )
{
  INT i, j;

  AG6_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
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
      AG6_GeomDraw(&Uni->Model);
    }
  AG6_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  AG6_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AG6_PrimDraw(&Uni->Pr);
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