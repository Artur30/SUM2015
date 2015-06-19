/* FILE NAME: T04DETERM.C
 * PROGRAMMER: AG6
 * DATE: 04.06.2015
 * PURPOSE: Determinant samples.
 */

#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define MAX 10

INT AG6_P[MAX];
DOUBLE Matr[MAX][MAX], DETERM = 0;
INT Parity = 0, N;
FILE *F;
VOID Load( CHAR *FileName )
{
  FILE *File;
  INT i, j;

  if ((File = fopen(FileName, "r")) != NULL)
  {
    fscanf(File, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(File, "%lf", &Matr[i][j]);

    fclose(File);
  }
} /* End of 'Load' function */

 
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */


VOID WriteToFile( DOUBLE A )
{
  if ((F = fopen("Res.log", "a")) != NULL)
  {
    fprintf(F, "The determinant is %g", A);
    fclose(F);
  }
} /* End of 'WriteToFile' function */

   
VOID Go( INT Pos )
{
  INT i, x, save;

  if (Pos == N)
  {
    DOUBLE PROD = 1;

    for (i = 0; i < N; i++)
    {
      PROD *= Matr[i][AG6_P[i]];

      if (Parity == 0)
        DETERM += PROD;
      else
        DETERM -= PROD;
    }

    return;
  }

  save = Parity;
  Go(Pos + 1);
  for (i = Pos + 1; i < N; i++)
  {
    Parity = !Parity;
    Swap(&AG6_P[Pos], &AG6_P[i]);
    Go(Pos + 1);
  }

  Parity = save;
  x = AG6_P[Pos];

  for (i = Pos + 1; i < N; i++)
    AG6_P[i - 1] = AG6_P[i];
  AG6_P[N - 1] = x;
} /* End of 'Go' function */


VOID main( VOID )
{
  INT i, j, k;

  if ((F = fopen("Res.log", "w")) != NULL)
    fclose(F);

  Load("m.txt");

  for (i = 0; i < N; i++)
    AG6_P[i] = i + 1;

  Go(0);
  WriteToFile(DETERM);
} /* End of 'main' function */

/* END OF 'T04DETERM.C' FILE */