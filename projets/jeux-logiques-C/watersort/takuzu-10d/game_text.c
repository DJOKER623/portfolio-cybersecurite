#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(void) {
  char c;
  char str[20];
  char str1[20];
  int z;
  int i, j;
  uint nb_sol;
  printf(
      "Given a grid of squares, you must fill it with zeros and ones. Some squares are initially provided as clues and "
      "are immutable. You must complete the remaining empty squares, according to the following rules.\n");
  printf("*Each row and column must contain the same number of zeros and ones.\n");
  printf("*No row or column may contain three consecutive squares of the same number.\n");
  game g = game_default();
  game g_sol = game_default_solution();
  while (!game_is_over(g)) {
    game_print(g);
    for (int a = 0; a < DEFAULT_SIZE; a++) {
      for (int b = 0; b < DEFAULT_SIZE; b++) {
        z = game_has_error(g, a, b);
        if (z != 0) {
          if (game_get_next_square(g, a, b, UP, 1) == 1 && game_get_next_square(g, a, b, UP, 2) == 1) {
            printf("commande non valide car %d aligné en %d %d\n", z - 1, a, b);
          } else if (game_get_next_square(g, a, b, UP, 1) == 1 && game_get_next_square(g, a, b, DOWN, 1) == 1) {
            printf("commande non valide car %d aligné en %d %d\n", z - 1, a, b);
          } else if (game_get_next_square(g, a, b, DOWN, 1) == 1 && game_get_next_square(g, a, b, DOWN, 2) == 1) {
            printf("commande non valide car %d aligné en %d %d\n", z - 1, a, b);
          } else if (game_get_next_square(g, a, b, LEFT, 1) == 1 && game_get_next_square(g, a, b, LEFT, 2) == 1) {
            printf("commande non valide car %d aligné en %d %d\n", z - 1, a, b);
          } else if (game_get_next_square(g, a, b, LEFT, 1) == 1 && game_get_next_square(g, a, b, RIGHT, 1) == 1) {
            printf("commande non valide car %d aligné en %d %d\n", z - 1, a, b);
          } else if (game_get_next_square(g, a, b, RIGHT, 1) == 1 && game_get_next_square(g, a, b, RIGHT, 2) == 1) {
            printf("commande non valide car %d aligné en %d %d\n", z - 1, a, b);
          }
        }
      }
    }
    printf("Please enter (q,r,w,b,e,z,y,s,l) then the coordinate to fill a square \n");
    scanf("\n%c", &c);
    if (c == 'h') {
      printf("Press q to quit the game\n");
      printf("Press r to restart your game\n");
      printf("Press e to put an empty square\n");
      printf("Press w to put a white square\n");
      printf("Press b to put a black square\n");
      printf("Press z to undo a move\n");
      printf("Press y to redo a move\n");
      printf("Press v to save the game\n");
      printf("Press l to load a saved game\n");
      printf("Press s to solve the game\n");
      printf("Press c to see how many solutions it can have\n");
    } else if (c == 'r') {
      game_restart(g);
    }
    else if ( c == 's'){
      game_solve(g);
    }
    else if ( c == 'c'){
      nb_sol = game_nb_solutions(g);
      printf("there is %u solutions\n",nb_sol);
    }
    if (c == 'v'){
      /*scanf("\n%s", &str);
      */
      fgets(str,19,stdin);
      game_save(g,str);
    }
    if (c == 'l'){
      //printf("Which game slot do you want to load\n");
      //scanf("\n%s", &str1);
      fgets(str1,19,stdin);
      g = game_load(str1);
    }
    if (c == 'z') {
      game_undo(g);
    }
    if (c == 'y') {
      game_redo(g);
    } else if (c == 'q') {
      game_delete(g);
      printf("You quit the game, shame on you\n");
      return EXIT_SUCCESS;
    } else if (c == 'e' || c == 'w' || c == 'b') {
      printf("please enter the coordination that you want to put the square on\n");
      scanf("\n%d %d", &i, &j);
      if (c == 'e') {
        if (game_check_move(g, i, j, S_EMPTY)) {
          game_play_move(g, i, j, S_EMPTY);
        } else {
          printf("this is an immutable square\n");
        }
      } else if (c == 'w') {
        if (game_check_move(g, i, j, S_ZERO)) {
          game_play_move(g, i, j, S_ZERO);
        } else {
          printf("this is an immutable square\n");
        }
      } else if (c == 'b') {
        if (game_check_move(g, i, j, S_ONE)) {
          game_play_move(g, i, j, S_ONE);
        } else {
          printf("this is an immutable square\n");
        }
      }
    }
    if (game_equal(g, g_sol)) {
      game_print(g);
      printf("Congratulations\n");
      break;
    }
  }
  game_delete(g);
  return EXIT_SUCCESS;
}