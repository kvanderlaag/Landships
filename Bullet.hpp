#ifndef _BULLET_H
#define _BULLET_H

#include "RenderableObject.hpp"
#include "Collider.hpp"
#include "Player.hpp"

class Bullet : public RenderableObject {
private:
    Collider mCollider;
    const Player& mOwner;
    Vector2D mDirection;
public:
    Bullet(float x, float y, const Vector2D& dir, const Player& owner, SDL_Renderer* ren);
    virtual ~Bullet();

    const Collider& GetCollider() const;
    const Player& GetOwner() const;
    const Vector2D& GetDirection() const;

    void Update();

};
#endif // _BULLET_H
