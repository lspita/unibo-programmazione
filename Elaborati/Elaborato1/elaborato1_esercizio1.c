/*
Matteo Tonelli
0001117913
matteo.tonelli9@studio.unibo.it
--------------------------------
Ludovico Maria Spitaleri
0001114169
ludovico.spitaleri@studio.unibo.it
*/

#include <float.h>
#include <limits.h>
#include <stdio.h>

// Direttiva per stampa del numero di caratteri stampati dal printf()
#define CARATTERI_STAMPATI                                                     \
  printf("Caratteri stampati dall'ultimo printf(): %d\n\n",                    \
         caratteriStampati);                                                   \
  caratteriStampati = 0;

int main() {
  // Interi
  int intVar = 5;
  signed int signedIntVar = -5;
  unsigned int unsignedIntVar = 5;

  long int longIntVar = 123456789;
  signed long int signedLongIntVar = -123456789;
  unsigned long int unsignedLongIntVar = 123456789;

  long long int longLongIntVar = 123456789123456789;
  signed long long int signedLongLongIntVar = -123456789123456789;
  unsigned long long int unsignedLongLongIntVar = 123456789123456789;

  short int shortIntVar = 3;
  signed short int signedShortIntVar = -3;
  unsigned short int unsignedShortIntVar = 0;

  int caratteriStampati = 0;

  // Stampe
  caratteriStampati = printf("\nInteri\n------\n");
  CARATTERI_STAMPATI

  // standard
  caratteriStampati =
      printf("-> Tipo: int, Valore: %d, Minimo: %d, Massimo: %d, Byte: %lu, "
             "Lunghezza stampa modificata: %4d\n",
             intVar, INT_MIN, INT_MAX, sizeof intVar, intVar);
  CARATTERI_STAMPATI

  caratteriStampati =
      printf("-> Tipo: signed int, Valore: %d, Minimo: %d, Massimo: %d, Byte: "
             "%lu, Lunghezza stampa modificata: %4d\n",
             signedIntVar, INT_MIN, INT_MAX, sizeof signedIntVar, signedIntVar);
  CARATTERI_STAMPATI

  caratteriStampati =
      printf("-> Tipo: unsigned int, Valore: %u, Minimo: %u, Massimo: %u, "
             "Byte: %u, Lunghezza stampa modificata: %4u\n",
             unsignedIntVar, 0, UINT_MAX, unsignedIntVar, unsignedIntVar);
  CARATTERI_STAMPATI

  // long
  caratteriStampati =
      printf("-> Tipo: long int, Valore: %ld, Minimo: %ld, Massimo: %ld, Byte: "
             "%lu, Lunghezza stampa modificata: %4ld\n",
             longIntVar, LONG_MIN, LONG_MAX, sizeof longIntVar, longIntVar);
  CARATTERI_STAMPATI

  caratteriStampati = printf("-> Tipo: signed long int, Valore: %ld, Minimo: "
                             "%ld, Massimo: %ld, Byte: %lu, Lunghezza "
                             "stampa modificata: %4ld\n",
                             signedLongIntVar, LONG_MIN, LONG_MAX,
                             sizeof signedLongIntVar, signedLongIntVar);
  CARATTERI_STAMPATI

  caratteriStampati = printf("-> Tipo: unsigned long int, Valore: %lu, Minimo: "
                             "%d, Massimo: %lu, Byte: %lu, "
                             "Lunghezza stampa modificata: %4lu\n",
                             unsignedLongIntVar, 0, ULONG_MAX,
                             unsignedLongIntVar, unsignedLongIntVar);
  CARATTERI_STAMPATI

  // long long
  caratteriStampati = printf("-> Tipo: long long int, Valore: %lld, Minimo: "
                             "%lld, Massimo: %lld, Byte: %lu, "
                             "Lunghezza stampa modificata: %4lld\n",
                             longLongIntVar, LLONG_MIN, LLONG_MAX,
                             sizeof longLongIntVar, longLongIntVar);
  CARATTERI_STAMPATI

  caratteriStampati =
      printf("-> Tipo: signed long long int, Valore: %lld, Minimo: %lld, "
             "Massimo: %lld, Byte: %lu, Lunghezza stampa "
             "modificata: %4lld\n",
             signedLongLongIntVar, LLONG_MIN, LLONG_MAX,
             sizeof signedLongLongIntVar, signedLongLongIntVar);
  CARATTERI_STAMPATI

  caratteriStampati =
      printf("-> Tipo: unsigned long long int, Valore: %lld, Minimo: %d, "
             "Massimo: %lld, Byte: %lu, Lunghezza stampa "
             "modificata: %4lld\n",
             unsignedLongLongIntVar, 0, ULLONG_MAX,
             sizeof unsignedLongLongIntVar, unsignedLongLongIntVar);
  CARATTERI_STAMPATI

  // short
  caratteriStampati =
      printf("-> Tipo: short int, Valore: %d, Minimo: %d, Massimo: %d, Byte: "
             "%lu, Lunghezza stampa modificata: %4d\n",
             shortIntVar, SHRT_MIN, SHRT_MAX, sizeof shortIntVar, shortIntVar);
  CARATTERI_STAMPATI

  caratteriStampati =
      printf("-> Tipo: signed short int, Valore: %d, Minimo: %d, Massimo: %d, "
             "Byte: %lu, Lunghezza stampa modificata: %4d\n",
             signedShortIntVar, SHRT_MIN, SHRT_MAX, sizeof signedShortIntVar,
             signedShortIntVar);
  CARATTERI_STAMPATI

  caratteriStampati = printf("-> Tipo: unsigned short int, Valore: %d, Minimo: "
                             "%d, Massimo: %d, Byte: %lu, Lunghezza "
                             "stampa modificata: %4d\n",
                             unsignedShortIntVar, 0, USHRT_MAX,
                             sizeof unsignedShortIntVar, unsignedShortIntVar);
  CARATTERI_STAMPATI

  // Float
  float floatVar = 1.234;

  // Stampe
  caratteriStampati = printf("\nFloat\n-----\n");
  CARATTERI_STAMPATI

  // standard
  caratteriStampati =
      printf("-> Tipo: float, Valore: %f, Minimo: %e, Massimo: %e, Byte: %lu, "
             "Precisione stampa modificata: %.4f\n",
             floatVar, FLT_MIN, FLT_MAX, sizeof floatVar, floatVar);
  CARATTERI_STAMPATI

  // Double
  double doubleVar = 5.678987654321;
  long double longDoubleVar = 123456789.123456789;

  // Stampe
  caratteriStampati = printf("\nDouble\n------\n");
  CARATTERI_STAMPATI

  // standard
  caratteriStampati =
      printf("-> Tipo: double, Valore: %f, Minimo: %e, Massimo: %e, Byte: %lu, "
             "Precisione stampa modificata: %.4f\n",
             doubleVar, DBL_MIN, DBL_MAX, sizeof doubleVar, doubleVar);
  CARATTERI_STAMPATI

  // long
  caratteriStampati = printf("-> Tipo: long double, Valore: %Lf, Minimo: %Le, "
                             "Massimo: %Le, Byte: %lu, Precisione "
                             "stampa modificata: %.4Lf\n",
                             longDoubleVar, LDBL_MIN, LDBL_MAX,
                             sizeof longDoubleVar, longDoubleVar);
  CARATTERI_STAMPATI

  // Char
  char charVar = 'k';
  signed char signedCharVar = 'k';
  unsigned char unsignedCharVar = 'k';

  // Stampe
  caratteriStampati = printf("\nChar\n----\n");
  CARATTERI_STAMPATI

  // standard
  caratteriStampati =
      printf("-> Tipo: char, Valore: %c, Minimo: %c, Massimo: %c\n, Byte: %lu",
             charVar, CHAR_MIN, CHAR_MAX, sizeof charVar);
  CARATTERI_STAMPATI

  // signed
  caratteriStampati = printf(
      "-> Tipo: signed char, Valore: %c, Minimo: %c, Massimo: %c\n, Byte: %lu",
      signedCharVar, CHAR_MIN, CHAR_MAX, sizeof signedCharVar);
  CARATTERI_STAMPATI

  // unsigned
  caratteriStampati =
      printf("-> Tipo: unsigned char, Valore: %c, Minimo: %c, Massimo: %c\n, "
             "Byte: %lu",
             unsignedCharVar, 0, UCHAR_MAX, sizeof unsignedCharVar);
  CARATTERI_STAMPATI

  return 0;
}