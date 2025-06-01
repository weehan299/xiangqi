// ---------- src/GameState.cpp ----------

#pragma once
#include "State.hpp"
#include "Board.hpp"
#include "AIEngine.hpp"

// Forward declaration is fine here since we only store a pointer/reference
class StateManager;

class GameState : public State {
public:
    GameState();
    
    void handleEvent(const sf::Event& e, sf::RenderWindow& w, StateManager& mgr) override;
    void update(sf::RenderWindow& w, StateManager& mgr, float dt) override;
    void draw(sf::RenderWindow& w) override;
    
    Board board;
    bool showCheckPopup = false;
    
private:
    AIEngine ai;
    // ------ drag and drop state -------
    Piece* selected = nullptr;
    sf::Vector2f dragOffset;
    sf::Vector2i originalPos;
};