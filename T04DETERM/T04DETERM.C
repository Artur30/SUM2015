/* FILE NAME: T04DETERM.C
 * PROGRAMMER: Gumirov Artur
 * DATE: 04.06.2015
 * PURPOSE: Permutation samples.
 */

#include <stdio.h>

#include <conio.h>

#include <windows.h>

#define MAX 30

INT N;
INT Parity = 0;
INT SUM = 0;
FILE *F;

DOUBLE A[MAX][MAX], DETERM = 0;
INT P[MAX];

VOID Load( CHAR *FileName )
{
  FILE *File;
  INT i, j;

  if ((File = fopen(FileName, "r")) != NULL)
  {
    fscanf(File, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(File, "%lf", &A[i][j]);

    fclose(File);
  }
} /* End of 'Load' function */

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

VOID Write( DOUBLE A )
{
  if ((F = fopen("Res.log", "a")) != NULL)
  {
    fprintf(F, "The determinant is %g", A);
    fclose(F);
  }
} /* End of 'Write' function */


VOID Go( INT Pos )
{
  INT i, x, save;

  if (Pos == N)
  {
    DOUBLE PROD = 1;

    for (i = 0; i < N; i++)
    {
      PROD *= A[i][P[i]];

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
    Swap(&P[Pos], &P[i]);
    Go(Pos + 1);
  }

  Parity = save;
  x = P[Pos];

  for (i = Pos + 1; i < N; i++)
    P[i - 1] = P[i];
  P[N - 1] = x;
} /* End of 'Go' function */

VOID main( VOID )
{
  INT i, j, k;

  if ((F = fopen("Res.log", "w")) != NULL)
    fclose(F);

  Load("m.txt");

  for (i = 0; i < N; i++)
    P[i] = i + 1;

  Go(0);
  Write(DETERM);

  getch();
} /* End of 'main' function */

/* END OF 'T04DETERM.C' FILE */