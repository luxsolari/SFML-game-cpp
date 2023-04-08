//
// Created by Lux Solari on 06/03/2023.
//

#include <iostream>
#include "InputManager.h"
#include "WindowManager.h"

InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
        : m_window(WindowManager::getInstance().getWindow())
{
    for (bool& key : m_keysPressed) {
        key = false;
    }
}

void InputManager::update(const sf::Event& event)
{
    handleEvent(event);
}

void InputManager::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            m_keysPressed[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            m_keysPressed[event.key.code] = false;
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // Do something when left mouse button is pressed
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                // Do something when right mouse button is pressed
            }
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                // Do something when middle mouse button is pressed
            }
            break;
        case sf::Event::MouseMoved:
            m_mousePosition.x = event.mouseMove.x;
            m_mousePosition.y = event.mouseMove.y;
            break;
        default:
            break;
    }
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
    return m_keysPressed[key];
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button) const
{
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i InputManager::getMousePosition() const
{
    return sf::Mouse::getPosition(m_window);
}

bool InputManager::isMouseMoving() const
{
    return sf::Mouse::getPosition() != m_mousePosition;
}

void InputManager::setKeyPressed(sf::Keyboard::Key key) {
    m_keysPressed[key] = true;
}

void InputManager::setKeyReleased(sf::Keyboard::Key key) {
    m_keysPressed[key] = false;
}

