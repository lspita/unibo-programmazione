#include <stdio.h>

int main() {
  int min = 0, max = 0, nTentativi = 0;

  do // prendo estremo inferiore con controllo
  {
    printf("Inserire estremo inferiore: ");
    scanf("%d", &min);
    fflush(stdin);
  } while (min < 0);

  do // prendo estremo superiore con controllo
  {
    printf("Inserire estremo superiore: ");
    scanf("%d", &max);
    fflush(stdin);
  } while (max < min);

  do // prendo numero di tentativi
  {
    printf("Inserire numero di tentativi (>0): ");
    scanf("%d", &nTentativi);
    fflush(stdin);
  } while (nTentativi < 1);

  printf("Pensa a un numero nel range %d-%d (estremi "
         "inclusi)\n------------------\n",
         min, max);

  max++; // includo max come estremo
  int guess = 0;
  char response = '\0';
  int victory = 0;

  while (nTentativi != 0 && victory == 0) // finchè ho tentativi e non ho vinto
  {
    guess = (int)((max - min) / 2) + min; // numero a metà tra min e max
    printf("%d tentativi rimasti.\nNumero: %d\n", nTentativi, guess);

    if (guess == min || guess == max - 1) {
      victory = -1;
    }
    unsigned short valid = 1;
    do {
      valid = 1;
      printf("Il tuo numero è maggiore, minore o uguale? [>/</=]: ");
      scanf("%c", &response);
      fflush(stdin);

      switch (response) // cambio estremi a seconda della risposta
      {
      case '>':
        min = guess;
        break;

      case '<':
        max = guess;
        break;

      case '=':
        victory = 1; // ho indovinato quindi esco
        break;

      default:
        valid = 0;
        break;
      }
    } while (valid == 0);

    nTentativi--;
    printf("------------------\n");
  }

  printf("\nFine partita\n");
  switch (victory) {
  case 1:
    printf("Ho vinto!\n");
    break;
  case 0:
    printf("Ho finito i tentativi, hai vinto!\n");
    break;
  case -1:
    printf("Ho raggiunto gli estremi, hai mentito!\n");
    break;
  }

  return 0;
}