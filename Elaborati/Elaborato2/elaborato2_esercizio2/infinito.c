#include <stdio.h>

int main() {
  int max = -1, min = -1, tmpMax, tmpMin, tentativi, q, cont = 0;
  short int fine = 0; // similbooleano per usare un ciclo
  char risposta;

  do {
    printf("digitare il MINIMO del range che deve comprendere il tuo numero "
           "(estremi inclusi) e premere invio: \n");
    fflush(stdin);
    scanf("%d", &min);
  } while (min < 0);

  do {
    printf("digitare il MASSIMO del range che deve comprendere il tuo numero "
           "(estremi inclusi) e premere invio: \n");
    fflush(stdin);
    scanf("%d", &max);
  } while (max <= (min + 1));

  tmpMax = max + 1;
  tmpMin = min;
  printf("bene ora pensa ad un numero compreso tra %d e %d!!!\n\n", min, max);
  while (fine == 0) {
    cont++;
    q = (int)((tmpMax - tmpMin) / 2) + tmpMin;
    do {
      printf("il tuo numero e' %d? [s/n]\n", q);
      fflush(stdin);
      scanf("%c", &risposta);
    } while (risposta != 's' && risposta != 'n');

    if (risposta == 's') {
      printf("EVVIVA! CI SONO VOLUTI BEN %d TENTATIVO/I\n", cont);
      fine = 1;
    } else {
      if (q == max || q == min) {
        printf("Ho perso!\n");
        fine = 1;
      } else {
        do {
          printf("il numero a cui stai pensando e' piu' grande o piu' piccolo "
                 "di %d? [g/p]\n",
                 q);
          fflush(stdin);
          scanf("%c", &risposta);
        } while (risposta != 'g' && risposta != 'p');
        if (risposta == 'g') {
          tmpMin = q;
        } else {
          tmpMax = q;
        }
      }
    }
  }
  return 0;
}