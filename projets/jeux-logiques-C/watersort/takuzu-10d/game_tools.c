#include "game_private.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
#include "queue.h"

game game_load(char *filename){
    //char str[255];
    uint au,bu,c,d;
    FILE* f=fopen(filename,"r");
    if(f == NULL){
        fprintf(stderr,"The file does not exist");
        exit(EXIT_FAILURE);
    }
    fscanf(f, "%u %u %u %u \n",&au,&bu,&c,&d);
    bool wra,uni;
    wra = c;
    uni = d;
    game g = game_new_empty_ext(au,bu,wra,uni);
    if (g==NULL){
        fprintf(stderr,"Game problem");
        exit(EXIT_FAILURE);
    }
    char valeur;
    for (uint i = 0; i <= g->nb_rows; i++) {
        for (uint j = 0; j <= g->nb_cols; j++) {
            valeur = fgetc(f);
            if (valeur == 'e') {
                SQUARE(g, i, j) = S_EMPTY;
            }
            if (valeur == 'b') {
                SQUARE(g, i, j) = S_ONE;
            }
            if (valeur == 'w') {
                SQUARE(g, i, j) = S_ZERO;
            }
            if (valeur == 'W') {
                SQUARE(g, i, j) = S_IMMUTABLE_ZERO;
            }
            if (valeur == 'B') {
                SQUARE(g, i, j) = S_IMMUTABLE_ONE;
            }
        }
    }
    /*
    for(int n = 1; n < 100; ++n){
         fread(15, 100, 1, f);
    }
    while(fscanf(f, "%s", str)!=EOF){
        printf("c'est pas bon\n");      
    } 
    while(!feof(f)){
        fgets(str,100,f);
        if ( ferror( f ) ) {
            fprintf( stderr, "Reading error with code \n");
            break;
        }
    }
    **/
   if(fclose(f) != 0){
    fprintf(stderr,"Error while closing the file\n");
    exit(EXIT_FAILURE);
   }
    return g;
    fclose(f);
}

void game_save(cgame g, char *filename){
    FILE* f=fopen(filename,"w");
    if(f == NULL){
        exit(EXIT_FAILURE);
    }
    fprintf(f,"%d ",game_nb_cols(g));
    fprintf(f,"%d ",game_nb_rows(g));
    fprintf(f,"%d ",game_is_wrapping(g));
    fprintf(f,"%d \n",game_is_unique(g));
    for(uint i = 0;i<game_nb_rows(g);i++){
        for(uint j=0;j<game_nb_cols(g);j++){
            if(game_get_square(g,i,j) == S_EMPTY){
                fprintf(f,"e");
            }
            if(game_get_square(g,i,j) == S_IMMUTABLE_ONE){
                fprintf(f,"B");
            }
            if(game_get_square(g,i,j) == S_IMMUTABLE_ZERO){
                fprintf(f,"W");
            }
            if(game_get_square(g,i,j) == S_ONE){
                fprintf(f,"b");
            }
            if(game_get_square(g,i,j) == S_ZERO){
                fprintf(f,"w");
            }
        }
        fprintf(f,"\n");
    }
    fclose(f);
}

/**void game_name_load(int argc, char const *argv[]){
    if(argc > 2){
        fprintf(stderr,"trop d'arguments\n");
        return EXIT_FAILURE;
    }
    if(argc == 1){
        game_load(argv[1]);
    }
}**/

/**
void game_solve_rec(game g, int rows, int cols) {
  if (rows > game_nb_rows(g) || cols > game_nb_cols(g)) {
    return;
  }
  game_play_move(g, rows, cols, S_ONE);
  game_solve_rec(g, rows, cols + 1);

  game_play_move(g, rows, cols, S_ONE);
  game_solve_rec(g, rows + 1, cols);
}
**/

static void game_solve_rec(game g, uint i, uint j, uint* pcount,uint pos, uint len);

/**
bool game_solve (game g){
    if(game_is_over(g)){
        return true;
    }
    return false;
}
**/

bool game_solve(game g) {
    uint longueur = game_nb_cols(g) * game_nb_rows(g);
  uint count = 0;
  for(int i = 0; i< game_nb_rows(g);i++){
    for(int j = 0; j<game_nb_cols(g);j++){
        game_solve_rec(g, 0, 0, &count,0,longueur);
    }
  }
  if (count != 0) {
    return true;
  }
  return false;
}

uint game_nb_solutions(cgame g) {
    uint cpt = 0;
    return cpt;
}

void game_solve_rec(game g, uint rows, uint cols, uint *pcount, uint pos,uint len){
    if(pos == len){
        if(game_is_over(g)){
            (*pcount)++;
            game_print(g);
        }
        return;
    }
    g->squares[pos] = 0;
    game_solve_rec(g,rows,cols,pcount,pos+1,len);

    g->squares[pos] = 1;
    game_solve_rec(g,rows,cols,pcount,pos+1,len);
}


/**
 * uint game_nb_solutions(cgame g){
    game g1 = game_copy(g);
    uint cpt = 0;
    for(int i = 0; i < game_nb_rows(g1);i++){
        for(int j=0; j< game_nb_cols(g1);j++){
            if(game_is_over(g1)){
                game_solve_rec(g1,i,j);
                cpt++;
            }
        }
    }
    return cpt;
}
**/