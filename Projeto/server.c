#include "server.h"

// Tamanho do tabuleiro
short BOARD_DIM = 0;

// Numero de jogadores
short NUMPLAYERS = 0;

// Mutexes
pthread_mutex_t threadLock, listLock;


/*void closeClient ()
{
  printf("SIGNAL CLOSE");
  //close(sock_fd);
  exit(1);
}*/

void* receivePlays(void *arg)
{
  playerNode *nodeJogador = (playerNode*)arg;
  playerStruct player = nodeJogador->player;
  int ret,sock_fd = player.player_fd;
  PT firstPlay,secondPlay;
  card firstCard = {0},secondCard = {0};
  struct pollfd fds;
  fds.fd = sock_fd;
  fds.events = POLLIN;
  card lixo;
  int retFirst = 0;


  while(1)
  {
    retFirst = 0;

    printf("NUMPLAYERS = %d\n",NUMPLAYERS);
    //Recebe a jogada, e a primeira
    if(NUMPLAYERS > 1)
    {
      if(recv(sock_fd, &firstPlay, sizeof(PT), 0) <= 0)
      {
        printf("remove na 1");
        removeClient(nodeJogador);
        return 0;
      }
      else
      {
        //Processar a jogada
        retFirst = fillCard(firstPlay,GRAY,player.color,player.playerNumber,0, &firstCard);

        //Enviar essa primeira carta aos clientes para pintar,se for valida
        if(retFirst != 1)
          sendPlayAll(nodeJogador,firstCard);
        else
          continue;

        // Ficar a espera dos 5 segundos
        if((ret = poll(&fds,1,TIMEOUT5)) > 0)
        {
          if (recv(sock_fd, &secondPlay, sizeof(PT), 0) > 0)
          {
            //Processar a 2a jogada
            fillCard(secondPlay,GRAY,player.color,player.playerNumber,0, &secondCard);


            if(compareCards(nodeJogador,firstCard,secondCard) == INVALIDPLAY)
              continue;


          }
          else
          {
            removeClient(nodeJogador);
            return 0;
          }
        }
        else if (ret == 0) // Se passaram os 5 segundos, virar a carta
        {
          firstCard.clear = 1;
          sendPlayAll(nodeJogador,firstCard);
          continue;
        }
        else // Erro no poll()
        {
          perror("  poll() failed");
          exit(-1);
        }
      }
    }
    else
      recv(sock_fd, &lixo, sizeof(card), 0);


  }
}
/*
Faz a gestão do jogador
*/
void* threadForPlayer(void *arg)
{

  playerNode *argPlayer = (playerNode*)arg;
  playerStruct player = (*argPlayer).player;
  pthread_t receiveThreadId;
  size_t conFlag = 0;


  /*printf("\nThread do player %d\n", player.playerNumber);
  printf("Cor na thread: %d\n", player.colorG);
  printf("PlayerNumber na thread: %d\n", player.playerNumber);
  printf("Player fd na thread: %d\n", player.player_fd);
  printf("Score na thread: %d\n", player.score);*/

  //Envia a estrutura deste jogador para o cliente
  conFlag = send(player.player_fd, &player, sizeof(struct _playerStruct), 0);

  //Se perdeu a ligacao ao cliente
  if (conFlag == -1)
  {
    printf("Player %d disconected", player.playerNumber);
    //Fecha a socket e sai da thread
    close(player.player_fd);
    //usar pthread_exit ou return?
    return 0;
  }

  renderBoard(argPlayer);

  //Criar threads para enviar e receber dados para este player
  pthread_create(&receiveThreadId, NULL, receivePlays, (void*)argPlayer);


  //Join as threads
  pthread_join(receiveThreadId, NULL);

  return 0;
}


int main (int argc, char * argv[])
{


  srand(time(NULL));
  //Endereço para a thread
  struct sockaddr_in local_addr;

  int playerCounter = 0;
  int newPlayer = 0;
  playerNode *head = NULL;
  playerNode *newPlayerNode = NULL;
  playerStruct newPlayerInfo;

  //Vetor de threads
  pthread_t thread_id[MAXPLAYERS];

  //signal(SIGPIPE, SIG_IGN);
  //signal(SIGINT, closeClient);

  if (argc < 2)
  {
    puts("Como compilar: ./server [dimensao]\n");
    exit(-1);
  }
  else if ((argc %2) == 1)
  {
    puts("ATT: [dimensao] tem de ser um numero par!\n");
    exit(-1);
  }

  // Tamanho do tabuleiro
  BOARD_DIM = atoi(argv[1]);

  //Cria a socket do SERVER_PORT
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd == -1)
  {
    perror("socket: ");
    exit(-1);
  }

  int tmp = 1;
  if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(sock_fd)) < 0)
    exit(-1);

  local_addr.sin_family = AF_INET;
  local_addr.sin_port= htons(SERVER_PORT);
  local_addr.sin_addr.s_addr= INADDR_ANY;

  //Faz o bind da socket
  int err = bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));

  if(err == -1)
  {
    perror("bind");
    exit(-1);
  }
  puts(" Socket created and binded! \n");

  init_board(BOARD_DIM);

  //faz o listen
  listen(sock_fd, MAXPLAYERS);

      // Inicializar mutexes
  if ((pthread_mutex_init(&threadLock, NULL) !=0) || (pthread_mutex_init(&threadLock, NULL) !=0))
  { 
      puts("\nMutex init has failed!\n"); 
      exit(-1); 
  }
  //Fica a espera dos clientes
  while(1)
  {
    puts("Waiting for players...\n");

    //Aceita um cliente
    newPlayer = accept(sock_fd, NULL, NULL);

    // Inicializar jogador novo
    newPlayerInfo = initializeNewPlayer(newPlayer, playerCounter, BOARD_DIM);

    //Adiciona o novo player na lista
    newPlayerNode = insertListTail (head, newPlayerInfo);

    //Se for o primeiro coloca o como head
    if (playerCounter == 0)
    {
      head = newPlayerNode;
    }

    printf("\n\nNew client connected (%d)!\n", playerCounter + 1);



    //Cria a thread para este jogador

    pthread_create(&thread_id[playerCounter], NULL, threadForPlayer, (void*)newPlayerNode);


    //Incrementa o contador de jogadores
    NUMPLAYERS = ++playerCounter;
  }
}
