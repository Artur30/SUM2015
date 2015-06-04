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

DOUBLE A[MAX][MAX];
INT P[MAX];


VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

VOID Load( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  if ((F = fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
} /* End of "Load" function */

 VOID Go( INT Pos )
{
  INT i;

  if (Pos == N)
  {
    for (i = 0; i < N; i++)
      printf("%d ", P[i]);
    printf(" - %s\n", Parity ? "odd" : "even");
    return;
  }
  for (i = Pos; i < N; i++)
  {
    if (Pos != i)
      Parity = !Parity;
    Swap(&P[Pos], &P[i]);
    Go(Pos + 1);
   
    if (Pos != i)
        Parity = !Parity;
    Swap(&P[Pos], &P[i]);
  }
} /* End of 'GO' function */

/*VOID main( VOID )
{
  INT i;

  Load("m.txt");
  for (i = 0; i < N; i++)
  printf("%i", SUM);

  getch(); */
} /* End of "main" function */