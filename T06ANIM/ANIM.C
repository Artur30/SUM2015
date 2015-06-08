/* FILENAME: ANIM.C
 * PROGRAMMER: Gumirov Artur
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"

/* Системный контекст анимации */
static vg4ANIM VG4_Anim;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  VG4_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  VG4_Anim.hDC = CreateCompatibleDC(hDC);
  VG4_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(VG4_Anim.hDC, VG4_Anim.hBmFrame);
  VG4_Anim.W = 30;
  VG4_Anim.H = 30;
  VG4_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  QueryPerformanceCounter(&li);
} /* End of 'VG4_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
    free(VG4_Anim.Units[i]);
    VG4_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(VG4_Anim.hBmFrame);
  DeleteDC(VG4_Anim.hDC);
} /* End of 'VG4_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(VG4_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(VG4_Anim.hBmFrame);
  VG4_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(VG4_Anim.hDC, VG4_Anim.hBmFrame);

  /* Сохранение размера */
  VG4_Anim.W = W;
  VG4_Anim.H = H;

  ReleaseDC(VG4_Anim.hWnd, hDC);
} /* End of 'VG4_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimRender( VOID )
{
  INT i;

  /* опрос на изменение состояний объектов */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  /* очистка фона */
  SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(VG4_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(VG4_Anim.hDC, RGB(0, 0, 0));
  Rectangle(VG4_Anim.hDC, 0, 0, VG4_Anim.W + 1, VG4_Anim.H + 1);

  /* рисование объектов */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(VG4_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(VG4_Anim.hDC, RGB(0, 0, 0));

    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
} /* End of 'VG4_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(VG4_Anim.hWnd);
  BitBlt(hDC, 0, 0, VG4_Anim.W, VG4_Anim.H, VG4_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(VG4_Anim.hWnd, hDC);
} /* End of 'VG4_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       vg4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Unit )
{
  if (VG4_Anim.NumOfUnits < VG4_MAX_UNITS)
  {
    VG4_Anim.Units[VG4_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &VG4_Anim);
  }
} /* End of 'VG4_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(VG4_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(VG4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(VG4_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(VG4_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(VG4_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'VG4_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimDoExit( VOID )
{
  PostMessage(VG4_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'VG4_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
