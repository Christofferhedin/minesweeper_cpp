#include "Board.h"
#include <iostream>
#include <ctime>

Board::Board() {
    // inilaize empty grid
    for (int y = 0; y < SIZE; y++) {
        std::vector<Cell> row;
        for (int x = 0; x < SIZE; x++) {
            row.emplace_back(x, y); // initilaize each cell
        }
        grid.push_back(row);
    }
    generateMines();
    calculateAdjacentMines();
}

void Board::generateMines() {
    srand(time(nullptr)); // Seed random number generator
    int placed = 0;
    while (placed < MINES) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        if (!grid[y][x].isMine) {
            grid[y][x].isMine = true;
            placed++;
        }
    }
}

void Board::calculateAdjacentMines() {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if(grid[y][x].isMine) {
                continue;
            }
            int count = 0; 
            for(int dy = -1; dy <= 1; dy++) {
                for(int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && ny >= 0 && nx < SIZE && ny < SIZE && grid[ny][nx].isMine) {
                        count++;
                    }
                }
            }
            grid[y][x].adjecentMines = count;
        }
    }
}

void Board::printDebug() {
    for(const auto& row : grid) { // ???
        for (const auto& cell : row) {
            if (cell.isMine) {
                std::cout << "* ";
            } else {
                std::cout << cell.adjecentMines << " ";
            }
            std::cout << "\n";
        }
    }
}

void Board::clearBoard() {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            grid[y][x].isMine = false;
            grid[y][x].adjecentMines = 0;
            grid[y][x].revealed = false;
            grid[y][x].flagged = false;
        }
    }
}