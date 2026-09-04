#include "game_sdl.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_tools.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

#define FONT "res/arial.ttf"
#define COMIC "res/Comic.ttf"
#define FONTSIZE 72
#define I_ONE "res/one_immutable.png"
#define I_ZERO "res/zero_immutable.png"
#define ZERO "res/zero.png"
#define ONE "res/one.png"

struct Env_t {
  /* PUT YOUR VARIABLES HERE */
  SDL_Texture *title;
  SDL_Texture *vict;
  SDL_Texture *shame;
  game g;
  int min;
  int x_hg, y_hg;
  int x_bd, y_bd;
  bool winner, end;
};
/***********************************************************************************/

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  /* PUT YOUR CODE HERE TO INIT TEXTURES, ...*/
  PRINT("Press ESC to quit.\n");

  // initialisation du jeu
  if (argc > 2) {
    fprintf(stderr, "trop d'arguments");
    exit(EXIT_FAILURE);
  }
  if (argc == 2) {
    env->g = game_load(argv[1]);
  } else {
    env->g = game_default();
  }
  env->winner = false;

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // texte pour le titre
  SDL_Color blanc = {0, 0, 0, 255}; 
  TTF_Font *font = TTF_OpenFont(COMIC, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", COMIC);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
  SDL_Surface *surf = TTF_RenderText_Blended(font, "TAKUZU", blanc);  // blended rendering for ultra nice text
  env->title = SDL_CreateTextureFromSurface(ren, surf);
  surf = TTF_RenderText_Blended(font, "1", blanc);
  TTF_CloseFont(font);

  TTF_Font *font_end = TTF_OpenFont(COMIC, 36);
  if (!font_end) ERROR("TTF_OpenFont: %s\n", COMIC);
  TTF_SetFontStyle(font_end, TTF_STYLE_BOLD);
  surf = TTF_RenderText_Blended(font_end, "Congratulations",blanc);  // blended rendering for ultra nice text
  env->vict = SDL_CreateTextureFromSurface(ren, surf);
  surf = TTF_RenderText_Blended(font_end, "1", blanc);
  SDL_FreeSurface(surf);

  return env;
}

/************************************************************************************/

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */

  /* get current window size */
  int w, h, l1, l2;
  SDL_GetWindowSize(win, &w, &h);

  SDL_Color grid_background = {233, 233, 233, 255}; // Barely white
  SDL_SetRenderDrawColor(ren, grid_background.r, grid_background.g,grid_background.b, grid_background.a);
  SDL_RenderClear(ren);

  int nb_cols = env->g->nb_cols;
  int nb_rows = env->g->nb_rows;

  // Calcule des variable utile
  CalcBord(w, h, nb_rows, nb_cols, env);

  /* render a grille with lines */
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_Rect rect = {env->x_hg, env->y_hg, nb_cols * env->min, nb_rows * env->min};
  SDL_RenderDrawRect(ren, &rect);

  l1 = env->x_hg;
  l2 = env->y_hg;

  // ligne interieur de la grille
  for (int i = 0; i < nb_cols - 1; i++) {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    l1 = l1 + ((env->x_bd - env->x_hg) / nb_cols);
    SDL_RenderDrawLine(ren, l1, env->y_hg, l1, env->y_bd);
  }

  for (int j = 0; j < nb_rows - 1; j++) {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    l2 = l2 + ((env->y_bd - env->y_hg) / nb_rows);
    SDL_RenderDrawLine(ren, env->x_hg, l2, env->x_bd, l2);
  }

  // les coups du jeu

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      SDL_Rect move = {(env->x_hg) + (i * env->min), (env->y_hg) + (j * env->min), env->min, env->min};
      // cas d'erreur
      if (game_has_error(env->g, i, j)) {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); /* couleur rouge */
        SDL_RenderFillRect(ren, &move);
      }
      // coup normal
      move.x = (env->x_hg + 10) + (i * env->min);
      move.y = (env->y_hg + 10) + (j * env->min);
      move.w = env->min - 20;
      move.h = env->min - 20;
      if (SQUARE(env->g, i, j) == S_IMMUTABLE_ONE || SQUARE(env->g, i, j) == S_ONE) {
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255); /* couleur bleu */
        SDL_RenderFillRect(ren, &move);
      }
      if (SQUARE(env->g, i, j) == S_IMMUTABLE_ZERO || SQUARE(env->g, i, j) == S_ZERO) {
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255); /* couleur blanc */
        SDL_RenderFillRect(ren, &move);
      }
    }
  }

  /* render text texture */

  SDL_QueryTexture(env->title, NULL, NULL, &rect.w, &rect.h);
  rect.x = w / 2 - rect.w / 2;
  rect.y = h / 20 - rect.h / 2;
  SDL_RenderCopy(ren, env->title, NULL, &rect);

  // Ecriture en cas de fin de jeu
  if (env->end == true) {
    if (env->winner == true) {
      // victoire
      SDL_QueryTexture(env->vict, NULL, NULL, &rect.w, &rect.h);
      rect.x = w / 2 - rect.w / 2;
      rect.y = h / 2 - rect.h / 2;
      SDL_RenderCopy(ren, env->vict, NULL, &rect);
    } else {
      SDL_QueryTexture(env->shame, NULL, NULL, &rect.w, &rect.h);
      rect.x = w / 2 - rect.w / 2;
      rect.y = h / 2 - rect.h / 2;
      SDL_RenderCopy(ren, env->shame, NULL, &rect);
    }
    SDL_RenderPresent(ren);
    SDL_Delay(5000);
  }

  SDL_RenderPresent(ren);
}

/*****************************************************************************************/

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  /* PUT YOUR CODE HERE TO PROCESS EVENTS*/
  int w, h, move_x, move_y, val;
  SDL_GetWindowSize(win, &w, &h);

  cgame f = env->g;

  if (e->type == SDL_QUIT || env->end == true) {
    return true;
  }

  // victoire
  if (game_is_over(f)) {
    env->winner = true;
    env->end = true;
  }
#ifdef __ANDROID__
  else if (e->type == SDL_FINGERDOWN) {
    // coordonnée du coup
    if (e->tfinger.x >= env->x_hg && e->tfinger.y >= env->y_hg) {
      move_x = ((e->tfinger.x - env->x_hg) / env->min);
      move_y = ((e->tfinger.y - env->y_hg) / env->min);
      if (game_check_move(f, move_x, move_y, S_ZERO)) {
        val = game_get_number(f, move_x, move_y);
        if (val == -1) {
          game_play_move(env->g, move_x, move_y, S_ZERO);
        }
        if (val == 0) {
          game_play_move(env->g, move_x, move_y, S_ONE);
        }
        if (val == 1) {
          game_play_move(env->g, move_x, move_y, S_EMPTY);
        }
      }
    }
  }
  // other events
#else
  if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    // coordonnée du coup
    if (mouse.x >= env->x_hg && mouse.y >= env->y_hg) {
      move_x = ((mouse.x - env->x_hg) / env->min);
      move_y = ((mouse.y - env->y_hg) / env->min);
      if (game_check_move(f, move_x, move_y, S_ZERO)) {
        val = game_get_number(f, move_x, move_y);
        if (val == -1) {
          game_play_move(env->g, move_x, move_y, S_ZERO);
        }
        if (val == 0) {
          game_play_move(env->g, move_x, move_y, S_ONE);
        }
        if (val == 1) {
          game_play_move(env->g, move_x, move_y, S_EMPTY);
        }
      }
    }
  } else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_r:
        game_restart(env->g);
        break;
      case SDLK_z:
        game_redo(env->g);
        break;
      case SDLK_y:
        game_undo(env->g);
        break;
      case SDLK_s:
        game_save(env->g, "save.txt");
        break;
      case SDLK_o:
        game_solve(env->g);
        break;
      case SDLK_q:
        env->end = true;
        break;
      case SDLK_h:
        env->end = true;
        break;
    }
  }

#endif

  return false;
}

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY*/
  SDL_DestroyTexture(env->title);
  SDL_DestroyTexture(env->vict);
  SDL_DestroyTexture(env->shame);
  free(env);
}

void CalcBord(int w, int h, int nb_rows, int nb_cols, Env *env) {
  if ((h / nb_rows + 2) < (w / nb_cols + 2)) {
    env->min = h / (nb_rows + 2);
  } else {
    env->min = w / (nb_cols + 2);
  }

  // coordonnée du coin haut_gauche de la grille
  env->x_hg = (w - (nb_cols * env->min)) / 2;  // distance entre grille et bordure haute de la fenêtre
  env->y_hg = (h - (nb_rows * env->min)) / 2;  // distance entre grille et bordure gauche de la fenêtre
}