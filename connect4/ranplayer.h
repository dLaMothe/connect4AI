/*
 * ranplayer.h
 *
 *  Created on: 2013-10-25
 *      Author: david
 */

#ifndef RANPLAYER_H_
#define RANPLAYER_H_

#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

const char Space = ' ';

class RANPlayer {
	private:
		static char num;
		int Rows;
		int Columns;
		int Goal;
		string name;
		char **board;
		int *top;
		char myMarble;
		char otherMarble;

  public:
    RANPlayer(int r = 6, int c = 7, int g = 4);
    void initBoard(char **board, int top[]);
    int move(int);
    string getName();
};


#endif /* RANPLAYER_H_ */
