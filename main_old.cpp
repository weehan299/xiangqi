

// ---------- src/main.cpp ----------
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include <iostream>
#include "AIEngine.hpp"
#include "NetworkManager.hpp"
#include "strategy/RandomStrategy.hpp"
#include "strategy/MinimaxStrategy.hpp"
#include "strategy/AlphaBetaStrategy.hpp"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window({ COLUMNS * (unsigned)CELL_SIZE,
                            ROWS    * (unsigned)CELL_SIZE },
                        "Xiangqi");

    sf::View boardView({0.f, 0.f, COLUMNS * CELL_SIZE, ROWS * CELL_SIZE});
    window.setView(boardView);
    Board board;
    // Create AI with random strategy
    AIEngine ai(Side::Black, std::make_unique<RandomStrategy>());
    // Later, switch to minimax of depth 3
    ai.setStrategy(std::make_unique<AlphaBetaStrategy>(3));
    //NetworkManager net;

    // ------ drag and drop state -------
    Piece* selected = nullptr;
    sf::Vector2f dragOffset;
    sf::Vector2i originalPos;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        // Convert pixel → world coords
                        sf::Vector2f m = window.mapPixelToCoords({
                            event.mouseButton.x,
                            event.mouseButton.y
                        });
                        int bx = int(m.x) / CELL_SIZE;
                        int by = int(m.y) / CELL_SIZE;
                        selected = board.pieceAt(bx, by);
                        if (selected) {
                            originalPos = selected->getBoardPosition();
                            // selected->getSprite().getPosition() is where SFML is currently drawing the center (origin) of that piece’s sprite.
                            // Subtracting the two gives a 2D vector (dragOffset) from the sprite’s origin to the mouse click point.
                            dragOffset = m - selected->getSprite().getPosition();
                        }
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (selected) {
                        sf::Vector2f m = window.mapPixelToCoords({
                            event.mouseMove.x,
                            event.mouseMove.y
                        });
                        selected->getSprite().setPosition(m - dragOffset);
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left && selected) {
                        sf::Vector2f m = window.mapPixelToCoords({
                            event.mouseButton.x,
                            event.mouseButton.y
                        });
                        int bx = int(m.x) / CELL_SIZE;
                        int by = int(m.y) / CELL_SIZE;
                        bool moved = board.movePiece(
                            originalPos.x, originalPos.y,
                            bx, by
                        );
                        if (!moved) {
                            // snap back if illegal
                            selected->setBoardPosition(
                                originalPos.x, originalPos.y
                            );
                        }
                        selected = nullptr;
                    }
                    break;

                default:
                    break;
            }
        }
        // so let AI reply here:
        if (board.getCurrentTurn() == Side::Black) {
            ai.makeMove(board);
        }

        window.clear(sf::Color::White);
        board.draw(window);
        window.display();
    }
    return 0;
}
