#include "playerManagement.h"



/*
Inicializa a estrutura de um novo jogador
player_fd: return do accept deste novo jogador
*/
playerStruct initializeNewPlayer(int player_fd, int playerNumber, int boardDim)
{
  playerStruct newPlayer;

  newPlayer.score = 0;
  newPlayer.boardDim = boardDim;
  newPlayer.player_fd = player_fd;
  newPlayer.playerNumber = playerNumber;

  //Atribuir uma cor ao jogador
  newPlayer.color.r = playerNumber + (rand()%(255 - playerNumber));
  newPlayer.color.g = rand() % ((240 - 15) + 15);
  newPlayer.color.b = rand() % ((150 - 15) + 15);
  //printf("sock_fd = %d\t Color = [%d,%d,%d]",newPlayer.player_fd,newPlayer.color.r,newPlayer.color.g,newPlayer.color.b);


  return newPlayer;
}

/*
* Cria um novo node para a lista de jogadores
*
*newPlayer: é a estação que pretendemos inserir no node da lista
*/
playerNode* newPlayerNode(playerStruct newPlayer)
{
    playerNode *newNode;

    newNode = (playerNode *) calloc (1,sizeof (playerNode));

    if (newNode == NULL)
    {
        printf("Memory was not allocated!\n");
        exit (EXIT_FAILURE);
    }

    newNode->player = newPlayer;
    return newNode;
}

/*
Insere o novo no da lista logo depois da head
head: cabeça da lista
newPlayer: Jogador que esta a ser adicionado a lista
return: no adicionado à lista
*/
playerNode *insertListTail (playerNode *head, playerStruct newPlayer)
{
    playerNode *aux, *newNode = NULL;

    newNode = newPlayerNode(newPlayer);

    //lista está vazia, retorna o newNode como head da lista
    if (head == NULL)
        return newNode;


    //percorrer todos os nos da lista ate chegar ao ultimo
    aux = head;

    while (aux->next != NULL)
        aux = aux->next;

    //inserir o newNode na lista
    aux->next = newNode;
    newNode->prev = aux;

    
    //retornar o no adicionado
    return newNode;
}


/*
Procura na lista por um determinado player
playerNumber: numero do player que queremos
head: cabeça da lista
return: retorna o player encontrado
*/
playerStruct searchList (playerNode *head, int playerNumber)
{
  playerNode *aux = head;
  playerStruct player;

  //Ate ao inicio da lista
  while (aux->prev != NULL)
    aux = aux->prev;

  //Percorre toda a lista
  while(aux->next != NULL)
  {
    //se encontrou
    if (aux->player.playerNumber == playerNumber)
    {
      player = aux->player;
      break;

    }
      aux = aux->next;
  }

  return player;
}

/*
Faz free a toda a lista de jogadores
head: cabeça da lista
*/
void freePLayerList(playerNode *head)
{
    playerNode *curr = NULL, *tmp = NULL;

    //se a lista está vazia sai da função
    if (head == NULL)
        return;

    curr = head;
    head->prev = NULL;


    //até ao fim da lista para apagar da cauda para traz
    while (curr->next != NULL)
        curr = curr->next;


    while (curr->prev != NULL)
    {
        tmp = curr;
        curr = curr->prev;
        curr->next = NULL;
        free(tmp);
    }
    free(head);
}

/*
Percorre a lista de jogadores e envia um play_code para todos
args:
  ->nodeJogador: cabeca da lista de jogadores
  ->jogada: play_code que vai enviar a todos os players
return:
*/
void sendPlayAll (playerNode *nodeJogador, card jogada)
{
  playerNode *curr = nodeJogador;
  int sock_fd = 0;


  //Se lista estiver vazia, sair
  if (!nodeJogador)
    return;

  //Ate ao inicio da lista
  while (curr->prev != NULL)
  {
    curr = curr->prev;
  }

  //Se so existir um elemento manda so para esse
  if (curr->next == NULL)
  {
    //atualiza o sock_fd deste jogador
    sock_fd = (*curr).player.player_fd;

    //Envia para este jogador
    if(send(sock_fd, &jogada, sizeof(jogada), 0) <= 0)
      removeClient(nodeJogador);

    return;
  }

  //Vai do inicio ao fim sempre a mandar
  while (curr != NULL)
  {
    //atualiza o sock_fd deste jogador
    sock_fd = (*curr).player.player_fd;

    //Envia para este jogador
    if(send(sock_fd, &jogada, sizeof(jogada), 0) <= 0)
      removeClient(nodeJogador);

    //Avança para o seguinte
    curr = curr->next;
  }

  return;
}

/* Apaga um cliente da lista */
void removeClient(playerNode *curr)
{
  pthread_mutex_lock(&listLock);
  //Se o no for null
  if (curr == NULL)
    return;

  int sock_fd = curr->player.player_fd;

  //printf("remove client\n");



  //Se for a cabeca
  if (curr->prev == NULL && curr->next)
  {
    //printf("remove client HEAD\n");
    curr->next->prev = NULL;
    close(sock_fd);
    free(curr);
  }
  //Se for a tail
  else if (curr->next == NULL && curr->prev)
  {
    //printf("remove TAIL\n");
    curr->prev->next = NULL;
    close(sock_fd);
    free(curr);
  }

  //Caso geral
  else if (curr->next && curr->prev)
  {
    //printf("remove client OUTROS\n");
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    close(sock_fd);
    free(curr);
  }
  NUMPLAYERS--;

  pthread_mutex_unlock(&listLock);
  return;
}

/*
Procura o vencedor na lista de jogadores e informa o
*/
void searchWinner (playerNode *nodeJogador)
{
  playerNode *curr = nodeJogador;
  int bestScore = 0;
  int winner = 1;
  int sock_fd;

  //Se lista estiver vazia, sair
  if (!nodeJogador)
    return;

  //Ate ao inicio da lista
  while (curr->prev != NULL)
  {
    curr = curr->prev;
  }

  //Vai ate ao fim
  while (curr->next != NULL)
  {
    //Se existir um novo bestScore
    if (curr->player.score >= bestScore)
    {
      bestScore = curr->player.score;
      sock_fd = curr->player.player_fd;
    }
    curr = curr->next;
  }

  if (curr->player.score >= bestScore)
  {
    bestScore = curr->player.score;
    sock_fd = curr->player.player_fd;
  }


  //anda para tras a enviar aos jogadores
  while (curr != NULL)
  {
    if (curr->player.score == bestScore)
    {
      winner = 1;
      send(curr->player.player_fd, &winner, sizeof(int), 0);
      send(curr->player.player_fd, &bestScore, sizeof(int), 0);
    }

    //Nao e winner
    else
    {
      winner = 0;
      send(curr->player.player_fd, &winner, sizeof(int), 0);
    }
    curr = curr->prev;
  }

  return;
}



void resetAllScores(playerNode *curr)
{
  playerNode *toLeft = curr, *toRight = curr;
  pthread_mutex_lock(&listLock);
  for(toLeft = curr; toLeft != NULL; toLeft = toLeft->prev)
      toLeft->player.score = 0;

  for(toRight = curr; toRight != NULL; toRight = toRight->next)
      toRight->player.score = 0;

  pthread_mutex_unlock(&listLock);
}
