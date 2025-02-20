#include "Game.h"
#include <iostream>
#include <queue>

Game::Game() : window(sf::VideoMode(900, 1000), "Minesweeper") { // each cell is 40, so 40 * 22 = 880
    font.loadFromFile("arial.ttf");
}

void Game::run() {
    while (window.isOpen()) {
       processEvents();
       render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // get the mouse position
                std::cout << "Left click at: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
                int x = event.mouseButton.x / 40; // Cell size (40) to convert screen coordinates to grid
                int y = event.mouseButton.y / 40; 

                if (!firstClick) {
                    handleFirstClick(x, y);
                    firstClick = true;
                }

                if (!board.grid[y][x].flagged) {
                    board.grid[y][x].reveal();
                }
            }

            if (event.mouseButton.button == sf::Mouse::Right) {
                int x = event.mouseButton.x / 40; // Cell size (40) to convert screen coordinates to grid
                int y = event.mouseButton.y / 40; 
                Cell& cell = board.grid[y][x];
                if (!cell.flagged) {
                    remainingMines--;
                } else {
                    remainingMines++;
                }
                cell.toggleFlag();
            }    
        

            if (event.mouseButton.button == sf::Mouse::Middle) {
                int x = event.mouseButton.x / 40; // Cell size (40) to convert screen coordinates to grid
                int y = event.mouseButton.y / 40; 
                handleMiddleClick(x, y);
            }
        }
    }
}

void Game::handleFirstClick(int x, int y) {
    firstClick = true;
    // clear board then place the bombs
    board.clearBoard();

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && ny >= 0 && nx < board.SIZE && ny < board.SIZE) {
                board.grid[ny][nx].isMine = false; // Ensure no bombs on first click
            }
        }
    }
    // now place the mines randomly
    int minesPlaced = 0;
    while (minesPlaced < board.MINES) {
        int mx = rand() % board.SIZE;
        int my = rand() % board.SIZE;
        
        // only place mine if the cell is not already a mine
        if(!board.grid[my][mx].isMine && (abs(mx - x) > 1 || abs(my-y) > 1)) {
            board.grid[my][mx].isMine = true;
            minesPlaced++;
        }
    }
   // update the number of adjacent mines for all cells
    for (int y = 0; y < board.SIZE; ++y) {
        for (int x = 0; x < board.SIZE; ++x) {
            int adjacentMines = 0;
            // check adjacent cells for mines
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && ny >= 0 && nx < board.SIZE && ny < board.SIZE) {
                        if (board.grid[ny][nx].isMine) {
                            adjacentMines++;
                        }
                    }
                }
            }

            board.grid[y][x].adjecentMines = adjacentMines;

        }
    }

    floodFillReveal(x, y);
    // reval some cells around the first click
    // for (int dy = -1; dy <= 1; dy++) {
    //     for (int dx = -1; dx <= 1; dx++) {
    //         int nx = x + dx;
    //         int ny = y + dy;

    //         if (nx >= 0 && ny >= 0 && nx < board.SIZE && ny < board.SIZE) {
    //             board.grid[ny][nx].reveal();
    //         }
    //     }
    // }
}

void Game::floodFillReveal(int x, int y) {
    std::queue<std::pair<int, int>> toReveal;
    toReveal.push({x, y});
    
    while (!toReveal.empty()) {
        auto [cx, cy] = toReveal.front();
        toReveal.pop();

        if (cx < 0 || cy < 0 || cx >= board.SIZE || cy >= board.SIZE) {
            continue;
        }
        Cell& cell = board.grid[cy][cx];

        if (cell.revealed || cell.flagged) {
            continue;
        }
        cell.reveal();

        if (cell.adjecentMines == 0) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx != 0 || dy != 0) {
                        toReveal.push({cx + dx, cy + dy});
                    }
                }
            }
        }
    }
}

void Game::handleMiddleClick(int x, int y) {
    Cell& cell = board.grid[y][x];
    // only allow middle-click if the cell has been revealed and shows a number
    if (cell.revealed && cell.adjecentMines > 0) {
        int flagcount = 0;
        
        // count how many adjecent cell are flagged
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                int nx = x + dx;
                int ny = y + dy;

                if (nx >= 0 && nx < board.SIZE && ny >= 0 && ny < board.SIZE) {
                    if (board.grid[ny][nx].flagged) {
                        flagcount++;
                    }
                }
            }
        }

        // if the number of flagged cells around the current cell matches the adjacent mines number, then reveal the rest
        if (flagcount == cell.adjecentMines) {
            for (int dy = -1; dy <= 1; dy++) {
                for(int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < board.SIZE && ny >= 0 && ny < board.SIZE) {
                        Cell& adjacentCell = board.grid[ny][nx];
                        if (!adjacentCell.revealed && !adjacentCell.flagged) {
                            adjacentCell.reveal();
                            if (adjacentCell.adjecentMines == 0) {
                                handleMiddleClick(nx, ny);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Game::render() {
    window.clear(sf::Color::White);
    float cellSize = 40.0f;

    for (int y = 0; y < board.SIZE; y++) {
        for (int x = 0; x < board.SIZE; x++) {
            Cell& cell = board.grid[y][x];

            // Create a rectangle for the cell
            sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
            rect.setPosition(x * cellSize, y * cellSize); // position based on grid

            // set color based on the cells state
            if (cell.revealed) {
                if(cell.isMine && cell.isMine) {
                    rect.setFillColor(sf::Color::Red); // mines are currently red
                } else if (cell.revealed) {
                    if (cell.adjecentMines == 0) {
                        rect.setFillColor(sf::Color(200, 200, 200)); // Empty cell (no adjacent mines)
                    } else {
                        rect.setFillColor(sf::Color(200, 200, 255));
                    }
                }
            } else {
                rect.setFillColor(sf::Color(169, 169, 169)); // unrevealed cells are gray
            }

            // Add an outline to each cell
            rect.setOutlineColor(sf::Color::Black); // Outline color (black)
            rect.setOutlineThickness(1); // Outline thickness

            window.draw(rect);

            // Draw the flag
            if (cell.flagged && !cell.revealed) {
                sf::Text flagText("F", font, 20);
                flagText.setPosition(x * cellSize + cellSize  / 4, y * cellSize + cellSize  / 4);
                flagText.setFillColor(sf::Color::Black);
                window.draw(flagText);
            }

            if (cell.revealed && cell.adjecentMines > 0 && !cell.isMine) {
                // font.loadFromFile("arial.ttf"); // Ensure you have this font available or load your own
                sf::Text text(std::to_string(cell.adjecentMines),font, 20);
                text.setPosition(x * cellSize + cellSize  / 4, y * cellSize + cellSize  / 4);
                text.setFillColor(sf::Color::Black);
                window.draw(text);
            }
        }
    }
    // display remaining mines in top right corner
    sf::Text mineCountText("Mines Left: " + std::to_string(remainingMines), font, 24);
    mineCountText.setPosition(700,10);
    mineCountText.setFillColor(sf::Color::Black);
    window.draw(mineCountText);

    window.display();
}