//
// Created by Lux Solari on 05/03/2023.
//

#ifndef CMAKESFMLPROJECT_WINDOWMANAGER_H
#define CMAKESFMLPROJECT_WINDOWMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Globals.h"
#include "EventManager.h"

class WindowManager {
private:
    sf::RenderWindow& m_window;
    bool m_isFullscreen = false;
    unsigned int m_current_width;
    unsigned int m_current_height;
public:
    static WindowManager& getInstance();
    WindowManager (const WindowManager&) = delete;
    void operator=(const WindowManager&) = delete;
    sf::RenderWindow& getWindow() const;
    sf::RenderWindow* Create();
    void Close(EventDetails* l_details);
    void ToggleFullscreen(EventDetails* l_details);
    sf::Vector2<unsigned int> GetWindowSize() const; 
private:
    explicit WindowManager();
};


#endif //CMAKESFMLPROJECT_WINDOWMANAGER_H
