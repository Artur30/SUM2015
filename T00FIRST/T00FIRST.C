#include <windows.h>

void main( void )
{
  if ((MassageBox(NULL, "Ask", "30", MB_ICONQUESTION | MB_YESNO) == IDYES)
    MassageBox(NULL, "Ask", "30", MB_OK);
}