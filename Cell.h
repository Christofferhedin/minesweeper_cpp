#ifndef CELL_H
#define CELL_H

class Cell {
    public:
        int x;
        int y;
        bool isMine;
        int adjecentMines;
        bool revealed;
        bool flagged;

        Cell(int x, int y);
        void reveal();
        void toggleFlag();
};

#endif