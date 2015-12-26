#include "Player.hpp"

//#define _COLLISION_DEBUG

Player::Player(const std::string& filename, SDL_Renderer* ren) :
    RenderableObject(filename, PLAYER_SIZE, PLAYER_SIZE, 0, 0, 0, ren),
    mForwardVel(0),
    mRotationVel(0),
    mTurretAngle(0),
    mTurretRotationVel(0),
    mCollider(PLAYER_SIZE, PLAYER_SIZE, 0, 0, 0, this),
    mMaxBullets(1),
    mBullets(0),
    mMaxBounce(2)
{
    SDL_QueryTexture(mTexture, NULL, NULL, &width, &height);
    width = width / (width / 16);
}

Player::~Player() {

}



const float Player::GetXVel() const {
    return xvel;
}

const float Player::GetYVel() const {
    return yvel;
}

void Player::SetXVel(float newvel) {
    xvel = newvel;
}

void Player::SetYVel(float newvel) {
    yvel = newvel;
}

void Player::SetX(float newx) {
    x = newx;
    mCollider.Move(newx, y);
}

void Player::SetY(float newy) {
    y = newy;
    mCollider.Move(x, newy);
}

void Player::SetForwardVel(float newvel) {
    mForwardVel = newvel;
}

const float Player::GetForwardVel() const {
    return mForwardVel;
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
    srcRect.h = height;
    srcRect.w = width;

    dstRect.x = x - (width / 2);
    dstRect.y = y - (height / 2);
    dstRect.h = height;
    dstRect.w = width;
    SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, angle, NULL, SDL_FLIP_NONE);

    srcRect.x = 16;
    srcRect.y = 0;
    srcRect.h = height;
    srcRect.w = width;
    SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, mTurretAngle, NULL, SDL_FLIP_NONE);

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
        Vector2D velocity(xvel * ticks / 100, yvel * ticks / 100);
        CollisionInfo coll = mCollider.CheckCollision(other, velocity);
        Vector2D mt = coll.MinimumTranslation();
        x += mt.GetX();
        y += mt.GetY();
    }
}

void Player::Update(uint32_t ticks) {
    float forwardPerFrame = mForwardVel * ticks / 16 ;
    float rotationPerFrame = mRotationVel * ticks / 16 ;
    float turretRotationPerFrame = mTurretRotationVel * ticks / 16 ;

    angle += rotationPerFrame;
    if (angle < 0) {
        angle += 360;
    }
    if (angle > 360) {
        angle -= 360;
    }

    mCollider.Rotate(rotationPerFrame);

    mTurretAngle += (rotationPerFrame + turretRotationPerFrame);
    if (mTurretAngle < 0) {
        mTurretAngle = 360 + mTurretAngle;
    }
    if (mTurretAngle > 360) {
        mTurretAngle = mTurretAngle - 360;
    }


    float a = angle * M_PI / 180;
    xvel = std::sin(a) * forwardPerFrame;
    yvel = std::cos(a) * -forwardPerFrame;
    if (xvel != 0) {
        if (xvel < 0 && x > (width / 2)) {
            x += xvel;
        }
        else if (xvel > 0 && x + (width / 2) < maxX) {
            x += xvel;
        }
    }
    if (yvel != 0) {
        if (yvel < 0 && y > (height / 2)) {
            y += yvel;
        }
        else if (yvel > 0 && y + (height / 2) < maxY) {
            y += yvel;
        }
    }

    mCollider.Move(x, y);
}

Bullet* Player::Fire() {
    if (mBullets >= mMaxBullets) {
        return nullptr;
    }
    mBullets++;
    float a = mTurretAngle * M_PI / 180;
    Vector2D direction(std::sin(a), -std::cos(a));
    Bullet* b = new Bullet(x, y, mTurretAngle, direction.Normalized(), *this, mRenderer);

    return b;
}

const int Player::GetMaxBullets() const {
    return mMaxBullets;
}

const int Player::GetBullets() const {
    return mBullets;
}

const int Player::GetMaxBounce() const {
    return mMaxBounce;
}

void Player::DestroyBullet() {
    if (mBullets > 0) {
        --mBullets;
    }
}
