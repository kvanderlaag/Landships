#ifndef _COLLISIONINFO_H
#define _COLLISIONINFO_H

#include "Vector2D.hpp"

class CollisionInfo {
private:
    bool bColliding, bWillCollide;
    Vector2D vMinimumTranslation;
    Vector2D vCollisionNormal;
public:
    CollisionInfo();
    CollisionInfo(const bool colliding, const bool willcollide, const Vector2D& minimumtranslation, const Vector2D& collisionnormal);
    virtual ~CollisionInfo();

    const bool Colliding() const;
    const bool WillCollide() const;
    const Vector2D& MinimumTranslation() const;
    const Vector2D& CollisionNormal() const;

    void SetColliding(const bool val);
    void SetWillCollide(const bool val);
    void SetMinimumTranslation(const Vector2D& val);
    void SetCollisionNormal(const Vector2D& val);
};

#endif // _COLLISIONINFO_H
