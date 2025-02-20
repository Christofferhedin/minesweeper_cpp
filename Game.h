#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <queue>
#include <fstream>

class Game {
    private:
        sf::RenderWindow window;
        Board board;
        bool firstClick;
        bool gameOver;
        bool gameWon;
        bool timerStarted = false;
        int bestTime;
        void handleFirstClick(int x, int y);
        sf::Font font;
        sf:: Clock clock;
        int remainingMines = board.MINES;

    public:
        Game();
        void run();
        void processEvents();
        void render();
        void handleMiddleClick(int x, int y);
        void floodFillReveal(int x, int y);
        void checkWin();
        void saveBestTime(int x);
        void loadBestTime();
        void restartGame();
};

#endif