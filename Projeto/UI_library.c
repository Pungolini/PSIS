#include "UI_library.h"

int screen_width;
int screen_height;
int n_ronw_cols;
int row_height;
int col_width;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const char cardsFont[] = "font.TTF";

SDL_Color  colors[NCOLORS] = {	{0,0,0},
								{255,255,255},
								{0xF9,0x65,0x74},
								{200,200,200},
								{107,200,100},
								{7,200,100},
								{0x38,0x95,0xD3},
								{0x12,0x61,0xA0}};


// Desenha carta com a o texto
void write_card(card toPaint ,char * text, SDL_Color bg)
{
	SDL_Rect rect;

	rect.x = toPaint.position.row * col_width;
	rect.y = toPaint.position.col * row_height;
	rect.w = col_width + 1;
	rect.h = row_height + 1;


	TTF_Font * font = TTF_OpenFont(cardsFont, row_height);

	SDL_Color color = colors[toPaint.lettersColor];
 	SDL_Surface * surface = TTF_RenderText_Shaded(font, toPaint.text, color, bg);

	SDL_Texture* Background_Tx = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface); /* we got the texture now -> free surface */


	SDL_RenderCopy(renderer, Background_Tx, NULL, &rect);

	// Outline preto
	SDL_SetRenderDrawColor(renderer, colors[BLACK].r,colors[BLACK].g,colors[BLACK].b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &rect);
}

// Pinta o background da carta
void paint_card(card toPaint)
{
	SDL_Rect rect;
	SDL_Color color = colors[toPaint.lettersColor];

	rect.x = toPaint.position.row * col_width;
	rect.y = toPaint.position.col * row_height;
	rect.w = col_width + 1;
	rect.h = row_height + 1;

	// Rect cinzento

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);

	// Outline preto
	SDL_SetRenderDrawColor(renderer, colors[BLACK].r,colors[BLACK].g,colors[BLACK].b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &rect);

}

void paint_default(card toPaint)
{
	SDL_Rect rect;

	rect.x = toPaint.position.row * col_width;
	rect.y = toPaint.position.col * row_height;
	rect.w = col_width + 1;
	rect.h = row_height + 1;

	SDL_Rect rect2 = {rect.x + 0.1*rect.w,rect.y + 0.1*rect.h,0.8*rect.w,0.8*rect.h};

	// Rect azul claro
	SDL_SetRenderDrawColor(renderer, colors[LIGHT_BLUE].r,colors[LIGHT_BLUE].g,colors[LIGHT_BLUE].b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);

	// Rect azul escuro
	SDL_SetRenderDrawColor(renderer, colors[BLUE].r,colors[BLUE].g,colors[BLUE].b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect2);

	// Outline preto
	SDL_SetRenderDrawColor(renderer, colors[BLACK].r,colors[BLACK].g,colors[BLACK].b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_RenderPresent(renderer);
	SDL_Delay(10);

}

// "Apaga" carta (pinta de preto)
void clear_card(card toPaint)
{
	toPaint.lettersColor = BLACK;
	paint_card(toPaint);
}

// Obtem indices da matriz a que corresponde o clique do rato
void get_board_card(short mouse_x, short mouse_y, short * board_x, short *board_y)
{
	*board_x = mouse_x / col_width;
	*board_y = mouse_y / row_height;
}

// Criar grelha do mapa
void create_board_window(int width, int height,  int dim,int flag)
{
	card myCard;
	screen_width = width;
	screen_height = height;
	n_ronw_cols = dim;
	row_height = height /n_ronw_cols;
	col_width = width /n_ronw_cols;
	screen_width = n_ronw_cols * col_width + 1;
	screen_height = n_ronw_cols *row_height+ 1;

	if (flag && (SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, &renderer)  != 0))
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		exit(-1);
	}

	SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);


	for (unsigned short i = 0; i < n_ronw_cols; ++i)
	{
		for (unsigned short j = 0; j < n_ronw_cols; ++j)
		{
			myCard.position.row = i;
			myCard.position.col = j;
			paint_default(myCard);
		}
	}

	SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, SDL_ALPHA_OPAQUE);

	for (unsigned short i = 0; i <= n_ronw_cols; i++)
		SDL_RenderDrawLine(renderer, 0, i*row_height, screen_width, i*row_height);

	for (unsigned short i = 0; i <= n_ronw_cols; i++)
		SDL_RenderDrawLine(renderer, i*col_width, 0, i*col_width, screen_height);



	SDL_RenderPresent(renderer);
	SDL_Delay(10);
}

void close_board_windows()
{
	if (renderer)
		SDL_DestroyRenderer(renderer);

	if (window)
		SDL_DestroyWindow(window);
}

void printJogada(card jogada)
{
   //showBoard();
  puts("===========================================");
  printf("Card : %s   - [%d %d]\n",jogada.text,jogada.position.row,jogada.position.col);
  puts("===========================================\n");
}

/*
Pinta a jogada consoante o seu resultado
*/
void paintPlay (card toPaint)
{
	
	if (toPaint.clear == 0)
	{
		paint_card(toPaint);
		write_card(toPaint,toPaint.text,toPaint.bgColor);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
		
	}
	else if (toPaint.clear == 1)
	{ // Nao virar a carta
		SDL_Delay(250);
		paint_default(toPaint);
		
		//toPaint.clear = 0;
	}
	
}
