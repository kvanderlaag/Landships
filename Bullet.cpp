#include "Bullet.hpp"

int Bullet::next = 0;

Bullet::Bullet(float x, float y, float a, const Vector2D& dir, Player& owner, SDL_Renderer* ren) :
    RenderableObject("Bullet.png", 4, 4, x, y, a, ren),
    mCollider(Collider(4, 4, x, y, a, this) ),
    mOwner(owner),
    mDirection(dir),
    mMaxBounce(owner.GetMaxBounce()),
    mBounce(0),
    dead(false)
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

void Bullet::Bounce(Vector2D mt) {
        if (mBounce < mMaxBounce) {
            //x += mt.GetX();
            //y += mt.GetY();
            mCollider.Move(x, y);
            if (mDirection.GetX() < 0) {
                if (mDirection.GetY() < 0) {
                    if (mt.GetX() > 0) {
                        // Rotate left
                        Rotate(90);
                    } else {
                        // Rotate right
                        Rotate(-90);
                    }
                } else if (mDirection.GetY() > 0) {
                    if (mt.GetX() > 0) {
                        // Rotate right
                        Rotate(-90);
                    } else {
                        // Rotate left
                        Rotate(90);
                    }
                } else {
                    Rotate(180);
                }
            } else if (mDirection.GetX() > 0) {
                if (mDirection.GetY() < 0) {
                    if (mt.GetX() > 0) {
                        // Rotate left
                        Rotate(90);
                    } else {
                        // Rotate right
                        Rotate(-90);
                    }
                } else if (mDirection.GetY() > 0) {
                    if (mt.GetX() < 0) {
                        // Rotate right
                        Rotate(90);
                    } else {
                        // Rotate left
                        Rotate(-90);
                    }
                } else {
                    Rotate(180);
                }
            } else {
                Rotate(180);
            }
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

const bool Bullet::IsDead() const {
    return dead;
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

void Bullet::Update(uint32_t ticks) {
    float forwardPerFrame = BULLET_SPEED * ticks / 16 ;
    x += mDirection.GetX() * 0.05;
    y += mDirection.GetY() * 0.05;
    mCollider.Move(x, y);
}

CollisionInfo Bullet::CheckCollision(const Player& player) {
    Vector2D vel(mDirection * BULLET_SPEED * 0.05);
    CollisionInfo coll = mCollider.CheckCollision(player.GetCollider(), vel);

    dead = true;

    return coll;
}

CollisionInfo Bullet::CheckCollision(const Collider& other) {
    Vector2D vel(mDirection * BULLET_SPEED * 0.05);
    CollisionInfo coll = mCollider.CheckCollision(other, vel);

    if (coll.Colliding()) {
        Bounce(coll.MinimumTranslation());
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
    /* Collider rendering
    std::vector<Point> colliderPoints = mCollider.GetPoints();
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderDrawLine(mRenderer, colliderPoints[0].x, colliderPoints[0].y, colliderPoints[1].x, colliderPoints[1].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[1].x, colliderPoints[1].y, colliderPoints[2].x, colliderPoints[2].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[2].x, colliderPoints[2].y, colliderPoints[3].x, colliderPoints[3].y);
    SDL_RenderDrawLine(mRenderer, colliderPoints[3].x, colliderPoints[3].y, colliderPoints[0].x, colliderPoints[0].y);
    /* End of Collider Rendering */
}
