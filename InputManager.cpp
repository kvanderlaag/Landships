#include "InputManager.hpp"

InputManager::InputManager() :
    mPlayer{nullptr, nullptr, nullptr, nullptr},
    mEscapeHeld(false),
    mEnterHeld(false),
    mSpaceHeld(false),
    mQuitFlag(false)
{

}

InputManager::~InputManager() {
    for (int i = 0; i < 4; ++i) {
        if (mPlayer[i] != nullptr) {
            delete mPlayer[i];
        }
    }
}

void InputManager::CheckInput() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            mQuitFlag = true;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                mEscapeHeld = true;
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                if (mAltHeld) {
                    if (gFullscreen) {
                        SDL_SetWindowFullscreen(win, SDL_WINDOW_SHOWN);
                        gFullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        gFullscreen = true;
                    }
                } else {
                    mEnterHeld = true;
                }
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                mSpaceHeld = true;
            } else if (e.key.keysym.sym == SDLK_LALT || e.key.keysym.sym == SDLK_RALT) {
                mAltHeld = true;
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                mEscapeHeld = false;
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                mEnterHeld = false;
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                mSpaceHeld = false;
            } else if (e.key.keysym.sym == SDLK_LALT || e.key.keysym.sym == SDLK_RALT) {
                mAltHeld = false;
            }
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            SDL_Joystick* joy = SDL_JoystickFromInstanceID(e.jdevice.which);
            for (int i = 0; i < 4; ++i) {
                if (mPlayer[i] == nullptr)
                    continue;

                if (mPlayer[i]->Joystick() == joy) {
                    delete mPlayer[i];
                    mPlayer[i] = nullptr;
                    break;
                }
            }
        } else if (e.type == SDL_JOYDEVICEADDED) {
            for (int i = 0; i < 4; ++i) {
                if (mPlayer[i] == nullptr) {
                    mPlayer[i] = new PlayerInput(i, e.jdevice.which);
                    break;
                }
            }
        }

    }
    for (int i = 0; i < 4; ++i) {
        if (mPlayer[i] == nullptr)
            continue;
        mPlayer[i]->CheckInput();
    }

}

const PlayerInput* InputManager::Player(const int index) const {
    return mPlayer[index];
}

const bool InputManager::EscapeHeld() const {
    return mEscapeHeld;
}

const bool InputManager::EnterHeld() const {
    return mEnterHeld;
}

const bool InputManager::SpaceHeld() const {
    return mSpaceHeld;
}

const bool InputManager::QuitFlag() const {
    return mQuitFlag;
}
