#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_tools.h"
#include "game_ext.h"

#define SE S_EMPTY
#define S0 S_ZERO
#define S1 S_ONE
#define SI0 S_IMMUTABLE_ZERO
#define SI1 S_IMMUTABLE_ONE

typedef struct game_s {
  uint length;
  uint width;
  square initial_state[DEFAULT_SIZE * DEFAULT_SIZE];
  square actual_state[DEFAULT_SIZE * DEFAULT_SIZE];
  square copy_state[DEFAULT_SIZE * DEFAULT_SIZE];
} game_s;

static square grid_4x4[] = {
    SE, SE, SE, SE,  /* row 0 */
    SE, SE, SE, SE,  /* row 1 */
    SE, SE, SE, SE,  /* row 2 */
    SE, SE, SE, SE,  /* row 3 */
};


static square grid_4x4_2[] = {
    SI1, SI0, SE, SE,  /* row 0 */
    SI0, SI1, SE, SE,  /* row 1 */
    SI1, SI1, SE, SE,  /* row 2 */
    SI0, SI0, SE, SE,  /* row 3 */
};


static square grid_4x4_Sless[] = {
    SI1, SI0, SE, SE,  /* row 0 */
    SI0, SI1, SE, SE,  /* row 1 */
    SI1, SI1, SE, SE,  /* row 2 */
    SI0, SI0, SI0, SE,  /* row 3 */
};

static square grid_4x4_Sless_Unique[] = {
    SI0, SI0, SE, SE,  /* row 0 */
    SI1, SI1, SE, SE,  /* row 1 */
    SI1, SI1, SE, SE,  /* row 2 */
    SI0, SI0, SE, SE,  /* row 3 */
};

static square grid_default_4x4[] = {
    S_IMMUTABLE_ZERO, S_IMMUTABLE_ONE,  S_EMPTY, S_EMPTY, /* row 0 */
    S_IMMUTABLE_ONE,  S_IMMUTABLE_ZERO, S_EMPTY, S_EMPTY, /* row 1 */
    S_IMMUTABLE_ZERO, S_IMMUTABLE_ZERO, S_EMPTY, S_EMPTY, /* row 2 */
    S_IMMUTABLE_ONE,  S_IMMUTABLE_ONE,  S_EMPTY, S_EMPTY, /* row 3 */
};

static square grid_empty_4x4[] = {
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, /* row 0 */
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, /* row 1 */
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, /* row 2 */
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, /* row 3 */
};

static square grid_4x4_empty[] = {
    SE, SE, SE, SE, /* row 0 */
    SE, SE, SE, SE, /* row 1 */
    SE, SE, SE, SE, /* row 2 */
    SE, SE, SE, SE, /* row 3 */
};

bool test_dummy(int argc, char* argv[]) {
  if (argc != 2) {
    return false;
  }
  if (strcmp("dummy", argv[1]) != 0) {
    return false;
  }
  return true;
}

// test game_default_solution

bool test_game_default_solution(void) {
  bool tt = true;
  game g = game_default_solution();
  if (game_is_over(g) != true) {
    tt = false;
  }
  game_delete(g);
  return tt;
}

// test game_new_empty

bool test_game_new_empty() {
  game g = game_new_empty();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (!game_is_empty(g, i, j)) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

// test game_new

bool test_game_new() {
  int n = DEFAULT_SIZE * DEFAULT_SIZE;
  square* squares = malloc(n * sizeof(square));
  for (int i = 0; i < n; i++) {
    squares[i] = S_EMPTY;
  }
  squares[1] = S_IMMUTABLE_ZERO;
  squares[35] = S_IMMUTABLE_ONE;
  game g = game_new(squares);
  square immutable_zero = game_get_square(g, 0, 1);
  square immutable_one = game_get_square(g, 5, 5);
  square empty = game_get_square(g, 0, 0);
  if (immutable_zero == S_IMMUTABLE_ZERO && immutable_one == S_IMMUTABLE_ONE && empty == S_EMPTY) {
    free(squares);
    game_delete(g);
    return true;
  }
  free(squares);
  game_delete(g);
  return false;
}

// test_game_set_square

bool test_game_set_square(void) {
  bool tt = false;
  game g = game_default();
  game g1 = game_default();
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(g, i, j, S_EMPTY);
    }
  }
  if (game_equal(g, g1) != true) {
    tt = true;
  }
  game g2 = game_default();
  game g3 = game_default();
  for (uint a = 0; a < DEFAULT_SIZE; a++) {
    for (uint b = 0; b < DEFAULT_SIZE; b++) {
      game_set_square(g2, a, b, S_EMPTY);
      game_set_square(g3, a, b, S_EMPTY);
    }
  }
  if (game_equal(g2, g3) == true) {
    tt = true;
  }
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return tt;
}

// test_game_get_square

bool test_game_get_square() {
  game g = game_default();
  game_play_move(g, 0, 4, S_ONE);
  game_play_move(g, 0, 5, S_ZERO);
  square empty = game_get_square(g, 0, 0);
  square zero = game_get_square(g, 0, 5);
  square one = game_get_square(g, 0, 4);
  square immutable_zero = game_get_square(g, 0, 2);
  square immutable_one = game_get_square(g, 0, 1);
  if (empty == S_EMPTY && zero == S_ZERO && one == S_ONE && immutable_zero == S_IMMUTABLE_ZERO &&
      immutable_one == S_IMMUTABLE_ONE) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

// test_game_get_next_square

bool test_game_get_next_square(void) {
  game g = game_default();
  game g1 = game_default();
  game_set_square(g, 0, 2, S_ZERO);
  game_set_square(g1, 0, 5, S_IMMUTABLE_ONE);
  bool t = false;
  bool t1 = false;
  if (game_get_next_number(g, 0, 0, RIGHT, 1) == S_ZERO || game_get_next_number(g, 0, 2, LEFT, 1) == S_ONE ||
      game_get_next_number(g, 1, 4, UP, 1) == S_EMPTY) {
    t = true;
  }
  if (game_get_next_number(g1, 0, 0, DOWN, 1) != S_EMPTY || game_get_next_number(g1, 0, 1, RIGHT, 1) != S_EMPTY ||
      game_get_next_number(g1, 0, 4, RIGHT, 1) != S_ZERO) {
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

// test_game_check_move

bool test_game_check_move(void) {
  bool tt = false;
  game g = game_default();
  if (game_check_move(g, 5, 0, S_ZERO)) {
    tt = true;
  }
  game g1 = game_default();
  game_set_square(g1, 5, 0, S_IMMUTABLE_ZERO);
  if (game_check_move(g1, 5, 0, S_ZERO) != true) {
    tt = true;
  }
  game_delete(g);
  game_delete(g1);
  return tt;
}

// test_game_play_move

bool test_game_play_move(void) {
  game g = game_default();
  game g1 = game_default();
  game_play_move(g, 0, 0, S_ONE);
  game_set_square(g1, 0, 0, S_ONE);
  if (game_equal(g, g1) == false) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_play_move(g, 0, 0, S_ZERO);
  game_set_square(g1, 0, 0, S_ZERO);
  if (game_equal(g, g1) == false) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_play_move(g, 0, 0, S_EMPTY);
  game_set_square(g1, 0, 0, S_EMPTY);
  if (game_equal(g, g1) == false) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

// test_game_is_immutable

bool test_game_is_immutable(void) {
  bool tt = false;
  game g = game_default();
  game_set_square(g, 5, 0, S_ZERO);
  if (game_is_immutable(g, 5, 0) == false) {
    tt = true;
  }
  game g1 = game_default();
  game_set_square(g1, 5, 0, S_IMMUTABLE_ZERO);
  if (game_is_immutable(g1, 5, 0) == true) {
    tt = true;
  }
  game_delete(g);
  game_delete(g1);
  return tt;
}

/** test TD11 : solveur **/

bool test_game_nb_solutions(void) {
  bool tt = false;
  game g = game_new_ext(4, 4, grid_4x4_empty, false, true);
  cgame cg = g;
  if (game_nb_solutions(cg) == 72) {
    tt = true;
  }
  game g1 = game_new_ext(4, 4, grid_4x4_empty, false, false);
  cgame cg1 = g1;
  if (game_nb_solutions(cg1) == 90) {
    tt = true;
  }
  game g2 = game_load("4x4_1");
  cgame cg2 = g2;
  if(game_nb_solutions(cg2)==1){
    tt = true;
  }
  game g3 = game_load("4x4_2");
  cgame cg3 = g3;
  if(game_nb_solutions(cg3)==2){
    tt = true;
  }
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return tt;
}

bool test_game_solve_4x4(void) {
  game g = game_new_ext(4, 4, grid_empty_4x4, false, false);
  if (game_solve(g)) {
    return true;
  }
  game g1 = game_new_ext(4, 4, grid_default_4x4, false, false);
  if (game_solve(g1)) {
    return true;
  }
  return false;
}

bool test_game_solve(void){
  game g= game_new_ext(4, 4,grid_4x4, false, true);
  if(game_solve(g)!=true) return false;
  g= game_new_ext(4, 4,grid_4x4, false, false);
  if(game_solve(g)!=true) return false;
  g= game_new_ext(2, 2,grid_4x4_2, false, false);
  if(game_solve(g)!=true) return false;
  g= game_new_ext(2, 2,grid_4x4_2, false, true);
  if(game_solve(g)!=true) return false;
  g= game_new_ext(2, 2,grid_4x4_2, true, true);
  if(game_solve(g)!=true) return false;
  g= game_default();
  if(game_solve(g)!=true) return false;
  g=game_new_ext(4, 4,grid_4x4_Sless, false, false);
  if(game_solve(g)!=false) return false;
  g=game_new_ext(4, 4,grid_4x4_Sless_Unique, false, false);
  if(game_solve(g)!=true) return false;
  g=game_new_ext(4, 4,grid_4x4_Sless_Unique, false, true);
  if(game_solve(g)!=false) return false;
  return true;
}


int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Erreur d'arguments\n");
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("test_game_default_solution", argv[1]) == 0) {
    ok = test_game_default_solution();
  } else if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy(argc, argv);
  } else if (strcmp("test_game_new_empty", argv[1]) == 0) {
    ok = test_game_new_empty();
  } else if (strcmp("test_game_new", argv[1]) == 0) {
    ok = test_game_new();
  } else if (strcmp("test_game_set_square", argv[1]) == 0) {
    ok = test_game_set_square();
  } else if (strcmp("test_game_get_square", argv[1]) == 0) {
    ok = test_game_get_square();
  } else if (strcmp("test_game_get_next_square", argv[1]) == 0) {
    ok = test_game_get_next_square();
  } else if (strcmp("test_game_check_move", argv[1]) == 0) {
    ok = test_game_check_move();
  } else if (strcmp("test_game_play_move", argv[1]) == 0) {
    ok = test_game_play_move();
  } else if (strcmp("test_game_is_immutable", argv[1]) == 0) {
    ok = test_game_is_immutable();
  } else if (strcmp("test_game_nb_solutions", argv[1]) == 0) {
    ok = test_game_nb_solutions();
  } else if (strcmp("test_game_solve", argv[1]) == 0) {
    ok = test_game_solve();
  } else if (strcmp("test_game_solve_4x4", argv[1]) == 0) {
    ok = test_game_solve_4x4();
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