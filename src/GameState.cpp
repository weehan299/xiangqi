
// ---------- src/GameState.cpp ----------
#include "GameState.hpp"
#include "CheckPopupState.hpp"
#include "strategy/AlphaBetaStrategy.hpp"
#include <memory>
#include <iostream>

GameState::GameState()
    : ai(Side::Black, std::make_unique<AlphaBetaStrategy>(3))
{
    // Constructor body (if needed)
}

void GameState::handleEvent(const sf::Event &e,
                            sf::RenderWindow &w,
                            StateManager &mgr)
{
    switch (e.type)
    {
    case sf::Event::Closed:
        w.close();
        break;

    case sf::Event::MouseButtonPressed:
        if (e.mouseButton.button == sf::Mouse::Left)
        {
            // Convert pixel → world coords
            sf::Vector2f m = w.mapPixelToCoords({e.mouseButton.x,
                                                      e.mouseButton.y});
            int bx = int(m.x) / CELL_SIZE;
            int by = int(m.y) / CELL_SIZE;
            selected = board.pieceAt(bx, by);
            if (selected)
            {
                originalPos = selected->getBoardPosition();
                // selected->getSprite().getPosition() is where SFML is currently drawing the center (origin) of that piece’s sprite.
                // Subtracting the two gives a 2D vector (dragOffset) from the sprite’s origin to the mouse click point.
                dragOffset = m - selected->getSprite().getPosition();
            }
        }
        break;

    case sf::Event::MouseMoved:
        if (selected)
        {
            sf::Vector2f m = w.mapPixelToCoords({e.mouseMove.x,
                                                      e.mouseMove.y});
            selected->getSprite().setPosition(m - dragOffset);
        }
        break;

    case sf::Event::MouseButtonReleased:
        if (e.mouseButton.button == sf::Mouse::Left && selected)
        {
            sf::Vector2f m = w.mapPixelToCoords({e.mouseButton.x,
                                                      e.mouseButton.y});
            int bx = int(m.x) / CELL_SIZE;
            int by = int(m.y) / CELL_SIZE;
            bool moved = board.movePiece(
                originalPos.x, originalPos.y,
                bx, by);
            if (!moved)
            {
                std::cout << "Illegal move! Snapping back to original position: " <<std::endl;
                // snap back if illegal
                selected->setBoardPosition(
                    originalPos.x, originalPos.y);
            }
            selected = nullptr;
        }
        break;

    default:
        break;
    }
}


void GameState::update(sf::RenderWindow &w,
                       StateManager &mgr,
                       float dt)
{
    if (board.isCheckmate())
    {
        // 3) Push the main menu
        std::cout << "Checkmate! Showing Popup." << std::endl;
        mgr.pushState(std::make_unique<CheckPopupState>());
        return;
    }
    // Run AI if it's AI's turn
    if (board.getCurrentTurn() == Side::Black)
    {
        ai.makeMove(board);

    }
    
}

void GameState::draw(sf::RenderWindow &w)
{
    w.clear(sf::Color::White);
    board.draw(w);
}