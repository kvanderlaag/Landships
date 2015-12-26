#ifndef _BULLET_H
#define _BULLET_H

#include "RenderableObject.hpp"
#include "Collider.hpp"
#include "CollisionInfo.hpp"
#include "Player.hpp"

class Player;

class Bullet : public RenderableObject {
private:
    Collider mCollider;
    Player& mOwner;
    Vector2D mDirection;
    const int BULLET_SPEED = 3000;
    int mMaxBounce, mBounce;
    bool dead;
public:
    static int next;
    Bullet(float x, float y, float a, const Vector2D& dir, Player& owner, SDL_Renderer* ren);
    virtual ~Bullet();

    const Collider& GetCollider() const;
    Player& GetOwner();
    const Vector2D& GetDirection() const;

    void Bounce(Vector2D mt);
    const int GetBounce() const;
    const int GetMaxBounce() const;
    const bool IsDead() const;

    void Rotate(float rotation);

    void Update(uint32_t ticks);

    CollisionInfo CheckCollision(const Player& player);
    CollisionInfo CheckCollision(const Collider& other);

    void Render();

};
#endif // _BULLET_H
