#include "game_private.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

// fonction auxiliaire récursif

/**void gengame(game g, unsigned long *count , int len , int pos_r , int pos_c){
    if(game_solve(g)){
        (*count)++;
        return;
    }
    if(pos_r > game_nb_rows(g) || pos_c > game_nb_cols(g)){
        return;
    }
    game_play_move(g,pos_r,pos_c,S_ONE);
    gengame(g,count,len,pos_r,pos_c+1);

    game_play_move(g,pos_r,pos_c,S_ONE);
    gengame(g,count,len,pos_r+1,pos_c);
}
**/
// Le main

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "nombre d'arguments incorrect");
    exit(EXIT_FAILURE);
  }
  game g = game_load(argv[2]);
  cgame cg = g;
  int valeur = strcmp(argv[1], "-s");
  if (valeur == 0) {
    if (!game_solve(g)) {
      fprintf(stderr, "jeu sans solution");
      return EXIT_FAILURE;
    }
    cgame c_solve = g;
    if (argc == 4) {
      game_save(c_solve, argv[4]);
    } else {
      game_print(c_solve);
    }
    return EXIT_SUCCESS;
  }
  valeur = strcmp(argv[1], "-c");
  if (valeur == 0) {
    uint nb_solution = game_nb_solutions(cg);
    if (argc == 4) {
      FILE* f = fopen(argv[4], "w");
      if (f == NULL) {
        fprintf(stderr, "fichier invalide");
        exit(EXIT_FAILURE);
      }
      fprintf(f, "%u\n", nb_solution);
      if (fclose(f) != 0) {
        fprintf(stderr, "erreur dans la fermeture du fichier");
        exit(EXIT_FAILURE);
      }
    } else {
      printf("%u\n", nb_solution);
    }
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
