#include <stdio.h>

int clear_board(char board[3][3]){
  int i, j;

  for(i = 0; i<3; i++)
    for (j = 0; j < 3; j++)
      board[i][j]=' ';
}

int print_board(char board[3][3]){
  int i, j;
  for(i = 0; i<3; i++){
    printf("|");
    for (j = 0; j < 3; j++)
      printf("%c", board[i][j]);
    printf("|\n");
  }
}

int ver_win(char board[3][3], char player){
  if((board[0][0]+board[1][0]+board[2][0])/3 == player)
    return 1;
  if((board[0][1]+board[1][1]+board[2][1])/3 == player)
      return 1;
  if((board[0][2]+board[1][2]+board[2][2])/3 == player)
      return 1;
  if((board[0][0]+board[0][1]+board[0][2])/3 == player)
        return 1;
  if((board[1][0]+board[1][1]+board[1][2])/3 == player)
        return 1;
  if((board[2][0]+board[2][1]+board[2][2])/3 == player)
        return 1;
  if((board[0][0]+board[1][1]+board[2][2])/3 == player)
        return 1;
  if((board[0][2]+board[1][1]+board[2][2])/3 == player)
        return 1;
}

void read_play(int play[2], char player){
  char linha[100];
  int play_ok=0;

  do{
    printf("please play 0..2 0..2 (%c): ", player);
    fgets(linha, 100, stdin);
    if (sscanf(linha, "%d %d", & play[0], &play[1]) == 2){
      if(play[0]>=0 && play[0]<=2&& play[1]>=0 && play[1]<=2){
        play_ok = 1;
      }
    }
  }while(!play_ok);
}

int put_play(char board[3][3], int play[2], char player){
  if (  board[play[0]][play[1]] == ' '){
    board[play[0]][play[1]]=player;
    return 1;
  }else{
    return 0;
  }
}

int main(){
  char b[3][3];
  int play[2];
   clear_board(b);
  while(1){
    do{
      print_board (b);
      read_play(play, 'x');
    }while (put_play(b, play, 'x') != 1);

    if (ver_win(b, 'x') ==1){
      print_board (b);
      printf("winner - x\n");
      break;
    }

    do{
      print_board (b);
      read_play(play, 'o');
    }while (put_play(b, play, 'o') != 1);

    if (ver_win(b, 'o') ==1){
      print_board (b);
      printf("winner - x\n");
      break;
    }

 }
}
