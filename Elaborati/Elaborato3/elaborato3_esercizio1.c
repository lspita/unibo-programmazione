#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Point {
  int x;
  int y;
};

struct Continent {
  char *name;
  struct Point sw;
  struct Point ne;
};

void flush_stdin() {
  while (getchar() != '\n') {
    // discard remaining chars (fflush(stdin) alternative for linux)
  }
}

int randint(int min, int max) { return rand() % (max - min + 1) + min; }

int middle(int l, int r) { return (l + r) / 2; }

void clearConsole() { printf("\e[1;1H\e[2J"); }

void horizontalBorder(const int min, const int max, const unsigned int unit,
                      bool showX) {
  putchar('+');
  for (int i = min; i <= max; i += unit) {
    i == 0 && showX ? printf(" %d", i) : printf(" -");
  }
  printf(" +\n");
}

// unit must be GCD of all continents' coordinates values and must hit 0 once in
// both axis
//(suggested: 1 or 2 with even x,y values)
void printGlobe(const struct Point *sw, const struct Point *ne,
                struct Continent *continents, int nContinents,
                const unsigned int scale) {
  const int minX = sw->x, minY = sw->y, maxX = ne->x, maxY = ne->y;

  bool idPrinted[nContinents];

  for (int i = 0; i < nContinents; i++) // ensure no unexpected behaviour
  {
    idPrinted[i] = false;
  }

  clearConsole();
  printf("Unit: %u\n\n", scale);
  horizontalBorder(minX, maxX, scale, false);

  for (int y = maxY; y >= minY; y -= scale) {
    putchar(y == 0 ? '0' : '|');
    for (int x = minX; x <= maxX; x += scale) {
      char printedChar = ' ';
      for (int i = 0; i < nContinents; i++) {
        const struct Continent c = continents[i];
        if (x == c.sw.x || x == c.ne.x) {
          if (y == c.sw.y || y == c.ne.y) {
            printedChar = '+';
            break;
          } else if (y > c.sw.y && y < c.ne.y) {
            printedChar = '|';
            break;
          }
        } else if (x > c.sw.x && x < c.ne.x) {
          if (y == c.sw.y || y == c.ne.y) {
            printedChar = '-';
            break;
          } else if (x >= middle(c.sw.x, c.ne.x) &&
                     y <= middle(c.sw.y, c.ne.y) &&
                     idPrinted[i] == false) // >= and idPrinted is needed
                                            // because unit can be >1
          {
            printedChar = i + '0'; // to ascii digit
            idPrinted[i] = true;
          }
        }
      }
      printf(" %c", printedChar);
    }
    printf(" |\n");
  }

  horizontalBorder(minX, maxX, scale, true);
}

void startTimer(const double seconds, bool showTimer) {
  double elapsed = 0;
  const clock_t start = clock();
  if (showTimer) {
    printf("Time map shown: %.3fs\n", seconds);
  }
  do {
    elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
  } while (elapsed < seconds);
}

bool getGuess(const struct Point *sw, const struct Point *ne,
              struct Continent *continents, int nContinents) {
  const int minX = sw->x, minY = sw->y, maxX = ne->x, maxY = ne->y;
  const struct Point generated = {randint(minX, maxX), randint(minY, maxY)};

  printf("Point: x: %d, y: %d\n\n", generated.x, generated.y);

  printf("-1) Ocean\n");
  for (int i = 0; i < nContinents; i++) {
    printf("%d) %s\n", i, continents[i].name);
  }

  printf("\n");

  int guess = -2;
  do {
    printf("Where is it located? ");
    scanf("%d", &guess);
    flush_stdin();
  } while (guess < -1 || guess >= nContinents);

  int continentIndex = -2;

  for (int i = 0; i < nContinents; i++) {
    const struct Continent c = continents[i];
    if (generated.x >= c.sw.x && generated.x <= c.ne.x &&
        generated.y >= c.sw.y && generated.y <= c.ne.y) {
      continentIndex = i;
      break;
    }
  }
  if (continentIndex ==
      -2) { // if no continent contains the point, it is in the ocean
    continentIndex = -1;
  }

  const bool correct = guess == continentIndex;

  if (correct) {
    printf("Correct!\n");
  } else {
    printf("Wrong! It was %s\n",
           continentIndex == -1 ? "Ocean" : continents[continentIndex].name);
  }

  return correct;
}

int main() {
  srand(time(NULL)); // Initialize random generator seed
  const struct Point GLOBE_SW = {-40, -20}, GLOBE_NE = {40, 20};

  struct Continent continents[] = {{"Europe", {-14, 10}, {4, 16}},
                                   {"Asia", {6, 6}, {32, 18}},
                                   {"Oceania", {14, -18}, {36, -8}},
                                   {"Africa", {-14, -16}, {4, 4}},
                                   {"America", {-38, -18}, {-22, 18}}};

  const int nContinents = sizeof(continents) / sizeof(struct Continent);

  bool correct = false;

  int scale = 0;
  do {
    printf("Enter scale of the map (suggested: 1-2): ");
    scanf("%d", &scale);
    flush_stdin();
  } while (scale <= 0);

  int timeMapShown = 0;
  do {
    printf("Enter the time the map is shown [s]: ");
    scanf("%d", &timeMapShown);
    flush_stdin();
  } while (timeMapShown < 0);

  bool play = true;
  while (play) {
    unsigned int points = 0;
    do {
      printGlobe(&GLOBE_SW, &GLOBE_NE, continents, nContinents, scale);
      printf("\n");
      startTimer(timeMapShown, true);
      clearConsole();

      correct = getGuess(&GLOBE_SW, &GLOBE_NE, continents, nContinents);

      if (correct) {
        points++;
        printf("Current points: %u\n", points);
        startTimer(1, false);
      }
    } while (correct);

    printf("You lost! Streak: %u\n", points);
    char c = '\0';
    do {
      printf("Do you want to retry? [y/n]: ");
      c = getchar();
      flush_stdin();
    } while (c != 'y' && c != 'n');
    if (c == 'n') {
      play = false;
    }
  }

  return 0;
}
