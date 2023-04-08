#include "managers/StateManager.h"

int main()
{
    StateManager& statesManager = StateManager::getInstance(); // Singleton. Only one instance of StateManager is created.
    statesManager.start();
    statesManager.update();
}
