#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>

class Game {
    private:
        sf::RenderWindow window;
        Board board;
        bool firstClick;
        void handleFirstClick(int x, int y);
        sf::Font font;
        int remainingMines = board.MINES;

    public:
        Game();
        void run();
        void processEvents();
        void render();
        void handleMiddleClick(int x, int y);
        void floodFillReveal(int x, int y);
};

#endif