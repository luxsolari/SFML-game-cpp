//
// Created by Lux Solari on 09/03/2023.
//
#include "Globals.h"
#include "OptionsMenuState.h"
#include "StateManager.h"
#include "MainMenuState.h"

void OptionsMenuState::start() {
    this->setIsRunning(true);
}

void OptionsMenuState::handleInput(InputManager &inputManager) {
    sf::Event event{};
    while (WindowManager::getInstance().getWindow().pollEvent(event)) {
        inputManager.update(event);
        if (event.type == sf::Event::Closed) {
            WindowManager::getInstance().getWindow().close();
            this->stop();
            // Exit the game
            StateManager::getInstance().stop();
        }
        else if (event.type == sf::Event::KeyPressed && inputManager.isKeyPressed(sf::Keyboard::Escape)) {
        	inputManager.setKeyPressed(sf::Keyboard::Escape);
        	this->stop();
        	StateManager::getInstance().popState();
        	StateManager::getInstance().getCurrentState().start();
        	inputManager.setKeyReleased(sf::Keyboard::Escape);
        }
        else if (event.type == sf::Event::KeyPressed && inputManager.isKeyPressed(sf::Keyboard::O)) {
            inputManager.setKeyPressed(sf::Keyboard::O);
            this->stop();
            StateManager::getInstance().popState();
            StateManager::getInstance().getCurrentState().start();
            inputManager.setKeyReleased(sf::Keyboard::O);
        }
    }
}

void OptionsMenuState::handleInput(EventManager& eventManager)
{
    sf::Event event {};
    while (WindowManager::getInstance().getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::LostFocus) {
            eventManager.SetFocus(false);
        }
        else if (event.type == sf::Event::GainedFocus) {
            eventManager.SetFocus(true);
        }
        else if (event.type == sf::Event::Closed) 
        {
            WindowManager::getInstance().getWindow().close();
			this->stop();
			// Exit the game
			StateManager::getInstance().stop();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            this->stop();
            StateManager::getInstance().popState();
            StateManager::getInstance().getCurrentState().start();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O) {
            this->stop();
            StateManager::getInstance().popState();
            StateManager::getInstance().getCurrentState().start();
        }
        eventManager.HandleEvent(event);
    }
    eventManager.Update();
}

void OptionsMenuState::update(sf::Time deltaTime) {
	if (isRunning()) {
		static long long deltaTimeSum = 0;
		// do update logic
        deltaTimeSum += deltaTime.asMicroseconds();
        if (deltaTimeSum >= Globals::SAMPLE_INTERVAL) {
            std::cout << "In Options Menu State! " << deltaTimeSum << std::endl;
            deltaTimeSum = 0;
        }
    }
}

void OptionsMenuState::render(sf::RenderWindow &renderWindow) {
    static const auto& color = sf::Color{155, 10, 36};
    renderWindow.clear(color);
    renderWindow.display();
}

void OptionsMenuState::stop() {
    setIsRunning(false);
    std::cout << "Stopped Options Menu state!\n";
}
