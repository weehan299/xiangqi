// ---------- src/CheckPopupState.cpp ----------

#include "CheckPopupState.hpp"
#include "GameState.hpp"      
#include "MenuState.hpp"     
#include <SFML/Graphics.hpp>    // (may already be pulled in by the header)

// ctor: load font, set up text & background
CheckPopupState::CheckPopupState() {
    font.loadFromFile("assets/arial.ttf");
    message.setFont(font);
    message.setString("Checkmate!\nReset game?");
    message.setCharacterSize(24);
    message.setPosition(150, 200);

    yesBtn.setFont(font);
    yesBtn.setString("Yes");
    yesBtn.setCharacterSize(20);
    yesBtn.setPosition(170, 300);

    noBtn.setFont(font);
    noBtn.setString("Return to Menu");
    noBtn.setCharacterSize(20);
    noBtn.setPosition(270, 300);

    background.setSize({300, 200});
    background.setFillColor({0, 0, 0, 150});
    background.setPosition(140, 180);
}

void CheckPopupState::handleEvent(const sf::Event& e,
                                  sf::RenderWindow& w,
                                  StateManager& mgr)
{
    if (e.type == sf::Event::MouseButtonPressed &&
        e.mouseButton.button == sf::Mouse::Left)
    {
        auto m = w.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});

        // If “Reset” was clicked:
        if (yesBtn.getGlobalBounds().contains(m)) {
            // 1) Pop THIS popup
            mgr.popState();
            // 2) Pop the old GameState
            mgr.popState();
            // 3) Push a brand‐new GameState
            mgr.pushState(std::make_unique<GameState>());
        }
        // If “Main Menu” was clicked:
        else if (noBtn.getGlobalBounds().contains(m)) {
            mgr.popState();
            mgr.popState();
            mgr.pushState(std::make_unique<MenuState>());
        }
    }
}

void CheckPopupState::update(sf::RenderWindow&,
                             StateManager&,
                             float)
{
    // nothing to animate/update here
}

void CheckPopupState::draw(sf::RenderWindow& w) {
    w.draw(background);
    w.draw(message);
    w.draw(yesBtn);
    w.draw(noBtn);
}