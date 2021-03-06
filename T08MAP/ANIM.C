/* FILENAME: ANIM.C
 * PROGRAMMER: AG6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

#define AG6_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

ag6ANIM AG6_Anim;

static INT64
  TimeFreq,
  TimeStart,
  TimeOld,
  TimePause,
  TimeFPS;
static INT
  FrameCounter;

static INT
  AG6_MouseOldX, AG6_MouseOldY;

BOOL AG6_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&AG6_Anim, 0, sizeof(ag6ANIM));
  AG6_Anim.hWnd = hWnd;
  AG6_Anim.hDC = GetDC(hWnd);

  AG6_Anim.W = 30;
  AG6_Anim.H = 30;
  AG6_Anim.NumOfUnits = 0;


  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AG6_Anim.hDC, &pfd);
  DescribePixelFormat(AG6_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(AG6_Anim.hDC, i, &pfd);


  AG6_Anim.hGLRC = wglCreateContext(AG6_Anim.hDC);

  wglMakeCurrent(AG6_Anim.hDC, AG6_Anim.hGLRC);

  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AG6_Anim.hGLRC);
    ReleaseDC(AG6_Anim.hWnd, AG6_Anim.hDC);
    memset(&AG6_Anim, 0, sizeof(ag6ANIM));
    return FALSE;
  }

  AG6_RndProg = AG6_ShaderLoad("TEST");

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AG6_Anim.IsPause = FALSE;
  FrameCounter = 0;

  GetCursorPos(&pt);
  ScreenToClient(AG6_Anim.hWnd, &pt);
  AG6_MouseOldX = pt.x;
  AG6_MouseOldY = pt.y;
  GetKeyboardState(AG6_Anim.KeysOld);

  AG6_RndCameraSet(&AG6_Anim.RndCamera, VecSet(100, 100, 100),
                   VecSet(0, 0, 0),
                   VecSet(0, 1, 0));

  return TRUE;
} /* End of 'AG6_AnimInit' function */

VOID AG6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < AG6_Anim.NumOfUnits; i++)
  {
    AG6_Anim.Units[i]->Close(AG6_Anim.Units[i], &AG6_Anim);
    free(AG6_Anim.Units[i]);
    AG6_Anim.Units[i] = NULL;
  }

  AG6_ShaderFree(AG6_RndProg);
  AG6_RndProg = 0;
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AG6_Anim.hGLRC);
  ReleaseDC(AG6_Anim.hWnd, AG6_Anim.hDC);
} /* End of 'AG6_AnimClose' function */

VOID AG6_AnimResize( INT W, INT H )
{
  AG6_Anim.W = W;
  AG6_Anim.H = H;

  glViewport(0, 0, W, H);

  if (W > H)
    AG6_RndWp = (DBL)W / H * 3, AG6_RndHp = 3;
  else
    AG6_RndHp = (DBL)H / W * 3, AG6_RndWp = 3;

  AG6_RndMatrProj = MatrFrustum(-AG6_RndWp / 2, AG6_RndWp / 2,
                                -AG6_RndHp / 2, AG6_RndHp / 2,
                                AG6_RndProjDist, 5000000);

} /* End of 'AG6_AnimResize' function */

VOID AG6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  QueryPerformanceCounter(&li);
  AG6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AG6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!AG6_Anim.IsPause)
    AG6_Anim.DeltaTime = AG6_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    AG6_Anim.DeltaTime = 0;
  }

  AG6_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AG6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  TimeOld = li.QuadPart;

  GetKeyboardState(AG6_Anim.Keys);
  for (i = 0; i < 256; i++)
    AG6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AG6_Anim.KeysClick[i] = AG6_Anim.Keys[i] && !AG6_Anim.KeysOld[i];
  memcpy(AG6_Anim.KeysOld, AG6_Anim.Keys, sizeof(AG6_Anim.KeysOld));

  AG6_Anim.MsWheel = AG6_MouseWheel;
  AG6_MouseWheel = 0;

  GetCursorPos(&pt);
  ScreenToClient(AG6_Anim.hWnd, &pt);
  AG6_Anim.MsX = pt.x;
  AG6_Anim.MsY = pt.y;

  AG6_Anim.MsDeltaX = pt.x - AG6_MouseOldX;
  AG6_Anim.MsDeltaY = pt.y - AG6_MouseOldY;
  AG6_MouseOldX = pt.x;
  AG6_MouseOldY = pt.y;


  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;


    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        memcpy(AG6_Anim.JButsOld, AG6_Anim.JButs, sizeof(AG6_Anim.JButs));
        for (i = 0; i < 32; i++)
          AG6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AG6_Anim.JButsClick[i] = AG6_Anim.JButs[i] && !AG6_Anim.JButsOld[i];

        AG6_Anim.JX = AG6_GET_AXIS_VALUE(X);
        AG6_Anim.JY = AG6_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          AG6_Anim.JZ = AG6_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          AG6_Anim.JU = AG6_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          AG6_Anim.JV = AG6_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          AG6_Anim.JR = AG6_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AG6_Anim.JPOV = 0;
          else
            AG6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  for (i = 0; i < AG6_Anim.NumOfUnits; i++)
    AG6_Anim.Units[i]->Response(AG6_Anim.Units[i], &AG6_Anim);

  glClearColor(0.3, 0.5, 0.7, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(TRUE, TRUE, TRUE, TRUE);


  /*
  glColor3d(0.3, 0.5, 0.7);
  glRectd(-2, -2, 2, 2);
  glColor3d(1, 0, 0);
  glRectd(0, 0, 0.88, 0.30);
  */

  for (i = 0; i < AG6_Anim.NumOfUnits; i++)
  {
    static DBL time = 5;

    time += AG6_Anim.GlobalDeltaTime;
    if (time > 5)
    {
      time = 0;
      AG6_ShaderFree(AG6_RndProg);
      AG6_RndProg = AG6_ShaderLoad("TEST");
    }

    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    AG6_RndMatrWorld = MatrIdentity();

    AG6_Anim.Units[i]->Render(AG6_Anim.Units[i], &AG6_Anim);
  }
  glFinish();
  FrameCounter++;
} /* End of 'AG6_AnimRender' function */

VOID AG6_AnimCopyFrame( VOID )
{
  SwapBuffers(AG6_Anim.hDC);
} /* End of 'AG6_AnimCopyFrame' function */

VOID AG6_AnimAddUnit( ag6UNIT *Unit )
{
  if (AG6_Anim.NumOfUnits < AG6_MAX_UNITS)
  {
    AG6_Anim.Units[AG6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AG6_Anim);
  }
} /* End of 'AG6_AnimAddUnit' function */


VOID AG6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    GetWindowRect(AG6_Anim.hWnd, &SaveRC);

    hmon = MonitorFromWindow(AG6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AG6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AG6_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(AG6_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AG6_AnimFlipFullScreen' function */

VOID AG6_AnimDoExit( VOID )
{
  PostMessage(AG6_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AG6_AnimDoExit' function */


VOID AG6_AnimSetPause( BOOL NewPauseFlag )
{
  AG6_Anim.IsPause = NewPauseFlag;
} /* End of 'AG6_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */