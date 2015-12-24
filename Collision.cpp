#include "Collision.hpp"

Collision::Collision(const bool intersecting, const bool willintersect, const float x, const float y) :
        mIntersecting(intersecting),
        mWillIntersect(willintersect),
        mMinimumTranslationVector(x, y)
{

}

Collision::~Collision() {

}

void Collision::SetIntersecting(const bool val) {
    mIntersecting = val;
}

void Collision::SetWillIntersect(const bool val) {
    mWillIntersect = val;
}

void Collision::SetMinimumTranslationVector(const float x, const float y) {
    mMinimumTranslationVector = std::vector<float>(x, y);
}

const bool Collision::Intersecting() const {
    return mIntersecting;
}

const bool Collision::WillIntersect() const {
    return mWillIntersect;
}

const std::vector<float> Collision::GetMinimumTranslationVector() const {
    return mMinimumTranslationVector;
}
