/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"

using std::vector;

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

// Check if the state is terminal.
bool MinimaxPlayer::checkTerminal(OthelloBoard b){
	char p1 = b.get_p1_symbol();
	char p2 = b.get_p2_symbol();
	if(b.has_legal_moves_remaining(p1) || b.has_legal_moves_remaining(p2)){
		return false;
	}
	return true;
}

// Utility function for the game.
// p1: X-O maximize
// p2: X-O minimize
// Thus, score(p1)-score(p2) is adequate for the utility function. 
int MinimaxPlayer::utility(OthelloBoard b){
	char p1 = b.get_p1_symbol();
	char p2 = b.get_p2_symbol();
	int p1score = b.count_score(p1);
	int p2score = b.count_score(p2);

	int u = p1score - p2score;

	return u;
}

// expands the node for maximizing or minimizing the results. 
void MinimaxPlayer::successor(OthelloBoard b, vector<OthelloBoard>& board, vector<int>& row, vector<int>& col, char player){
	// For the entier board
	// column
	for(int i = 0; i < 4; i++){
		// row
		for(int j = 0; j < 4; j++){
			// If the movement is valid
			if(b.is_legal_move(i,j,player)){
				// push the current state to the board vector.
				OthelloBoard tmp = b;
				tmp.play_move(i,j,player);
				board.push_back(tmp);
				// push the column and row state to the column and row vector
				col.push_back(i);
				row.push_back(j);
			}
			
		}
	}
	
}

// Returns maximum values among minimum values made by p2.
// p1 will be the player for this function
int MinimaxPlayer::maxValue(OthelloBoard b){
	int v = -2147483648, tmp;

	// Check if it is terminal state. 
	if(checkTerminal(b)){
		int u = utility(b);
		return u;
	}
	
	vector<OthelloBoard> board;
	vector<int> R;
	vector<int> C;
	char p1 = b.get_p1_symbol();
	
	// expand nodes for p1. 
	successor(b, board, R,C, p1);

	// If there is no expansion, then return the minimum value. 
	if(board.size()== 0){
		v= minValue(b);
	}
	
	// for each action, choose the maximum among minimum values. 
	for(int i = 0; i < board.size();i++){
		tmp = minValue(board[i]);
		if(tmp > v){
			v = minValue(board[i]);
		}
		else{
			v = v;
		}
	}

	board.clear();
	R.clear();
	C.clear();


	return v;
}

// Returns minimum values among maximum values made by p1.
// p2 will be the player for this function
int MinimaxPlayer::minValue(OthelloBoard b){
	
	int v = 2147483647, tmp;

	//check if the state is terminal 
	if(checkTerminal(b)){
		return utility(b);
	}
	
	vector<OthelloBoard> board;
	vector<int> R;
	vector<int> C;
	char p2 = b.get_p2_symbol();

	// Expand the nodes for p2
	successor(b,board,  R,C, p2);
	
	// If there is no expansion, then return the maximum value. 
	if(board.size() == 0){
		v = maxValue(b);
	}
	
	// for each action, choose the minimum among the maximum values. 
	for(int i = 0; i < board.size();i++){
		tmp = maxValue(board[i]);
		if(tmp < v){
			v = maxValue(board[i]);
		}
		else{
			v = v;
		}
	}

	board.clear();
	R.clear();
	C.clear();
	
	return v;
}

// p1 always chooses maximum value among minimum values chosen by p2
// p2 always chooses minimum value among maximum values chosen by p1
void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
    // To be filled in by you
	char p1 = b->get_p1_symbol();
	char p2 = b->get_p2_symbol();

	int m = -2147483648;
	int n = 2147483647;
	int tmp;

	vector<OthelloBoard> board;
	vector<int> R;
	vector<int> C;

	// Expand nodes for the symbol
	successor(*b, board,  R, C, symbol);

	// for each action,
	// Case p1: find the maximum among the minimum values
	// Case p2: find the minimum among the maximum values. 
	// Changes the row and column variables for each action. 
	for(int i = 0; i < board.size(); i++){
		if(symbol == p1){
			tmp = minValue(board[i]);
			if(tmp > m){
				m = minValue(board[i]);
				row = R[i];
				col = C[i];
			}
		}
		else if(symbol == p2){
			tmp = maxValue(board[i]);
			if(n > tmp){
				n =  maxValue(board[i]);
				row = R[i];
				col = C[i];
			}
		}
	}

	board.clear();
	R.clear();
	C.clear();

}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
