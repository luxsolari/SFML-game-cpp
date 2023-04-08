//
// Created by Lux Solari on 05/03/2023.
//

#ifndef CMAKESFMLPROJECT_STATE_H
#define CMAKESFMLPROJECT_STATE_H
#include <utility>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "WindowManager.h"
#include "InputManager.h"
#include "EventManager.h"

class State {
private:
    bool m_isRunning = false;
public:
    State() = default;
    virtual ~State() = default;

    // Copy constructor
    State(const State& other) = delete;
    State(State&& other) noexcept : m_isRunning(std::exchange(other.m_isRunning, false)) {}

    // Copy assignment operator
    State& operator=(const State& other) {
        if (this != &other) {
            m_isRunning = other.m_isRunning;
        }
        return *this;
    }

    // Move assignment operator 
    State& operator=(State&& other) noexcept {
        if (this != &other) {
            m_isRunning = std::exchange(other.m_isRunning, false);
        }
        return *this;
    }
    
    virtual void start()  = 0;
    virtual void handleInput(InputManager& inputManager) = 0;
    virtual void handleInput(EventManager& eventManager) = 0;
    virtual void update(sf::Time deltaTime) = 0;
    virtual void render(sf::RenderWindow& renderWindow) = 0;
    virtual void stop()   = 0;

    [[nodiscard]] bool isRunning() const {
        return this->m_isRunning;
    }
    void setIsRunning(const bool running) {
        this->m_isRunning = running;
    }
};

#endif //CMAKESFMLPROJECT_STATE_H
