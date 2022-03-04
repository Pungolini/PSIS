
#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define NCOLORS 8
#define DEFAULT 1
#define WIDTH 600
#define HEIGHT 600

typedef enum cores
{
	BLACK = 0,
	WHITE = 1,
	RED = 2,
	GRAY = 3,
	GREEN = 4,
	CYAN = 5,
	LIGHT_BLUE = 6,
	BLUE = 7,
	BG = GRAY
}COLOR;


extern short BOARD_DIM;
extern SDL_Color  colors[NCOLORS];

//Estrutura que é enviada pelo client com a jogada
typedef struct _pt
{
  short row,col;
}PT;

// Estrutura com a descricao de uma carta
typedef struct card
{
  int clear;//se for para dar clear
  int gameOver;
  int numOfplayers;
  int lettersColor;//1 preto
                //2 cinzentas
                //3 vermelhas
  PT position;
  SDL_Color bgColor;
  char text[3];
}card;

typedef struct auxData
{
    int numOfPlayers;
    int pairsInBoard;
}auxData;

void clear_card(card toPaint);
void paintPlay(card toPaint );
void paint_card(card toPaint);
void paint_default(card toPaint);
void write_card(card toPaint,char *text, SDL_Color bg );
void get_board_card(short mouse_x, short mouse_y, short * board_x, short *board_y);
void create_board_window(int width, int height,  int dim,int flag);
void close_board_windows();

