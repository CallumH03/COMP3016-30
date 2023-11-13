#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

    char move;
    while (true) {
        displayMaze(maze);

        std::cout << "Enter move (W/A/S/D to move, Q to quit): ";
        std::cin >> move;



        switch (move) {
        case 'W':
        case 'w':
            if (maze[playerRow - 1][playerCol] != '#') {
                maze[playerRow][playerCol] = ' ';
                maze[--playerRow][playerCol] = 'P';
            }
            break;
        case 'A':
        case 'a':
            if (maze[playerRow][playerCol - 1] != '#') {
                maze[playerRow][playerCol] = ' ';
                maze[playerRow][--playerCol] = 'P';
            }
            break;
        case 'S':
        case 's':
            if (maze[playerRow + 1][playerCol] != '#') {
                maze[playerRow][playerCol] = ' ';
                maze[++playerRow][playerCol] = 'P';
            }
            break;
        case 'D':
        case 'd':
            if (maze[playerRow][playerCol + 1] != '#') {
                maze[playerRow][playerCol] = ' ';
                maze[playerRow][++playerCol] = 'P';
            }
            break;
        case 'Q':
        case 'q':
            return 0;
        default:
            std::cout << "Invalid move. Try again." << std::endl;
        }
    }

    return 0;
}