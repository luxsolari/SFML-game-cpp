//
// Created by Lux Solari on 05/03/2023.
//

#include "WindowManager.h"
#include "StateManager.h"

WindowManager& WindowManager::getInstance() {
    static WindowManager instance;
    return instance;
}

sf::RenderWindow& WindowManager::getWindow() const {
    return this->m_window;
}

sf::RenderWindow* WindowManager::Create()
{
    auto style = this->m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
    static sf::RenderWindow window;
    if (style == sf::Style::Fullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "SFML Game", style);
        this->m_current_width = Globals::SCREEN_WIDTH;
        this->m_current_height = Globals::SCREEN_HEIGHT;
    }
    else {
        window.create(sf::VideoMode(Globals::SCREEN_WIDTH_WINDOWED, Globals::SCREEN_HEIGHT_WINDOWED, 32), "SFML Game", style);
        this->m_current_width = Globals::SCREEN_WIDTH_WINDOWED;
        this->m_current_height = Globals::SCREEN_HEIGHT_WINDOWED;
    }
    window.setActive(true);

    return &window;
}

WindowManager::WindowManager() : m_window(*Create()) {}

void WindowManager::ToggleFullscreen(EventDetails* l_details)
{
    this->m_isFullscreen = !this->m_isFullscreen;
    this->m_window.close();
    auto style = this->m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
    if (style == sf::Style::Fullscreen) {
        // check if there are any fullscreen modes available
        if (sf::VideoMode::getFullscreenModes().empty()) {
            // if not, switch to windowed mode
            this->m_isFullscreen = false;
            style = sf::Style::Default;
            LOG("No fullscreen modes available, switching to windowed mode");
            this->m_window.create(sf::VideoMode(Globals::SCREEN_WIDTH_WINDOWED, Globals::SCREEN_HEIGHT_WINDOWED, 32), "SFML Game", style);
        }
        else {
            this->m_window.create(sf::VideoMode::getDesktopMode(), "SFML Game", style);
            this->m_current_width = Globals::SCREEN_WIDTH;
            this->m_current_height = Globals::SCREEN_HEIGHT;
        }
    }
    else {
        this->m_window.create(sf::VideoMode(Globals::SCREEN_WIDTH_WINDOWED, Globals::SCREEN_HEIGHT_WINDOWED, 32), "SFML Game", style);
        this->m_current_width = Globals::SCREEN_WIDTH_WINDOWED;
        this->m_current_height = Globals::SCREEN_HEIGHT_WINDOWED;
    }
    this->m_window.setActive(true);
}

sf::Vector2<unsigned int> WindowManager::GetWindowSize() const
{
    return sf::Vector2<unsigned int>(this->m_current_width, this->m_current_height);
}

void WindowManager::Close(EventDetails* l_details) {
    StateManager::getInstance().stop();
    this->m_window.close();
}
