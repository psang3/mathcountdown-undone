// Author: Pathik Sangani
// Date: December-06-2024
/* Description : A program to enhance the Countdown Math game by adding
 undo functionality using a custom linked list and solving puzzles 
 automatically with recursive algorithms. */



#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Game.h"
#include "mersenne-twister.h"
#include "GameNode.h"
#include "GameList.h"

using namespace std;

// Function prototypes for utility functions used in the main game loop
int promptAndGetNumLarge();
void processCalculation(string userInput, int& num1, char& operation, int& num2);

int main(){
    // Initialize random number generation with a user-provided seed
    int randseed;
    cout << "Enter random seed: " << endl;
    cin >> randseed;
    seed(randseed);

    // Manage game state and history using linked list of game states
    GameList userGame;

    // control variable for multiple game sessions
    string userContinue;
    do {
        // Reset game list for a new game
        userGame.clearGame();

        // Prompt user to select number of large numbers for game
        int numLarge = promptAndGetNumLarge();

        // Initialize a new game with specified num of large numbers
        Game game(numLarge);

        // Add the initialize game state to the game list
        userGame.addToBeginning(game);
        cout << endl;

        // Clear input buffer
        string userInput;
        getline(cin, userInput);

        // Main game loop continues until the game reaches terminal state
        while (!game.gameOver()) {
            // Display current game state
            userGame.displayHeadNodeData();

            // Track the number of game states in the list
            int nodeCount = userGame.countNodes();

            // Prompt for user input with game options
            cout << "Enter a calculation (or U to Undo, S to Solve, or Q to Quit): ";
            getline(cin, userInput);
            
            // Handle game exit option
            if (userInput == "q" || userInput == "Q") {
                cout << "Exiting this game." << endl << endl;
                break;
            }

            // Implement undo functionality by reverting to previous game state
            else if (userInput == "u" || userInput == "U"){
                if(nodeCount > 1){
                    userGame.removeFromBeginning();
                    game = userGame.getLink() -> getGame();
                }
                else{
                    cout << "Cannot undo." << endl << endl;
                }
                continue;
            }

            // Attempt to solve the game automatically
            else if(userInput == "s" || userInput == "S"){
                vector <string> solution;
                if(game.solveGame(solution)){
                    cout << "Solution:" << endl;
                    for (size_t i = 0; i < solution.size();i++){
                        cout << solution[i] << endl;
                    }
                }
                else{
                    cout << "Unable to find a solution" << endl;;
                }
                continue;
            }

            // Parse user's calculation input
            int num1;
            char operation;
            int num2;
            processCalculation(userInput, num1, operation, num2);

            // Validate and process the user's calculation
            if (!game.removeNumber(num1)) {
                cout << num1 << " is not a valid number." << endl;
            }
            else if (!game.removeNumber(num2)) {
                cout << num2 << " is not a valid number." << endl;
                // Restore the first number if the second is invalid
                game.addNumber(num1);
            }
            else {
                // Perform the calculation and display the result
                int result = game.performOperation(num1, operation, num2);
                cout << num1 << " " << operation << " " << num2 << " = " << result << endl << endl;

                // Add the result to available numbers
                game.addNumber(result);

                // Record the new game state in the game list
                userGame.addToBeginning(game);

                // Check for game completion
                if (game.wonGame()) {
                    cout << "You win!" << endl;
                } else if (game.lostGame()) {
                    cout << "You lose." << endl;
                }
            }
        }

        // Prompt for another game session
        cout << endl;
        cout << "Would you like to play again (y/n)? ";
        getline(cin, userContinue);
    } while (userContinue == "y" || userContinue == "Y");
}

// Interactively prompt user to select number of large numbers for the game
// ensures the input is between 0 and 4
// Returns validated number of large numbers
int promptAndGetNumLarge(){
    int numLarge = 0;

    // Introduce game and request large number selection
    cout << "Let's play Countdown!" << endl;
    cout << "How many big numbers would you like (0-4)? ";
    cin >> numLarge;

    // Validate input, repeatedly prompting until valid number is entered
    while(numLarge < 0 || numLarge > 4){
        cout << "That's not a valid number. You must choose between 0 and 4." << endl;
        cout << "How many large numbers would you like? ";
        cin >> numLarge;
    }

    return numLarge;
}

// Parse user's calculation input into constituent parts
// Extracts first number , operation and second number from string input
void processCalculation(string userInput, int& num1, char& operation, int& num2) {
    // Convert the input string to a stream for easy parsing
    stringstream ss(userInput);

    // Extract calculation components from the stream
    ss >> num1 >> operation >> num2;
}