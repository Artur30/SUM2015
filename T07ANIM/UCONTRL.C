/* FILENAME: UCONTRL.C
 * PROGRAMMER: AG6
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>

#include "anim.h"

typedef struct tagag6UNIT_CTRL
{
  AG6_UNIT_BASE_FIELDS;
  HFONT hFnt;
} ag6UNIT_CTRL;

static VOID AG6_AnimUnitInit( ag6UNIT_CTRL *Uni, ag6ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'AG6_AnimUnitInit' function */

static VOID AG6_AnimUnitClose( ag6UNIT_CTRL *Uni, ag6ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'AG6_AnimUnitClose' function */

static VOID AG6_AnimUnitResponse( ag6UNIT_CTRL *Uni, ag6ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AG6_AnimDoExit();
  if (Ani->KeysClick['F'])
    AG6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AG6_AnimSetPause(!Ani->IsPause);
} /* End of 'AG6_AnimUnitResponse' function */

static VOID AG6_AnimUnitRender( ag6UNIT_CTRL *Uni, ag6ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }


  /*
  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);
  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);
  SelectObject(Ani->hDC, hFntOld);
  */
} /* End of 'AG6_AnimUnitRender' function */

ag6UNIT * AG6_UnitControlCreate( VOID )
{
  ag6UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AG6_AnimUnitCreate(sizeof(ag6UNIT_CTRL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)AG6_AnimUnitInit;
  Uni->Close = (VOID *)AG6_AnimUnitClose;
  Uni->Response = (VOID *)AG6_AnimUnitResponse;
  Uni->Render = (VOID *)AG6_AnimUnitRender;
  return (ag6UNIT *)Uni;
} /* End of 'AG6_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */