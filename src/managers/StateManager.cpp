//
// Created by Lux Solari on 04/03/2023.
//
#include "StateManager.h"

StateManager &StateManager::getInstance() {
    static StateManager instance;
    instance.getEventManager().SetFocus(true);
    return instance;
}

void StateManager::start() {
    std::cout << "Started state machine manager" << std::endl;
    this->m_isRunning = true;

    this->m_eventManager.AddCallback( "Fullscreen_toggle", &WindowManager::ToggleFullscreen, &this->m_windowManager);
    this->m_eventManager.AddCallback( "Window_close", &WindowManager::Close, &this->m_windowManager);

    this->pushState(std::make_unique<MainMenuState>());
    this->m_states.back()->start();
}

void StateManager::update() {
    // Set maximum frame rate
    const sf::Time frameInterval = sf::seconds(1.f / Globals::MAX_FRAME_RATE);
    //this->m_windowManager.getWindow().setFramerateLimit(Globals::MAX_FRAME_RATE);
    sf::Clock fpsClock;
    static long long frameCount = 0;
    static long long frameTimeSum = 0;

    // Set maximum tick rate
    const sf::Time tickInterval = sf::seconds(1.f / Globals::MAX_TICK_RATE);
    sf::Clock tickClock;

    while (this->m_isRunning) {
        if (this->getCurrentState().isRunning()) {
            if (tickClock.getElapsedTime() >= tickInterval) {
                getCurrentState().handleInput(this->m_eventManager);
                getCurrentState().update(tickInterval);
                tickClock.restart();
            }

            // Render at maximum frame rate set in Globals.h
            if (fpsClock.getElapsedTime() >= frameInterval) {
            	getCurrentState().render(this->m_windowManager.getWindow());
            	fpsClock.restart();
            	frameCount++;
            	frameTimeSum += frameInterval.asMilliseconds();

                // show fps clock if debug macro is set
                #ifdef DEBUG
                if (frameCount >= Globals::MAX_FRAME_RATE) {
                	std::cout << "FPS: " << 1000.f / (frameTimeSum / frameCount) << std::endl;
                	frameCount = 0;
                	frameTimeSum = 0;
                }
                #endif                
            }

        }

        if (!this->getCurrentState().isRunning()) {
            this->stop();
        }
    }
}

void StateManager::stop() {
    std::cout << "Stopped state machine manager" << std::endl;
    this->m_isRunning = false;
}

void StateManager::pushState(std::unique_ptr<State> state) {
    this->m_states.emplace_back(std::move(state));
}

void StateManager::popState() {
    if (!this->m_states.empty()) {
        this->m_states.pop_back();
    }
}

void StateManager::changeState(std::unique_ptr<State> state) {
    if (!this->m_states.empty()) {
        this->m_states.back()->stop(); // We don't want to pop the state, we want to stop it. This way we can resume it later.
    }
    this->pushState(std::move(state));
}

State& StateManager::getCurrentState() const {
    return *(this->m_states.back());
}

EventManager& StateManager::getEventManager() {
    return this->m_eventManager;
}

StateManager::StateManager() : m_windowManager(WindowManager::getInstance()), m_inputManager(InputManager::getInstance()) {
	std::cout << "Created state machine manager instance" << std::endl;
}

StateManager::~StateManager() {
    std::cout << "Destroyed state machine manager instance" << std::endl;
    this->m_windowManager.Close(nullptr);
}


