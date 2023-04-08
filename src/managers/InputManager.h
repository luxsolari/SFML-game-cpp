//
// Created by Lux Solari on 06/03/2023.
//

#ifndef CMAKESFMLPROJECT_INPUTMANAGER_H
#define CMAKESFMLPROJECT_INPUTMANAGER_H

#include <SFML/Graphics.hpp>

class InputManager {
private:
    sf::RenderWindow& m_window;
    sf::Vector2i m_mousePosition;
    bool m_keysPressed[sf::Keyboard::KeyCount]{};

public:
    static InputManager& getInstance();
    InputManager(const InputManager&) = delete;
    InputManager(const InputManager&&) = delete;
    void operator=(const InputManager&) = delete;
    void operator=(const InputManager&&) = delete;

    void update(const sf::Event& event);
    void handleEvent(const sf::Event& event);

    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    bool isMouseMoving() const;
    sf::Vector2i getMousePosition() const;

    void setKeyPressed(sf::Keyboard::Key key);
    void setKeyReleased(sf::Keyboard::Key key);

private:
    InputManager();
    ~InputManager() = default;

};


#endif //CMAKESFMLPROJECT_INPUTMANAGER_H
