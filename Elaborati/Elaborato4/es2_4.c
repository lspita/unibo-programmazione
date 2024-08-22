#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_X 10
#define FIELD_Y 10

int randint(int min, int max) { return rand() % (max - min + 1) + min; }

void prepareField(unsigned int fieldY, unsigned int fieldX,
                  char field[fieldY][fieldX]) {
  for (unsigned int y = 0; y < FIELD_Y; y++) {
    for (unsigned int x = 0; x < FIELD_X; x++) {
      field[y][x] = '.';
    }
  }
}

struct Point {
  int x;
  int y;
};

struct Point addPoints(const struct Point *p1, const struct Point *p2) {
  struct Point sum = {
      p1->x + p2->x,
      p1->y + p2->y,
  };
  return sum;
}

bool randomStep(struct Point *currentPos, unsigned int fieldY,
                unsigned int fieldX, char field[fieldY][fieldX]) {
  const struct Point movements[4] = {
      {1, 0},  // dx
      {-1, 0}, // sx
      {0, 1},  // up
      {0, -1}, // down
  };
  unsigned int length = sizeof(movements) / sizeof(movements[0]);

  bool excludedIndexes[length];

  for (unsigned int i = 0; i < length; i++) // prevent unwanted behaviours
  {
    excludedIndexes[i] = false;
  }

  int maxIndex = length - 1;

  do {
    unsigned int rndIndex = randint(0, maxIndex);
    int counter = -1;

    // loop over movements, counter is used as index translator to only possible
    // movements remaining
    for (unsigned int i = 0; i < length; i++) {
      if (!excludedIndexes[i]) {
        counter++;
        if (counter == rndIndex) {
          struct Point newPos = addPoints(currentPos, &movements[i]);
          if ((newPos.y >= 0 && newPos.y < fieldY) && // y in boundaries
              (newPos.x >= 0 && newPos.x < fieldX) && // x in boundaries
              field[newPos.y][newPos.x] == '.')       // cell is free
          {
            *(currentPos) = newPos;
            return true;
          } else {
            excludedIndexes[i] = true;
            maxIndex--;
            break;
          }
        }
      }
    }
  } while (maxIndex >= 0);

  return false;
}

void printField(unsigned int maxY, unsigned int maxX, char field[maxY][maxX]) {
  for (unsigned int y = 0; y < FIELD_Y; y++) {
    for (unsigned int x = 0; x < FIELD_X; x++) {
      printf("%c ", field[y][x]);
    }
    putchar('\n');
  }
}

int main() {
  char field[FIELD_Y][FIELD_X];

  prepareField(FIELD_Y, FIELD_X, field);

  const char pins[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  };

  unsigned int steps = sizeof(pins) / sizeof(pins[0]);

  srand(time(NULL));

  struct Point currentPos = {
      randint(0, FIELD_X - 1),
      randint(0, FIELD_Y - 1),
  };

  bool done = false;
  for (unsigned int i = 0; i < steps; i++) {
    done = randomStep(&currentPos, FIELD_Y, FIELD_X, field);
    if (done) {
      field[currentPos.y][currentPos.x] = pins[i];
    } else {
      break;
    }
  }

  printField(FIELD_Y, FIELD_X, field);

  return 0;
}