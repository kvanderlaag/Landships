#include "Bullet.hpp"

int Bullet::next = 0;

Bullet::Bullet(float x, float y, const Vector2D& dir, Player& owner, SDL_Renderer* ren) :
    RenderableObject("Bullet.png", 4, 4, x, y, 0, ren),
    mCollider(Collider(4, 4, x, y, 0, this) ),
    mOwner(owner),
    mDirection(dir)
{
    next++;
    SDL_QueryTexture(mTexture, NULL, NULL, &width, &height);
}

Bullet::~Bullet() {
    mOwner.DestroyBullet();
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

void Bullet::Update(uint32_t ticks) {
    x += mDirection.GetX() * 0.05;
    y += mDirection.GetY() * 0.05;
    mCollider.Move(x, y);
}

CollisionInfo Bullet::CheckCollision(const Player& player) {
    Vector2D vel(mDirection * BULLET_SPEED);
    CollisionInfo coll = mCollider.CheckCollision(player.GetCollider(), vel);

    return coll;
}

CollisionInfo Bullet::CheckCollision(const Collider& other) {
    Vector2D vel(mDirection * BULLET_SPEED);
    CollisionInfo coll = mCollider.CheckCollision(other, vel);

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

    SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);
    //SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //SDL_RenderDrawRect(mRenderer, &dstRect);
}
