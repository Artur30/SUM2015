/* FILENAME: UBALL.C
 * PROGRAMMER: Gumirov Artur
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Òèï ïðåäñòàâëåíèÿ ìÿ÷à */
typedef struct tagpd6UNIT_BALL
{
  PD6_UNIT_BASE_FIELDS;

  VEC Pos;     /* Ïîçèöèÿ ìÿ÷à */
  DWORD Color; /* Öâåò ìÿ÷à */
  DBL
    Amplitude,  /* Àìïëèòóäà */
    PhaseShift, /* Ñäâèã ïî ôàçå */
    ScaleShift; /* Ìàñøòàá âðåìåíè */
} pd6UNIT_BALL;

/* Ôóíêöèÿ èíèöèàëèçàöèè îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_AnimUnitInit( pd6UNIT_BALL *Uni, pd6ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1920, rand() % 1080, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'PD6_AnimUnitInit' function */

/* Ôóíêöèÿ äåèíèöèàëèçàöèè îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_AnimUnitClose( pd6UNIT_BALL *Uni, pd6ANIM *Ani )
{
} /* End of 'PD6_AnimUnitClose' function */

/* Ôóíêöèÿ îáíîâëåíèÿ ìåæêàäðîâûõ ïàðàìåòðîâ îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_AnimUnitResponse( pd6UNIT_BALL *Uni, pd6ANIM *Ani )
{
  if (Ani->JButs[3])
    //Uni->Amplitude += 30 * Ani->MsWheel;
    Uni->Amplitude += 5;
  if (Ani->JButs[1])
    Uni->Amplitude -= 5;

} /* End of 'PD6_AnimUnitResponse' function */

/* Ôóíêöèÿ ïîñòðîåíèÿ îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       pd6UNIT_BALL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       pd6ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
static VOID PD6_AnimUnitRender( pd6UNIT_BALL *Uni, pd6ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'PD6_AnimUnitRender' function */

/* Ôóíêöèÿ ñîçäàíèÿ îáúåêòà àíèìàöèè "ìÿ÷".
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ:
 *   (pd6UNIT *) óêàçàòåëü íà ñîçäàííûé îáúåêò àíèìàöèè.
 */
pd6UNIT * PD6_UnitBallCreate( VOID )
{
  pd6UNIT_BALL *Uni;

  if ((Uni = (VOID *)PD6_AnimUnitCreate(sizeof(pd6UNIT_BALL))) == NULL)
    return NULL;
  /* çàïîëíÿåì ïîëÿ */
  Uni->Init = (VOID *)PD6_AnimUnitInit;
  Uni->Close = (VOID *)PD6_AnimUnitClose;
  Uni->Response = (VOID *)PD6_AnimUnitResponse;
  Uni->Render = (VOID *)PD6_AnimUnitRender;
  return (pd6UNIT *)Uni;
} /* End of 'PD6_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */