/*
 * player.h
 *
 *  Created on: 2013-10-25
 *      Author: david
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;


class Player {
    public:
        // get information of the board and the goal number
        Player( int rows = 6, int columns = 7, int goal = 4);
        //initialize the empty board for this player
        void initBoard(char**,int*);
        // Read the other player's last move and make your choice.
        // Return your next move, i.e., which column to drop your marble.
        // otherPlayersLastMove will be -1 if you are to move first
        // and this is your first move.
        int move( int otherPlayersLastMove);
        //Evaluation function, examines the board
        //state and returns the move to maximize the
        //utility of the player
        int evaluationFunction(char**,int*,int,int,bool,int,int);
		//copy function returns a new board
		char **copyBoard(char **);
		//copy function returns a new top
		int *copyTop(int *);
        //finds the heuristic value of a given state
        int heuristic(char **);
        //returns a list of valid moves given
        //the current board state
        int *getValidMoves(int*);
        //allow the player to set the depth of the search
        //larger depths are slower but get better results
        int setDepth(int);
        //allow the player to set their name
        string setName(string);
        // return your name or what ever this player wants to be called
        string getName();
        //functions to check the terminal values, lets the player
        //know whether they have won or not
        //pass in a position of a move
        int checkWinner(char**,char);
        //pass in several valus to check the specific line for a win
        //in order it's the board, the row, the column,
        //the row delta, and the column delta.
        int checkLine(char**, int, int, int, int,char);
    private:
        int Rows;
        int Columns;
        int Goal;
        int Depth;
        string name;
        char **board;
        int *top;
        char myMarble;
        char otherMarble;
        char Space;
};

#endif /* PLAYER_H_ */
