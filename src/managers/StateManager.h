//
// Created by Lux Solari on 04/03/2023.
//

#ifndef CMAKESFMLPROJECT_STATEMANAGER_H
#define CMAKESFMLPROJECT_STATEMANAGER_H
#include <vector>
#include <memory>
#include "InputManager.h"
#include "WindowManager.h"
#include "EventManager.h"
#include "Globals.h"
#include "MainMenuState.h"
#include "State.h"

class StateManager
{
public:
	static StateManager& getInstance();
	StateManager(const StateManager&) = delete;
	StateManager(const StateManager&&) = delete;
	void operator=(const StateManager&) = delete;
	void operator=(const StateManager&&) = delete;

	void start();
	void update();
	void stop();

	void pushState(std::unique_ptr<State> state);
	void popState();
    void changeState(std::unique_ptr<State> state);
	State& getCurrentState() const;
	EventManager& getEventManager();

private:
	StateManager();
	~StateManager();

	bool m_isRunning = false;
	WindowManager& m_windowManager;
	InputManager& m_inputManager;
	EventManager m_eventManager;
	std::vector<std::unique_ptr<State>> m_states;
};


#endif //CMAKESFMLPROJECT_STATEMANAGER_H
