#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
//#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include "UI_library.h"
#include "board_library.h"
#include "playerManagement.h"


#define SERVER_PORT 4000
#define GAMEOVER 3000

SDL_Event event;

extern int errno;
int sock_fd;


void * sendThread (void *sock)
{
  short done = 0;
  int sock_fd = (intptr_t)sock;
  PT playCoord;

  while( !done)
  {
    while (!done && SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
          done = SDL_TRUE;
          close (sock_fd);
          close_board_windows();
          exit(1);
          break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
          // Obter indices da carta escolhida pelo cliente
          get_board_card(event.button.x, event.button.y, &playCoord.row, &playCoord.col);


          //Envia a estrutura deste jogador para o cliente
          if((send(sock_fd, &playCoord, sizeof(struct _pt), 0) <= 0))
              done = SDL_TRUE;

          break;
        }
      }
     }
   }
  return NULL;
}



void* receiveThread (void *arg)
{
  playerStruct *player = (playerStruct*)arg;
  int sock_fd = player->player_fd;
  card jogada;
  
  while(1)
  {

    //Recebe a resposta a jogada
    if((recv(sock_fd, &jogada, sizeof(jogada), 0) > 0))
    {

      paintPlay(jogada);

      if (jogada.gameOver)
      {
        int winner = 0,bestScore = 0;
        puts("\n--------------------GAME OVER!---------------------\n");
        recv(sock_fd, &winner, sizeof(int), 0);

        if (winner == 1)
        {
          
          recv(sock_fd, &bestScore, sizeof(int), 0);
          printf("YOU WON!\tSCORE: %d\n",bestScore);
        }
        SDL_Delay(GAMEOVER);
        create_board_window(WIDTH,HEIGHT,player->boardDim,0);

      }
    }
    else
    {
     close(sock_fd);
     //continue;
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

  if(inet_pton(AF_INET,address, &(server_addr->sin_addr)) <= 0)
  {
    puts("\nInvalid address/ Address not supported \n");
    exit (-1);
  }


  //Conecta com o server
   int ret = connect(sock_fd,(const struct sockaddr *) server_addr,sizeof(struct sockaddr_in));
  if(ret == -1)
  {
    puts("Can't connect to the server\n");
    printf("ret = %d\tsock_fd = %d\n",ret,sock_fd);
    fprintf(stderr, "Error connecting: %s\n", strerror( errno ));
    exit(-1);
  }

  puts("Player connected to the server\n");
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
  printf("ip = %s\n",argv[1]);
  makeConnection(argv[1],&server_addr);
  puts("Connection established!");


  //Iniciar o player antes de o ler
  memset(&player, -1 , sizeof(player));

  //Le do sever o player number
  if(recv(sock_fd, &player, sizeof(struct _playerStruct), 0) <= 0)
    exit(-1);

  player.player_fd = sock_fd;
  

  //Faz do playerPtr um ponteiro para a estrutura de jogador
  playerPtr = &player;




    //Cria o board
  
    create_board_window(WIDTH,HEIGHT, player.boardDim,1);
    // Criar threads de envio e rececao
    pthread_create(&sendThreadId, NULL, sendThread, (void*)(intptr_t)sock_fd);

    pthread_create(&receiveThreadId, NULL, receiveThread, (void*)playerPtr);



  // Fechar tudo
  pthread_join(sendThreadId, NULL);
  pthread_join(receiveThreadId, NULL);

  close(sock_fd);
}
