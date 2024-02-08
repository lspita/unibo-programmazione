/*
Matteo Tonelli
0001117913
matteo.tonelli9@studio.unibo.it
--------------------------------
Ludovico Maria Spitaleri
0001114169
ludovico.spitaleri@studio.unibo.it
*/

#include <stdio.h>

#define MPG_LPK 235.2145 // costante per conversione da miglia a litri per 100km

int main()
{
    float miglia;
    float galloni;
    float MPG; // miglia per gallone
    float LPK; // litri per kilometro

    printf("Inserire il numero di miglia: ");
    scanf("%f", &miglia);

    fflush(stdin); // pulisco il buffer

    printf("Inserire il numero di galloni consumati: ");
    scanf("%f", &galloni);

    MPG = miglia / galloni;
    LPK = MPG_LPK / MPG;

    printf("\nRisultati:\nMiglia/gallone: %.2f\nLitri/100km: %.3f \n", MPG, LPK);
}
