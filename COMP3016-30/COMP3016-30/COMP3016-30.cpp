#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

void displaymenu() {
    std::cout << "Welcome to Maze Expand" << std::endl;
    std::cout << "You are represented by 'P' on the maze." << std::endl;
    std::cout << "Use W/A/S/D to move (Up/Left/Down/Right)." << std::endl;
    std::cout << "Try to find your way to the key ('K') to open the door ('D')" << std::endl;
    std::cout << "You cannot go through walls ('#')" << std::endl;
    std::cout << "Type 'Q' to quit the game at any time." << std::endl;
    std::cout << "Type 'Play' to start" << std::endl;
}

void displayMaze(const std::vector<std::string>& maze) {
    for (const auto& line : maze) {
        std::cout << line << std::endl;
    }
}

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

void handleMove(std::vector<std::string>& maze, int& playerRow, int& playerCol, int newRow, int newCol) {
    maze[playerRow][playerCol] = ' ';
    maze[newRow][newCol] = 'P';
    playerRow = newRow;
    playerCol = newCol;
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

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

    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == 'D') {
                doorRow = i;
                doorCol = j;
            }
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

    std::string startCommand;
    displaymenu();

    while (true) {
        std::cin >> startCommand;

        std::transform(startCommand.begin(), startCommand.end(), startCommand.begin(), ::tolower);

        if (startCommand == "play") {
            std::string file_path = "Maze" + std::to_string(currentLevel) + ".txt";
            if (!loadMaze(file_path, maze, keyRow, keyCol, doorRow, doorCol)) {
                std::cerr << "Error loading maze for level " << currentLevel << std::endl;
                return 1;
            }

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

    char move;
    while (true) {
        clearConsole();
        displayMaze(maze);

        if (hasKey) {
            std::cout << "You have a key!" << std::endl;
        }

        std::cout << "Enter move (W/A/S/D to move, Q to quit): ";
        std::cin >> move;

        switch (move) {
        case 'W':
        case 'w':
            if (maze[playerRow - 1][playerCol] != '#' && maze[playerRow - 1][playerCol] != 'D') {
                handleMove(maze, playerRow, playerCol, playerRow - 1, playerCol);
            }
            else if (maze[playerRow - 1][playerCol] == 'D' && hasKey) {
                handleMove(maze, playerRow, playerCol, playerRow - 1, playerCol);
            }
            break;
        case 'A':
        case 'a':
            if (maze[playerRow][playerCol - 1] != '#' && maze[playerRow][playerCol - 1] != 'D') {
                handleMove(maze, playerRow, playerCol, playerRow, playerCol - 1);
            }
            else if (maze[playerRow][playerCol - 1] == 'D' && hasKey) {
                handleMove(maze, playerRow, playerCol, playerRow, playerCol - 1);
            }
            break;
        case 'S':
        case 's':
            if (maze[playerRow + 1][playerCol] != '#' && maze[playerRow + 1][playerCol] != 'D') {
                handleMove(maze, playerRow, playerCol, playerRow + 1, playerCol);
            }
            else if (maze[playerRow + 1][playerCol] == 'D' && hasKey) {
                handleMove(maze, playerRow, playerCol, playerRow + 1, playerCol);
            }
            break;
        case 'D':
        case 'd':
            if (maze[playerRow][playerCol + 1] != '#' && maze[playerRow][playerCol + 1] != 'D') {
                handleMove(maze, playerRow, playerCol, playerRow, playerCol + 1);
            }
            else if (maze[playerRow][playerCol + 1] == 'D' && hasKey) {
                handleMove(maze, playerRow, playerCol, playerRow, playerCol + 1);
            }
            break;
        case 'Q':
        case 'q':
            std::cout << "Successfully quit the game" << std::endl;
            return 0;
        default:
            std::cout << "Invalid move. Try again." << std::endl;
        }

        if (playerRow == keyRow && playerCol == keyCol) {
            hasKey = true;
            maze[keyRow][keyCol] = 'P';
        }

        if (hasKey && playerRow == doorRow && playerCol == doorCol) {
            std::cout << "Congratulations! You opened the door and completed level " << currentLevel << "!" << std::endl;
            ++currentLevel;

            std::string nextLevelPath = "Maze" + std::to_string(currentLevel) + ".txt";
            if (!loadMaze(nextLevelPath, maze, keyRow, keyCol, doorRow, doorCol)) {
                std::cerr << "Error loading maze for level " << currentLevel << std::endl;
                return 1;
            }

            findPlayerPosition(maze, playerRow, playerCol);
            hasKey = false;
        }
    }

    return 0;
}