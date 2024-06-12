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
int computerMoveIA(vector<char> board, char computer, char human);
void announceWinner(char winner, char computer, char human);
int askOpponent();

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

    int opponentChoice = askOpponent(); // Choix de l'adversaire

    while (winner(board) == NO_ONE)
    {
        if (turn == human)
        {
            move = humanMove(board, human);
            board[move] = human;
        }
        else
        {
            if (opponentChoice == 1) // Adversaire simple
            {
                move = computerMove(board, computer);
            }
            else // Adversaire IA avec Minimax
            {
                move = computerMoveIA(board, computer, human);
            }
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
    cout << "Welcome to my Tic Tac Toe Game\n";
    cout << "Prepare to lose\n";

    cout << "Make your choice. Choose a number between 0-8\n";

    cout << " 0 | 1 | 2 \n";
    cout << "-----------\n";
    cout << " 3 | 4 | 5 \n";
    cout << "-----------\n";
    cout << " 6 | 7 | 8 \n\n";

    cout << "Let's begin";
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
        cout << "Then take the first move. You will need it.\n";
        return X;
    }
    else
    {
        cout << "Your bravery will be your undoing... I will go first.\n";
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
    cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
    cout << "\n\t" << "---------";
    cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
    cout << "\n\t" << "---------";
    cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
    cout << "\n\n";
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

// algorithm for computer move without using minimax
int computerMove(vector<char> board, char computer)
{
    unsigned int move = 0;
    bool found = false;

    // if computer can win on next move, that's the move to make
    while (!found && move < board.size())
    {
        if (isLegal(move, board))
        {
            board[move] = computer;
            found = winner(board) == computer;
            board[move] = EMPTY;
        }

        if (!found)
        {
            move++;
        }
    }

    // otherwise, if human can win on next move, that's the move to make
    if (!found)
    {
        move = 0;
        char human = opponent(computer);

        while (!found && move < board.size())
        {
            if (isLegal(move, board))
            {
                board[move] = human;
                found = winner(board) == human;
                board[move] = EMPTY;
            }

            if (!found)
            {
                move++;
            }
        }
    }

    // otherwise, moving to the best open square is the move to make
    if (!found)
    {
        move = 0;
        unsigned int i = 0;
        const int BEST_MOVES[] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 };

        // pick best open square
        while (!found && i < board.size())
        {
            move = BEST_MOVES[i];
            if (isLegal(move, board))
            {
                found = true;
            }

            i++;
        }
    }

    cout << "I shall take square number " << move << endl;
    return move;
}

int minimax(vector<char>& board, char player, char computer, char human)
{
    char result = winner(board);

    if (result != NO_ONE)
    {
        if (result == computer)
        {
            return 1;
        }
        else if (result == human)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

    int bestScore = (player == computer) ? -1000 : 1000;

    for (int i = 0; i < board.size(); ++i)
    {
        if (isLegal(i, board))
        {
            board[i] = player;
            int currentScore = minimax(board, opponent(player), computer, human);
            board[i] = EMPTY;

            if (player == computer)
            {
                bestScore = max(bestScore, currentScore);
            }
            else
            {
                bestScore = min(bestScore, currentScore);
            }
        }
    }

    return bestScore;
}

// algorithm for computer move using minimax
int computerMoveIA(vector<char> board, char computer, char human)
{
    int bestMove = -1;
    int bestScore = -1000;

    for (int i = 0; i < board.size(); ++i)
    {
        if (isLegal(i, board))
        {
            board[i] = computer;
            int currentScore = minimax(board, opponent(computer), computer, human);
            board[i] = EMPTY;

            if (currentScore > bestScore)
            {
                bestScore = currentScore;
                bestMove = i;
            }
        }
    }

    cout << "I shall take square number " << bestMove << endl;
    return bestMove;
}

void announceWinner(char winner, char computer, char human)
{
    if (winner == computer)
    {
        cout << winner << " wins!\n";
        cout << "As I predicted, human, I am triumphant once more.\n";
    }
    else if (winner == human)
    {
        cout << winner << " wins!\n";
        cout << "No, no! It cannot be! Somehow you tricked me, human.\n";
    }
    else
    {
        cout << "It's a tie.\n";
        cout << "You were most lucky, human, and somehow managed to tie me.\n";
    }
}

int askOpponent()
{
    cout << "Choose your opponent:\n";
    cout << "1. Simple Computer\n";
    cout << "2. AI using Minimax\n";
    int choice;
    do
    {
        cout << "Enter 1 or 2: ";
        cin >> choice;
    } while (choice != 1 && choice != 2);

    return choice;
}
