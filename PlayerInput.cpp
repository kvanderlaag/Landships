#include "PlayerInput.hpp"

PlayerInput::PlayerInput(const int pID, const int jIndex) :
    mPlayerID(pID),
    mControllerType(XBOX_360_CONTROLLER),
    mJoystick(SDL_JoystickOpen(jIndex)),
    mHaptic(SDL_HapticOpenFromJoystick(mJoystick)),
    mUpHeld(false),
    mDownHeld(false),
    mRightHeld(false),
    mLeftHeld(false),
    mFireHeld(false),
    mCancelHeld(false),
    mSelectHeld(false),
    mStartHeld(false),
    mBackHeld(false)
{
    if (mJoystick) {
        mJoystickName = SDL_JoystickName(mJoystick);
        if (mJoystickName == "Wireless Controller") {
            mControllerType = PS4_CONTROLLER;
        }
        if (mControllerType == PS4_CONTROLLER) {
            std::cout << "Joystick " << mPlayerID << ": " << mJoystickName << " (PS4)" << std::endl;
        } else if (mControllerType == XBOX_360_CONTROLLER) {
            std::cout << "Joystick " << mPlayerID << ": " << mJoystickName << " (Xbox)" << std::endl;
        }
    }

    if (mControllerType == XBOX_360_CONTROLLER) {
        mButtonFire = XBOX_JBUTTON_RBUMPER;
        mAxisFire = XBOX_JAXIS_RTRIGGER;
        mAxisLeftStickX = XBOX_JAXIS_LSTICKX;
        mAxisLeftStickY = XBOX_JAXIS_LSTICKY;
        mAxisRightStickX = XBOX_JAXIS_RSTICKX;
        mAxisRightStickY = XBOX_JAXIS_RSTICKY;

        mButtonA = XBOX_JBUTTON_A;
        mButtonB = XBOX_JBUTTON_B;
        mButtonBack = XBOX_JBUTTON_BACK;
        mButtonStart = XBOX_JBUTTON_START;
    } else if (mControllerType == PS4_CONTROLLER) {
        mButtonFire = PS4_JBUTTON_R1;
        mAxisFire = PS4_JAXIS_R2;
        mAxisLeftStickX = PS4_JAXIS_LSTICKX;
        mAxisLeftStickY = PS4_JAXIS_LSTICKY;
        mAxisRightStickX = PS4_JAXIS_RSTICKX;
        mAxisRightStickY = PS4_JAXIS_RSTICKY;

        mButtonA = PS4_JBUTTON_X;
        mButtonB = PS4_JBUTTON_CIRCLE;
        mButtonBack = PS4_JBUTTON_SHARE;
        mButtonStart = PS4_JBUTTON_OPTIONS;
    }
}

PlayerInput::~PlayerInput() {
    std::cout << "Closing Joystick " << mPlayerID << std::endl;
    SDL_JoystickClose(mJoystick);
    //SDL_HapticClose(mHaptic);
}

void PlayerInput::CheckInput() {
    int hatPos = SDL_JoystickGetHat(mJoystick, 0);
    int lsX = SDL_JoystickGetAxis(mJoystick, mAxisLeftStickX);
    int lsY = SDL_JoystickGetAxis(mJoystick, mAxisLeftStickY);

    int rsX = SDL_JoystickGetAxis(mJoystick, mAxisRightStickX);
    int rsY = SDL_JoystickGetAxis(mJoystick, mAxisRightStickY);

    int fireAxis = SDL_JoystickGetAxis(mJoystick, mAxisFire);
    int fireButton = SDL_JoystickGetButton(mJoystick, mButtonFire);

    int readyButton = SDL_JoystickGetButton(mJoystick, mButtonA);
    int cancelButton = SDL_JoystickGetButton(mJoystick, mButtonB);

    int backButton = SDL_JoystickGetButton(mJoystick, mButtonBack);
    int startButton = SDL_JoystickGetButton(mJoystick, mButtonStart);

    if (hatPos == JHAT_DPADCENTER) {
        mUpHeld = false;
        mDownHeld = false;
        mLeftHeld = false;
        mRightHeld = false;
    } else if (hatPos == JHAT_DPADUP) {
        mUpHeld = true;
        mDownHeld = false;
        mLeftHeld = false;
        mRightHeld = false;
    } else if (hatPos == JHAT_DPADUPRIGHT) {
        mUpHeld = true;
        mDownHeld = false;
        mLeftHeld = false;
        mRightHeld = true;
    } else if (hatPos == JHAT_DPADRIGHT) {
        mUpHeld = false;
        mDownHeld = false;
        mLeftHeld = false;
        mRightHeld = true;
    } else if (hatPos == JHAT_DPADDOWNRIGHT) {
        mUpHeld = false;
        mDownHeld = true;
        mLeftHeld = false;
        mRightHeld = true;
    } else if (hatPos == JHAT_DPADDOWN) {
        mUpHeld = false;
        mDownHeld = true;
        mLeftHeld = false;
        mRightHeld = false;
    } else if (hatPos == JHAT_DPADDOWNLEFT) {
        mUpHeld = false;
        mDownHeld = true;
        mLeftHeld = true;
        mRightHeld = false;
    } else if (hatPos == JHAT_DPADLEFT) {
        mUpHeld = false;
        mDownHeld = false;
        mLeftHeld = true;
        mRightHeld = false;
    } else if (hatPos == JHAT_DPADUPLEFT) {
        mUpHeld = false;
        mDownHeld = false;
        mLeftHeld = true;
        mRightHeld = false;
    }

    /*
    if (lsX < JOYLSTICK_DEADZONE && lsX > -JOYLSTICK_DEADZONE) {
        lsX = 0;
    }
    if (lsY < JOYLSTICK_DEADZONE && lsY > -JOYLSTICK_DEADZONE) {
        lsY = 0;
    }
    if (rsX < JOYRSTICK_DEADZONE && rsX > -JOYRSTICK_DEADZONE) {
        rsX = 0;
    }
    if (rsY < JOYRSTICK_DEADZONE && rsY > -JOYRSTICK_DEADZONE) {
        rsY = 0;
    }
    */
    if (fireAxis < JOYRTRIGGER_DEADZONE) {
        fireAxis = 0;
    }

    mLeftStickX = lsX;
    mLeftStickY = lsY;
    mRightStickX = rsX;
    mRightStickY = rsY;

    if (lsX < -JOYLSTICK_DEADZONE) {
        mLeftHeld = true;
        mRightHeld = false;

    } else if (lsX > JOYLSTICK_DEADZONE) {
        mRightHeld = true;
        mLeftHeld = false;
    }
    if (lsY < -JOYLSTICK_DEADZONE) {
        mUpHeld = true;
        mDownHeld = false;

    } else if (lsY > JOYLSTICK_DEADZONE) {
        mDownHeld = true;
        mUpHeld = false;
    }

    if (fireButton || fireAxis)
        mFireHeld = true;
    else
        mFireHeld = false;

    if (readyButton)
        mSelectHeld = true;
    else
        mSelectHeld = false;

    if (cancelButton)
        mCancelHeld = true;
    else
        mCancelHeld = false;

    if (startButton)
        mStartHeld = true;
    else
        mStartHeld = false;

    if (backButton)
        mBackHeld = true;
    else
        mBackHeld = false;
}

const int PlayerInput::ID() const {
    return mPlayerID;
}

const int PlayerInput::ControllerType() const {
    return mControllerType;
}

const std::string& PlayerInput::JoystickName() const {
    return mJoystickName;
}

SDL_Joystick* PlayerInput::Joystick() {
    return mJoystick;
}

const bool PlayerInput::UpHeld() const {
    return mUpHeld;
}

const bool PlayerInput::DownHeld() const {
    return mDownHeld;
}

const bool PlayerInput::LeftHeld() const {
    return mLeftHeld;
}

const bool PlayerInput::RightHeld() const {
    return mRightHeld;
}

const bool PlayerInput::FireHeld() const {
    return mFireHeld;
}

const bool PlayerInput::SelectHeld() const {
    return mSelectHeld;
}

const bool PlayerInput::CancelHeld() const {
    return mCancelHeld;
}

const bool PlayerInput::StartHeld() const {
    return mStartHeld;
}

const bool PlayerInput::BackHeld() const {
    return mBackHeld;
}

const int PlayerInput::LeftStickX() const {
    return mLeftStickX;
}

const int PlayerInput::LeftStickY() const {
    return mLeftStickY;
}

const int PlayerInput::RightStickX() const {
    return mRightStickX;
}

const int PlayerInput::RightStickY() const {
    return mRightStickY;
}

const Vector2D PlayerInput::LeftStickVector() const {
    if (std::sqrt(mLeftStickX * mLeftStickX + mLeftStickY * mLeftStickY) > JOYLSTICK_DEADZONE) {
        return Vector2D(-mLeftStickX, -mLeftStickY);
    } else {
        return Vector2D(0, 0);
    }
}

const Vector2D PlayerInput::RightStickVector() const {
    if (std::sqrt(mRightStickX * mRightStickX + mRightStickY * mRightStickY) > JOYRSTICK_DEADZONE) {
        return Vector2D(-mRightStickX, -mRightStickY);
    } else {
        return Vector2D(0, 0);
    }
}

void PlayerInput::FireRumble() const {
    SDL_HapticRumblePlay( mHaptic, 0.5, 200 );

}

void PlayerInput::DieRumble() const {
    SDL_HapticRumblePlay( mHaptic, 1, 2000 );
}
