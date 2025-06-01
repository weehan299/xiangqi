
#pragma once
#include "State.hpp"
#include <vector>
#include <memory>

class StateManager {
public:
    void pushState(std::unique_ptr<State> s)  { stack_.push_back(std::move(s)); }
    void popState()                           { if (!stack_.empty()) stack_.pop_back(); }
    State* topState() const                   { return stack_.empty() ? nullptr : stack_.back().get(); }
    
    void handleEvent(const sf::Event& e, sf::RenderWindow& w) {
        if (State* s = topState()) s->handleEvent(e, w, *this);
    }
    void update(sf::RenderWindow& w, float dt) {
        if (State* s = topState()) s->update(w, *this, dt);
    }
    void draw(sf::RenderWindow& w) {
        if (State* s = topState()) s->draw(w);
    }
private:
    std::vector<std::unique_ptr<State>> stack_;
};