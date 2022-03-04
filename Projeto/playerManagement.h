//#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>


/*
player_fd: return do accept deste player
color: cor associada a este jogadores
score: numero de casas descobertas
*/

//inicializa o player
#pragma once

#include "UI_library.h"

typedef struct _playerStruct
{
  int player_fd;
  int score;
  int playerNumber;
  int boardDim;
  SDL_Color color;
}playerStruct;

//Node da lista de jogadores
typedef struct nodeP
{
    playerStruct player;
    struct nodeP *next;
    struct nodeP *prev;
}playerNode;

#include "board_library.h"
playerStruct initializeNewPlayer(int,int, int);
//Cria um novo node para a lista
playerNode* newPlayerNode(playerStruct newPlayer);
//Insere o no na lista
playerNode *insertListTail (playerNode *head, playerStruct newPlayer);
//precorre a lista ate encontrar um PlayerNumber
playerStruct searchList (playerNode*, int);
//Faz free a lista
void freePLayerList(playerNode*);
//envia a todos
void sendPlayAll(playerNode*, card);
void imprimeLista (playerNode *);
void resetAllScores(playerNode *curr);
void searchWinner (playerNode *nodeJogador);
void removeClient(playerNode *curr);
