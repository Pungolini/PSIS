#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board_library.h"

/******* GLOBALS *******/

int play1[2];
int dim_board;
int n_corrects;
board_place * board;
short PAIRS = 0;
/***********************/


int linear_conv(int i, int j)
{
  return j*dim_board+i;
}

char * get_board_place_str(int row, int col)
{
  return board[linear_conv(row, col)].v;
}

void setBoardCellState(card toPaint, int value)
{

  board[linear_conv(toPaint.position.row, toPaint.position.col)].estado = value;
}


void setBoardCellPlayer(card toPaint, int player)
{

  board[linear_conv(toPaint.position.row, toPaint.position.col)].playerNumber = player;
}


int getBoardCellPlayer(card toPaint)
{

  return board[linear_conv(toPaint.position.row, toPaint.position.col)].playerNumber;
}

int getBoardState(int row, int col)
{
  return board[linear_conv(row,col)].estado;
}

int cardsAreSame(card first, card second)
{
  return (first.position.row == second.position.row) &&
        (first.position.col == second.position.col);
}

void init_board(int dim)
{
  int count  = 0;
  int i, j;
  char * str_place;

  dim_board = dim;
  n_corrects = 0;

  board = malloc(sizeof(board_place)* dim *dim);

  // Inicializar board
  for( i = 0; i < (dim_board*dim_board); i++)
  {
    board[i].v[0] = '\0';
    board[i].playerNumber = -1;
    board[i].estado = DOWN;
  }

  for (char c1 = 'A' ; c1 < ('A'+ dim_board ); c1++)
  {
    for (char c2 = 'A' ; c2 < ('A'+ dim_board ); c2++)
    {
      do
      {
          i = rand()% dim_board;
          j = rand()% dim_board;
          str_place = get_board_place_str(i, j);
      }while(str_place[0] != '\0');

      str_place[0] = c1;
      str_place[1] = c2;
      str_place[2] = '\0';

      do
      {
        i = rand()% dim_board;
        j = rand()% dim_board;

        str_place = get_board_place_str(i, j);
      }while(str_place[0] != '\0');

      str_place[0] = c1;
      str_place[1] = c2;
      str_place[2] = '\0';

      count += 2;
      if (count == dim_board*dim_board)
        return;
    }
  }
}

/*
int cardOwned(card toCompare)
{
  for (size_t i = 0; i < dim_board; i++)
  {
    for (size_t j = 0; j < dim_board; j++)
    {
      if(strcmp(toCompare.text))
      printf("[%s|%d]\t",board[linear_conv(j,i)].v,board[linear_conv(j,i)].estado);
    }
    putchar('\n');
  }
}
*/
void showBoard()
{
  for (size_t i = 0; i < dim_board; i++)
  {
    for (size_t j = 0; j < dim_board; j++)
    {
      printf("[%s|%d]\t",board[linear_conv(j,i)].v,board[linear_conv(j,i)].estado);
    }
    putchar('\n');
  }
  puts("\n\n");
}



/* Dada uma posicao do tabuleiro, processa uma jogada
return: retorna a validade da jogada
*/
int fillCard(PT coords,int textColor,SDL_Color bgColor,int playerNumber,int clear, card *result)
{
  result->numOfplayers = NUMPLAYERS;
  int ignorePlay = 0;


  // Ver o estado do tabuleiro nessas coordenadas
  short boardState = getBoardState(coords.row,coords.col);
  //short cellPlayer =;getBoardCellPlayer(*result);

  //Se ja estiver ocupada
  if(boardState == UP || boardState == LOCKED)
  {
    ignorePlay = 1;
    return ignorePlay;
  }


  
  if(boardState == DOWN)
  {
        result->clear = clear;
        result->position = coords;
        result->bgColor = bgColor;
        result->lettersColor = textColor;
        strcpy(result->text , get_board_place_str(coords.row,coords.col));
        setBoardCellPlayer(*result,playerNumber);
        return ignorePlay;
  } else return ignorePlay;

}

// Compara as 2 cartas correspondentes a uma jogada
int compareCards(playerNode *nodeJogador,card firstCard,card secondCard)
{
    int result = INVALIDPLAY;
    struct pollfd fds;
    fds.fd = nodeJogador->player.player_fd;
    fds.events = POLLIN;
    int ret,lixo;

    // Cliquei numa carta ocupada na segunda jogada..
    short cellState = getBoardState(secondCard.position.row,secondCard.position.col);
    if (cellState == UP || cellState == LOCKED || cardsAreSame(firstCard,secondCard))
    {
      // Mandar o sinal ao cliente para virar a primeira carta
      firstCard.clear = secondCard.clear = 1;

      // Desocupar as celulas
      setBoardCellState(firstCard,DOWN);

      // Enviar para os clientes
      sendPlayAll(nodeJogador,firstCard);
      result = INVALIDPLAY;
    }
    //Carta livre
    else if(cellState == DOWN)
    {
      // Par encontrado
      if (strcmp(firstCard.text,secondCard.text) == 0)
      {
        firstCard.lettersColor = secondCard.lettersColor = BLACK;

        // Incrementar a pontuacao do jogador e numero de pares no tabuleiro
        nodeJogador->player.score++;
        PAIRS++;


        // Bloquear celulas para outros jogadores
        setBoardCellState(firstCard,LOCKED);
        setBoardCellState(secondCard,LOCKED);

        // Enviar aos clientes
        sendPlayAll(nodeJogador,firstCard);
        sendPlayAll(nodeJogador,secondCard);

        if (gameIsOver(BOARD_DIM))
        {
          firstCard.gameOver = secondCard.gameOver = 1;
          sendPlayAll(nodeJogador,firstCard);
          searchWinner(nodeJogador);
          init_board(BOARD_DIM);
          resetAllScores(nodeJogador);
          PAIRS = 0;


        }

        result =  VALIDPLAY;

      }
      else // Cartas diferentes, virar as cartas
      {

        // Pintar as letras de vermelho
        firstCard.lettersColor = secondCard.lettersColor = RED;
        sendPlayAll(nodeJogador,firstCard);
        sendPlayAll(nodeJogador,secondCard);

        struct timespec time_beg, time_now;
        long elapsed_ms = 0;

        if (clock_gettime(CLOCK_REALTIME, &time_beg) == -1)
          exit(-1);
        while(1)
        {
          clock_gettime(CLOCK_REALTIME, &time_now);
          elapsed_ms = TIMEOUT2 - (time_now.tv_sec - time_beg.tv_sec)*1000 - (time_now.tv_nsec - time_beg.tv_nsec)/1000000;
          printf("elapsed = %li\n",elapsed_ms);
          ret = poll(&fds,1,elapsed_ms);  
          if(ret < 0)
          {
            perror("poll() failed!");
            exit(-1);
          }

          else if(ret > 0)
          {
            PT coords;
            if(recv(nodeJogador->player.player_fd,&coords,sizeof(coords),0) < 0)
            {
              removeClient(nodeJogador);
            } 
          }
          else if (ret == 0)
          {
                      // Virar as cartas
        firstCard.clear = secondCard.clear = 1;
        setBoardCellState(firstCard,DOWN);
        setBoardCellState(secondCard,DOWN);

        sendPlayAll(nodeJogador,firstCard);
        sendPlayAll(nodeJogador,secondCard);
              return VALIDPLAY;
          }
        }

        firstCard.clear = secondCard.clear = 1;
        setBoardCellState(firstCard,DOWN);
        setBoardCellState(secondCard,DOWN);

        sendPlayAll(nodeJogador,firstCard);
        sendPlayAll(nodeJogador,secondCard);





        result =  WRONGPLAY;

      }
    }

    //showBoard();
    return result;
}


void renderBoard(playerNode *head)
{
  playerStruct foundPlayer;
  card toPaint;
  board_place currCell;

  for (size_t i = 0; i < BOARD_DIM; i++)
  {
    for (size_t j = 0; j < BOARD_DIM ; j++)
    {
      currCell = board[linear_conv(i,j)];
      if (currCell.estado == UP || currCell.estado == LOCKED)
      {
        foundPlayer = searchList (head, currCell.playerNumber);

        strcpy(toPaint.text,currCell.v);
        toPaint.lettersColor = (currCell.estado == 0) ? GRAY : BLACK;
        toPaint.bgColor = foundPlayer.color;
        toPaint.position.row = i;
        toPaint.position.col = j;

        // Enviar tabuleiro atual ao novo jogador
        send(head->player.player_fd,&toPaint,sizeof(card),0);

      }
    }
  }
}
