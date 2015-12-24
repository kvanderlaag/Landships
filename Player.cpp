#include "Player.hpp"

//#define _COLLISION_DEBUG

Player::Player(const std::string& filename, SDL_Renderer* ren, const Map& m) :
    mtexture(Utility::LoadTexture(ren, filename)),
    mRenderer(ren),
    mAngle(270),
    mForwardVel(0),
    mRotationVel(0),
    mTurretAngle(270),
    mTurretRotationVel(0),
    mX(0),
    mY(0),
    parentMap(&m)
{
    SDL_QueryTexture(mtexture, NULL, NULL, &mWidth, &mHeight);
    mWidth = mWidth / (mWidth / 16);
    mCollider = std::unique_ptr<Collider>(new Collider(mWidth, mHeight, mX, mY, mAngle, mRenderer));
}

Player::~Player() {
    SDL_DestroyTexture(mtexture);
}

const float Player::GetX() const {
    return mX;
}

const float Player::GetY() const {
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

void Player::SetX(float newx) {
    mX = newx;
    mCollider->SetX(newx);
}

void Player::SetY(float newy) {
    mY = newy;
    mCollider->SetY(newy);
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

    mCollider->Render();
}

void Player::CheckCollision(const Collider& other) {
    std::vector<float> velocity = { mXvel, mYvel };
    Collision coll = mCollider->IsColliding(other, velocity);
    if (coll.WillIntersect() || coll.Intersecting()) {
        std::vector<float> mt = coll.GetMinimumTranslationVector();
        if (mt.size() == 2) {
            mX += mt[0];
            mY += mt[1];
        }
    }
}

void Player::Move(int maxX, int maxY, uint32_t ticks) {
    float forwardPerFrame = mForwardVel * ticks / 16 ;
    float rotationPerFrame = mRotationVel * ticks / 16 ;
    float turretRotationPerFrame = mTurretRotationVel * ticks / 16 ;

    mAngle += rotationPerFrame;
    if (mAngle < 0) {
        mAngle = 360 + mAngle;
    }
    if (mAngle > 360) {
        mAngle = mAngle - 360;
    }
    mCollider->SetAngle(mAngle);

    mTurretAngle += (rotationPerFrame + turretRotationPerFrame);
    if (mTurretAngle < 0) {
        mTurretAngle = 360 + mTurretAngle;
    }
    if (mTurretAngle > 360) {
        mTurretAngle = mTurretAngle - 360;
    }


    float a = mAngle * M_PI / 180;
    mXvel = std::cos(a) * forwardPerFrame;
    mYvel = std::sin(a) * forwardPerFrame;
    if (mXvel != 0) {
        if (mXvel < 0 && mX > 0) {
            mX += mXvel;
            mCollider->SetX(mCollider->GetX() + mXvel);
        }
        else if (mXvel > 0 && mX + mWidth < maxX) {
            mX += mXvel;
            mCollider->SetX(mCollider->GetX() + mXvel);
        }
    }
    if (mYvel != 0) {
        if (mYvel < 0 && mY > 0) {
            mY += mYvel;
            mCollider->SetY(mCollider->GetY() + mYvel);
        }
        else if (mYvel > 0 && mY + mHeight < maxY) {
            mY += mYvel;
            mCollider->SetY(mCollider->GetY() + mYvel);
        }
    }

    for (const Collider* c : parentMap->GetColliders()) {
        CheckCollision(*c);
    }


}
