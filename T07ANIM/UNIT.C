/* FILENAME: UNIT.C
 * PROGRAMMER: AG6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ag6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ag6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AG6_AnimUnitInit( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ag6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ag6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AG6_AnimUnitClose( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ag6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ag6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AG6_AnimUnitResponse( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ag6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ag6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AG6_AnimUnitRender( ag6UNIT *Uni, ag6ANIM *Ani )
{
} /* End of 'AG6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ag6UNIT *) ��������� �� ��������� ������ ��������.
 */
ag6UNIT * AG6_AnimUnitCreate( INT Size )
{
  ag6UNIT *Uni;

  if (Size < sizeof(ag6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ���� ��-��������� */
  Uni->Size = Size;
  Uni->Init = AG6_AnimUnitInit;
  Uni->Close = AG6_AnimUnitClose;
  Uni->Response = AG6_AnimUnitResponse;
  Uni->Render = AG6_AnimUnitRender;
  return Uni;
} /* End of 'AG6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */