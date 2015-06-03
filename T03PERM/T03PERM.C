/* FILE NAME: T03PERM.C
 * PROGRAMMER: Gumirov Artur
 * DATE: 03.06.2015
 * PURPOSE: Permutation samples.
 */

#include <stdio.h>

#include <conio.h>

#include <windows.h>

#define N 5

INT P[N];
INT Parity = 0;

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

VOID Write( INT A )
{
  FILE *F;
  static INT k = 0;

  if ((F = fopen("Res.log", "a")) != NULL)
  {
    k++;
    fprintf(F, "%d ", A);

    if ((k % 5) == 0)
      fprintf(F, "\n");

    fclose(F);

  }
} /* End of 'WriteToFile' function */

INT CountParity( VOID )
{
  INT i, j, cnt = 0;

  for (i = 0; i < N - 1; i++)
    for (j = i + 1; j < N; j++)
      cnt += P[i] > P[j];

  return cnt;
} /* End of 'CountParity' function */

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
} 

VOID main( VOID )
{
  INT i;

  for (i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);

  getch();
} /* End of 'main' function */

/* END OF 'T03PERM.C' FILE */