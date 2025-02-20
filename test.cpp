#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

const int SIZE = 10;
const int MINES = 10;


int main() {

    sf:: RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::White);
        window.display();
    }
    return 0;
}