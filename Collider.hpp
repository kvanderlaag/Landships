#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "GameObject.hpp"
#include "Rectangle.hpp"
#include "Vector2D.hpp"
#include "CollisionInfo.hpp"
#include "Utility.hpp"

#include <cfloat>
#include <cmath>
#include <vector>
#include "Point.hpp"

class Collider : public GameObject {
public:
    Collider(float w, float h, float x, float y, float a, const GameObject* parent, const bool passable, const bool stopsShots);
    virtual ~Collider();

    void Move(float x, float y);
    void Rotate(float rotation);
    void SetAngle(float angle);
    void Update(uint32_t ticks) {};

    const CollisionInfo CheckCollision(const Collider& other, const Vector2D& velocity) const;

    const Rectangle& GetRectangle() const;

    const std::vector<Point> GetPoints() const;
    const GameObject& GetOwner() const;

    const bool Passable() const;
    const bool StopsShots() const;

private:
    Rectangle mRectangle;
    const GameObject& owner;
    bool mPassable;
    bool mStopsShots;


};
#endif // _COLLIDER_H
