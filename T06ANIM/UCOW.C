/* FILENAME: UBALL.C
 * PROGRAMMER: AG6
 * PURPOSE: Cow object unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"
#include "vec.h"
#include "render.h"

/* Òèï ïðåäñòàâëåíèÿ ìÿ÷à */
typedef struct tagpd6UNIT_COW
{
  PD6_UNIT_BASE_FIELDS;

  pd6GOBJ Model;
} pd6UNIT_ÑOW;

/* Ôóíêöèÿ èíèöèàëèçàöèè îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_COW *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_CowUnitInit( pd6UNIT_ÑOW *Uni, pd6ANIM *Ani )
{
  PD6_RndGObjLoad(&Uni->Model, "cube.object");
} /* End of 'PD6_AnimUnitInit' function */

/* Ôóíêöèÿ äåèíèöèàëèçàöèè îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_CowUnitClose( pd6UNIT_ÑOW *Uni, pd6ANIM *Ani )
{
  PD6_RndGObjFree(&Uni->Model);
} /* End of 'PD6_AnimUnitClose' function */

/* Ôóíêöèÿ îáíîâëåíèÿ ìåæêàäðîâûõ ïàðàìåòðîâ îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_CowUnitResponse( pd6UNIT_ÑOW *Uni, pd6ANIM *Ani )
{
/*  srand(30);
  if (Ani->Keys[VK_UP])
    Uni->Pos.Y -= 10;
  if (Ani->Keys[VK_DOWN])
    Uni->Pos.Y += 10;
  if (Ani->Keys[VK_LEFT])
    Uni->Pos.X -= 10;
  if (Ani->Keys[VK_RIGHT])
    Uni->Pos.X += 10;
  if (Ani->KeysClick['R'])
    Uni->Color = RGB(rand() % 255, rand() % 255, rand() % 255);
  /* Controling by JoyStick */
/*  if (!Ani->IsPause)
  {
    if (Ani->JButs[0])
      Uni->Pos.X -= 10;
    if (Ani->JButs[2])
      Uni->Pos.X += 10;
    if (Ani->JButs[1])
      Uni->Pos.Y += 10;
    if (Ani->JButs[3])
      Uni->Pos.Y -= 10;
  }*/

/*  if (!Ani->IsPause)
  {
    if (Ani->JY >= 0)
      Uni->Pos.Y += 10;
    if (Ani->JY < -0.00783)
      Uni->Pos.Y -= 10;
    if (Ani->JX >= 0)
      Uni->Pos.X += 10;
    if (Ani->JX < -0.00783)
      Uni->Pos.X -= 10;
  }        */

} /* End of 'PD6_AnimUnitResponse' function */

/* Ôóíêöèÿ ïîñòðîåíèÿ îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_CowUnitRender( pd6UNIT_ÑOW *Uni, pd6ANIM *Ani )
{
  PD6_RndMatrWorld = MatrMulMatr(MatrScale(0.4, 0.4, 0.4),
    MatrRotateX(30 * Ani->Time));

  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  PD6_RndGObjDraw(&Uni->Model);
} /* End of 'PD6_AnimUnitRender' function */

/* Ôóíêöèÿ ñîçäàíèÿ îáúåêòà àíèìàöèè "êîðîâà".
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ:
 *   (pd6UNIT *) óêàçàòåëü íà ñîçäàííûé îáúåêò àíèìàöèè.
 */
pd6UNIT * PD6_UnitCowCreate( VOID )
{
  pd6UNIT_ÑOW *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_ÑOW))) == NULL)
    return NULL;
  /* çàïîëíÿåì ïîëÿ */
  Uni->Init = (VOID *)PD6_CowUnitInit;
  Uni->Close = (VOID *)PD6_CowUnitClose;
  Uni->Response = (VOID *)PD6_CowUnitResponse;
  Uni->Render = (VOID *)PD6_CowUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitCowCreate' function */

/* END OF 'UCOW.C' FILE */