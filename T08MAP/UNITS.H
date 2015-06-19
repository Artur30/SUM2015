/* FILENAME: UNITS.H
 * PROGRAMMER: AG6
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ag6UNIT *) указатель на созданный объект анимации.
 */
ag6UNIT * AG6_UnitBallCreate( VOID );

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ag6UNIT *) указатель на созданный объект анимации.
 */
ag6UNIT * AG6_UnitControlCreate( VOID );

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ag6UNIT *) указатель на созданный объект анимации.
 */
ag6UNIT * AG6_UnitModelCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */