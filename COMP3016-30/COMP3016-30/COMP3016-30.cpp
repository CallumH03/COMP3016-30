#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

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

int main() {
    std::string file_path = "Maze.txt";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << file_path << std::endl;
        return 1;
    }

    std::vector<std::string> maze;
    std::string line;

    while (std::getline(file, line)) {
        maze.push_back(line);
    }

    file.close();

    int playerRow, playerCol;
    findPlayerPosition(maze, playerRow, playerCol);

    int keyRow = -1, keyCol = -1;
    int doorRow{}, doorCol{};
    bool hasKey = false;

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
            std::cout << "Congratulations! You opened the door and completed the level!" << std::endl;
            return 0;
        }
    }

    return 0;
}