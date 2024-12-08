#include <iostream>
#include <string>
#include <vector>
#include "Game.h"
#include "mersenne-twister.h"

using namespace std;

// Initialize new game with empty state
// Sets all number slots to zero and target to zero
// Prepares game for initial setup or manual population
Game :: Game(){
    for(int i = 0; i < NUMBERS_COUNT; i++){
        numbers[i] = 0;
    }
    target = 0;
}

// Construct game with randomly generated numbers
Game :: Game(int numLarge){
    // Populate large and small numbers based on user-specified count
    for(int i = 0; i < NUMBERS_COUNT; i++){
        if (i < numLarge){
            numbers[i] = chooseRandomNumber(1,4) * 25;
        }
        else {
            numbers[i] = chooseRandomNumber(1,10);
        }
    }

    // Generate challenging target number
    target = chooseRandomNumber(101, 999);
}

// Print current game state
// Shows available numbers and the target to be reached
void Game::displayNumbers(){
    cout << "Your numbers are: ";

    // Iterate through numbers
    for (int i = 0; i < NUMBERS_COUNT; i++){
        if (numbers[i] != 0){
            cout << numbers[i] << " ";
        }
    }
    cout << endl;

    cout << "The target is: " << target << endl;
}

// Add a new number to available numbers
// Finds the first empty 4slot and places the number there
void Game::addNumber(int addNum) {
    // Scan for the first available empty slot
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        if (numbers[i] == 0) {
            numbers[i] = addNum;
            return;
        }
    }

    // Error handling for full number array
    cout << "Error: no space for added number!" << endl;
}

// Determine if the game is lost 
// A game is lost when:
// - Target is not achieved
// - Insufficient numbers remain to reach the target
bool Game::lostGame() {
    // Prioritize winning condition
    if (wonGame()) {
        return false;
    }

    // Count remaining available numbers
    int countNumbersRemain = 0;
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        if (numbers[i] != 0) {
            countNumbersRemain++;
        }
    }

    // Lost if 1 or fewer numbers remain
    return (countNumbersRemain <= 1);
}

// Remove a specific number from available numbers
bool Game::removeNumber(int userNumber){
    // search for the number in the array
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        if (numbers[i] == userNumber) {
            numbers[i] = 0;
            return true;
        }
    }

    // Number not found
    return false;
}

// Perform arithmetic operation between two numbers
int Game::performOperation(int num1, char operation, int num2){
    // execute calculation based on operation type
    switch(operation){
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '/':
            return num1 / num2;
        case '*':
            return num1 * num2;
    }

    // Handle invalid operation
    return 0;
}

bool Game::wonGame() {
    // Scan available numbers for target match
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        if (numbers[i] == target) {
            return true;
        }
    }

    // Target not found
    return false;
}

// Determine if the game has reached a terminal state
// @return true if game is won or lost, false if still in progress
bool Game::gameOver() {
    return wonGame() || lostGame();
}

// Recursively solve the game by exploring number combinations
// @param solution Vector to store calculation steps
// @return true if a solution is found, false otherwise
// Approach:
// - Tries all possible number combinations
// - Applies all arithmetic operations
// - Uses recursive backtracking to explore solutions
bool Game :: solveGame(vector <string>& solution){
    // Check if target is already in available numbers
    for(int i = 0; i < NUMBERS_COUNT; i++){
        if(numbers[i] == target){
            return true;        
        }
    }

    // Explore combinations of available numbers
    for (int i = 0; i < NUMBERS_COUNT; i++){
        if(numbers[i] == 0) {
            continue;      // Skip empty slots
        }
        for(int j = 0; j < NUMBERS_COUNT; j++){
            if(numbers[j] == 0 || j == i) {
                continue;  // Avoid using same number or empty slots
            }
            
            // Available arithmetic operations
            char operations[] = {'+', '-', '*', '/'};

            // Try each operation on number pairs
            for (int k = 0; k < 4; k++){
                char operation = operations[k];
                int num1 = numbers[i];
                int num2 = numbers[j];

                // Calculate result of current operation
                int result = performOperation(num1, operation, num2);

                // Create a copy of current game state
                Game copiedGame = *this;

                // Update game state with calculation
                copiedGame.removeNumber(num1);
                copiedGame.removeNumber(num2);
                copiedGame.addNumber(result);

                // Record calculation step
                string calString = to_string(num1) + " " + operation + " " + to_string(num2) + " = " + to_string(result);
                solution.push_back(calString);

                // Recursively attempt to solve with new game state
                if(copiedGame.solveGame(solution)){
                    return true;
                } 

                // Backtrack if no solution found
                solution.pop_back();
            }
        }
    }
    
    // No solution discovered
    return false;
}