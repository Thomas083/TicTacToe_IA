#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const char O = 'O';
const char X = 'X';
const char empty = ' ';
const char Tie = 'T';
const char NO_ONE = 'N';

void introductions();
char askYesNo(string question);
int askNumber(string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const vector<char>& board);
char winner(const vector<char>& board);
bool isLegal(const vector<char>& board, int move);
int humanMove(const vector<char>& board, char human);
int computerMove(const vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);