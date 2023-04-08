//
// Created by Lux Solari on 05/03/2023.
//
#include "MainMenuState.h"
#include "Globals.h"
#include "StateManager.h"
#include "OptionsMenuState.h"
#include <cmath>

void MainMenuState::start() {
	setIsRunning(true);
	this->m_shape = sf::CircleShape(50.f, 3);
	this->m_shape.setFillColor(sf::Color::Green);
	this->m_shape.setOrigin(this->m_shape.getRadius(), this->m_shape.getRadius());
	this->m_shape.setPosition(WindowManager::getInstance().GetWindowSize().x / 2.f, WindowManager::getInstance().GetWindowSize().y / 2.f);
	this->m_currentPosition = this->m_shape.getPosition();
	this->m_targetPosition = this->m_currentPosition;

	// add a red tip to the shape to indicate the direction it is facing, anchor its pivot to the center of the shape
	this->m_tip = sf::CircleShape(10.f, 3);
	this->m_tip.setFillColor(sf::Color::Red);
	this->m_tip.setPosition(this->m_shape.getPosition());
	this->m_tip.setOrigin((this->m_shape.getOrigin().x / 2.f) - (this->m_shape.getRadius() / 2.f) + (this->m_tip.getRadius()), this->m_shape.getOrigin().y);

	// calculate bounding circle
	float radius = this->m_shape.getRadius();
	this->m_boundingCircle = sf::CircleShape(radius);
	this->m_boundingCircle.setFillColor(sf::Color::Transparent);
	this->m_boundingCircle.setOutlineColor(sf::Color::Red);
	this->m_boundingCircle.setOutlineThickness(1.f);
	this->m_boundingCircle.setOrigin(this->m_boundingCircle.getRadius(), this->m_boundingCircle.getRadius());
	this->m_boundingCircle.setPosition(this->m_shape.getPosition());

	// load font
	// TODO: move this to a resource manager
	if (!this->m_font.loadFromFile("assets/fonts/arial.ttf")) {
		LOG("Failed to load font");
	}

	// set up text for current position, default font
	this->m_currentPositionText.setFont(this->m_font);
	// set string with integer numbers, dont show decimals
	this->m_currentPositionText.setString("Current position: " + std::to_string(static_cast<int>(this->m_currentPosition.x)) + ", " + std::to_string(static_cast<int>(this->m_currentPosition.y)));
	this->m_currentPositionText.setCharacterSize(20);
	this->m_currentPositionText.setFillColor(sf::Color::White);
	// set position relative to the shape, top left corner
	this->m_currentPositionText.setPosition(this->m_shape.getPosition());
	// set shape origin as pivot for the text 
	this->m_currentPositionText.setOrigin(this->m_shape.getOrigin().x + 7.5f,
		this->m_shape.getOrigin().y - this->m_shape.getRadius() * 2.f);

	// register a method to move the shape when the mouse is clicked
	StateManager::getInstance().getEventManager().AddCallback("Move", &MainMenuState::moveShape, this);
}

void MainMenuState::moveShape(EventDetails* details) {
	// set target position to mouse position. round in case of float, integer conversion
	this->m_targetPosition = sf::Vector2f(
		std::round(details->m_mouse.x),
		std::round(details->m_mouse.y));
}

void MainMenuState::handleInput(InputManager& inputManager) {
	/*sf::Event event{};*/
}

void MainMenuState::handleInput(EventManager& eventManager)
{
	sf::Event event{};
	while (WindowManager::getInstance().getWindow().pollEvent(event))
	{
		if (event.type == sf::Event::LostFocus) {
			eventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus) {
			eventManager.SetFocus(true);
		}
		// get current mouse position on the screen
		sf::Vector2i mousePos = sf::Mouse::getPosition(WindowManager::getInstance().getWindow());
		// convert mouse position to world coordinates
		sf::Vector2f mousePosWorld = WindowManager::getInstance().getWindow().mapPixelToCoords(mousePos);
		// apply acceleration towards the mouse position when up arrow is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			// calculate the direction vector
			sf::Vector2f direction = mousePosWorld - this->m_shape.getPosition();
			// normalize direction vector
			direction = sf::Vector2f(
				direction.x / std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2)),
				direction.y / std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2)));
			// apply acceleration in the direction of the mouse 
			this->m_acceleration = direction * 980.f;
		}
		else
		{
			this->m_acceleration = sf::Vector2f(0.f, 0.f);
		}

		eventManager.HandleEvent(event);
	}
	eventManager.Update();
}

void MainMenuState::update(const sf::Time deltaTime) {
	if (isRunning()) {
		static long long deltaTimeSum = 0;
		// do update logic
		deltaTimeSum += deltaTime.asMicroseconds();
		if (deltaTimeSum >= Globals::SAMPLE_INTERVAL)
		{
			LOG("In Main Menu State! ")
				deltaTimeSum = 0;
		}

		// apply acceleration to velocity
		this->m_velocity += this->m_acceleration * deltaTime.asSeconds();
		// apply velocity to position
		this->m_currentPosition += this->m_velocity * deltaTime.asSeconds();
		// set shape position
		this->m_shape.setPosition(this->m_currentPosition);
		// set tip position
		this->m_tip.setPosition(this->m_currentPosition);
		// set bounding circle position
		this->m_boundingCircle.setPosition(this->m_currentPosition);
		// set text position
		this->m_currentPositionText.setPosition(this->m_currentPosition);
		// set text string
		this->m_currentPositionText.setString("Current position: " + std::to_string(static_cast<int>(this->m_currentPosition.x)) + ", " + std::to_string(static_cast<int>(this->m_currentPosition.y)));

		// apply friction to velocity to slow down the shape over time
		this->m_velocity *= 0.95f;

		// keep the shape and tip pointing towards the mouse position on screen

		// get the current mouse position on the screen
		sf::Vector2i mousePosition = sf::Mouse::getPosition(WindowManager::getInstance().getWindow());

		// calculate the angle between the shape and the target position
		float angle = atan2(mousePosition.y - this->m_shape.getPosition().y, mousePosition.x - this->m_shape.getPosition().x);
		// convert the angle to degrees
		angle = (angle * 180.f / static_cast<float>(3.14f)) + 90;
		// rotate the shape
		this->m_shape.setRotation(angle);
		// rotate the tip
		this->m_tip.setRotation(angle);


		// reverse direction if the shape hits the edge of the screen
		if (this->m_shape.getPosition().x + this->m_shape.getRadius() > WindowManager::getInstance().GetWindowSize().x)
		{
			this->m_shape.setPosition(WindowManager::getInstance().GetWindowSize().x - this->m_shape.getRadius(), this->m_shape.getPosition().y);
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.x = -this->m_velocity.x;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();
		}
		else if (this->m_shape.getPosition().x - this->m_shape.getRadius() < 0)
		{
			this->m_shape.setPosition(this->m_shape.getRadius(), this->m_shape.getPosition().y);
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.x = -this->m_velocity.x;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();
		}
		if (this->m_shape.getPosition().y + this->m_shape.getRadius() > WindowManager::getInstance().GetWindowSize().y)
		{
			this->m_shape.setPosition(this->m_shape.getPosition().x, WindowManager::getInstance().GetWindowSize().y - this->m_shape.getRadius());
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.y = -this->m_velocity.y;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();
		}
		else if (this->m_shape.getPosition().y - this->m_shape.getRadius() < 0)
		{
			this->m_shape.setPosition(this->m_shape.getPosition().x, this->m_shape.getRadius());
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.y = -this->m_velocity.y;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();
		}


		// log the shape's position with LOG macro every one second
		static long long logTime = 0;
		logTime += deltaTime.asMicroseconds();
		if (logTime >= 1000000)
		{
			LOG("Shape position: " << this->m_shape.getPosition().x << ", " << this->m_shape.getPosition().y)
				LOG("Shape velocity: " << this->m_velocity.x << ", " << this->m_velocity.y)
				LOG("Shape acceleration: " << this->m_acceleration.x << ", " << this->m_acceleration.y)
				logTime = 0;
		}

	}
}
void MainMenuState::render(sf::RenderWindow& renderWindow)
{
	static const auto color = sf::Color{ 10, 10, 10 };
	renderWindow.clear(color);
	renderWindow.draw(this->m_shape);
	renderWindow.draw(this->m_tip);
	renderWindow.draw(this->m_boundingCircle);
	renderWindow.draw(this->m_currentPositionText);
	renderWindow.display();
}

void MainMenuState::stop() {
	setIsRunning(false);
	std::cout << "Stopped Main Menu state!\n";
}


