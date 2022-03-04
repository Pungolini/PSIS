#include "board_library.h"
#include "UI_library.h"
#include <unistd.h>
#include <time.h>




int main()
{

	SDL_Event event;
	play_response resp;
	short done = 0;

	srand(time(NULL));

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		 printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		 exit(-1);
	}
	if(TTF_Init()==-1) {
			printf("TTF_Init: %s\n", TTF_GetError());
			exit(2);
	}


	create_board_window(WIDTH,HEIGHT,  BOARD_DIM);
	init_board(BOARD_DIM);

	while(!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					done = SDL_TRUE;
					break;
				}
			}
		}
		botPlay(BOARD_DIM,BOARD_DIM, &resp);
		printf("resp.code = %d\n",resp.code );
		sleep(1);

		switch (resp.code)
		{
			case 1:
				paint_card(resp.play1[0], resp.play1[1] , colors[CYAN].r, colors[CYAN].g, colors[CYAN].b);
				write_card(resp.play1[0], resp.play1[1], resp.str_play1, colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
				break;

			case 3:
			  done = 1;
			  break;

			case 2:
				paint_card(resp.play1[0], resp.play1[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
				write_card(resp.play1[0], resp.play1[1], resp.str_play1, colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);

				paint_card(resp.play2[0], resp.play2[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
				write_card(resp.play2[0], resp.play2[1], resp.str_play2, colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
				break;

			case -2:
				paint_card(resp.play1[0], resp.play1[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
				write_card(resp.play1[0], resp.play1[1], resp.str_play1, colors[RED].r, colors[RED].g, colors[RED].b);

				paint_card(resp.play2[0], resp.play2[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
				write_card(resp.play2[0], resp.play2[1], resp.str_play2, colors[RED].r, colors[RED].g, colors[RED].b);

				SDL_Delay(DELAY);

				paint_default(resp.play1[0], resp.play1[1]);
				paint_default(resp.play2[0], resp.play2[1]);
				break;
		}
	}
/*
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					done = SDL_TRUE;
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					int board_x, board_y;
					get_board_card(event.button.x, event.button.y, &board_x, &board_y);

					printf("click (%d %d) -> (%d %d)\n", event.button.x, event.button.y, board_x, board_y);
					botPlay(DIM,DIM, &resp);
					switch (resp.code)
					{
						case 1:
							paint_card(resp.play1[0], resp.play1[1] , colors[CYAN].r, colors[CYAN].g, colors[CYAN].b);
							write_card(resp.play1[0], resp.play1[1], resp.str_play1, colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
							break;

						case 3:
						  done = 1;

						case 2:
							paint_card(resp.play1[0], resp.play1[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
							write_card(resp.play1[0], resp.play1[1], resp.str_play1, colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);

							paint_card(resp.play2[0], resp.play2[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
							write_card(resp.play2[0], resp.play2[1], resp.str_play2, colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
							break;

						case -2:
							paint_card(resp.play1[0], resp.play1[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
							write_card(resp.play1[0], resp.play1[1], resp.str_play1, colors[RED].r, colors[RED].g, colors[RED].b);

							paint_card(resp.play2[0], resp.play2[1] , colors[WHITE].r, colors[WHITE].g, colors[WHITE].b);
							write_card(resp.play2[0], resp.play2[1], resp.str_play2, colors[RED].r, colors[RED].g, colors[RED].b);

							SDL_Delay(DELAY);

							paint_default(resp.play1[0], resp.play1[1]);
							paint_default(resp.play2[0], resp.play2[1]);
							break;
					}
				}
			}
		}
	}*/
	puts("GAME OVER!\n");
	close_board_windows();
}
