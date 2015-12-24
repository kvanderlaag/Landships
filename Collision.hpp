#ifndef _COLLISION_HPP
#define _COLLISION_HPP

#include <vector>

#include <cmath>

class Collision {
private:
    bool mWillIntersect;
    bool mIntersecting;
    std::vector<float> mMinimumTranslationVector;

public:
    Collision(const bool willIntersect, const bool intersecting, const float x, const float y);
    virtual ~Collision();

    void SetIntersecting(bool val);
    void SetWillIntersect(bool val);
    void SetMinimumTranslationVector(const float x, const float y);

    const bool WillIntersect() const;
    const bool Intersecting() const;

    const std::vector<float> GetMinimumTranslationVector() const;
};


#endif // _COLLISION_HPP
