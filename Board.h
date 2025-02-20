#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
#include <vector>

class Board {
    public: 
        static const int SIZE = 22;
        static const int MINES = 100;

        std::vector<std::vector<Cell>> grid;

        Board();
        void generateMines();
        void calculateAdjacentMines();
        void revealCell(int x, int y);
        void printDebug();
        void clearBoard();

};

#endif