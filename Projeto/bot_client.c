#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "UI_library.h"
#include "board_library.h"
#include "playerManagement.h"

#define TIMEOUT 3000
#define SERVER_PORT 3100


short** plays;


int sock_fd;
short NPLAYERS = 0;
short BOARD_DIM = 0;

void * sendThread (void *sock)
{
  short done = 0;
  int sock_fd = (intptr_t)sock;
  PT playCoord;
  SDL_Event event;
  printf("Send Thread\n");


	while( !done)
  	{
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
          done = SDL_TRUE;
          close (sock_fd);
          exit(1);
          break;
        }
	  }
	}
		playCoord.row = rand()%BOARD_DIM;
		playCoord.col = rand()%BOARD_DIM;

		//Envia a estrutura deste jogador para o cliente
		if((send(sock_fd, &playCoord, sizeof(struct _pt), 0) > 0));

		SDL_Delay(1000);
	}

  return NULL;
}



void* receiveThread (void *arg)
{
  playerStruct *player = (playerStruct*)arg;
  //Retira o fd e a cor da estrutura de player
  int sock_fd = player->player_fd;
  
  card jogada;
  
  while(1)
  {

    //Recebe a resposta a jogada
    if((recv(sock_fd, &jogada, sizeof(jogada), 0) > 0))
    {

      if (jogada.gameOver)
      {
        SDL_Delay(2000);
      }

      // Pintar carta recebida
    }
  }

  return 0;
}

void makeConnection(char *address,struct sockaddr_in *server_addr)
{

    //cria a socket


  //Verifica a socket
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket: ");
    exit(-1);
  }

  server_addr->sin_family = AF_INET;
  server_addr->sin_port= htons(SERVER_PORT);

  if(inet_aton(address, &(server_addr->sin_addr)) <= 0)
  {
        puts("\nInvalid address/ Address not supported \n");
        exit (-1);
  }


  //Conecta com o server
  if(connect(sock_fd,(const struct sockaddr *) server_addr,sizeof(*server_addr)) == -1)
  {
        puts("Can't connect to the server\n");
        exit(-1);
  }

  puts("Connected to the server\n");
}




int main(int argc, char *argv[])
{
  struct sockaddr_in server_addr;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();



  playerStruct player;
  playerStruct *playerPtr;
  //threads
  pthread_t sendThreadId;
  pthread_t receiveThreadId;

  //verifica os argumentps
  if (argc < 2)
  {
    printf("second argument should be server address\n");
    exit(-1);
  }



  // Estabelecer ligacao com o servidor
  makeConnection(argv[1],&server_addr);
  puts("Connection established!");


  //Iniciar o player antes de o ler
  memset(&player, -1 , sizeof(player));

  //Le do sever o player number
  if(recv(sock_fd, &player, sizeof(struct _playerStruct), 0) <= 0)
    exit(-1);

  player.player_fd = sock_fd;
  BOARD_DIM = player.boardDim;

  //Faz do playerPtr um ponteiro para a estrutura de jogador
  playerPtr = &player;




    //Cria o board
  
    //create_board_window(WIDTH,HEIGHT, player.boardDim,1);
    // Criar threads de envio e rececao
    pthread_create(&sendThreadId, NULL, sendThread, (void*)(intptr_t)sock_fd);

    pthread_create(&receiveThreadId, NULL, receiveThread, (void*)playerPtr);



  // Fechar tudo
  pthread_join(sendThreadId, NULL);
  pthread_join(receiveThreadId, NULL);

  close(sock_fd);
}
