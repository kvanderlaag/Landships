#ifndef _BULLET_H
#define _BULLET_H

#include "RenderableObject.hpp"
#include "Collider.hpp"
#include "CollisionInfo.hpp"
#include "Player.hpp"
#include "Container.hpp"

extern Mix_Chunk* sfxBounce[3];
extern std::default_random_engine generator;

class Player;

class Bullet : public RenderableObject {
private:
    Collider mCollider;
    Player& mOwner;
    Vector2D mDirection;
    Vector2D mVelocity;
    const int BULLET_SPEED = 60;
    int mMaxBounce, mBounce;
public:
    static int next;
    Bullet(float x, float y, float a, const Vector2D& dir, Player& owner, SDL_Renderer* ren);
    virtual ~Bullet();

    const Collider& GetCollider() const;
    Player& GetOwner();
    const Vector2D& GetDirection() const;

    void Bounce(const CollisionInfo& coll, const uint32_t ticks);
    const int GetBounce() const;
    const int GetMaxBounce() const;

    void Rotate(float rotation);
    void SetAngle(float ang);

    void Update(uint32_t ticks);

    CollisionInfo CheckCollision(const Player& player, const uint32_t ticks);
    CollisionInfo CheckCollision(const Container& container, const uint32_t ticks);
    CollisionInfo CheckCollision(const Collider& other, const uint32_t ticks);

    void Render();

};
#endif // _BULLET_H
