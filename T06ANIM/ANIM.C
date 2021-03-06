/* FILENAME: ANIM.C
 * PROGRAMMER: Gumirov Artur
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"
#include "render.h"
#include <stdio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Ïîëó÷åíèå çíà÷åíèÿ îñè äæîéñòèêà */
#define PD6_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Äàííûå äëÿ ñèíõðîíèçàöèè ïî âðåìåíè */
static INT64
  TimeFreq,  /* åäèíèö èçìåðåíèÿ â ñåêóíäó */
  TimeStart, /* âðåìÿ íà÷àëà àíèìàöèè */
  TimeOld,   /* âðåìÿ ïðîøëîãî êàäðà */
  TimePause, /* âðåìÿ ïðîñòîÿ â ïàóçå */
  TimeFPS;   /* âðåìÿ äëÿ çàìåðà FPS */
static INT
  FrameCounter; /* ñ÷åò÷èê êàäðîâ */

/* Ñîõðàíåííûå ìûøèíûå êîîðäèíàòû */
static INT
  PD6_MouseOldX, PD6_MouseOldY;

/* Ñèñòåìíûé êîíòåêñò àíèìàöèè */
pd6ANIM PD6_Anim;

/* Ôóíêöèÿ èíèöèàëèçàöèè àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - äåñêðèïòîð îêíà:
 *       HWND hWnd;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  POINT pt;

  memset(&PD6_Anim, 0, sizeof(pd6ANIM));
  PD6_Anim.hWnd = hWnd;
  /* Èíèöèàëèçèðóåì áóôåð êàäðà */
  PD6_Anim.hDC = CreateCompatibleDC(hDC);
  PD6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(PD6_Anim.hDC, PD6_Anim.hBmFrame);
  PD6_Anim.W = 30;
  PD6_Anim.H = 30;
  PD6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Èíèöèàëèçàöèÿ òàéìåðà */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  PD6_Anim.IsPause = FALSE;
  FrameCounter = 0;

  /* Èíèöèàëèçàöèÿ ââîäà */
  GetCursorPos(&pt);
  ScreenToClient(PD6_Anim.hWnd, &pt);
  PD6_MouseOldX = pt.x;
  PD6_MouseOldY = pt.y;
  GetKeyboardState(PD6_Anim.KeysOld);
} /* End of 'PD6_AnimInit' function */

/* Ôóíêöèÿ äåèíèöèàëèçàöèè àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimClose( VOID )
{
  INT i;

  /* Îñâîáîæäåíèå ïàìÿòè èç-ïîä îáúåêòîâ àíèìàöèè */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
  {
    PD6_Anim.Units[i]->Close(PD6_Anim.Units[i], &PD6_Anim);
    free(PD6_Anim.Units[i]);
    PD6_Anim.Units[i] = NULL;
  }

  /* Óäàëÿåì îáúåêòû GDI */
  DeleteObject(PD6_Anim.hBmFrame);
  DeleteDC(PD6_Anim.hDC);
} /* End of 'PD6_AnimClose' function */

/* Ôóíêöèÿ îáðàáîòêè èçìåíåíèÿ ðàçìåðà îáëàñòè âûâîäà.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - íîâûé ðàçìåð îáëàñòè âûâîäà:
 *       INT W, H;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(PD6_Anim.hWnd);

  /* Ïåðåâûäåëåíèå ïàìÿòè ó áóôåðà êàäðà */
  DeleteObject(PD6_Anim.hBmFrame);
  PD6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(PD6_Anim.hDC, PD6_Anim.hBmFrame);

  /* Ñîõðàíåíèå ðàçìåðà */
  PD6_Anim.W = W;
  PD6_Anim.H = H;

  ReleaseDC(PD6_Anim.hWnd, hDC);

  if (W > H)
    PD6_RndWp = (DBL)W / H * 3, PD6_RndHp = 3;
  else
    PD6_RndHp = (DBL)H / W * 3, PD6_RndWp = 3;

  PD6_RndMatrProj = MatrFrustum(-PD6_RndWp / 2, PD6_RndWp / 2, -PD6_RndHp / 2, PD6_RndHp / 2, PD6_RndProjDist, 800);
} /* End of 'PD6_AnimResize' function */

/* Ôóíêöèÿ ïîñòðîåíèÿ êàäðà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  static CHAR Buf[100];
  HFONT hFnt, hOldFnt;
  POINT pt;

  /* î÷èñòêà ôîíà */
  SelectObject(PD6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(PD6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(PD6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(PD6_Anim.hDC, 0, 0, PD6_Anim.W + 1, PD6_Anim.H + 1);

  /*** Îáíîâëåíèå òàéìåðà ***/
  QueryPerformanceCounter(&li);
  PD6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  PD6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!PD6_Anim.IsPause)
    PD6_Anim.DeltaTime = PD6_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    PD6_Anim.DeltaTime = 0;
  }

  PD6_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* âû÷èñëÿåì FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    PD6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* âðåìÿ "ïðîøëîãî" êàäðà */
  TimeOld = li.QuadPart;


  /*** Îáíîâëåíèå ââîäà ***/

  /* Êëàâèàòóðà */
  GetKeyboardState(PD6_Anim.Keys);
  for (i = 0; i < 256; i++)
    PD6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    PD6_Anim.KeysClick[i] = PD6_Anim.Keys[i] && !PD6_Anim.KeysOld[i];
  memcpy(PD6_Anim.KeysOld, PD6_Anim.Keys, sizeof(PD6_Anim.KeysOld));

  /* Ìûøü */
  /* êîëåñî */
  PD6_Anim.MsWheel = PD6_MouseWheel;
  PD6_MouseWheel = 0;
  /* àáñîëþòíàÿ ïîçèöèÿ */
  GetCursorPos(&pt);
  ScreenToClient(PD6_Anim.hWnd, &pt);
  PD6_Anim.MsX = pt.x;
  PD6_Anim.MsY = pt.y;
  /* îòíîñèòåëüíîå ïåðåìåùåíèå */
  PD6_Anim.MsDeltaX = pt.x - PD6_MouseOldX;
  PD6_Anim.MsDeltaY = pt.y - PD6_MouseOldY;
  PD6_MouseOldX = pt.x;
  PD6_MouseOldY = pt.y;

  /* Äæîéñòèê */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ïîëó÷åíèå îáùåé èíôîðìàöèè î äæîñòèêå */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ïîëó÷åíèå òåêóùåãî ñîñòîÿíèÿ */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Êíîïêè */
        memcpy(PD6_Anim.JButsOld, PD6_Anim.JButs, sizeof(PD6_Anim.JButs));
        for (i = 0; i < 32; i++)
          PD6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          PD6_Anim.JButsClick[i] = PD6_Anim.JButs[i] && !PD6_Anim.JButsOld[i];

        /* Îñè */
        PD6_Anim.JX = PD6_GET_AXIS_VALUE(X);
        PD6_Anim.JY = PD6_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          PD6_Anim.JZ = PD6_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          PD6_Anim.JU = PD6_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          PD6_Anim.JV = PD6_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          PD6_Anim.JR = PD6_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            PD6_Anim.JPOV = 0;
          else
            PD6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* îïðîñ íà èçìåíåíèå ñîñòîÿíèé îáúåêòîâ */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
    PD6_Anim.Units[i]->Response(PD6_Anim.Units[i], &PD6_Anim);

  /* ðèñîâàíèå îáúåêòîâ */
  for (i = 0; i < PD6_Anim.NumOfUnits; i++)
  {
    SelectObject(PD6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(PD6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(PD6_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(PD6_Anim.hDC, RGB(255, 255, 255));

    PD6_Anim.Units[i]->Render(PD6_Anim.Units[i], &PD6_Anim);
  }

  /* Writing the time and the date at the moment */
  hFnt = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH | FF_ROMAN, "");
  hOldFnt = SelectObject(PD6_Anim.hDC, hFnt);

  SetTextColor(PD6_Anim.hDC, RGB(59, 255, 230));
  SetBkColor(PD6_Anim.hDC, RGB(255, 255, 0));
  SetBkMode(PD6_Anim.hDC, TRANSPARENT);

  TextOut(PD6_Anim.hDC, PD6_Anim.W - 200, PD6_Anim.H / 30 - 15, Buf,
      sprintf(Buf, "FPS: %.5f", PD6_Anim.FPS));

  SetTextColor(PD6_Anim.hDC, RGB(255, 55, 30));
  SetBkColor(PD6_Anim.hDC, RGB(255, 255, 0));
  SetBkMode(PD6_Anim.hDC, TRANSPARENT);

  TextOut(PD6_Anim.hDC, PD6_Anim.W - 200, PD6_Anim.H / 30 + 15, Buf,
      sprintf(Buf, "The time: %.2f", PD6_Anim.Time));

  DeleteObject(hFnt);

  SelectObject(PD6_Anim.hDC, hOldFnt);
  DeleteObject(hFnt);

  FrameCounter++;
} /* End of 'PD6_AnimRender' function */

/* Ôóíêöèÿ âûâîäà êàäðà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* âûâîä êàäðà */
  hDC = GetDC(PD6_Anim.hWnd);
  BitBlt(hDC, 0, 0, PD6_Anim.W, PD6_Anim.H, PD6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(PD6_Anim.hWnd, hDC);
} /* End of 'PD6_AnimCopyFrame' function */

/* Ôóíêöèÿ äîáàâëåíèÿ â ñèñòåìó îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - äîáàâëÿåìûé îáúåêò àíèìàöèè:
 *       pd6UNIT *Unit;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimAddUnit( pd6UNIT *Unit )
{
  if (PD6_Anim.NumOfUnits < PD6_MAX_UNITS)
  {
    PD6_Anim.Units[PD6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &PD6_Anim);
  }
} /* End of 'PD6_AnimAddUnit' function */


/* Ôóíêöèÿ ïåðåêëþ÷åíèÿ â/èç ïîëíîýêðàííîãî ðåæèìà
 * ñ ó÷åòîì íåñêîëüêèõ ìîíèòîðîâ.
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* òåêóùèé ðåæèì */
  static RECT SaveRC;               /* ñîõðàíåííûé ðàçìåð */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ñîõðàíÿåì ñòàðûé ðàçìåð îêíà */
    GetWindowRect(PD6_Anim.hWnd, &SaveRC);

    /* îïðåäåëÿåì â êàêîì ìîíèòîðå íàõîäèòñÿ îêíî */
    hmon = MonitorFromWindow(PD6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* ïîëó÷àåì èíôîðìàöèþ äëÿ ìîíèòîðà */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ïåðåõîäèì â ïîëíûé ýêðàí */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(PD6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(PD6_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* âîññòàíàâëèâàåì ðàçìåð îêíà */
    SetWindowPos(PD6_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'PD6_AnimFlipFullScreen' function */

/* Ôóíêöèÿ âûõîäà èç àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ: Íåò.
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimDoExit( VOID )
{
  PostMessage(PD6_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'PD6_AnimDoExit' function */

/* Ôóíêöèÿ óñòàíîâêè ïàóçû àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - ôëàã ïàóçû:
 *       BOOL NewPauseFlag;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */
VOID PD6_AnimSetPause( BOOL NewPauseFlag )
{
  PD6_Anim.IsPause = NewPauseFlag;
} /* End of 'PD6_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */