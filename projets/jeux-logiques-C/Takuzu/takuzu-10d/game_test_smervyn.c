#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#define GAME game_default
#define GAME_SOL game_default_solution
#define SE S_EMPTY
#define S0 S_ZERO
#define S1 S_ONE
#define SI0 S_IMMUTABLE_ZERO
#define SI1 S_IMMUTABLE_ONE

typedef struct game_s {
  uint length;
  uint width;
  square actual_state[DEFAULT_SIZE * DEFAULT_SIZE];
} game_s;

static square grid_6x6_default[] = {
    SE, S1, S0, SE, SE, SE, /* row 0 */
    SE, SE, SE, SE, SE, SE, /* row 1 */
    SE, S0, SE, SE, S0, SE, /* row 2 */
    SE, S0, S1, SE, SE, SE, /* row 3 */
    SE, SE, S1, SE, SE, S0, /* row 4 */
    SE, SE, SE, SE, SE, S0 /* row 5 */
};

void usage(int argc, char* argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

static bool test_dummy(int argc, char* argv[]) {
  if (argc != 2) {
    return false;
  }
  if (strcmp("dummy", argv[1]) != 0) {
    return false;
  }
  return true;
}

bool test_game_copy(void) {
  game g = game_default();
  game g1;
  g1 = game_copy(g);
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != game_get_square(g1, i, j)) {
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

bool test_game_equal(void) {
  bool tt = true;
  game g = game_default();
  game g1 = game_copy(g);
  if (game_equal(g, g1) == true) {
    tt = true;
  } else if (game_equal(g, g1) == false) {
    tt = false;
  }
  game g2 = game_default();
  game g3 = game_default();
  game_set_square(g3, 0, 0, S_ONE);
  if (game_equal(g2, g3) == false) {
    tt = true;
  } else if (game_equal(g2, g3) == true) {
    tt = false;
  }
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return tt;
}

bool test_game_delete(void) {
  game g = game_default();
  game_delete(g);
  return true;
}

bool test_game_get_number(void) {
  game g = game_default();
  game g1 = game_default();
  game_set_square(g, 0, 0, S_ONE);
  game_set_square(g, 0, 1, S_ZERO);
  game_set_square(g1, 0, 5, S_IMMUTABLE_ONE);
  bool t = false;
  bool t1 = false;
  if (game_get_number(g, 0, 0) == 1 || game_get_number(g, 0, 1) == 0 || game_get_number(g, 0, 5) == -1) {
    t = true;
  }
  if (game_get_number(g1, 0, 0) != -1 || game_get_number(g, 0, 1) != -1 || game_get_number(g, 0, 5) != 0) {
    t1 = true;
  }
  if ((t && t1) == true) {
    game_delete(g);
    game_delete(g1);
    return true;
  }
  game_delete(g);
  game_delete(g1);
  return false;
}

bool test_game_get_next_number(void) {
  game g = game_default();
  game g1 = game_default();
  game_set_square(g, 0, 2, S_ZERO);
  game_set_square(g1, 0, 5, S_IMMUTABLE_ONE);
  bool t = false;
  bool t1 = false;
  if (game_get_next_number(g, 0, 0, RIGHT, 1) == 0 || game_get_next_number(g, 0, 2, LEFT, 1) == 1 ||
      game_get_next_number(g, 1, 4, UP, 1) == -1) {
    t = true;
  }
  if (game_get_next_number(g1, 0, 0, DOWN, 1) != -1 || game_get_next_number(g1, 0, 1, RIGHT, 1) != -1 ||
      game_get_next_number(g1, 0, 4, RIGHT, 1) != 0) {
    t1 = true;
  }
  if ((t && t1) == true) {
    game_delete(g);
    game_delete(g1);
    return true;
  }
  game_delete(g);
  game_delete(g1);
  return false;
}

bool test_game_is_empty(void) {
  game g = game_new_empty();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_number(g, i, j) != -1) {
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_has_error(void) {
  bool tt = false;
  game g = game_default();
  game_set_square(g, 5, 0, S_ONE);
  game_set_square(g, 5, 1, S_ONE);
  game_set_square(g, 5, 2, S_ONE);
  int a = game_has_error(g, 5, 2);
  if (a > 2) {
    tt = true;
  }
  game g1 = game_default();
  game_set_square(g, 5, 0, S_ZERO);
  if (game_has_error(g, 5, 0) == 0) {
    tt = true;
  }
  game_delete(g);
  game_delete(g1);
  return tt;
}

bool test_game_is_over(void) {
  bool tt = false;
  game g = game_default_solution();
  if (game_is_over(g) == false) {
    tt = false;
  }
  tt = true;
  game_delete(g);
  return tt;
}

bool test_game_restart(void) {
  game g = game_default();
  game g1 = game_default();
  game_restart(g);
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != game_get_square(g1, i, j)) {
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

bool test_game_print(void) {
  game g = game_default();
  game_print(g);
  game_delete(g);
  return true;
}

bool test_game_default(void) {
  game g = game_default();
  if (game_get_square(g, 0, 1) == S_IMMUTABLE_ONE || game_get_square(g, 0, 2) == S_IMMUTABLE_ZERO ||
      game_get_square(g, 2, 1) == S_IMMUTABLE_ZERO) {
    game_delete(g);
    return true;
  }
  return false;
}

//============================== game test V2 ===================================================
bool test_game_new_ext() {
  int n = 8 * 4;
  square* squares = malloc(n * sizeof(square));
  if (squares == NULL) {
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < n; i++) {
    squares[i] = S_EMPTY;
  }
  squares[1] = S_IMMUTABLE_ZERO;
  squares[31] = S_IMMUTABLE_ONE;
  game g = game_new_ext(8, 4, squares, true, true);
  square immutable_zero = game_get_square(g, 0, 1);
  square immutable_one = game_get_square(g, 7, 3);
  square empty = game_get_square(g, 0, 0);
  if (!game_is_wrapping(g) || !game_is_unique(g)) {
    free(squares);
    game_delete(g);
    return false;
  }
  if (immutable_zero == S_IMMUTABLE_ZERO && immutable_one == S_IMMUTABLE_ONE && empty == S_EMPTY) {
    free(squares);
    game_delete(g);
    return true;
  }
  free(squares);
  game_delete(g);
  return false;
}

bool test_game_new_empty_ext() {
  game g = game_new_empty_ext(8, 4, true, true);
  if (!game_is_wrapping(g) || !game_is_unique(g)) {
    game_delete(g);
    return false;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 4; j++) {
      if (game_get_square(g, i, j) != S_EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_nb_rows() {
  game g = game_new_empty_ext(8, 4, true, true);
  if (game_nb_rows(g) == 8) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

bool test_game_nb_cols() {
  game g = game_new_empty_ext(8, 4, true, true);
  if (game_nb_cols(g) == 4) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

bool test_game_is_unique() {
  game g = game_new_empty_ext(8, 4, true, true);
  if (game_is_unique(g)) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

bool test_game_is_wrapping(void) {
  game g = game_new_empty_ext(6, 2, false, false);
  bool ok = true;

  ok = ok && !game_is_wrapping(g);
  game_delete(g);

  g = game_new_empty_ext(12, 24, true, false);
  ok = ok && game_is_wrapping(g);
  game_delete(g);

  g = game_new_empty_ext(2, 2, true, true);
  ok = ok && game_is_wrapping(g);
  game_delete(g);

  g = game_new_empty_ext(8, 6, false, true);
  ok = ok && !game_is_wrapping(g);
  game_delete(g);

  if (ok == true) {
    return true;
  }
  return false;
}

bool test_game_undo() {
  game g = game_new_empty_ext(8, 4, true, true);
  game_play_move(g, 0, 0, S_ZERO);
  game_play_move(g, 1, 0, S_ONE);
  game_undo(g);
  if (game_get_square(g, 1, 0) != S_EMPTY) {
    game_delete(g);
    return false;
  }
  game_undo(g);
  if (game_get_square(g, 0, 0) != S_EMPTY) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_redo() {
  game g = game_new_empty_ext(8, 4, true, true);
  game_play_move(g, 0, 0, S_ZERO);
  game_undo(g);
  game_redo(g);

  if (game_get_square(g, 0, 0) != S_ZERO) {
    game_delete(g);
    return false;
  }
  game_undo(g);
  if (game_get_square(g, 0, 0) != S_EMPTY) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/*
bool game_equal_ext_test(void) {
  game g = game_new_empty_ext(5, 5, true, true);
  game g1 = game_new_empty_ext(3, 3, true, true);
  game g2 = game_new_empty_ext(5, 5, true, true);
  game g3 = game_new_empty_ext(5, 5, true, true);
  if (game_equal(g, g1) == false && game_equal(g2, g3) == true) {
    return true;
  }
  return false;
}

bool game_copy_ext_test(void) {
  game g = game_new_empty_ext(5, 5, true, true);
  game g1 = game_copy(g);
  for (int i = 0; i < g->length; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) != game_get_square(g1, i, j)) {
        return false;
      }
    }
  }
  return true;
}
*/

//===============================================================================================

bool test_game_save_and_load(void) {
  game g = game_default();
  game_save(g, "test");
  game g1 = game_load("test");
  if (!game_equal(g, g1)) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

bool test_game_load(void) {
  game g = game_new_ext(6,6,grid_6x6_default,false,false);
  game g1 = game_load("default");
  if (!game_equal(g, g1)) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Erreur d'arguments\n");
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("test_game_copy", argv[1]) == 0) {
    ok = test_game_copy();
  } else if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy(argc, argv);
  } else if (strcmp("test_game_equal", argv[1]) == 0) {
    ok = test_game_equal();
  } else if (strcmp("test_game_delete", argv[1]) == 0) {
    ok = test_game_delete();
  } else if (strcmp("test_game_get_number", argv[1]) == 0) {
    ok = test_game_get_number();
  } else if (strcmp("test_game_get_next_number", argv[1]) == 0) {
    ok = test_game_get_next_number();
  } else if (strcmp("test_game_is_empty", argv[1]) == 0) {
    ok = test_game_is_empty();
  } else if (strcmp("test_game_has_error", argv[1]) == 0) {
    ok = test_game_has_error();
  } else if (strcmp("test_game_is_over", argv[1]) == 0) {
    ok = test_game_is_over();
  } else if (strcmp("test_game_restart", argv[1]) == 0) {
    ok = test_game_restart();
  } else if (strcmp("test_game_default", argv[1]) == 0) {
    ok = test_game_default();
  } else if (strcmp("test_game_new_ext", argv[1]) == 0) {
    ok = test_game_new_ext();
  } else if (strcmp("test_game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  } else if (strcmp("test_game_nb_rows", argv[1]) == 0) {
    ok = test_game_nb_rows();
  } else if (strcmp("test_game_nb_cols", argv[1]) == 0) {
    ok = test_game_nb_cols();
  } else if (strcmp("test_game_is_unique", argv[1]) == 0) {
    ok = test_game_is_unique();
  } else if (strcmp("test_game_undo", argv[1]) == 0) {
    ok = test_game_undo();
  } else if (strcmp("test_game_redo", argv[1]) == 0) {
    ok = test_game_redo();
  } else if (strcmp("test_game_is_wrapping", argv[1]) == 0) {
    ok = test_game_is_wrapping();
  } else if (strcmp("test_game_print", argv[1]) == 0) {
    ok = test_game_print();
  } else if (strcmp("test_game_save_and_load", argv[1]) == 0) {
    ok = test_game_save_and_load();
  } else if (strcmp("test_game_load", argv[1]) == 0) {
    ok = test_game_load();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: le test est un succès\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: le test est un échec\n", argv[1]);
    return EXIT_FAILURE;
  }
}