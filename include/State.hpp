
#pragma once
#include <SFML/Graphics.hpp>

class StateManager;  // forward

struct State {
    virtual ~State() = default;
    virtual void handleEvent(const sf::Event&, sf::RenderWindow&, StateManager&) = 0;
    virtual void update          (sf::RenderWindow&, StateManager&, float dt) = 0;
    virtual void draw            (sf::RenderWindow&)                       = 0;
};