#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "Menu.hpp"
#include "Game.hpp"

#include <cinttypes>

class GameState {
private:
    Menu mMenu;
    Game mGame;
    bool running;
public:

    enum state {
        stateMenu,
        stateGame
    } state;

    GameState();
    ~GameState();

    void Update(uint32_t ticks);
    const bool IsRunning() const;

    void Exit();
};
#endif // _GAMESTATE_H
