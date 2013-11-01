/*
 * main.cpp
 *
 *  Created on: 2013-10-25
 *      Author: david
 */
// Required header files

#include <iomanip>
#include "player.h"
#include "ranplayer.h"

const char Filling = ' ';
const int Columns = 7;
const int Rows = 6;
const int Goal = 4;
const int Players = 2;

enum Status {Win, Draw, Continue};

const char playerMarble[Players] = {'X', 'O'};

void initBoard(char board[Rows][Columns], int top[])
{
   for(int i = 0; i < Rows; i++)
      for(int j = 0; j < Columns; j++)
         board[i][j] = Filling;

   for(int i = 0; i < Columns; i++)
      top[i] = 0;

}

void displayBoard(char board[Rows][Columns])
{
   for(int i = Rows-1; i >= 0; i--)
   {
      cout << "-";
      for(int j = 0; j < Columns; j++)
         cout << "----";
      cout << endl;
      cout << "|";
      for(int j = 0; j < Columns; j++)
         cout << " " << board[i][j] << " |";
      cout << endl;
   }
   cout << "-";
   for(int j = 0; j < Columns; j++)
      cout << "----" ;
   cout << endl;
   cout << " ";
   for(int j = 0; j < Columns; j++)
      cout << setw(2) << j+1 << "  ";
   cout << endl << endl;
}

Status checkLine(char board[Rows][Columns], int row, int col,
                         int rdelta, int cdelta, int player)
{
   Status status = Continue;
   int count = 0;

   while (row < Rows && row >= 0 && col < Columns && col >= 0) {
      if (board[row][col] == playerMarble[player]) {
         count++;
         if (count == Goal) return Win;
      } else {
         count = 0;
      }
      row = row + rdelta;
      col = col + cdelta;
   }
   return Continue;
}

Status checkWinner(char board[Rows][Columns], int player)
{
   Status status = Continue;

   for(int i = 0; i < Columns && status == Continue; i++) {
      status = checkLine(board, 0, i, +1, 0, player);
   }
   for(int i = 0; i < Rows && status == Continue; i++) {
      status = checkLine(board, i, 0, 0, +1, player);
   }
   for(int i=0; i < Columns && status == Continue; i++) {
      status = checkLine(board, 0, i, +1, -1, player);
   }
   for(int i=0; i < Columns && status == Continue; i++) {
      status = checkLine(board, 0, i, +1, +1, player);
   }
   for(int i=0; i < Columns && status == Continue; i++) {
      status = checkLine(board, Rows-1, i, +1, -1, player);
   }
   for(int i=0; i < Columns && status == Continue; i++) {
      status = checkLine(board, Rows-1, i, +1, +1, player);
   }

   return status;
}

Status checkDraw(int top[])
{
    Status status = Draw;

    for(int i = 0; i < Columns && status == Draw; i++)
    {
       if (top[i] < Rows) status = Continue;
    }

    return status;
}

// Start of main function
int main()
{
   char board[Rows][Columns];
   int top[Columns];
   int player = 0;
   RANPlayer *p1;
   Player *p2;
   Status status = Continue;
   int position = -1;

   p1 = new RANPlayer(Rows, Columns, Goal);
   p2 = new Player(Rows, Columns, Goal);

   initBoard(board, top);

   cout << "\n=== Connect Four ===\n\n";

   while (status != Win && status != Draw)
   {
      displayBoard(board);
      if(player == 0) {
    	  position = p1->move(position);
      } else if (player == 1) {
    	  position = p2->move(position);
      }
      top[position]++;
      // record the move
	  if(player == 0) {
      	board[top[position]-1][position] = 'X';
	  } else if(player == 1) {	
    	board[top[position]-1][position] = 'O';
	  }
      status = checkWinner(board, player);
      if(player == 0) {
    	  player++;
      } else if(player == 1) {
    	  player--;
      }

      if (status != Win) status = checkDraw(top);
   }

   displayBoard(board);

   if (status == Draw)
      cout << "\n   *** It is a draw! ***\n\n";
   else {
	   string winner;
	   if(player == 1) {
		   winner = p1->getName();
	   } else if (player == 0) {
		   winner = p2->getName();
	   }
      cout << "\n   *** Congratulations! Player " << winner
             << " ***\n";
      cout << "   *** You won the game. ***\n\n";
   }
}

