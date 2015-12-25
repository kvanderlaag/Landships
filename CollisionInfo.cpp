#include "CollisionInfo.hpp"

CollisionInfo::CollisionInfo() :
    bColliding(true),
    bWillCollide(true),
    vMinimumTranslation(Vector2D(0, 0))
{

}

CollisionInfo::CollisionInfo(const bool colliding, const bool willcollide, const Vector2D& minimumtranslation) :
    bColliding(colliding),
    bWillCollide(willcollide),
    vMinimumTranslation(minimumtranslation)
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

void CollisionInfo::SetColliding(const bool val) {
    bColliding = val;
}

void CollisionInfo::SetWillCollide(const bool val) {
    bWillCollide = val;
}

void CollisionInfo::SetMinimumTranslation(const Vector2D& val) {
    vMinimumTranslation = val;
}
