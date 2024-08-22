#include <limits.h>
#include <math.h>
#include <stdio.h>

int main() {
  // Operatori aritmetici
  printf("Operatori aritmetici:\n\n");

  printf("%d + %d = %d\n", 10, 20, 10 + 20);
  printf("%d - %d = %d\n", 30, 10, 30 - 10);
  printf("%d * %d = %d\n", 5, 10, 5 * 10);
  printf("%d / %d = %d\n", 20, 5, 20 / 5);
  printf("%d %% %d = %d\n", 10, 3, (10 % 3));

  // Operatori di confronto
  printf("\n** Operatori di confronto **\n\n");

  printf("* %d == %d = %d\n", 10, 10, 10 == 10);
  printf("* %d != %d = %d\n", 10, 20, 10 != 20);
  printf("* %d > %d = %d\n", 10, 20, 10 > 20);
  printf("* %d < %d = %d\n", 10, 20, 10 < 20);
  printf("* %d >= %d = %d\n", 10, 10, 10 >= 10);
  printf("* %d <= %d = %d\n", 10, 20, 10 <= 20);

  // Operatori logici
  printf("\n** Operatori logici **\n\n");

  printf("* %d && %d = %d\n", 1, 1, 1 && 1);
  printf("* %d || %d = %d\n", 1, 0, 1 || 0);
  printf("* !%d = %d\n", 1, !1);

  // Operatori di assegnamento
  printf("\n** Operatori di assegnamento **\n\n");

  int a = 10;
  printf("* a = %d\n", a);
  a += 10;
  printf("* a += 10; a = %d\n", a);
  a -= 10;
  printf("* a -= 10; a = %d\n", a);
  a *= 10;
  printf("* a *= 10; a = %d\n", a);
  a /= 10;
  printf("* a /= 10; a = %d\n", a);
  a %= 10;
  printf("* a %% = 10; a = %d\n", a);

  // Operatori di incremento e decremento
  printf("\n** Operatori di incremento e decremento **\n\n");

  int b = 10;
  printf("* b = %d\n", b);
  b++;
  printf("* b++; b = %d\n", b);
  b--;
  printf("* b--; b = %d\n", b);

  // Operatore di indirizzo
  printf("\n** Operatore di indirizzo **\n\n");

  int *p = &a;
  printf("* p = &a; p = %p\n", p);
  printf("* *p = %d\n", *p);

  printf("\n\n Utilizzo scorretto tipi:\n");
  char c = 100;
  printf("Il valore di c è %d\n",
         c); // Questo stamperà un valore diverso da 100, perché 100 è un numero
             // troppo grande per essere rappresentato da un byte

  printf("\n\nUtilizzo di un tipo di dato troppo grande per rappresentare un "
         "valore:\n");

  short s = 1000000;
  printf("Il valore di s è %d\n",
         s); // Questo stamperà un valore diverso da 1000000, perché 1000000 è
             // un numero troppo grande per essere rappresentato da un short

  printf("\n\nUtilizzo di un tipo di dato non appropriato per "
         "un'operazione/Conversione implicita:\n");

  int a2 = 10;
  float b2 = 2.5;
  printf("Il risultato di a / b è %d\n",
         a2 / b2); // Questo stamperà un valore diverso da 4, perché
                   // l'operazione di divisione tra due interi viene troncata

  printf("\n\nUtilizzo di più operatori con side effect:\n");

  a = b++;

  printf("a = %d, b = %d\n", a, b);

  printf("\n\nSuperamento del range massimo dei valori:\n");
  int max = INT_MAX; // Il valore massimo di un int

  // Overflow

  printf("max + 1 = %d\n",
         max + 1); // Questo stamperà un valore diverso da 2147483648, perché
                   // 2147483647 è il valore massimo di un int

  printf("\n\nTutte le funzioni di math.h:\n");
  // Funzioni di base

  double adb = 10.0;
  double bdb = 5.0;

  printf("adb + bdb = %f\n", adb + bdb);
  printf("adb - bdb = %f\n", adb - bdb);
  printf("adb * bdb = %f\n", adb * bdb);
  printf("adb / bdb = %f\n", adb / bdb);
  printf("adb ^ bdb = %f\n", pow(adb, bdb));

  // Funzioni di trigonometria

  printf("sin(adb) = %f\n", sin(adb));
  printf("cos(adb) = %f\n", cos(adb));
  printf("tan(adb) = %f\n", tan(adb));
  printf("cot(adb) = %f\n", 1 / tan(adb));
  printf("sec(adb) = %f\n", 1 / cos(adb));
  printf("csc(adb) = %f\n", 1 / sin(adb));

  // Funzioni di iperbole

  printf("sinh(adb) = %f\n", sinh(adb));
  printf("cosh(adb) = %f\n", cosh(adb));
  printf("tanh(adb) = %f\n", tanh(adb));

  // Funzioni di logaritmo

  printf("log(adb) = %f\n", log(adb));
  printf("log10(adb) = %f\n", log10(adb));
  printf("log2(adb) = %f\n", log2(adb));

  // Funzioni di esponenziale

  printf("exp(adb) = %f\n", exp(adb));
  printf("pow(adb, bdb) = %f\n", pow(adb, bdb));

  // Funzioni di radice

  printf("sqrt(adb) = %f\n", sqrt(adb));
  printf("cbrt(adb) = %f\n", cbrt(adb));

  // Funzioni di gamma

  printf("gamma(adb) = %f\n", tgamma(adb));

  // Funzioni di errore

  printf("erf(adb) = %f\n", erf(adb));
  printf("erfc(adb) = %f\n", erfc(adb));
  return 0;
}