/*
 * player.cpp
 *
 *  Created on: 2013-10-25
 *      Author: david
 */

#include "player.h"

Player::Player(int r, int c, int g) {
	Rows = r;
	Columns = c;
	Goal = g;
	Depth = 6;
	name = "David";
    top = new int[Columns];
    board = new char *[Rows];
    myMarble = 'O';
    otherMarble = 'X';
    Space = ' ';
    for(int i = 0; i < r; i++) {
       board[i] = new char[c];
    }
    initBoard(board, top);

}

void Player::initBoard(char **board, int top[]) {
   for(int i = 0; i < Rows; i++) {
	  for(int j = 0; j < Columns; j++) {
		 board[i][j] = Space;
	  }
   }
   for(int i = 0; i < Columns; i++) {
	  top[i] = 0;
   }
}

int Player::move(int lastMove) {
	//This updates the board in this object
	//fills the opponents move in the
	//chosen space then makes the column
	//one bigger
   if (lastMove != -1) {
	  board[top[lastMove]][lastMove] = otherMarble;
	  top[lastMove]++;
   }
   //creates a list of which columns are full
   //and which arent
   int* candidates = getValidMoves(top);
   //Variables to track which column is currently
   //considered best
   //currBest not initiated because it has to
   //be started as the first available move
   int currBest;
   //set bestMove to a very low amount to make
   //sure the program always takes some available
   //move
   int bestMove = -1000000000;
   int bestColumn;
   //sets the alpha beta values to arbitrarily large integers
   //will be shrunk as used in the evaluation function
   int alpha = -100000;
   int beta = 100000;
   //keeps track of whether current player is max or min
   bool maxPlayer = true;
   //goes through list and evaluates that node
   //if it's not full
   for(int i = 0; i < Columns; i++) {
	   //calls a recursive evaluation function
	   //on the possible columns to check which
	   //will result in the highest maximum
	   //utility
	   if(candidates[i] == 1) {
		   currBest = evaluationFunction(board,top,Depth,i,maxPlayer,alpha,beta);
		   if(currBest > bestMove) {
			   bestMove = currBest;
			   bestColumn = i;
		   }
	   }
   }
   board[top[bestColumn]][bestColumn] = myMarble;
   top[bestColumn]++;
   return bestColumn;
}

int Player::evaluationFunction(char **tempState, int* tempTop,int currDepth,
								int col, bool maxPlayer, int alpha, int beta) {

	char **boardState = copyBoard(tempState);
	int *newTop = copyTop(tempTop);
	//if the current decisions is the player (max) then update the board and add to the top
	//else do the same with the other player
	if(maxPlayer == true) {
		boardState[newTop[col]][col] = myMarble;
		newTop[col]++;
	} else {
		boardState[newTop[col]][col] = otherMarble;
		newTop[col]++;
	}

	//now that this is updated the win conditions can be checked
		if(checkWinner(boardState,myMarble) == true) {
			//this function basically checks if the current board state
			//contains a win it's set to a high number because the utlity
			//of moving to this state is very beneficial
			int winMove = 1000;
			//delete this node as it is no longer needed
			delete boardState;
			return winMove;
		}
		if(checkWinner(boardState,otherMarble) == true) {
			//this function basically checks if the current board state
			//contains a loss it's set to a low number because the utlity
			//of moving to this state is very low
			int lossMove = -1000;
			//delete this node as it is no longer needed
			delete boardState;
			return lossMove;
		}
	//if current depth is ero, the tree has finished searching and
	//evaluation of the current board state needs to take place
	if(currDepth == 0) {
		int bestMove = heuristic(boardState);
		//delete this node as it is no longer needed
		delete boardState;
		delete newTop;
		return bestMove;
	}
	//now we get the next possible candidates, which is the basis for recursion
	//down the tree
    	int *candidates = getValidMoves(newTop);
    if(maxPlayer == true) {
    	for(int i = 0; i < Columns; i++) {
    		if(candidates[i] == 1) {
					alpha = max(alpha,evaluationFunction(boardState,newTop,
														currDepth-1,i,false,alpha,beta));
				if(beta <= alpha) {
					break;
				}
			}
    	}
    	//delete this node as it is no longer needed
    	delete boardState;
		delete newTop;
		return alpha;
    } else {
    	for(int j = 0; j < Columns; j++) {
    		if(candidates[j] == 1) {
    			beta = min(beta,evaluationFunction(boardState,newTop,
    											   currDepth-1,j,true,alpha,beta));
    			if(beta <= alpha) {
    				break;
    			}
    		}
    	}
    	//delete this node as it is no longer needed.
    	delete boardState;
		delete newTop;
    	return beta;
    }
}
//copy functions for board and top, needed to
//create different nodes in the tree
char **Player::copyBoard(char **oldBoard) {
 	//create a board that is the same
	//size as the old one
    char **newBoard = new char *[Rows];
    for (int i = 0; i < Rows; i++) {
        newBoard[i] = new char[Columns];
    }
    
    // copy the values from the old board
	// into the new one
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            newBoard[i][j] = oldBoard[i][j];
        }
    }
    
    // return the carbon copy
    return newBoard;
}

int *Player::copyTop(int *oldTop) {
	//create a top that is the same
	//size as the old one
	int *newTop = new int[Columns];
	//now copy all the elements of the old
	//top into the new one
	for(int i = 0; i < Columns; i++) {
		newTop[i] = oldTop[i];
	}
	return newTop;
}

//gets all the moves that are currently valid
//a move is not valid if the column is completely
//full
int* Player::getValidMoves(int *currTop) {
	int *validMoves = new int[Columns];
    for (int i = 0; i < Columns; i++) {
    	if (currTop[i] < Rows) {
    		validMoves[i] = 1;
    	} else {
    		validMoves[i] = 0;
    	}
    }
    return validMoves;

}

//heuristic function, checks how much value should be assigned to
//a generalized state in the board based on certain factors
int Player::heuristic(char  	**terminalBoard) {
	//create and initialize two arrays which store
	//number of unblocked two's and unblocked
	//three's for the specific state
	int two[2];
	int three[2];

	for(int i = 0; i < 2; i++) {
		two[i] = 0;
		three[i] = 0;
	}

	//variable to track which player has
	//the marble is the slot
	char currPlayer = myMarble;
	//keeps track of the utility of this state
	int utility = 0;
	//this loops through the entire board and checks for indicators
	//of positive or negative utility, where we define a positive
	//utility as beneficial to MAX a negative utility is beneficial
	//to min
	for(int i = 0; i < 2; i++) {
		if(i == 0) {
			currPlayer = myMarble;
		} else {
			currPlayer = otherMarble;
		}
		//checks for pairs of two horizontally
		for(int j = 0; j < Rows; j++) {
			for(int k = 0; k < Columns; k++) {
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j][k+1] == currPlayer && terminalBoard[j][k+2] == ' ' && terminalBoard[j][k+3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j][k+1] == ' ' && terminalBoard[j][k+2] == currPlayer && terminalBoard[j][k+3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j][k+1] == ' ' && terminalBoard[j][k+2] == ' ' && terminalBoard[j][k+3] == currPlayer) {
                    two[i]++;
				}
                if (terminalBoard[j][k] == ' ' && terminalBoard[j][k+1] == currPlayer && terminalBoard[j][k+2] == currPlayer && terminalBoard[k][j+3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j][k+1] == currPlayer && terminalBoard[j][k+2] == ' ' && terminalBoard[j][k+3] == currPlayer) {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j][k+1] == ' ' && terminalBoard[j][k+2] == currPlayer && terminalBoard[j][k+3] == currPlayer) {
                    two[i]++;
                }

			}
		}
		//checks for pairs of three horizontally
		for(int j = 0; j < Rows; j++) {
			for(int k = 0; k < Columns; k++) {
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j][k+1] == currPlayer && terminalBoard[j][k+2] == currPlayer && terminalBoard[j][k+3] == ' ') {
                    three[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j][k+1] == currPlayer && terminalBoard[j][k+2] == ' ' && terminalBoard[j][k+3] == currPlayer) {
                    three[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j][k+1] == ' ' && terminalBoard[j][k+2] == currPlayer && terminalBoard[j][k+3] == currPlayer) {
                    three[i]++;
				}
                if (terminalBoard[j][k] == ' ' && terminalBoard[j][k+1] == currPlayer && terminalBoard[j][k+2] == currPlayer && terminalBoard[k][j+3] == currPlayer) {
                    three[i]++;
                }
			}
		}
		//checks for pairs and threes vertically
		for(int j = 0; j < Columns; j++) {
			for(int k = 0; k < Rows - 4; k++) {
                if (terminalBoard[k][j] == currPlayer && terminalBoard[k+1][j] == currPlayer && terminalBoard[k+2][j] == ' ' && terminalBoard[k+3][j] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[k][j] == currPlayer && terminalBoard[k+1][j] == currPlayer && terminalBoard[k+2][j] == currPlayer && terminalBoard[k+3][j] == ' ') {
                    three[i]++;
                }
			}
		}
		//find all pairs of two's in normal diagonals
        for (int j = 0; j < Rows-4; j++) {
            for (int k = 0; k < Columns-4; k++) {
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == ' ' && terminalBoard[j+3][k+3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k+1] == ' ' && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k+1] == ' ' && terminalBoard[j+2][k+2] == ' ' && terminalBoard[j+3][k+3] == currPlayer) {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == ' ' && terminalBoard[j+3][k+3] == currPlayer) {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k+1] == ' ' && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == currPlayer) {
                    two[i]++;
                }
            }
        }

        // find all three's in normal diagonals
        for (int j = 0; j < Rows-4; j++) {
            for (int k = 0; k < Columns-4; k++) {
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == ' ') {
                    three[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == ' ' && terminalBoard[j+3][k+3] == currPlayer) {
                    three[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k+1] == ' ' && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == currPlayer) {
                    three[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == currPlayer) {
                    three[i]++;
                }
            }
        }
		//find all pairs of two's in reverse diagonals
        for (int j = 0; j < Rows-4; j++) {
            for (int k = 4; k < Columns; k++) {
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k-1] == currPlayer && terminalBoard[j+2][k-2] == ' ' && terminalBoard[j+3][k-3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k-1] == ' ' && terminalBoard[j+2][k-2] == currPlayer && terminalBoard[j+3][k-3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k-1] == ' ' && terminalBoard[j+2][k-2] == ' ' && terminalBoard[j+3][k-3] == currPlayer) {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k-1] == currPlayer && terminalBoard[j+2][k-2] == currPlayer && terminalBoard[j+3][k-3] == ' ') {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k+1] == currPlayer && terminalBoard[j+2][k+2] == ' ' && terminalBoard[j+3][k+3] == currPlayer) {
                    two[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k+1] == ' ' && terminalBoard[j+2][k+2] == currPlayer && terminalBoard[j+3][k+3] == currPlayer) {
                    two[i]++;
                }
            }
        }

        // find all three's in reverse diagonals
        for (int j = 0; j < Rows - 4; j++) {
            for (int k = 4; k < Columns; k++) {
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k-1] == currPlayer && terminalBoard[j+2][k-2] == currPlayer && terminalBoard[j+3][k-3] == ' ') {
                    three[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k-1] == currPlayer && terminalBoard[j+2][k-2] == ' ' && terminalBoard[j+3][k-3] == currPlayer) {
                    three[i]++;
                }
                if (terminalBoard[j][k] == currPlayer && terminalBoard[j+1][k-1] == ' ' && terminalBoard[j+2][k-2] == currPlayer && terminalBoard[j+3][k-3] == currPlayer) {
                    three[i]++;
                }
                if (terminalBoard[j][k] == ' ' && terminalBoard[j+1][k-1] == currPlayer && terminalBoard[j+2][k-2] == currPlayer && terminalBoard[j+3][k-3] == currPlayer) {
                    three[i]++;
                }
            }
        }
    }
	utility = 40*three[0] + 10*two[0] - 40*three[1] - 10*two[1];
	return utility;
}

//various mutators and accessors
int Player::setDepth(int newDepth) {
	Depth = newDepth;
}

string Player::setName(string newName) {
	name = newName;
}

string Player::getName() {
    return name;
}

//Checks to see if there's a winner
int Player::checkWinner(char **checkBoard,char marble) {
   bool win = false;

   for(int i = 0; i < Columns; i++) {
      win = checkLine(checkBoard, 0, i, +1, 0, marble);
   }
   for(int i = 0; i < Rows; i++) {
      win = checkLine(checkBoard, i, 0, 0, +1, marble);
   }
   for(int i=0; i < Columns; i++) {
      win = checkLine(checkBoard, 0, i, +1, -1, marble);
   }
   for(int i=0; i < Columns; i++) {
      win = checkLine(checkBoard, 0, i, +1, +1, marble);
   }
   for(int i=0; i < Columns; i++) {
      win = checkLine(checkBoard, Rows-1, i, +1, -1, marble);
   }
   for(int i=0; i < Columns; i++) {
      win = checkLine(checkBoard, Rows-1, i, +1, +1, marble);
   }

   return win;
}

//Checks the lines to see if the player has won or not
int Player::checkLine(char **checkBoard, int row, int col, int rdelta,
					  int cdelta, char marble)
{
   int count = 0;

   while (row < Rows && row >= 0 && col < Columns && col >= 0) {
      if (checkBoard[row][col] == marble) {
         count++;
         if (count == Goal) return true;
      } else {
         count = 0;
      }
      row = row + rdelta;
      col = col + cdelta;
   }
   return false;
}
