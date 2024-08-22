#include <stdbool.h>
#include <stdio.h>

void fflush_stdin(bool err) {
  if (err) {
    printf("Not valid input, retry:");
  }
  while (getchar() != '\n')
    ;
}

int main() {
  char retry;

  int myInt;
  float myFloat;
  double myDouble;
  char myChar;
  int myArray[5] = {10, 20, 30, 40, 50};

  do {
    int *intPtr = &myInt;
    float *floatPtr = &myFloat;
    double *doublePtr = &myDouble;
    char *charPtr = &myChar;

    printf("Inserisci un valore intero per myInt: ");
    do {
      if (scanf("%d", intPtr) == 1) {
        break;
      } else {
        fflush_stdin(true);
      }
    } while (1);
    fflush_stdin(false);

    printf("Inserisci un valore decimale per myFloat: ");
    do {
      if (scanf("%f", floatPtr) == 1) {
        break;
      } else {
        fflush_stdin(true);
      }
    } while (1);
    fflush_stdin(false);

    printf("Inserisci un valore decimale per myDouble: ");
    do {
      if (scanf("%lf", doublePtr) == 1) {
        break;
      } else {
        fflush_stdin(true);
      }
    } while (1);
    fflush_stdin(false);

    printf("Inserisci un carattere per myChar: ");
    do {
      if (scanf(" %c", charPtr) == 1) {
        break;
      } else {
        fflush_stdin(true);
      }
    } while (1);
    fflush_stdin(false);

    for (int i = 0; i < 5; i++) {
      printf("Inserisci un valore intero per myArray[%d]: ", i);
      do {
        if (scanf("%d", &myArray[i]) == 1) {
          break;
        } else {
          fflush_stdin(true);
        }
      } while (1);
      fflush_stdin(false);
    }
    printf("----------------------------");

    printf("\nValori inseriti:\n");
    printf("myInt (accesso diretto): %d\n", myInt);
    printf("myInt  (accesso indiretto): %d\n", *intPtr);

    printf("myFloat (accesso diretto): %.2f\n", myFloat);
    printf("myFloat (accesso indiretto): %.2f\n", *floatPtr);

    printf("myDouble (accesso diretto): %.2lf\n", myDouble);
    printf("myDouble (accesso indiretto): %.2lf\n", *doublePtr);

    printf("myChar (accesso diretto): %c\n", myChar);
    printf("myChar (accesso indiretto): %c\n", *charPtr);

    printf("myArray (accesso diretto): ");
    for (int i = 0; i < 5; i++) {
      printf("%d ", myArray[i]);
    }
    printf("\n");

    printf("\tmyArray (accesso indiretto): ");
    for (int i = 0; i < 5; i++) {
      printf("%d ", *(myArray + i));
    }
    printf("\n");

    printf("Indirizzo in memoria delle variabili (accesso diretto):\n");
    printf("\tmyInt: %p\n", (void *)&myInt);
    printf("\tmyFloat: %p\n", (void *)&myFloat);
    printf("\tmyDouble: %p\n", (void *)&myDouble);
    printf("\tmyChar: %p\n", (void *)&myChar);

    for (int i = 0; i < 5; i++) {
      printf("\tmyArray[%d]: %p\n", i, (void *)&myArray[i]);
    }

    printf("Indirizzo in memoria delle variabili tramite il puntatore (accesso "
           "indiretto):\n");
    printf("\tmyInt: %p\n", (void *)intPtr);
    printf("\tmyFloat: %p\n", (void *)floatPtr);
    printf("\tmyDouble: %p\n", (void *)doublePtr);
    printf("\tmyChar: %p\n", (void *)charPtr);

    for (int i = 0; i < 5; i++) {
      printf("\tmyArray[%d]: %p\n", i, (void *)&myArray[i]);
    }

    printf("Indirizzo in memoria dei puntatori:\n");
    printf("\tintPtr: %p\n", (void *)&intPtr);
    printf("\tfloatPtr: %p\n", (void *)&floatPtr);
    printf("\tdoublePtr: %p\n", (void *)&doublePtr);
    printf("\tcharPtr: %p\n", (void *)&charPtr);

    do {
      printf("Do you want to retry? y/n \n");
      scanf("%c", &retry);
      fflush_stdin(false);
    } while (retry != 'y' && retry != 'n');

  } while (retry == 'y');
  return 0;
}
