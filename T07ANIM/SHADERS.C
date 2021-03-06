/* FILENAME: SHADERS.C
 * PROGRAMMER: AG6
 * PURPOSE: Shader support module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

static VOID AG6_SaveLog( CHAR *Text )
{
  FILE *F;

  if ((F = fopen("{_}SHD{30}.LOG", "a")) != NULL)
  {
    fprintf(F, "%s\n", Text);
    fclose(F);
  }
} /* End of 'AG6_SaveLog' function */

static CHAR * AG6_TextLoad( CHAR *FileName )
{
  FILE *F;
  CHAR *mem = NULL;

  if ((F = fopen(FileName, "r")) != NULL)
  {
    LONG len;

    fseek(F, 0, SEEK_END);
    len = ftell(F);

    if ((mem = malloc(len + 1)) != NULL)
    {
      fseek(F, 0, SEEK_SET);
      len = fread(mem, 1, len, F);
      mem[len] = 0;
    }
    fclose(F);
  }
  return mem;
} /* End of 'AG6_ShaderLoad' function */

UINT AG6_ShaderLoad( CHAR *FileNamePrefix )
{
  INT res, i;
  CHAR *txt;
  INT NumOfShaders = 2;
  UINT
    Shaders[2] = {0},
    ShTypes[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER},
     Prg = 0;
  CHAR *Suff[2] = {"VERT", "FRAG"};
  BOOL isok = TRUE;
  static CHAR Buf[1000];

  for (i = 0; i < NumOfShaders; i++)
  {
    sprintf(Buf, "%s.%s", FileNamePrefix, Suff[i]);
    if ((Shaders[i] = glCreateShader(ShTypes[i])) == 0)
    {
      isok = FALSE;
      AG6_SaveLog("Error create shader");
      break;
    }
    if ((txt = AG6_TextLoad(Buf)) == NULL)
    {
      isok = FALSE;
      AG6_SaveLog("Error load file");
      AG6_SaveLog(Buf);
      break;
    }
    glShaderSource(Shaders[i], 1, &txt, NULL);
    free(txt);
    glCompileShader(Shaders[i]);
    glGetShaderiv(Shaders[i], GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(Shaders[i], sizeof(Buf), &res, Buf);
      AG6_SaveLog(Buf);
      isok = FALSE;
      break;
    }
  }

  if (isok)
    if ((Prg = glCreateProgram()) == 0)
      isok = FALSE;
    else
    {
      for (i = 0; i < NumOfShaders; i++)
        if (Shaders[i] != 0)
          glAttachShader(Prg, Shaders[i]);
      glLinkProgram(Prg);
      glGetProgramiv(Prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(Prg, sizeof(Buf), &res, Buf);
        AG6_SaveLog(Buf);
        isok = FALSE;
      }
    }
  if (!isok)
  {
    for (i = 0; i < 2; i++)
      if (Shaders[i] != 0)
      {
        if (Prg != 0)
          glDetachShader(Prg, Shaders[i]);
        glDeleteShader(Shaders[i]);
      }
    if (Prg != 0)
      glDeleteProgram(Prg);
    return 0;
  }
  return Prg;
} /* End of 'AG6_ShaderLoad' function */

VOID AG6_ShaderFree( UINT PrgId )
{
  UINT i, n, shdrs[5];

  if (PrgId == 0)
    return;

  glGetAttachedShaders(PrgId, 5, &n, shdrs);

  for (i = 0; i < n; i++)
  {
    glDetachShader(PrgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(PrgId);
} /* End of 'AG6_ShaderFree' function */

/* END OF 'RENDER.C' FILE */