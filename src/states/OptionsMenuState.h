//
// Created by Lux Solari on 09/03/2023.
//

#ifndef CMAKESFMLPROJECT_OPTIONSMENUSTATE_H
#define CMAKESFMLPROJECT_OPTIONSMENUSTATE_H

#include "State.h"

class OptionsMenuState : public State {
public:
    void start() override;
    void handleInput(InputManager &inputManager) override;
    void handleInput(EventManager &eventManager) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow &renderWindow) override;
    void stop() override;
};


#endif //CMAKESFMLPROJECT_OPTIONSMENUSTATE_H
