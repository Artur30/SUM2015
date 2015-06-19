/* FILENAME: RENDER.H
 * PROGRAMMER: AG6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Матрицы */
extern MATR
  AG6_RndMatrWorld,
  AG6_RndMatrView,
  AG6_RndMatrProj,
  AG6_RndMatrWorldViewProj;

/* Матрица изменения примитива при создании */
extern MATR AG6_RndPrimMatrConvert;

/* Параметры проецирования */
extern DBL
  AG6_RndWp, AG6_RndHp,    /* размеры обрасти проецирования */
  AG6_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT AG6_RndProg;

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT AG6_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AG6_ShaderFree( UINT PrgId );

/***
 * Работа с материалами
 ***/

/* Тип задания материала поверхности */
typedef struct tagag6MATERIAL
{
  /* Имя материала */
  CHAR Name[300];

  /* Коэффициенты отражения */
  VEC Ka, Kd, Ks;
  FLT Kp; /* Степень "бликовости" */

  /* Коэффициент прозрачности */
  FLT Kt;

  /* Текстура поверхности */
  INT TexId;
} ag6MATERIAL;

/* Глобальная таблица материалов */
#define AG6_MATERIAL_MAX 300
extern INT AG6_MtlLibSize;
extern ag6MATERIAL AG6_MtlLib[AG6_MATERIAL_MAX];

/* Функция добавления материала в библиотеку.
 * АРГУМЕНТЫ:
 *   - структура с памаметрами материала:
 *       ag6MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер добавленного материала (0 при ошибке).
 */
INT AG6_MtlAdd( ag6MATERIAL *Mtl );

/* Функция поиска материала по имени.
 * АРГУМЕНТЫ:
 *   - имя материала:
 *       CHAR *Name;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер найденного материала (0 при ошибке).
 */
INT AG6_MtlFind( CHAR *Name );

/* Функция загрузки материала из файла (*.MTL).
 * АРГУМЕНТЫ:
 *   - имя файла материала:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество загруженных материалов.
 */
INT AG6_MtlLoad( CHAR *FileName );

/***
 * Работа с примитивами
 ***/

/* Виды примитивов */
typedef enum tagag6PRIM_TYPE
{
  AG6_PRIM_TRIMESH,  /* примитив - набор треугольников */
  AG6_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} ag6PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagag6PRIM
{
  ag6PRIM_TYPE Type; /* Тип примитива (AG6_PRIM_***) */
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfI,      /* количество индексов для вывода примитива */
    MtlNo;       /* номер материала из библиотеки */
} ag6PRIM;

/* Тип хранения текстурных координат */
typedef struct tagag6UV
{
  FLT U, V; /* текстурные координаты */
} ag6UV;

/* Функция задания текстурных координат.
 * АРГУМЕНТЫ:
 *   - новые значени координат:
 *       FLT U, V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ag6UV) построенные координаты.
 */
__inline ag6UV UVSet( FLT U, FLT V )
{
  ag6UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagag6VERTEX
{
  VEC P;    /* позиция вершины */
  ag6UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} ag6VERTEX;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ag6PRIM *Prim;
 *   - тип примитива (AG6_PRIM_***):
 *       ag6PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       ag6VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AG6_PrimCreate( ag6PRIM *Prim, ag6PRIM_TYPE Type,
                     INT NoofV, INT NoofI, ag6VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ag6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AG6_PrimFree( ag6PRIM *Prim );

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ag6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AG6_PrimDraw( ag6PRIM *Prim );

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ag6PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AG6_PrimCreatePlane( ag6PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ag6PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AG6_PrimCreateSphere( ag6PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ag6PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AG6_PrimLoad( ag6PRIM *GObj, CHAR *FileName );

/***
 * Работа с геометрическими объектами
 ***/

/* Геометрический объект - набор примитивов */
typedef struct tagag6GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  ag6PRIM *Prims;      /* массив примитивов */
} ag6GEOM;

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ag6GEOM *G;
 *   - указатель на добавляемый примитив:
 *       ag6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT AG6_GeomAddPrim( ag6GEOM *G, ag6PRIM *Prim );

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ag6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AG6_GeomFree( ag6GEOM *G );

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ag6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AG6_GeomDraw( ag6GEOM *G );

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ag6GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL AG6_GeomLoad( ag6GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT AG6_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
