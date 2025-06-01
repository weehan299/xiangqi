
// ---------- src/main.cpp ----------
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include <iostream>
#include "AIEngine.hpp"
#include "NetworkManager.hpp"
#include "strategy/AlphaBetaStrategy.hpp"
#include <SFML/Graphics.hpp>
#include "StateManager.hpp"
#include "MenuState.hpp"

int main() {
    sf::RenderWindow window({ COLUMNS * (unsigned)CELL_SIZE,
                        ROWS    * (unsigned)CELL_SIZE },
                    "Xiangqi");
    StateManager mgr;
    mgr.pushState(std::make_unique<MenuState>());

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            mgr.handleEvent(e, window);
        }

        float dt = clock.restart().asSeconds();
        mgr.update(window, dt);

        mgr.draw(window);
        window.display();
    }
    return 0;
}