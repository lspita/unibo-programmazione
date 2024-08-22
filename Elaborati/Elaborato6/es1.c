#include <float.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

// All emojis or all not emojis, because emojis are wider than text
#define ROBOT_PIN L'👾'
#define START_PIN L'🏁'
#define BLANK_PIN L'⚪'
#define HIGHLIGHT_PIN L'🔵'
#define POWER_PIN L'🔋'
#define MINE_PIN L'💣'
#define END_PIN L'🏆'

#define PINS_WIDTH 1.5 // width of emojis (insert 1 if using normal chars)

// Box drawing
#define WALL L'┃'
#define FLOOR L'━'
#define NWANGLE L'┏'
#define NEANGLE L'┓'
#define SWANGLE L'┗'
#define SEANGLE L'┛'

// Probabilities that thing is inserted in field (1/n)
#define BOMB_PROB 20
#define ENERGY_PROB 15

#define N_ACTIONS 4

// Printing
#define DBL_MAX_CHAR_LENGTH 316
#define BOLD "\e[1m"
#define RESET "\e[m"

// Points
#define STEP_REWARD -1
#define MINE_REWARD -100
#define POWER_REWARD 1
#define END_REWARD 100
#define LOOP_REWARD -50
#define STUCK_REWARD -100

// Training
#define LEARNING_RATE 0.1
#define DISCOUNT_FUTURE 0.99

#define CHANCE_THRESHOLD 25
#define CHANCE_REDUCER 0.95

enum StateType {
  START,
  BLANK,
  POWER,
  MINE,
  END,
};

enum Action {
  UP,
  RIGHT,
  DOWN,
  LEFT,
};

struct Point {
  int row;
  int col;
};

struct Robot {
  struct Point position;
  const wchar_t PIN;
};

int to_state(int row, int col, int max_col) { return (row * max_col) + col; }

void move_point(const struct Point *const current_pos,
                const struct Point *const movement, const int rows,
                const int cols, struct Point *out_pos) {
  int new_row = (*current_pos).row + (*movement).row,
      new_col = (*current_pos).col + (*movement).col;

  (*out_pos) = (struct Point){
      new_row >= 0 ? (new_row >= rows ? new_row - rows : new_row)
                   : rows + new_row,
      new_col >= 0 ? (new_col >= cols ? new_col - cols : new_col)
                   : cols + new_col,
  };
}

int randint(const int min, const int max) {
  return min + (rand() % (max - min + 1));
}

void init() {
  setlocale(LC_ALL, ""); // permit print of UTF-8 emoji and symbols ("" =
                         // portable to all locales)
  srand(time(NULL));
}

void clearConsole() { printf("\e[1;1H\e[2J"); }

void fflush_stdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void random_in_border(const int rows, const int cols, struct Point *out_pos) {
  const int random_row = randint(0, rows - 1);
  const int random_col =
      random_row == 0 || random_row == rows - 1
          ? randint(0, cols - 1)        /* all cells are in border */
          : randint(0, 1) * (cols - 1); /* only extremes are in border */

  *out_pos = (struct Point){
      random_row,
      random_col,
  };
}

void wait(const double seconds) {
  double elapsed = 0;
  const clock_t start = clock();
  do {
    elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
  } while (elapsed < seconds);
}

void setup_field(const int rows, const int cols,
                 enum StateType (*const field)[cols],
                 struct Robot *const robot_ptr) {
  // fill labirinth
  for (enum StateType *cell = &field[0][0]; cell <= &field[rows - 1][cols - 1];
       cell++) {
    int random = randint(0, BOMB_PROB - 1);
    if (random == 0) {
      *cell = MINE;
    } else {
      random = randint(0, ENERGY_PROB - 1);
      if (random == 0) {
        *cell = POWER;
      } else {
        *cell = BLANK;
      }
    }
  }

  // create start and end
  struct Point start_pos = {
      randint(1, rows - 2),
      randint(1, cols - 2),
  };
  field[start_pos.row][start_pos.col] = START;

  (*robot_ptr).position = start_pos;

  struct Point end_pos;
  random_in_border(rows, cols, &end_pos);

  field[end_pos.row][end_pos.col] = END;
}

void print_field(const int rows, const int cols,
                 const enum StateType (*const field)[cols],
                 const wchar_t *const PINS,
                 const enum StateType *const highlight,
                 const struct Robot *const robot_ptr) {

  printf("%lc", NWANGLE);
  for (int i = 0; i <= cols * 2 * PINS_WIDTH; i++) {
    printf("%lc", FLOOR);
  }
  printf("%lc\n", NEANGLE);

  for (int row = 0; row < rows; row++) {
    printf("%lc ", WALL);
    for (int col = 0; col < cols; col++) {
      wchar_t pin = '\0';
      if (highlight != NULL && &field[row][col] == highlight) {
        pin = HIGHLIGHT_PIN;
      } else if ((*robot_ptr).position.row == row &&
                 (*robot_ptr).position.col == col) {
        pin = (*robot_ptr).PIN;
      } else {
        pin = PINS[field[row][col]];
      }
      printf("%lc ", pin);
    }
    printf("%lc\n", WALL);
  }

  printf("%lc", SWANGLE);
  for (int i = 0; i <= cols * 2 * PINS_WIDTH; i++) {
    printf("%lc", FLOOR);
  }
  printf("%lc\n", SEANGLE);
  putchar('\n');
}

void print_qtable(const int state, const int cols,
                  const double (*const qtable)[cols]) {
  const char *const TOP_ROW[] = {"Up", "Right", "Down", "Left"};

  printf("%sState %d Q-Table values%s\n", BOLD, state + 1, RESET);

  for (int c = 0; c < cols; c++) {
    printf("[%u] %s: %f\n", c, TOP_ROW[c], qtable[state][c]);
  }
  putchar('\n');
}

void show_frame(const int qtable_state, const int qtable_cols,
                const double (*const qtable)[qtable_cols], const int field_rows,
                const int field_cols,
                const enum StateType (*const field)[field_cols],
                const wchar_t *PINS, const enum StateType *const highlight,
                const struct Robot *const robot_ptr) {
  print_field(field_rows, field_cols, field, PINS, highlight, robot_ptr);
  print_qtable(qtable_state, qtable_cols, qtable);
}

enum Action best_action(const int state, const int n_actions,
                        const double (*const qtable)[n_actions]) {
  enum Action best_index = -1;

  int equal_maxes[n_actions];
  for (int *cell = equal_maxes; cell <= &equal_maxes[n_actions - 1]; cell++) {
    *cell = -1;
  }
  int n_maxes = 0;

  double max_qval = -DBL_MAX;
  for (int i = 0; i < n_actions; i++) {
    const double qval = qtable[state][i];
    if (qval > max_qval) {
      max_qval = qval;
      best_index = i;
      n_maxes = 1;
      for (int *cell = equal_maxes; cell <= &equal_maxes[n_actions - 1];
           cell++) {
        *cell = -1;
      }
      equal_maxes[0] = i;
    } else if (qval == max_qval) {
      equal_maxes[n_maxes] = i;
      n_maxes++;
    }
  }

  if (n_maxes > 1) {
    const int random_max = randint(0, n_maxes - 1);
    best_index = equal_maxes[random_max];
  }

  return best_index;
}

int get_reward(const enum StateType statetype, bool loop, bool stuck) {
  if (loop) {
    return LOOP_REWARD;
  }
  if (stuck) {
    return STUCK_REWARD;
  }

  switch (statetype) {
  case END:
    return END_REWARD;
    break;
  case MINE:
    return MINE_REWARD;
  case POWER:
    return POWER_REWARD;
  default:
    return STEP_REWARD;
  }
}

void update_qtable(const int current_state, const enum Action action,
                   const int reward, const int new_state, const int n_actions,
                   double (*const qtable)[n_actions]) {
  enum Action best_future = best_action(new_state, n_actions, qtable);

  qtable[current_state][action] +=
      LEARNING_RATE * (reward + (DISCOUNT_FUTURE * best_future) -
                       qtable[current_state][action]);
}

void train_qtable(const long long iterations, const int qtable_cols,
                  double (*const qtable)[qtable_cols], const int field_rows,
                  const int field_cols,
                  const enum StateType (*const field)[field_cols],
                  const wchar_t *PINS, struct Robot *const robot_ptr,
                  const struct Point *const MOVEMENTS) {
  const struct Point START_POS = (*robot_ptr).position;

  double chance_variable = 100 - CHANCE_THRESHOLD;

  for (long long i = 1; i <= iterations; i++) {
    double random_chance = chance_variable + CHANCE_THRESHOLD;
    (*robot_ptr).position = START_POS;
    bool exit = false;
    long long remaining_steps = field_rows * field_cols;

    bool passed_cells[field_rows][field_cols];
    for (bool *c = &passed_cells[0][0];
         c < &passed_cells[field_rows - 1][field_cols - 1]; c++) {
      *c = false;
    }

    while (!exit) {
      clearConsole();
      printf("%sIteration %llu/%llu%s\n\n", BOLD, i, iterations, RESET);
      printf("Remaining steps: %llu\n", remaining_steps);
      printf("Random chance: %.2f%%\n", random_chance);

      remaining_steps--;
      const struct Point current_pos = (*robot_ptr).position;
      passed_cells[current_pos.row][current_pos.col] = true;

      const int current_state =
          to_state(current_pos.row, current_pos.col, field_cols);
      const enum StateType current_statetype =
          field[current_pos.row][current_pos.col];

      int do_random = randint(1, 100);
      enum Action action =
          do_random <= random_chance
              ? randint(0, qtable_cols - 1)
              : best_action(current_state, qtable_cols, qtable);

      struct Point new_pos;
      move_point(&current_pos, &MOVEMENTS[action], field_rows, field_cols,
                 &new_pos);

      (*robot_ptr).position = new_pos;

      const int new_state = to_state(new_pos.row, new_pos.col, field_cols);
      const enum StateType new_statetype = field[new_pos.row][new_pos.col];

      bool stuck = remaining_steps == 0;
      int reward = get_reward(new_statetype,
                              passed_cells[new_pos.row][new_pos.col], stuck);
      update_qtable(current_state, action, reward, new_state, qtable_cols,
                    qtable);

      exit = stuck || new_statetype == END || new_statetype == MINE;

      show_frame(current_state, qtable_cols, qtable, field_rows, field_cols,
                 field, PINS, &field[current_pos.row][current_pos.col],
                 robot_ptr);
    }

    chance_variable *= CHANCE_REDUCER;
  }

  printf("\nFinished training\n\n");
  printf("Press enter to continue...");
  getchar();
}

void run_game(const int qtable_cols, const double (*const qtable)[qtable_cols],
              const int field_rows, const int field_cols,
              const enum StateType (*const field)[field_cols],
              const wchar_t *const PINS, struct Robot *const robot_ptr,
              const struct Point *const MOVEMENTS) {
  bool victory = false, exit = false;
  long long moves = 0;

  while (!exit) {
    moves++;
    struct Point current_pos = (*robot_ptr).position;
    const int current_state =
        to_state(current_pos.row, current_pos.col, field_cols);

    enum Action action = best_action(current_state, qtable_cols, qtable);

    struct Point new_pos;
    move_point(&current_pos, &MOVEMENTS[action], field_rows, field_cols,
               &new_pos);

    (*robot_ptr).position = new_pos;
    const enum StateType new_statetype = field[new_pos.row][new_pos.col];

    victory = new_statetype == END;
    exit = victory || new_statetype == MINE;
    clearConsole();
    printf("%sMoves: %llu%s\n\n", BOLD, moves, RESET);
    show_frame(current_state, qtable_cols, qtable, field_rows, field_cols,
               field, PINS, &field[current_pos.row][current_pos.col],
               robot_ptr);
    wait(0.5);
  }

  printf("\n%s%s%s\n", BOLD, victory ? "YOU WON" : "YOU LOST", RESET);
  printf("Finished in %llu moves\n\n", moves);
  printf("Press enter to continue...");
  getchar();
}

int main() {
  init();

  int field_rows = 0, field_cols = 0;
  do {
    printf("Enter number of rows [min: 3]: ");
    scanf("%d", &field_rows);
    fflush_stdin();
  } while (field_rows < 3);

  do {
    printf("Enter number of columns [min: 3]: ");
    scanf("%d", &field_cols);
    fflush_stdin();
  } while (field_cols < 3);

  enum StateType field[field_rows][field_cols];
  const wchar_t PINS[] = {START_PIN, BLANK_PIN, POWER_PIN, MINE_PIN, END_PIN};

  const struct Point MOVEMENTS[] = {
      {-1, 0}, // UP
      {0, 1},  // RIGHT
      {1, 0},  // DOWN
      {0, -1}, // LEFT
  };

  struct Robot robot = {
      .position = {0, 0},
      .PIN = ROBOT_PIN,
  };

  char yesno = '\0';
  do {
    clearConsole();
    setup_field(field_rows, field_cols, field, &robot);
    print_field(field_rows, field_cols, field, PINS, NULL, &robot);

    do {
      printf("Use this field? [y/n]: ");
      yesno = getchar();
      fflush_stdin();
    } while (yesno != 'y' && yesno != 'n');
  } while (yesno != 'y');

  bool exit = false;

  const int QTABLE_ROWS = field_rows * field_cols;
  const int QTABLE_COLS = N_ACTIONS;
  double qtable[QTABLE_ROWS][QTABLE_COLS];
  for (double *cell = &qtable[0][0];
       cell <= &qtable[QTABLE_ROWS - 1][QTABLE_COLS - 1]; cell++) {
    *cell = 0;
  }

  const struct Point START_POS = robot.position;
  while (!exit) {
    clearConsole();
    robot.position = START_POS;
    const int current_state =
        to_state(robot.position.row, robot.position.col, field_cols);

    show_frame(current_state, QTABLE_COLS, qtable, field_rows, field_cols,
               field, PINS, NULL, &robot);

    puts("1) Train Q-Table\n2) Run on current\n3) Exit\n");

    int choice = 0;
    do {
      printf("Select an option: ");
      scanf("%d", &choice);
      fflush_stdin();
    } while (choice < 1 && choice > 3);

    long long iterations = 0;
    bool run = false;
    switch (choice) {
    case 1:
      do {
        printf("Insert number of iterations: ");
        scanf("%llu", &iterations);
        fflush_stdin();
      } while (iterations < 0);
      break;
    case 2:
      run = true;
      break;
    case 3:
      exit = true;
      break;
    }

    if (exit) {
      break;
    }

    if (run) {
      run_game(QTABLE_COLS, qtable, field_rows, field_cols, field, PINS, &robot,
               MOVEMENTS);
    } else {
      train_qtable(iterations, QTABLE_COLS, qtable, field_rows, field_cols,
                   field, PINS, &robot, MOVEMENTS);
    }
  }
}