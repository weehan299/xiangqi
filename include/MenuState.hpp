// -------- MenuState.hpp --------
#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>

// Forward‐declare StateManager and GameState to avoid circular includes
class StateManager;
class GameState;

struct MenuState : State {
    sf::Font font;
    sf::Text title;
    sf::Text playBtn;

    MenuState();

    void handleEvent(const sf::Event& e, sf::RenderWindow& w, StateManager& mgr) override;
    void update(sf::RenderWindow& w, StateManager& mgr, float dt) override;
    void draw(sf::RenderWindow& w) override;
};