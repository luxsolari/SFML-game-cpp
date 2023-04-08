//
// Created by Lux Solari on 05/03/2023.
//
#include "MainMenuState.h"
#include "Globals.h"
#include "StateManager.h"
#include "OptionsMenuState.h"

void MainMenuState::start() {
	setIsRunning(true);
	this->m_shape = sf::CircleShape(50.f, 3);
	this->m_shape.setFillColor(sf::Color::Green);
	this->m_shape.setOrigin(this->m_shape.getRadius(), this->m_shape.getRadius());
	this->m_shape.setPosition(WindowManager::getInstance().GetWindowSize().x/ 2.f, WindowManager::getInstance().GetWindowSize().y / 2.f);
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
	//LOG("Mouse clicked at: " << details->m_mouse.x << ", " << details->m_mouse.y);

	// rotate the shape towards the target position so that the tip points towards the target position
		// calculate the angle between the shape and the target position
		float angle = atan2(this->m_targetPosition.y - this->m_shape.getPosition().y, this->m_targetPosition.x - this->m_shape.getPosition().x);
		// convert the angle to degrees
		angle = (angle * 180.f / static_cast<float>(3.14f)) + 90;
		// rotate the shape
		this->m_shape.setRotation(angle);
		// rotate the tip
		this->m_tip.setRotation(angle);

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

		// update bounding circle position and rotation
		this->m_boundingCircle.setPosition(this->m_shape.getPosition());
		this->m_boundingCircle.setRotation(this->m_shape.getRotation());
		
		// update the position text, position and rotation
		this->m_currentPositionText.setString("x: " + std::to_string(static_cast<int>(this->m_shape.getPosition().x)) + 
			", y:" + std::to_string(static_cast<int>(this->m_shape.getPosition().y)));
		this->m_currentPositionText.setPosition(this->m_shape.getPosition());
		//this->m_currentPositionText.setRotation(this->m_shape.getRotation());



		// if the shape is not at the target position, move it towards the target position.
		// use acceleration and friction calculations.
		static float magnitude = 0.f;
		if (this->m_shape.getPosition() != this->m_targetPosition)
		{
			// calculate the distance between the shape and the target position
			sf::Vector2f distance = this->m_targetPosition - this->m_shape.getPosition();
			// calculate the magnitude of the distance vector
			magnitude = static_cast<float>(sqrt(pow(distance.x, 2) + pow(distance.y, 2)));
			// calculate the unit vector of the distance vector
			sf::Vector2f unitVector = distance / magnitude;
			// calculate the acceleration vector
			this->m_acceleration = unitVector * 980.f;
			// calculate the velocity vector by adding the acceleration vector to the current velocity vector and multiplying by the time step (deltaTime)
			this->m_velocity += this->m_acceleration * deltaTime.asSeconds();
			// calculate the new position by adding the velocity vector to the current position and multiplying by the time step (deltaTime)
			this->m_currentPosition += this->m_velocity * deltaTime.asSeconds(); 
			// set the shape's position to the new position calculated above 
			this->m_shape.setPosition(this->m_currentPosition);
			this->m_tip.setPosition(this->m_shape.getPosition());
			// multiply velocity by 0.95 to simulate friction and slow down the shape over time 
			this->m_velocity *= 0.95f;

			// stop when the shape is within 1 pixel of the target position or if the magnitude of the distance vector is less than 1 pixel
			if (magnitude <= .5f || this->m_shape.getPosition() == this->m_targetPosition)
			{
				this->m_shape.setPosition(this->m_targetPosition);
				this->m_tip.setPosition(this->m_shape.getPosition());
				this->m_velocity = sf::Vector2f(0.f, 0.f);
				this->m_acceleration = sf::Vector2f(0.f, 0.f);
				magnitude = 0.f;
			}
			else if (magnitude <= 30.f)
			{
				this->m_velocity *= 0.85f;
			}
		}

		// reverse direction if the shape hits the edge of the screen
		if (this->m_shape.getPosition().x + this->m_shape.getRadius() > WindowManager::getInstance().GetWindowSize().x)
		{
			this->m_shape.setPosition(WindowManager::getInstance().GetWindowSize().x - this->m_shape.getRadius(), this->m_shape.getPosition().y);
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.x = -this->m_velocity.x;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();

			// fix magnitude
			magnitude = static_cast<float>(sqrt(pow(this->m_targetPosition.x - this->m_shape.getPosition().x, 2) + pow(this->m_targetPosition.y - this->m_shape.getPosition().y, 2)));
		}
		else if (this->m_shape.getPosition().x - this->m_shape.getRadius() < 0)
		{
			this->m_shape.setPosition(this->m_shape.getRadius(), this->m_shape.getPosition().y);
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.x = -this->m_velocity.x;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();

			// fix magnitude
			magnitude = static_cast<float>(sqrt(pow(this->m_targetPosition.x - this->m_shape.getPosition().x, 2) + pow(this->m_targetPosition.y - this->m_shape.getPosition().y, 2)));
		}
		if (this->m_shape.getPosition().y + this->m_shape.getRadius() > WindowManager::getInstance().GetWindowSize().y)
		{
			this->m_shape.setPosition(this->m_shape.getPosition().x, WindowManager::getInstance().GetWindowSize().y - this->m_shape.getRadius());
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.y = -this->m_velocity.y;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();

			// fix magnitude
			magnitude = static_cast<float>(sqrt(pow(this->m_targetPosition.x - this->m_shape.getPosition().x, 2) + pow(this->m_targetPosition.y - this->m_shape.getPosition().y, 2)));
		}
		else if (this->m_shape.getPosition().y - this->m_shape.getRadius() < 0)
		{
			this->m_shape.setPosition(this->m_shape.getPosition().x, this->m_shape.getRadius());
			// fix the tip position
			this->m_tip.setPosition(this->m_shape.getPosition());
			this->m_velocity.y = -this->m_velocity.y;
			this->m_acceleration = sf::Vector2f(0.f, 0.f);

			this->m_currentPosition = this->m_shape.getPosition();

			// fix magnitude
			magnitude = static_cast<float>(sqrt(pow(this->m_targetPosition.x - this->m_shape.getPosition().x, 2) + pow(this->m_targetPosition.y - this->m_shape.getPosition().y, 2)));
		}

		// log the shape's position with LOG macro every one second
		static long long logTime = 0;
		logTime += deltaTime.asMicroseconds();
		if (logTime >= 1000000)
		{
			LOG("Shape position: " << this->m_shape.getPosition().x << ", " << this->m_shape.getPosition().y)
			LOG("Shape velocity: " << this->m_velocity.x << ", " << this->m_velocity.y)
			LOG("Shape acceleration: " << this->m_acceleration.x << ", " << this->m_acceleration.y)
			LOG("Magnitude: " << magnitude)
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


