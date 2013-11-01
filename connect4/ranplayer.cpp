/*
 * ranplayer.cpp
 *
 *  Created on: 2013-10-25
 *      Author: david
 */

#include "ranplayer.h"

RANPlayer::RANPlayer(int r, int c, int g) {
       Rows = r;
       Columns = c;
       Goal = g;
       name = "Enemy";
       num ++;
       top = new int[Columns];
       board = new char *[Rows];
       myMarble = 'X';
       otherMarble = 'O';
       for(int i = 0; i < r; i++)
          board[i] = new char[c];
       initBoard(board, top);
       time_t seconds;
       time(&seconds);
       srand((unsigned int)seconds);
}

void RANPlayer::initBoard(char **board, int top[]) {
	for(int i = 0; i < Rows; i++) {
	  for(int j = 0; j < Columns; j++) {
		 board[i][j] = Space;
	  }
	}
	for(int i = 0; i < Columns; i++) {
		top[i] = 0;
	}
}

int RANPlayer::move(int otherPlayersLastMove){
   if (otherPlayersLastMove != -1) {
      board[top[otherPlayersLastMove]][otherPlayersLastMove]
                    = otherMarble;
      top[otherPlayersLastMove]++;
   }
   int count = 0;
   for (int i = 0; i < Columns; i++)
     if (top[i] < Rows) count ++;

   int index = rand() % count;
   for (int col = 0; col < Columns; col ++) {
      if (top[col] < Rows) index--;
      if (index < 0) {
         board[top[col]][col] = myMarble;
         top[col]++;
         return col;
      }
   }
}

string RANPlayer::getName() {
	return name;
}

char RANPlayer::num = 'A';
