// -------- MenuState.cpp --------
#include "MenuState.hpp"
#include "StateManager.hpp"
#include "GameState.hpp"    // needed for std::make_unique<GameState>()
#include <iostream>

MenuState::MenuState() {
    // Load font
    font.loadFromFile("assets/arial.ttf");

    // Set up title text
    title.setFont(font);
    title.setString("Xiangqi");
    title.setCharacterSize(48);
    title.setPosition(100.f, 100.f);

    // Set up "Play" button text
    playBtn.setFont(font);
    playBtn.setString("Play");
    playBtn.setCharacterSize(24);
    playBtn.setPosition(120.f, 200.f);
    std::cout << "MenuState initialized" << std::endl;
}

void MenuState::handleEvent(const sf::Event& e, sf::RenderWindow& w, StateManager& mgr) {
    if (e.type == sf::Event::MouseButtonPressed &&
        e.mouseButton.button == sf::Mouse::Left)
    {
        // Convert mouse pixel to world coordinates
         sf::Vector2f m = w.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });
        if (playBtn.getGlobalBounds().contains(m)) {
            // Switch to the game state
            mgr.pushState(std::make_unique<GameState>());
        }
    }
}

void MenuState::update(sf::RenderWindow& /*w*/, StateManager& /*mgr*/, float /*dt*/) {
    // No update logic for menu
}

void MenuState::draw(sf::RenderWindow& w) {
    w.clear(sf::Color::Black);
    w.draw(title);
    w.draw(playBtn);
}