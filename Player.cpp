#include "Player.hpp"

Player::Player(const std::string& filename, SDL_Renderer* ren) :
    mtexture(Utility::LoadTexture(ren, filename)),
    mRenderer(ren),
    mAngle(270),
    mForwardVel(0),
    mRotationVel(0),
    mTurretAngle(270),
    mTurretRotationVel(0)
{
    SDL_QueryTexture(mtexture, NULL, NULL, &mWidth, &mHeight);
    mWidth = mWidth / (mWidth / 16);
}

Player::~Player() {
    SDL_DestroyTexture(mtexture);
}

const int Player::GetX() const {
    return mX;
}

const int Player::GetY() const {
    return mY;
}

const float Player::GetXVel() const {
    return mXvel;
}

const float Player::GetYVel() const {
    return mYvel;
}

void Player::SetXVel(float newvel) {
    mXvel = newvel;
}

void Player::SetYVel(float newvel) {
    mYvel = newvel;
}

void Player::SetX(int newx) {
    mX = newx;
}

void Player::SetY(int newy) {
    mY = newy;
}

SDL_Texture* Player::GetTexture() {
    return mtexture;
}

void Player::SetForwardVel(float newvel) {
    mForwardVel = newvel;
}

const float Player::GetForwardVel() const {
    return mForwardVel;
}

const float Player::GetAngle() const {
    return mAngle;
}

const float Player::GetRotationVel() const {
    return mRotationVel;
}

void Player::SetRotationVel(float newvel) {
    mRotationVel = newvel;
}

const float Player::GetTurretRotationVel() const {
    return mTurretRotationVel;
}

void Player::SetTurretRotationVel(float newvel) {
    mTurretRotationVel = newvel;
}

void Player::Render() {
    SDL_Rect srcRect, dstRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.h = mHeight;
    srcRect.w = mWidth;

    dstRect.x = mX;
    dstRect.y = mY;
    dstRect.h = mHeight;
    dstRect.w = mWidth;
    SDL_RenderCopyEx(mRenderer, mtexture, &srcRect, &dstRect, mAngle - 270, NULL, SDL_FLIP_NONE);

    srcRect.x = 16;
    srcRect.y = 0;
    srcRect.h = mHeight;
    srcRect.w = mWidth;
    SDL_RenderCopyEx(mRenderer, mtexture, &srcRect, &dstRect, mTurretAngle - 270, NULL, SDL_FLIP_NONE);
}

void Player::Move(int maxX, int maxY) {
    mAngle += mRotationVel;
    if (mAngle < 0) {
        mAngle = 360 + mAngle;
    }
    if (mAngle > 360) {
        mAngle = mAngle - 360;
    }

    mTurretAngle += mRotationVel + mTurretRotationVel;
    if (mTurretAngle < 0) {
        mTurretAngle = 360 + mTurretAngle;
    }
    if (mTurretAngle > 360) {
        mTurretAngle = mTurretAngle - 360;
    }


    float a = mAngle * M_PI / 180;
    mXvel = std::cos(a) * mForwardVel;
    mYvel = std::sin(a) * mForwardVel;
    if (mXvel != 0) {
        if (mXvel < 0 && mX > 0)
            mX += mXvel;
        else if (mXvel > 0 && mX + mWidth < maxX)
            mX += mXvel;
    }
    if (mYvel != 0) {
        if (mYvel < 0 && mY > 0)
            mY += mYvel;
        else if (mYvel > 0 && mY + mHeight < maxY)
            mY += mYvel;
    }
}
