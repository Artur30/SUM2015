/* FILENAME: UBALL.C
 * PROGRAMMER: AG6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

typedef struct tagag6UNIT_BALL
{
  AG6_UNIT_BASE_FIELDS;

  VEC Pos;
  DWORD Color;
  DBL
    Amplitude,
    PhaseShift,
    ScaleShift;
} ag6UNIT_BALL;

static VOID AG6_AnimUnitInit( ag6UNIT_BALL *Uni, ag6ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AG6_AnimUnitInit' function */

static VOID AG6_AnimUnitResponse( ag6UNIT_BALL *Uni, ag6ANIM *Ani )
{
  Uni->Amplitude += 30 * 30 * Ani->MsWheel * Ani->GlobalDeltaTime;
} /* End of 'AG6_AnimUnitResponse' function */

static VOID AG6_AnimUnitRender( ag6UNIT_BALL *Uni, ag6ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time + Uni->PhaseShift));

  /*
  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
  */
} /* End of 'AG6_AnimUnitRender' function */

ag6UNIT * AG6_UnitBallCreate( VOID )
{
  ag6UNIT_BALL *Uni;

  if ((Uni = (VOID *)AG6_AnimUnitCreate(sizeof(ag6UNIT_BALL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)AG6_AnimUnitInit;
  Uni->Response = (VOID *)AG6_AnimUnitResponse;
  Uni->Render = (VOID *)AG6_AnimUnitRender;
  return (ag6UNIT *)Uni;
} /* End of 'AG6_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */