//Libraries (All C++ Standard Library)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <limits>

//Function for game menu
void displaymenu() {
    std::cout << "Welcome to Maze Expand" << std::endl;
    std::cout << "You are represented by 'P' on the maze." << std::endl;
    std::cout << "Use W/A/S/D to move (Up/Left/Down/Right)." << std::endl;
    std::cout << "Try to find your way to the key ('K') to open the door ('D')" << std::endl;
    std::cout << "You cannot go through walls ('#')" << std::endl;
    std::cout << "There are 10 levels to complete" << std::endl;
    std::cout << "Type 'Q' to quit the game at any time." << std::endl;
    std::cout << "Type 'Play' to start" << std::endl;
}

//Function to display the current state of the maze
void displayMaze(const std::vector<std::string>& maze) {
    for (const auto& line : maze) {
        std::cout << line << std::endl;
    }
}

//Function to find the position of the player on the maze
void findPlayerPosition(const std::vector<std::string>& maze, int& playerRow, int& playerCol) {
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == 'P') {
                playerRow = i;
                playerCol = j;
                return;
            }
        }
    }
}

//Function to handle player movement in the maze
void handleMove(std::vector<std::string>& maze, int& playerRow, int& playerCol, int newRow, int newCol) {
    maze[playerRow][playerCol] = ' ';
    maze[newRow][newCol] = 'P';
    playerRow = newRow;
    playerCol = newCol;
}

//Funcxtion to clear the console
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//Function to load the maze from the file
bool loadMaze(const std::string& file_path, std::vector<std::string>& maze, int& keyRow, int& keyCol, int& doorRow, int& doorCol) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << file_path << std::endl;
        return false;
    }

    maze.clear();

    std::string line;
    while (std::getline(file, line)) {
        maze.push_back(line);
    }

    file.close();

    //Finds position of the door
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == 'D') {
                doorRow = i;
                doorCol = j;
            }
            //Finds position of the key
            else if (maze[i][j] == 'K') {
                keyRow = i;
                keyCol = j;
            }
        }
    }

    return true;
}


int main() {
    int currentLevel = 1;
    std::vector<std::string> maze;
    int playerRow, playerCol;
    int keyRow, keyCol;
    int doorRow, doorCol;
    bool hasKey = false;

    //Displays the menu and gets the users input
    std::string startCommand;
    displaymenu();

    while (true) {
        std::cin >> startCommand;

        //Converts the users input to lowercase
        std::transform(startCommand.begin(), startCommand.end(), startCommand.begin(), ::tolower);

        //Checks if user has inputted play
        if (startCommand == "play") {
            //Loads maze depending on current level
            std::string file_path = "Maze" + std::to_string(currentLevel) + ".txt";
            if (!loadMaze(file_path, maze, keyRow, keyCol, doorRow, doorCol)) {
                std::cerr << "Error loading maze for level " << currentLevel << std::endl;
                return 1;
            }

            //Finds the initial position of the player
            findPlayerPosition(maze, playerRow, playerCol);
            break;
        }
        else if (startCommand == "quit" || startCommand == "q") {
            std::cout << "Successfully quit the game" << std::endl;
            return 0;
        }
        else {
            std::cout << "Invalid command. Type 'play' to start or 'quit' to exit: ";
        }
    }

    //Main Game Loop
    std::string move;
    while (true) {
        //Clear console and displays current state of the maze
        clearConsole();
        displayMaze(maze);

        //Checks if the user has a key if so displays message
        if (hasKey) {
            std::cout << "You have a key!" << std::endl;
        }

        //Instructions, gets the players input
        std::cout << "Enter move (W/A/S/D to move, Q to quit): ";
        std::getline(std::cin, move);


        //Proccesses the players move if it is one character (error handling)
        if (move.length() == 1) {
            char direction = move[0];

            switch (direction) {
            //Move upwards if the new position is valid
            case 'W':
            case 'w':
                if (playerRow - 1 >= 0 && maze[playerRow - 1][playerCol] != '#' && maze[playerRow - 1][playerCol] != 'D') {
                    handleMove(maze, playerRow, playerCol, playerRow - 1, playerCol);
                }
                else if (playerRow - 1 >= 0 && maze[playerRow - 1][playerCol] == 'D' && hasKey) {
                    handleMove(maze, playerRow, playerCol, playerRow - 1, playerCol);
                }
                break;
            //Move left if the new position is valid
            case 'A':
            case 'a':
                if (playerCol - 1 >= 0 && maze[playerRow][playerCol - 1] != '#' && maze[playerRow][playerCol - 1] != 'D') {
                    handleMove(maze, playerRow, playerCol, playerRow, playerCol - 1);
                }
                else if (playerCol - 1 >= 0 && maze[playerRow][playerCol - 1] == 'D' && hasKey) {
                    handleMove(maze, playerRow, playerCol, playerRow, playerCol - 1);
                }
                break;
            //Move downwards if the new position is valid
            case 'S':
            case 's':
                if (playerRow + 1 < maze.size() && maze[playerRow + 1][playerCol] != '#' && maze[playerRow + 1][playerCol] != 'D') {
                    handleMove(maze, playerRow, playerCol, playerRow + 1, playerCol);
                }
                else if (playerRow + 1 < maze.size() && maze[playerRow + 1][playerCol] == 'D' && hasKey) {
                    handleMove(maze, playerRow, playerCol, playerRow + 1, playerCol);
                }
                break;
            //Move right if the new position is valid
            case 'D':
            case 'd':
                if (playerCol + 1 < maze[playerRow].size() && maze[playerRow][playerCol + 1] != '#' && maze[playerRow][playerCol + 1] != 'D') {
                    handleMove(maze, playerRow, playerCol, playerRow, playerCol + 1);
                }
                else if (playerCol + 1 < maze[playerRow].size() && maze[playerRow][playerCol + 1] == 'D' && hasKey) {
                    handleMove(maze, playerRow, playerCol, playerRow, playerCol + 1);
                }
                break;
            //Quit the game
            case 'Q':
            case 'q':
                std::cout << "Successfully quit the game" << std::endl;
                return 0;
            }
        }


        //Checks if the player has gone onto Key
        if (playerRow == keyRow && playerCol == keyCol) {
            hasKey = true;
            maze[keyRow][keyCol] = 'P';
        }

        //Checks if the player has reached the door and has the key
        if (hasKey && playerRow == doorRow && playerCol == doorCol) {
            //Checks to see if they have completed the final level
            if (currentLevel == 10) {
                std::cout << "Congratulations! You completed the game!" << std::endl;
                std::cout << "Press Enter to close the game...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return 0;
            }
            else
                std::cout << "Congratulations! You opened the door and completed the level " << currentLevel << "!" << std::endl;

            //Move to the next level
            ++currentLevel;

            //Load the maze for the next level
            std::string nextLevelPath = "Maze" + std::to_string(currentLevel) + ".txt";
            if (!loadMaze(nextLevelPath, maze, keyRow, keyCol, doorRow, doorCol)) {
                std::cerr << "Error loading maze for level " << currentLevel << std::endl;
                return 1;
            }

            //Find the initial position of the player in the new maze
            findPlayerPosition(maze, playerRow, playerCol);
            //Reset key status for new level
            hasKey = false;
        }
    }

    return 0;
}