/* FILE NAME: MAIN.C
 * PROGRAMMER: Gumirov Artur
 * DATE: 06.06.2015
 * PURPOSE: WinAPI windowed application sample
 */

#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <windows.h>

#include "anim.h"
#include "units.h"

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"

/* ���������� ���������� - ������� ��������� ������ ���� */
INT PD6_MouseWheel;

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �� �����.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  INT i;
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW;                   /* ����� ����: ��������� ��������������
                                                             * ��� ��������� ������������� ���
                                                             * ��������������� ��������
                                                             * ��� ����� CS_DBLCLKS ��� ����������
                                                             * ��������� �������� ������� */
  wc.cbClsExtra = 0;                                    /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0;                                    /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);             /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);              /* �������� ����������� (���������) */
  wc.hInstance = hInstance;                             /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL;                               /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc;                        /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Object",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* Adding the units */

/*  for (i = 0; i < 59; i++)
    PD6_AnimAddUnit(PD6_UnitRectCreate());*/
  /* Add the balls */
  for (i = 0; i < 300; i++)
    PD6_AnimAddUnit(PD6_UnitBallCreate());
  /* Add the cow */
  PD6_AnimAddUnit(PD6_UnitCowCreate());
  /* Add the Control Unit */
  PD6_AnimAddUnit(PD6_UnitControlCreate());

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: 
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* Save old size of window */
    GetWindowRect(hWnd, &SaveRC);

    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    /* ��� ������ ��������:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CXSCREEN);
    */

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOPMOST, SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'FlipFullScreen' function */


/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    PD6_AnimInit(hWnd);
    return 0;

  case WM_SIZE:
    PD6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    PD6_AnimRender();
    return 0;

  case WM_TIMER:
    PD6_AnimRender();
    PD6_AnimCopyFrame();
    return 0;

  case WM_MOUSEWHEEL:
    PD6_MouseWheel += (SHORT)HIWORD(wParam) / WHEEL_DELTA;
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    PD6_AnimCopyFrame();
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_DESTROY:
    PD6_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MainWindowFunc' function */

/* END OF 'MAIN.C' FILE */