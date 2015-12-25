#ifndef _COLLISIONINFO_H
#define _COLLISIONINFO_H

#include "Vector2D.hpp"

class CollisionInfo {
private:
    bool bColliding, bWillCollide;
    Vector2D vMinimumTranslation;
public:
    CollisionInfo();
    CollisionInfo(const bool colliding, const bool willcollide, const Vector2D& minimumtranslation);
    virtual ~CollisionInfo();

    const bool Colliding() const;
    const bool WillCollide() const;
    const Vector2D& MinimumTranslation() const;

    void SetColliding(const bool val);
    void SetWillCollide(const bool val);
    void SetMinimumTranslation(const Vector2D& val);
};

#endif // _COLLISIONINFO_H
