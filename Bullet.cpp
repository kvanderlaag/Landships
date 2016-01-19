#include "Bullet.hpp"

#include <iostream>
#include <random>
#include <ctime>

int Bullet::next = 0;

Bullet::Bullet(float x, float y, float a, const Vector2D& dir, Player& owner, SDL_Renderer* ren) :
    RenderableObject("Bullet.png", 4, 4, x, y, a, ren),
    mCollider(Collider(3, 3, x, y, a, this) ),
    mOwner(owner),
    mDirection(dir),
    mVelocity(Vector2D()),
    mMaxBounce(owner.GetMaxBounce()),
    mBounce(0)

{
    next++;
    SDL_QueryTexture(mTexture, NULL, NULL, &width, &height);
}

Bullet::~Bullet() {

}

const Collider& Bullet::GetCollider() const {
    return mCollider;
}

Player& Bullet::GetOwner() {
    return mOwner;
}

const Vector2D& Bullet::GetDirection() const {
    return mDirection;
}

void Bullet::Bounce(const CollisionInfo& coll, const uint32_t ticks) {

        if (mBounce < mMaxBounce) {
            std::uniform_int_distribution<int> dist(1,3);
            int rnd = dist(generator);
            Utility::PlaySound(sfxBounce[rnd - 1]);
            //std::cout << "X: " << mDirection.GetX() << " Y: " << mDirection.GetY() << " Angle: " << mDirection.Angle() << std::endl;
            //std::cout << "MT Angle: " << coll.MinimumTranslation().Normalized().Angle() << std::endl;
            mDirection = mDirection.Reflect(coll.MinimumTranslation().Normalized());
            angle = mDirection.Angle();
            mCollider.SetAngle(angle);
            mVelocity.SetX(mDirection.GetX() * ((float) ticks / 1000) * BULLET_SPEED);
            mVelocity.SetY(mDirection.GetY() * ((float) ticks / 1000) * BULLET_SPEED);
            x += mVelocity.GetX();
            y += mVelocity.GetY();
            if (coll.Colliding() ) {
                x += coll.MinimumTranslation().GetX();
                y += coll.MinimumTranslation().GetY();
                mCollider.Move(x, y);
            }

            //std::cout << "X: " << mDirection.GetX() << " Y: " << mDirection.GetY() << " Angle: " << mDirection.Angle() << std::endl;

            mBounce++;
        } else {
            dead = true;
        }
}
const int Bullet::GetBounce() const {
    return mBounce;
}
const int Bullet::GetMaxBounce() const {
    return mMaxBounce;
}

void Bullet::Rotate(float rotation) {
    angle += rotation;
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
    mCollider.Rotate(rotation);
    float a = angle * M_PI / 180;
    mDirection = Vector2D(std::sin(a), -std::cos(a));
}

void Bullet::SetAngle(float ang) {
    angle = ang;
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
    mCollider.SetAngle(angle);
    float a = angle * M_PI / 180;
    mDirection = Vector2D(std::sin(a), -std::cos(a));
}

void Bullet::Update(uint32_t ticks) {

    if (x > ARENA_WIDTH || x < 0 || y > ARENA_HEIGHT || y < 0) {
        Die();
        mOwner.DestroyBullet();
    }

    mVelocity.SetX(mDirection.GetX() * ((float) ticks / 1000) * BULLET_SPEED);
    mVelocity.SetY(mDirection.GetY() * ((float) ticks / 1000) * BULLET_SPEED);

    x += mVelocity.GetX();
    y += mVelocity.GetY();
    mCollider.Move(x, y);
}

CollisionInfo Bullet::CheckCollision(const Player& player, const uint32_t ticks) {
    CollisionInfo coll = mCollider.CheckCollision(player.GetCollider(), mVelocity);

    //dead = true;

    return coll;
}

CollisionInfo Bullet::CheckCollision(const Container& container, const uint32_t ticks) {
    CollisionInfo coll = mCollider.CheckCollision(container.GetCollider(), mVelocity);

    //dead = true;

    return coll;
}

CollisionInfo Bullet::CheckCollision(const DestructibleBlock& destr, const uint32_t ticks) {
    CollisionInfo coll = mCollider.CheckCollision(destr.GetCollider(), mVelocity);

    if (coll.Colliding() || coll.WillCollide()) {
        if (destr.Health() > 1 && destr.Bounce()) {
            Bounce(coll, ticks);
        }
    }

    return coll;
}


CollisionInfo Bullet::CheckCollision(const Bullet& bullet, const uint32_t ticks) {
    CollisionInfo coll = mCollider.CheckCollision(bullet.GetCollider(), mVelocity);

    //dead = true;

    return coll;
}

CollisionInfo Bullet::CheckCollision(const Collider& other, const uint32_t ticks) {
    CollisionInfo coll = mCollider.CheckCollision(other, mVelocity);

    if (coll.Colliding() || coll.WillCollide() ) {
        Bounce(coll, ticks);
    }

    return coll;
}

void Bullet::Render() {
    SDL_Rect srcRect, dstRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = width;
    srcRect.h = height;

    dstRect.x = x - (width / 2);
    dstRect.y = y - (height / 2);
    dstRect.w = width;
    dstRect.h = height;

    SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, angle, NULL, SDL_FLIP_NONE);
    /* Collider rendering */
    #ifdef _COLLISION_DEBUG
    std::vector<Point> colliderPoints = mCollider.GetPoints();
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderDrawLine(mRenderer, colliderPoints[0].x, colliderPoints[0].y, colliderPoints[1].x, colliderPoints[1].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[1].x, colliderPoints[1].y, colliderPoints[2].x, colliderPoints[2].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[2].x, colliderPoints[2].y, colliderPoints[3].x, colliderPoints[3].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[3].x, colliderPoints[3].y, colliderPoints[0].x, colliderPoints[0].y);
    /* End of Collider Rendering */
    #endif
}

const Vector2D& Bullet::GetVelocity() const {
    return mVelocity;
}
