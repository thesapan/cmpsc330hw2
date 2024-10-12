// Name: Sapan Shah
// Email: scs6041@psu.edu
// Class: CMPSC 330
// Program: Dots and Boxes
// Current Date: 11/10/24
// Due Date: 11/10/24 11:59 PM
//
// Description: This program reads a file containing a list of moves in
// a game of Dots and Boxes and determines the winner or if there is a tie.
// The program reads the grid size (number of rows and columns of boxes) and
// a list of moves made by players. The program then processes each move, checking
// for invalid moves and updating the board with lines and boxes. The program then
// determines the winner based on the number of boxes completed by each player and
// checks for ties. The program prints the final board, the scores of all players,
// and the winner or tie status.
//
// Acknowledgements: I discussed it with and received help from:
// Sandipsinh Rathod and Pranav Balachander.

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

// Structure to represent a move
struct Move {
    char player;
    int row, col;
    Move *next; // Pointer to the next move
};

// Structure to represent a player and their box count
struct PlayerNode {
    char player;
    int boxes;  // Number of boxes completed by the player
    PlayerNode *next; // Pointer to the next player
};

// Function to create and initialize the board with dots
char **createBoard(int rows, int cols) {
    // Dynamically allocate memory for the board with dots and spaces for lines
    char **board = new char *[2 * rows + 1];
    for (int i = 0; i < 2 * rows + 1; i++) {
        board[i] = new char[2 * cols + 1];
        for (int j = 0; j < 2 * cols + 1; j++) {
            if (i % 2 == 0 && j % 2 == 0) {
                board[i][j] = '.'; // Place dots
            } else {
                board[i][j] = ' '; // Initialize spaces for lines
            }
        }
    }
    return board;
}

// Function to add a move to the linked list
void addMove(Move *&head, char player, int row, int col) {
    // Create a new move and add it to the linked list
    Move *newMove = static_cast<Move *>(malloc(sizeof(Move)));
    newMove->player = player;
    newMove->row = row;
    newMove->col = col;
    newMove->next = nullptr;

    if (head == nullptr) {
        head = newMove;
    } else {
        Move *temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newMove;
    }
}

// Function to add a player to the linked list
void addPlayer(PlayerNode *&head, char player) {
    // Create a new player and add it to the linked list
    PlayerNode *newPlayer = static_cast<PlayerNode *>(malloc(sizeof(PlayerNode)));
    newPlayer->player = player;
    newPlayer->boxes = 0; // Initialize box count
    newPlayer->next = nullptr;

    if (head == nullptr) {
        head = newPlayer;
    } else {
        PlayerNode *temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newPlayer;
    }
}

// Function to find a player in the linked list
PlayerNode *findPlayer(PlayerNode *head, char player) {
    // Search for the player in the linked list and return the pointer
    PlayerNode *temp = head;
    while (temp != nullptr) {
        if (temp->player == player) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr; // Return null if player not found
}

// Function to print the board
void printBoard(char **board, int rows, int cols) {
    // Print the tens and ones places of the column numbers
    cout << "    ";  // Space for row numbers
    for (int col = 0; col < cols * 2 - 1; ++col) {
        if (!(col % 10)) {
            cout << col / 10; // Tens place of column numbers
        } else {
            cout << " ";  // For spaces between dots
        }
        cout << flush;
    }
    cout << endl;

    // Print the ones place of the column numbers
    cout << "    ";  // Space for row numbers
    for (int col = 0; col < cols * 2 - 1; ++col) {
        cout << (col) % 10;  // Print ones place
        cout << flush;
    }
    cout << endl;

    // Print the board rows with row numbers
    for (int row = 0; row < rows * 2 - 1; ++row) {
        // Print row number on the left
        if (row / 10 > 9) {
            if (row % 10 == 0) {
                cout << row / 10;
            } else {
                cout << "  ";
            }
        } else {
            cout << " ";
            if (row % 10 == 0) {
                cout << row / 10;
            } else {
                cout << " ";
            }
        }
        cout << row % 10 << " ";  // Print the ones place of the row number

        // Print the content of the row (dots, lines, spaces)
        for (int col = 0; col < cols * 2 - 1; ++col) {
            cout << board[row][col];
        }
        cout << endl;
    }
}

// Function to place horizontal or vertical lines with lowercase initials (b for B, r for R)
bool placeLine(char **board, int row, int col, char player, bool isHorizontal) {
    // Place a line on the board
    char line = tolower(player);
    if (isHorizontal) {
        if (board[2 * row][2 * col + 1] != ' ') {
            board[2 * row][2 * col + 1] = 'X'; // Mark invalid move with 'X'
            return false;
        }
        board[2 * row][2 * col + 1] = line;
    } else {
        if (board[2 * row + 1][2 * col] != ' ') {
            board[2 * row + 1][2 * col] = 'X'; // Mark invalid move with 'X'
            return false;
        }
        board[2 * row + 1][2 * col] = line;
    }
    return true;
}

// Function to check if a box is completed after placing a line
int checkAndMarkBox(char **board, int row, int col, char player, bool isHorizontal, int rows, int cols) {
    // Check if placing the line completes any boxes
    int completedBoxes = 0;

    if (isHorizontal) {
        // Check box above the line (if row > 0)
        if (row > 0 &&
            board[2 * (row - 1)][2 * col + 1] != ' ' &&  // Top line
            board[2 * (row - 1) + 2][2 * col + 1] != ' ' &&  // Bottom line
            board[2 * (row - 1) + 1][2 * col] != ' ' &&  // Left line
            board[2 * (row - 1) + 1][2 * col + 2] != ' ') {  // Right line
            board[2 * (row - 1) + 1][2 * col + 1] = player;  // Mark box with player's initial
            completedBoxes++;
        }

        // Check box below the line (if row < rows - 1)
        if (row < rows - 1 &&
            board[2 * row][2 * col + 1] != ' ' &&  // Top line
            board[2 * row + 2][2 * col + 1] != ' ' &&  // Bottom line
            board[2 * row + 1][2 * col] != ' ' &&  // Left line
            board[2 * row + 1][2 * col + 2] != ' ') {  // Right line
            board[2 * row + 1][2 * col + 1] = player;  // Mark box with player's initial
            completedBoxes++;
        }
    } else {
        // Check box to the left (if col > 0)
        if (col > 0 &&
            board[2 * row][2 * (col - 1) + 1] != ' ' &&  // Top line
            board[2 * row + 2][2 * (col - 1) + 1] != ' ' &&  // Bottom line
            board[2 * row + 1][2 * (col - 1)] != ' ' &&  // Left line
            board[2 * row + 1][2 * (col - 1) + 2] != ' ') {  // Right line
            board[2 * row + 1][2 * (col - 1) + 1] = player;  // Mark box with player's initial
            completedBoxes++;
        }

        // Check box to the right (if col < cols - 1)
        if (col < cols - 1 &&
            board[2 * row][2 * col + 1] != ' ' &&  // Top line
            board[2 * row + 2][2 * col + 1] != ' ' &&  // Bottom line
            board[2 * row + 1][2 * col] != ' ' &&  // Left line
            board[2 * row + 1][2 * col + 2] != ' ') {  // Right line
            board[2 * row + 1][2 * col + 1] = player;  // Mark box with player's initial
            completedBoxes++;
        }
    }

    return completedBoxes;
}

// Function to sort players by their player letter
void sortPlayers(PlayerNode *&head) {
    if (!head || !head->next) {
        return;
    }

    PlayerNode *sorted = nullptr;
    PlayerNode *current = head;

    while (current) {
        PlayerNode *next = current->next;
        if (!sorted || sorted->player >= current->player) {
            current->next = sorted;
            sorted = current;
        } else {
            PlayerNode *temp = sorted;
            while (temp->next && temp->next->player < current->player) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    head = sorted;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "No input file provided" << endl;
        cerr << "Usage: dotsandboxes /path/to/input.txt" << endl;
        return 1;
    }

    // File input
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (!file) {
        cerr << "File not found" << endl;
        return 1;
    }

    fseek(file, 0L, SEEK_END);
    const long sz = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char *myStr = static_cast<char *>(malloc(sz + 1 * sizeof(char)));
    myStr[sz] = '\0';

    for (int i = 0; i < sz; ++i) {
        fscanf(file, "%c", &myStr[i]);
    }
    istringstream iss(myStr);

    // Read grid size (number of rows and columns of boxes)
    int rows, cols;
    iss >> rows >> cols;

    Move *moves = nullptr;  // Linked list of moves
    PlayerNode *players = nullptr;  // Linked list of players

    // Read and process the moves from the input file
    char player;
    int row, col;
    while (iss >> player >> row >> col) {
        if (player == 'E') break;

        // Add player to the list if not already present
        if (findPlayer(players, player) == nullptr) {
            addPlayer(players, player);
        }

        // Add move to the list
        addMove(moves, player, row, col);
    }

    // Create the board
    char **board = createBoard(rows, cols);
    char playerWithIllegalMove = '\0'; // Track the player who made the illegal move

    // Process each move
    Move *currentMove = moves;
    while (currentMove != nullptr) {
        PlayerNode *currentPlayer = findPlayer(players, currentMove->player);

        // Check for invalid move (if the move is placed in an invalid position)
        if ((currentMove->col & 1) == (currentMove->row & 1)) {
            playerWithIllegalMove = currentMove->player; // Mark the player with the illegal move
            board[currentMove->row][currentMove->col] = 'X'; // Mark invalid move
            break;
        }

        // Place the line and check for completed boxes
        bool isHorizontal = currentMove->row % 2 == 0;
        if (!placeLine(board, currentMove->row / 2, currentMove->col / 2, currentMove->player, isHorizontal)) {
            playerWithIllegalMove = currentMove->player; // Mark the player with the illegal move
            break;
        }

        // Check how many boxes were completed
        int boxesCompleted = checkAndMarkBox(board, currentMove->row / 2, currentMove->col / 2, currentMove->player,
                                             isHorizontal, rows, cols);

        // Update the player's box count
        if (boxesCompleted > 0) {
            currentPlayer->boxes += boxesCompleted;
        }

        currentMove = currentMove->next;
    }

    // Print the final board
    printBoard(board, rows, cols);

    // Determine the winner or if there's a tie
    char winner = '\0';
    int maxBoxes = -1;
    bool tie = false;
    int numPlayersWithMaxBoxes = 0;  // Variable to track the number of players with the max number of boxes

    // Find the player with the most boxes and check for ties, excluding the player with the illegal move
    PlayerNode *tempPlayer = players;
    while (tempPlayer != nullptr) {
        if (tempPlayer->player != playerWithIllegalMove) {  // Exclude the player with the illegal move
            if (tempPlayer->boxes > maxBoxes) {
                winner = tempPlayer->player;
                maxBoxes = tempPlayer->boxes;
                numPlayersWithMaxBoxes = 1;  // Reset the counter for players with max boxes
            } else if (tempPlayer->boxes == maxBoxes) {
                numPlayersWithMaxBoxes++;  // Increment the counter if another player has the same number of max boxes
            }
        }
        tempPlayer = tempPlayer->next;
    }

    // Special case: If all players have 0 boxes and one made an illegal move
    if (numPlayersWithMaxBoxes > 1) {
        tie = true;  // More than one player has max boxes (0 in this case), so it's a tie
    } else if (numPlayersWithMaxBoxes == 1 && playerWithIllegalMove != '\0') {
        tie = false;  // Only one valid player has the most boxes, they win
    }

    // Sort the players by player letter
    sortPlayers(players);

    // Print the final results
    tempPlayer = players;
    while (tempPlayer != nullptr) {
        cout << "Player " << tempPlayer->player << " has " << tempPlayer->boxes << " boxes";
        if (tempPlayer->player != playerWithIllegalMove) {
            if (tie && tempPlayer->boxes == maxBoxes) {
                cout << " (tie)";
            } else if (!tie && tempPlayer->player == winner) {
                cout << " (win)";
            }
        }
        cout << "." << endl;
        tempPlayer = tempPlayer->next;
    }

    // Clean up memory for linked lists and board
    while (moves != nullptr) {
        Move *temp = moves;
        moves = moves->next;
        delete temp;
    }

    for (int i = 0; i < 2 * rows + 1; i++) {
        delete[] board[i];
    }
    delete[] board;

    PlayerNode *current = players;
    while (current != nullptr) {
        PlayerNode *temp = current;
        current = current->next;
        delete temp;
    }

    fclose(file);

    return 0;
}
