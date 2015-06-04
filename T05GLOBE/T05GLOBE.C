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
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    EndPaint(hWnd, &ps);
    return 0;

  case WM_KEYDOWN:
    return 0;

  case WM_MOUSEMOVE:

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