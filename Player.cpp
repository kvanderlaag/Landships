#include "Player.hpp"

//#define _COLLISION_DEBUG

Player::Player(const std::string& filename, SDL_Renderer* ren) :
    RenderableObject(PLAYER_SIZE, PLAYER_SIZE, 0, 0, 0, ren),
    mtexture(Utility::LoadTexture(ren, filename)),
    mRenderer(ren),
    mAngle(0),
    mForwardVel(0),
    mRotationVel(0),
    mTurretAngle(0),
    mTurretRotationVel(0),
    mX(0),
    mY(0),
    mCollider(PLAYER_SIZE, PLAYER_SIZE, 0, 0, 0, this)
{
    SDL_QueryTexture(mtexture, NULL, NULL, &mWidth, &mHeight);
    mWidth = mWidth / (mWidth / 16);
}

Player::~Player() {
    SDL_DestroyTexture(mtexture);
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
    mCollider.Move(newx, mY);
}

void Player::SetY(float newy) {
    mY = newy;
    mCollider.Move(mX, newy);
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

    dstRect.x = mX - (mWidth / 2);
    dstRect.y = mY - (mHeight / 2);
    dstRect.h = mHeight;
    dstRect.w = mWidth;
    SDL_RenderCopyEx(mRenderer, mtexture, &srcRect, &dstRect, mAngle, NULL, SDL_FLIP_NONE);

    srcRect.x = 16;
    srcRect.y = 0;
    srcRect.h = mHeight;
    srcRect.w = mWidth;
    SDL_RenderCopyEx(mRenderer, mtexture, &srcRect, &dstRect, mTurretAngle, NULL, SDL_FLIP_NONE);

    /* Collider rendering
    std::vector<Point> colliderPoints = mCollider.GetPoints();
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderDrawLine(mRenderer, colliderPoints[0].x, colliderPoints[0].y, colliderPoints[1].x, colliderPoints[1].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[1].x, colliderPoints[1].y, colliderPoints[2].x, colliderPoints[2].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[2].x, colliderPoints[2].y, colliderPoints[3].x, colliderPoints[3].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[3].x, colliderPoints[3].y, colliderPoints[0].x, colliderPoints[0].y);
    /* End of Collider Rendering */
}

const Collider& Player::GetCollider() const {
    return mCollider;
}

void Player::CheckCollision(const Collider& other, uint32_t ticks) {
    if (&mCollider != &other) {
        Vector2D velocity(mXvel * ticks / 100, mYvel * ticks / 100);
        Vector2D mt = mCollider.CheckCollision(other, velocity);
        mX += mt.GetX();
        mY += mt.GetY();
    }
}

void Player::Update(uint32_t ticks) {
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

    mCollider.Rotate(rotationPerFrame);

    mTurretAngle += (rotationPerFrame + turretRotationPerFrame);
    if (mTurretAngle < 0) {
        mTurretAngle = 360 + mTurretAngle;
    }
    if (mTurretAngle > 360) {
        mTurretAngle = mTurretAngle - 360;
    }


    float a = mAngle * M_PI / 180;
    mXvel = std::sin(a) * forwardPerFrame;
    mYvel = std::cos(a) * -forwardPerFrame;
    if (mXvel != 0) {
        if (mXvel < 0 && mX > (mWidth / 2)) {
            mX += mXvel;
        }
        else if (mXvel > 0 && mX + (mWidth / 2) < maxX) {
            mX += mXvel;
        }
    }
    if (mYvel != 0) {
        if (mYvel < 0 && mY > (mHeight / 2)) {
            mY += mYvel;
        }
        else if (mYvel > 0 && mY + (mHeight / 2) < maxY) {
            mY += mYvel;
        }
    }

    mCollider.Move(mX, mY);
}
