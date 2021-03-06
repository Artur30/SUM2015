/* FILENAME: UNITS.H
 * PROGRAMMER: AG6
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (ag6UNIT *) ��������� �� ��������� ������ ��������.
 */
ag6UNIT * AG6_UnitBallCreate( VOID );

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (ag6UNIT *) ��������� �� ��������� ������ ��������.
 */
ag6UNIT * AG6_UnitControlCreate( VOID );

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (ag6UNIT *) ��������� �� ��������� ������ ��������.
 */
ag6UNIT * AG6_UnitModelCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */