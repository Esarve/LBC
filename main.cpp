/**
 * Base Code = https://www.geeksforgeeks.org/dsa/n-queen-problem-local-search-using-hill-climbing-with-random-neighbour/
 * Modified for AMOD 5470H - Assignment 2 - N-Queen Problem
 * By: Sourav Das, Student ID: 0884762
 *
 * Acknowledgment: I have referred to the original code for structure and logic,
 * but I have rewritten and commented the code extensively to ensure it is my own work and to enhance readability.
 * 
 * The base code uses a hill climbing algorithm with random neighbor selection to solve the N-Queen problem, which is a classic combinatorial optimization problem.
 * This will be converted to a Local Beam Search algorithm for the assignment.
 *
 */

// C++ implementation of the
// above approach
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#ifndef N
#define N 8
#endif
#ifndef k
#define k 4
#endif
#define MAX_ITERATIONS 1000
#define SUCCESSOR_COUNT (N * (N - 1))
#define SUCCESSOR_POOL_SIZE (k * SUCCESSOR_COUNT)
using namespace std;

// A utility function that configures
// the 2D array "board" and
// array "state" randomly to provide
// a starting point for the algorithm.
void configureRandomly(int board[][N],
                       int* state)
{
    // Iterating through the
    // column indices
    for (int i = 0; i < N; i++) {

        // Getting a random row index
        state[i] = rand() % N;

        // Placing a queen on the
        // obtained place in
        // chessboard.
        board[state[i]][i] = 1;
    }
}

// A utility function that prints
// the 2D array "board".
void printBoard(int board[][N])
{

    for (int i = 0; i < N; i++) {
        cout << " ";
        for (int j = 0; j < N; j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

// A utility function that prints
// the array "state".
void printState(int* state)
{

    for (int i = 0; i < N; i++) {
        cout << " " << state[i] << " ";
    }
    cout << endl;
}

// A utility function that compares
// two arrays, state1 and state2 and
// returns true if equal
// and false otherwise.
bool compareStates(int* state1,
                   int* state2)
{

    for (int i = 0; i < N; i++) {
        if (state1[i] != state2[i]) {
            return false;
        }
    }
    return true;
}

// A utility function that fills
// the 2D array "board" with
// values "value"
void fill(int board[][N], int value)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = value;
        }
    }
}

// This function calculates the
// objective value of the
// state(queens attacking each other)
// using the board by the
// following logic.
int calculateObjective(int board[][N],
                       int* state)
{

    // For each queen in a column, we check
    // for other queens falling in the line
    // of our current queen and if found,
    // any, then we increment the variable
    // attacking count.

    // Number of queens attacking each other,
    // initially zero.
    int attacking = 0;

    // Variables to index a particular
    // row and column on board.
    int row, col;

    for (int i = 0; i < N; i++) {

        // At each column 'i', the queen is
        // placed at row 'state[i]', by the
        // definition of our state.

        // To the left of same row
        // (row remains constant
        // and col decreases)
        row = state[i], col = i - 1;
        while (col >= 0
               && board[row][col] != 1) {
            col--;
        }
        if (col >= 0
            && board[row][col] == 1) {
            attacking++;
        }

        // To the right of same row
        // (row remains constant
        // and col increases)
        row = state[i], col = i + 1;
        while (col < N
               && board[row][col] != 1) {
            col++;
        }
        if (col < N
            && board[row][col] == 1) {
            attacking++;
        }

        // Diagonally to the left up
        // (row and col simultaneously
        // decrease)
        row = state[i] - 1, col = i - 1;
        while (col >= 0 && row >= 0
               && board[row][col] != 1) {
            col--;
            row--;
        }
        if (col >= 0 && row >= 0
            && board[row][col] == 1) {
            attacking++;
        }

        // Diagonally to the right down
        // (row and col simultaneously
        // increase)
        row = state[i] + 1, col = i + 1;
        while (col < N && row < N
               && board[row][col] != 1) {
            col++;
            row++;
        }
        if (col < N && row < N
            && board[row][col] == 1) {
            attacking++;
        }

        // Diagonally to the left down
        // (col decreases and row
        // increases)
        row = state[i] + 1, col = i - 1;
        while (col >= 0 && row < N
               && board[row][col] != 1) {
            col--;
            row++;
        }
        if (col >= 0 && row < N
            && board[row][col] == 1) {
            attacking++;
        }

        // Diagonally to the right up
        // (col increases and row
        // decreases)
        row = state[i] - 1, col = i + 1;
        while (col < N && row >= 0
               && board[row][col] != 1) {
            col++;
            row--;
        }
        if (col < N && row >= 0
            && board[row][col] == 1) {
            attacking++;
        }
    }

    // Return pairs.
    return (int)(attacking / 2);
}

// A utility function that
// generates a board configuration
// given the state.
void generateBoard(int board[][N],
                   int* state)
{

    fill(board, 0);
    for (int i = 0; i < N; i++) {
        board[state[i]][i] = 1;
    }
}

// A utility function that copies
// contents of state2 to state1.
void copyState(int* state1, int* state2)
{

    for (int i = 0; i < N; i++) {
        state1[i] = state2[i];
    }
}

// run local beam search with a fixed number of parallel states
// returns true when we hit objective 0
bool localBeamSearch(int board[][N], int* state) {
    int currentBeamStates[k][N] = {};
    int currentBeamBoards[k][N][N] = {};
    int currentBeamObjectives[k] = {};

    for (int i = 0; i < k; i++) {
        configureRandomly(currentBeamBoards[i], currentBeamStates[i]);
        currentBeamObjectives[i]
            = calculateObjective(currentBeamBoards[i], currentBeamStates[i]);

        if (currentBeamObjectives[i] == 0) {
            copyState(state, currentBeamStates[i]);
            generateBoard(board, state);
            return true;
        }
    }

    int previousBestObjective = *min_element(currentBeamObjectives, currentBeamObjectives + k);
    int noImprovementCount = 0;
    const int PATIENCE = 10;

    for (int iterationNumber = 0; iterationNumber < MAX_ITERATIONS; iterationNumber++) {
        int successorStatesPool[SUCCESSOR_POOL_SIZE][N] = {};
        int successorObjectiveValues[SUCCESSOR_POOL_SIZE] = {};
        bool isSelected[SUCCESSOR_POOL_SIZE] = {};
        int successorCount = 0;

        for (int beamIndex = 0; beamIndex < k; beamIndex++) {
            for (int columnIndex = 0; columnIndex < N; columnIndex++) {
                for (int rowIndex = 0; rowIndex < N; rowIndex++) {
                    if (rowIndex == currentBeamStates[beamIndex][columnIndex]) {
                        continue;
                    }

                    copyState(successorStatesPool[successorCount], currentBeamStates[beamIndex]);
                    successorStatesPool[successorCount][columnIndex] = rowIndex;

                    int candidateBoard[N][N] = {};
                    generateBoard(candidateBoard, successorStatesPool[successorCount]);
                    successorObjectiveValues[successorCount]
                        = calculateObjective(candidateBoard, successorStatesPool[successorCount]);

                    successorCount++;
                }
            }
        }

        // pick top beam states in a deterministic way so ties stay predictable
        for (int beamSlot = 0; beamSlot < k; beamSlot++) {
            int best_index = -1;

            for (int successorIndex = 0; successorIndex < successorCount; successorIndex++) {
                if (isSelected[successorIndex]) {
                    continue;
                }

                if (best_index == -1
                    || successorObjectiveValues[successorIndex]
                        < successorObjectiveValues[best_index]) {
                    best_index = successorIndex;
                }
            }

            if (best_index == -1) {
                break;
            }

            // Check it isn't a duplicate of already selected beams
            bool isDuplicate = false;
            for (int prev = 0; prev < beamSlot; prev++) {
                if (compareStates(successorStatesPool[best_index], currentBeamStates[prev])) {
                    isDuplicate = true;
                    break;
                }
            }
            if (isDuplicate) {
                isSelected[best_index] = true; // skip it
                beamSlot--; // retry this slot
                continue;
            }

            isSelected[best_index] = true;
            copyState(currentBeamStates[beamSlot], successorStatesPool[best_index]);
            generateBoard(currentBeamBoards[beamSlot], currentBeamStates[beamSlot]);
            currentBeamObjectives[beamSlot]
                = successorObjectiveValues[best_index];

            if (currentBeamObjectives[beamSlot] == 0) {
                copyState(state, currentBeamStates[beamSlot]);
                generateBoard(board, state);
                return true;
            }
        }

        // Check for local minimum: no improvement over previous best
        int newBestObjective = *min_element(currentBeamObjectives, currentBeamObjectives + k);
        if (newBestObjective >= previousBestObjective) {
            noImprovementCount++;
            if (noImprovementCount >= PATIENCE) break;
        } else {
            noImprovementCount = 0;
            previousBestObjective = newBestObjective;
        }
    }

    // if no perfect state is found, return the best one we got
    int bestBeamIndex = 0;
    for (int i = 1; i < k; i++) {
        if (currentBeamObjectives[i] < currentBeamObjectives[bestBeamIndex]) {
            bestBeamIndex = i;
        }
    }

    copyState(state, currentBeamStates[bestBeamIndex]);
    generateBoard(board, state);
    return false;
}

// Driver code
int main() {
    srand(time(0) + clock());

    int state[N] = {};
    int board[N][N] = {};

    clock_t startTime = clock();
    bool isSolutionFound = localBeamSearch(board, state);
    clock_t endTime = clock();

    double elapsedTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    int attackingPairs = calculateObjective(board, state);

    if (isSolutionFound) {
        cout << "Solution found using Local Beam Search:\n";
    } else {
        cout << "Local minimum reached. Best board configuration:\n";
    }

    printBoard(board);
    cout << "Number of attacking pairs: " << attackingPairs << "\n";
    cout << "Time taken: " << elapsedTime << " seconds\n";

    return 0;
}