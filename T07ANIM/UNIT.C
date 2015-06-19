/* FILENAME: UNIT.C
 * PROGRAMMER: AG6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ag6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ag6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AG6_AnimUnitInit( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ag6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ag6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AG6_AnimUnitClose( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ag6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ag6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AG6_AnimUnitResponse( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ag6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ag6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AG6_AnimUnitRender( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ag6UNIT *) указатель на созданный объект анимации.
 */
ag6UNIT * AG6_AnimUnitCreate( INT Size )
{
  ag6UNIT *Uni;

  if (Size < sizeof(ag6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* заполняем поля по-умолчанию */
  Uni->Size = Size;
  Uni->Init = AG6_AnimUnitInit;
  Uni->Close = AG6_AnimUnitClose;
  Uni->Response = AG6_AnimUnitResponse;
  Uni->Render = AG6_AnimUnitRender;
  return Uni;
} /* End of 'AG6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */