/* FILENAME: ANIM.H
 * PROGRAMMER: Gumirov Artur
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* Максимальное количество элементов анимации */
#define VG4_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagvg4UNIT vg4UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  vg4UNIT *Units[VG4_MAX_UNITS]; 
  INT NumOfUnits;

} vg4ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( vg4UNIT *Unit, vg4ANIM *Ani );
 */
#define VG4_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( vg4UNIT *Unit, vg4ANIM *Ani );       \
  VOID (*Close)( vg4UNIT *Unit, vg4ANIM *Ani );      \
  VOID (*Response)( vg4UNIT *Unit, vg4ANIM *Ani );   \
  VOID (*Render)( vg4UNIT *Unit, vg4ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagvg4UNIT
{
  VG4_UNIT_BASE_FIELDS;
};

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimInit( HWND hWnd );

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       vg4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект анимации.
 */
vg4UNIT * VG4_AnimUnitCreate( INT Size );

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimDoExit( VOID );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
