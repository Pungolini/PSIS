#pragma once



#include <semaphore.h>
#include <poll.h>
#include <pthread.h>
#include "playerManagement.h"
//#define _POSIX_C_SOURCE >= 199309L
#include <time.h>

#define TIMEOUT2 2000
#define TIMEOUT5  5000
#define TIMEOUT10  4
#define NANOSEC 1000000000
#define gameIsOver(size)    (PAIRS == ((size*size)/2))


enum {UP = 0, DOWN = -1, LOCKED = 2};
enum {INVALIDPLAY = -1, WRONGPLAY = 0, VALIDPLAY = 1};

typedef struct board_place
{
  char v[3];
  int estado;//-1 livre
            //0 ocupada temporariamente
            //2 descoberta
  int playerNumber; //dono da casa
                    //-1 quando livre
} board_place;




extern board_place * board;
extern short PAIRS;
extern short NUMPLAYERS;
extern pthread_mutex_t listLock;

#include "UI_library.h"

int getBoardState(int , int );
int getBoardCellPlayer(card toPaint);
int cardsAreSame(card first, card second);
int compareCards(playerNode *nodeJogador,card firstCard,card secondCard);



char * get_board_place_str(int , int );



void init_board(int dim);
void renderBoard(playerNode *head);
void setBoardCellState(card toPaint, int value);
void setBoardCellPlayer(card toPaint, int player);
int fillCard(PT coords,int textColor,SDL_Color bgColor,int playerNumber,int clear, card *result);
