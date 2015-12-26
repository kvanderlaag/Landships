#include "CollisionInfo.hpp"

CollisionInfo::CollisionInfo() :
    bColliding(true),
    bWillCollide(true),
    vMinimumTranslation(Vector2D(0, 0)),
    vCollisionNormal(Vector2D(0, 0))
{

}

CollisionInfo::CollisionInfo(const bool colliding, const bool willcollide, const Vector2D& minimumtranslation, const Vector2D& collisionnormal) :
    bColliding(colliding),
    bWillCollide(willcollide),
    vMinimumTranslation(minimumtranslation),
    vCollisionNormal(collisionnormal)
{

}

CollisionInfo::~CollisionInfo() {

}

const bool CollisionInfo::Colliding() const {
    return bColliding;
}

const bool CollisionInfo::WillCollide() const {
    return bWillCollide;
}

const Vector2D& CollisionInfo::MinimumTranslation() const {
    return vMinimumTranslation;
}

const Vector2D& CollisionInfo::CollisionNormal() const {
    return vCollisionNormal;
}

void CollisionInfo::SetColliding(const bool val) {
    bColliding = val;
}

void CollisionInfo::SetWillCollide(const bool val) {
    bWillCollide = val;
}

void CollisionInfo::SetMinimumTranslation(const Vector2D& val) {
    vMinimumTranslation = val;
}

void CollisionInfo::SetCollisionNormal(const Vector2D& val) {
    vCollisionNormal = val;
}
