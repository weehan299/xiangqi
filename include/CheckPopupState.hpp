
// ---------- src/CheckPopupState.hpp----------
#pragma once
#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>

// forward‐declare so we can dynamic_cast<GameState*>
class GameState;  

struct CheckPopupState : State {
    CheckPopupState();

    void handleEvent(const sf::Event& e,
                     sf::RenderWindow& w,
                     StateManager& mgr) override;

    void update(sf::RenderWindow& w,
                StateManager& mgr,
                float dt) override;

    void draw(sf::RenderWindow& w) override;

  private:
    sf::Font font;
    sf::Text message, yesBtn, noBtn;
    sf::RectangleShape background;
};