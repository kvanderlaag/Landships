#include "Collider.hpp"

Collider::Collider(float w, float h, float x, float y, float a, const GameObject* parent, const bool passable, const bool stopsShots) :
    GameObject(w, h, x, y, a),
    mRectangle(w, h, x, y, a),
    owner(*parent),
    mPassable(passable),
    mStopsShots(stopsShots)
{

}

Collider::~Collider() {

}

void Collider::Move(float newx, float newy) {
    mRectangle.Move(newx, newy);
    x = newx;
    y = newy;

}

void Collider::Rotate(float rotation) {
    mRectangle.Rotate(rotation);
    angle = mRectangle.GetAngle();
}

void Collider::SetAngle(float angle) {
    mRectangle.SetAngle(angle);
    angle = mRectangle.GetAngle();
}

const CollisionInfo Collider::CheckCollision(const Collider& other, const Vector2D& velocity) const {

    //Initialize collision info object
    CollisionInfo returnCollision;


    float minIntervalDistance = FLT_MAX;
    Vector2D translationAxis;

    std::vector<Vector2D> normals;
    if (angle == 0 && other.GetAngle() == 0) {
        normals = {Vector2D(0, -1), Vector2D(-1, 0)};
    } else {
        normals = {mRectangle.GetUpNormal(), mRectangle.GetLeftNormal(), other.GetRectangle().GetUpNormal(), other.GetRectangle().GetLeftNormal()};
    }

    for (Vector2D& axis : normals) {

        // Check for collision
        float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
        Utility::ProjectRectangle(axis, mRectangle, minA, maxA);
        Utility::ProjectRectangle(axis, other.GetRectangle(), minB, maxB);

        if (Utility::IntervalDistance(minA, maxA, minB, maxB) > 0) {
            returnCollision.SetColliding(false);
        }


        // Check for potential collision with velocity
        float velocityProjection = axis.Dot(velocity);

        if (velocityProjection < 0) {
            minA += velocityProjection;
        } else {
            maxA += velocityProjection;
        }

        float intervalDistance = Utility::IntervalDistance(minA, maxA, minB, maxB);
        if (intervalDistance > 0) returnCollision.SetWillCollide(false);

        // If the polygons are not intersecting and won't intersect, exit the loop
        if (!returnCollision.Colliding() && !returnCollision.WillCollide())
            break;

        // Check if the current interval distance is the minimum one. If so store
        // the interval distance and the current distance.
        // This will be used to calculate the minimum translation vector
        intervalDistance = std::abs(intervalDistance);
        if (intervalDistance < minIntervalDistance) {
            minIntervalDistance = intervalDistance;
            translationAxis = axis;

            Vector2D d( mRectangle.GetCenter().x - other.GetRectangle().GetCenter().x, mRectangle.GetCenter().y - other.GetRectangle().GetCenter().y );
            if (d.Dot(translationAxis) < 0) {
                translationAxis = Vector2D(-translationAxis.GetX(), -translationAxis.GetY());
            }
        }


    }

        if (returnCollision.WillCollide())
            returnCollision.SetMinimumTranslation(translationAxis * minIntervalDistance);

        return returnCollision;
}


const Rectangle& Collider::GetRectangle() const {
    return mRectangle;
}

const std::vector<Point> Collider::GetPoints() const {
    return mRectangle.GetPoints();
}

const GameObject& Collider::GetOwner() const {
    return owner;
}

const bool Collider::Passable() const {
     return mPassable;
}

const bool Collider::StopsShots() const {
     return mStopsShots;
}
