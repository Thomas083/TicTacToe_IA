#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const char O = 'O';
const char X = 'X';
const char EMPTY = ' ';
const char TIE = 'T';
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

int main()
{
	int move;
	const int NUM_SQUARES = 9;
	vector<char> board(NUM_SQUARES, EMPTY);

	introductions();
	char human = humanPiece();
	char computer = opponent(human);
	char turn = X;
	displayBoard(board);

	while (winner(board) == NO_ONE)
	{
		if (turn == human)
		{
			move = humanMove(board, human);
			board[move] = human;
		}
		else
		{
			move = computerMove(board, computer);
			board[move] = computer;
		}
		displayBoard(board);
		turn = opponent(turn);
	}

	announceWinner(winner(board), computer, human);

	return 0;
}

void introductions()
{
	cout << "Welcome to my Tic Tac Toe Game";
	cout << "Prepare to lose";

	cout << "Make your choice. Chooose a number between 0-8";
	
	cout << " 0 | 1 | 2 ";
	cout << "-----------";
	cout << " 3 | 4 | 5 ";
	cout << "-----------";
	cout << " 6 | 7 | 8 ";
	cout << "-----------";

	cout << "Lets begin";
}

char askYesNo(string question) 
{
	char response;
		do 	
		{
			cout << question << " (y/n): ";
			cin >> response;
		} while (response != 'y' && response != 'n');

	return response;
}

int askNumber(string question, int high, int low)
{
	int number;
	do
	{
		cout << question << " (" << low << " - " << high << "): ";
		cin >> number;
	} while (number > high || number < low);

	return number;
}

char humanPiece()
{
	char go_first = askYesNo("Do you require the first move?");
	if (go_first == 'y')
	{
		cout << "Then take the first move. You will need it.";
		return X;
	}
	else
	{
		cout << "Your bravery will be your undoing... I will go first.";
		return O;
	}
}

char opponent(char piece)
{
	if (piece == 'X')
	{
		return O;
	}
	else
	{
		return X;
	}
}

void displayBoard(const vector<char>& board)
{
	cout << endl;
	cout << board[0] << " | " << board[1] << " | " << board[2] << endl;
	cout << "---------" << endl;
	cout << board[3] << " | " << board[4] << " | " << board[5] << endl;
	cout << "---------" << endl;
	cout << board[6] << " | " << board[7] << " | " << board[8] << endl;
	cout << endl;
}

char winner(const vector<char>& board)
{
	const int WINNING_ROWS[8][3] = { {0, 1, 2},
									 {3, 4, 5},
									 {6, 7, 8},
									 {0, 3, 6},
									 {1, 4, 7},
									 {2, 5, 8},
									 {0, 4, 8},
									 {2, 4, 6} };

	const int TOTAL_ROWS = 8;

	for (int row = 0; row < TOTAL_ROWS; row++)
	{
		if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
			(board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
			(board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
		{
			return board[WINNING_ROWS[row][0]];
		}
	}

	if (count(board.begin(), board.end(), EMPTY) == 0)
	{
		return TIE;
	}

	return NO_ONE;
}

inline bool isLegal(int move, const vector<char>& board)
{
	return (board[move] == EMPTY);
}

int humanMove(const vector<char>& board, char human)
{
	int move = askNumber("Where will you move?", (board.size() - 1));
	while (!isLegal(move, board))
	{
		cout << "\nThat square is already occupied, foolish human.\n";
		move = askNumber("Where will you move?", (board.size() - 1));
	}
	cout << "Fine...\n";

	return move;
}

