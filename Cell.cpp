#include "Cell.h"

Cell::Cell(int x, int y) : x(x), 
y(y), 
isMine(false),
adjecentMines(0),
revealed(false),
flagged(false) {}

void Cell::reveal() {
    if (!flagged) {
        revealed = true;
        
    }

}

void Cell::toggleFlag() {
    if(!revealed) {
        flagged = !flagged;
    }
}