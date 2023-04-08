//
// Created by Lux Solari on 05/03/2023.
//

#ifndef CMAKESFMLPROJECT_MAINMENUSTATE_H
#define CMAKESFMLPROJECT_MAINMENUSTATE_H
#include <iostream>
#include "State.h"

class MainMenuState final : public State {
private:
    sf::CircleShape m_shape;
	sf::CircleShape m_tip;
	// bounding circle of m_shape
	sf::CircleShape m_boundingCircle;
	sf::Vector2f m_currentPosition = sf::Vector2f(0.f, 0.f);
	// drawable text for the current position
	sf::Text m_currentPositionText;
	// font for the text
	sf::Font m_font;

	sf::Vector2f m_targetPosition = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_velocity = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_acceleration = sf::Vector2f(0.f, 0.f);

public:
	void start() override;
    void handleInput(InputManager& inputManager) override;
	void handleInput(EventManager& eventManager) override;
	void update(sf::Time deltaTime) override;
	void stop() override;
	void render(sf::RenderWindow& renderWindow) override;
	void moveShape(EventDetails* details);
};


#endif //CMAKESFMLPROJECT_MAINMENUSTATE_H
