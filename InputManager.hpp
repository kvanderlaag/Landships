#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <SDL.h>

#include "PlayerInput.hpp"

extern bool gFullscreen;
extern SDL_Window* win;

class InputManager {
public:

    InputManager();
    ~InputManager();

    void CheckInput();

    const PlayerInput* Player(const int index) const;

    const bool EscapeHeld() const;
    const bool EnterHeld() const;
    const bool SpaceHeld() const;
    const bool QuitFlag() const;

private:
    PlayerInput* mPlayer[4];

    bool mEscapeHeld;
    bool mEnterHeld;
    bool mSpaceHeld;
    bool mQuitFlag;

    bool mAltHeld;

};
#endif // _INPUTMANAGER_H
