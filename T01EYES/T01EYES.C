/* File Name: T01EYES.C
 * PROGRAMMER: Gumirov Artur
 * DATE: 01.06.2015
 * PURPOSE: WinAPI windowed applicatoin sample
 */

#include <windows.h>
#include <math.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 *   0 - при успехе.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* Регистрация класса окна */
  wc.style = CS_VREDRAW | CS_HREDRAW;                   /* Стиль окна: полностью перерисовывать
                                                             * при изменении вертикального или
                                                             * горизонтального размеров
                                                             * еще можно CS_DBLCLKS для добавления
                                                             * отработки двойного нажатия */
  wc.cbClsExtra = 0;                                    /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0;                                    /* Дополнительное количество байт для окна */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);             /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);              /* Загрузка пиктограммы (системной) */
  wc.hInstance = hInstance;                             /* Дескриптор приложения, регистрирующего класс */
  wc.lpszMenuName = NULL;                               /* Имя ресурса меню */
  wc.lpfnWndProc = MyWindowFunc;                        /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* Имя класса окна */
      "EYES",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
      NULL,                         /* Дескриптор родительского окна */
      NULL,                         /* Дескриптор загруженного меню */
      hInstance,                    /* Дескриптор приложения */
      NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* Запуск цикла сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  return 30;
} /* End of 'WinMain' function */


/* Функция рисования глаза.
 * АГРУМЕНТЫ:
 *   - сонтекст окна:
 *       HDC hDC;
 *   - абсцисса левого верхнего края прямоугольника, ограничивающего глаз:
 *       int left;
 *   - ордината левого верхнего края прямоугольника, ограничивающего глаз:
 *       int top;
 *   - абсцисса правого нижнего края прямоугольника, ограничивающего глаз:
 *       int right;
 *   - ордината правого нижнего края прямоугольника, ограничивающего глаз:
 *       int bottom;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (VOID) ничего
 */
void DrawEye( HDC hDC, int left, int top, int right, int bottom )
{
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, left, top, right, bottom);
} /* End of 'DrawEye' function */


/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения (см. WM_***):
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;         /* Display context */
  static INT w, h, x, y;
  POINT pt;        /* The point of cursor */
  PAINTSTRUCT ps;
  INT CEyeF = 240, CEyeFy = 240, CEyeS = 560, CEyeSy = 240, R = 65;
  INT x1, y1, x2, y2; /* The coordinats of center */

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 111, 50, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 0));
    Rectangle(hDC, 0, 0, w, h);
    SetDCBrushColor(hDC, RGB(255, 255, 255));

    x = pt.x;
    y = pt.y;

    Ellipse(hDC, 160, 160, 321, 321);
    Ellipse(hDC, 480, 160, 641, 321);

    x1 = R * abs(CEyeF - x) / (INT)sqrt((CEyeF - x) * (CEyeF - x) + (CEyeFy - y) * (CEyeFy - y));
    y1 = R * abs(CEyeFy - y) / (INT)sqrt((CEyeF - x) * (CEyeF - x) + (CEyeFy - y) * (CEyeFy - y));
    x2 = R * abs(CEyeS - x) / (INT)sqrt((CEyeS - x) * (CEyeS - x) + (CEyeSy - y) * (CEyeSy - y));
    y2 = R * abs(CEyeSy - y) / (INT)sqrt((CEyeS - x) * (CEyeS - x) + (CEyeSy - y) * (CEyeSy - y));

    if (x > CEyeF && x < CEyeS)
      x2 = -x2;
    if (x < CEyeF)
      x1 = -x1, x2 = -x2;
    if (y < CEyeFy)
      y1 = -y1, y2 = -y2;

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 0, 0));
    SetDCPenColor(hDC, RGB(0, 0, 0));
    DrawEye(hDC, CEyeF + x1 - 16, CEyeFy + y1 - 16, CEyeF + x1 + 16, CEyeFy + y1 + 16);
    DrawEye(hDC, CEyeS + x2 - 16, CEyeSy + y2 - 16, CEyeS + x2 + 16, CEyeSy + y2 + 16);
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(NULL_BRUSH));

    EndPaint(hWnd, &ps);
    return 0;

  case WM_KEYDOWN:
    if (wParam == 27)
      DestroyWindow(hWnd);
    return 0;

  case WM_MOUSEMOVE:
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    hDC = GetDC(hWnd);

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 255));
    SetDCPenColor(hDC, RGB(0, 0, 0));

    if (wParam & MK_LBUTTON)
      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);

    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to exit this program?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_DESTROY:
    KillTimer(hWnd, 111);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T01FWIN.C' FILE */