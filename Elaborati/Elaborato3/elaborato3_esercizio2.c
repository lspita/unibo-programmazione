#include <ctype.h>
#include <stdio.h>

int main() {
  char input;
  int spazi = 0, caratteriNuovaLinea = 0, caratteriLetti = 0,
      caratteriControllo = 0;
  int maiuscoli = 0, minuscoli = 0;
  int caratteriAlfaNumerici = 0, caratteriNumerici = 0, caratteriAlfabetici = 0,
      caratteriEsadecimali = 0, caratteriAscii = 0;
  int caratteriStampabili = 0, caratteriGrafici = 0;
  int punteggiatura = 0;

  printf("Inserisci un testo terminato da '#' e premi Enter:\n");

  do {
    input = getchar();

    caratteriLetti++; // conto ogni char in input per restituire il numero dei
                      // caratteri letti

    if (isprint(input)) // contatore caratteri stampabili (spazio incluso)
    {
      caratteriStampabili++;
      if (isgraph(input)) // contatore caratteri stampabili (spazio escluso)
      {
        caratteriGrafici++;

        if (isascii(input)) // contatore caratteri ascii
        {
          caratteriAscii++;

          if (isalnum(input)) // contatore caratteri alfanumerici
          {
            caratteriAlfaNumerici++;

            if (isxdigit(input)) // contatore caratteri esadecimali [A - F,a -
                                 // f,0 - 9]
            {
              caratteriEsadecimali++;
            }

            if (isdigit(input)) { // contatore delle cifre numeriche
              caratteriNumerici++;
            } else if (isalpha(input)) { // contatore caratteri dell'alfabeto
              caratteriAlfabetici++;
              if (isupper(input)) { // contatore caratteri maiuscoli
                maiuscoli++;
              } else if (islower(input)) { // contatore caratteri minuscoli
                minuscoli++;
              }
            }
          } else if (ispunct(input)) { // contatore punteggiatura
            punteggiatura++;
          }
        }
      } else {
        if (isspace(input)) { // contatore degli spazi
          spazi++;
        } else if (iscntrl(input)) { // contatore caratteri di controllo
          caratteriControllo++;
          if (input == '\n') { // contatore andata a capo
            caratteriNuovaLinea++;
          }
        }
      }
    }

  } while (input != '#');

  printf("STATISTICHE:\n");
  printf("- Caratteri letti: %d di cui:\n", caratteriLetti);
  printf("\t- Caratteri stampabili(spazi inclusi):%d di cui:\n",
         caratteriStampabili);
  printf("\t\t- Caratteri di nuova linea: %d\n", caratteriNuovaLinea);
  printf("\t\t- Spazi: %d\n", spazi);
  printf("\t\t- Caratteri stampabili(spazi esclusi):%d di cui:\n",
         caratteriGrafici);
  printf("\t\t\t- Caratteri ascii: %d di cui:\n", caratteriAscii);
  printf("\t\t\t\t- Caratteri di punteggiatura: %d\n", punteggiatura);
  printf("\t\t\t\t- Caratteri alfanumerici: %d di cui:\n",
         caratteriAlfaNumerici);
  printf("\t\t\t\t\t- Caratteri numerici: %d\n", caratteriNumerici);
  printf("\t\t\t\t\t- Caratteri alfabetici: %d di cui:\n", caratteriAlfabetici);
  printf("\t\t\t\t\t\t- Caratteri in maiuscolo: %d\n", maiuscoli);
  printf("\t\t\t\t\t\t- Caratteri in minuscolo: %d\n", minuscoli);
  printf("\t\t\t\t\t (Caratteri esadecimale: %d)\n", caratteriEsadecimali);

  return 0;
}