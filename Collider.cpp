#include "Collider.hpp"

//#define _COLLISION_DEBUG

Collider::Collider(int width, int height, float x, float y, float a, SDL_Renderer* ren) :
    mWidth(width),
    mHeight(height),
    mX(x),
    mY(y),
    mAngle(a),
    mRenderer(ren)
{

}

Collider::~Collider() {

}

const int Collider::GetWidth() const {
    return mWidth;
}

const int Collider::GetHeight() const {
    return mHeight;
}

const float Collider::GetAngle() const {
    return mAngle;
}

const float Collider::GetX() const {
    return mX;
}

const float Collider::GetY() const {
    return mY;
}

void Collider::SetWidth(int newwidth) {
    mWidth = newwidth;
}

void Collider::SetHeight(int newheight) {
    mHeight = newheight;
}

void Collider::SetAngle(float newangle) {
    mAngle = newangle;
}

void Collider::SetX(float newx) {
    mX = newx;
}

void Collider::SetY(float newy) {
    mY = newy;
}

const Collision Collider::IsColliding(const Collider& other, std::vector<float> velocity) const {
    float minIntervalDistance = FLT_MAX;

    std::vector<float> translationAxis = {0 , 0};

    /* Polygon points */
    std::vector<std::vector<float>> thisPoints = GetPoints();
    std::vector<std::vector<float>> otherPoints = other.GetPoints();

    /* Polygon edges */
    std::vector<float> thisTop    =  { thisPoints[0][0], thisPoints[0][1], thisPoints[1][0], thisPoints[1][1] };
    std::vector<float> thisLeft   =  { thisPoints[0][0], thisPoints[0][1], thisPoints[2][0], thisPoints[2][1] };

    std::vector<float> otherTop    = { otherPoints[0][0], otherPoints[0][1], otherPoints[1][0], otherPoints[1][1] };
    std::vector<float> otherLeft   = { otherPoints[0][0], otherPoints[0][1], otherPoints[2][0], otherPoints[2][1] };

    /* Polygon normals */
    std::vector<float> thisNormal1 = { thisTop[3] - thisTop[1], -(thisTop[2] - thisTop[0]) };
    std::vector<float> thisNormal2 = { thisLeft[3] - thisLeft[1], -(thisLeft[2] - thisLeft[0]) };
    Utility::NormalizeVector2D(thisNormal1);
    Utility::NormalizeVector2D(thisNormal2);

    std::vector<float> otherNormal1 = { otherTop[3] - otherTop[1], -(otherTop[2] - otherTop[0]) };
    std::vector<float> otherNormal2 = { otherLeft[3] - otherLeft[1], -(otherLeft[2] - otherLeft[0]) };
    Utility::NormalizeVector2D(otherNormal1);
    Utility::NormalizeVector2D(otherNormal2);

    std::vector<std::vector<float>> normals = { thisNormal1, thisNormal2, otherNormal1, otherNormal2 };

    bool colliding = true;
    bool willCollide = true;

    for (std::vector<float> normal : normals) {
        float tMin, tMax;
        float oMin, oMax;
        tMin = tMax = oMin = oMax = 0;

        Utility::ProjectCollider(*this, thisNormal1, tMin, tMax);
        Utility::ProjectCollider(other, thisNormal1, oMin, oMax);

        if (Utility::IntervalDistance(tMin, tMax, oMin, oMax) <= 0) {
            // collision on axis
        } else {
            colliding = false;
            break;
        }

        float velocityProjection = Utility::DotProduct(velocity, normal);

        if (velocityProjection < 0) {
            tMin += velocityProjection;
        } else {
            tMax += velocityProjection;
        }

        // Do the same test as above for the new projection
        float intervalDistance = Utility::IntervalDistance(tMin, tMax, oMin, oMax);
        if (intervalDistance > 0) willCollide = false;

        // If the polygons are not intersecting and won't intersect, exit the loop
        if (!colliding && !willCollide) break;

        // Check if the current interval distance is the minimum one. If so store
        // the interval distance and the current distance.
        // This will be used to calculate the minimum translation vector
        intervalDistance = std::abs(intervalDistance);
        if (intervalDistance < minIntervalDistance) {
            minIntervalDistance = intervalDistance;
            translationAxis = normal;

            std::vector<float> thisCenter, otherCenter;
            thisCenter = {((thisPoints[3][0] - thisPoints[0][0]) / 2), ((thisPoints[3][1] - thisPoints[0][1]) / 2)};
            otherCenter = {((otherPoints[3][0] - otherPoints[0][0]) / 2), ((otherPoints[3][1] - otherPoints[0][1]) / 2)};

            std::vector<float> d = {thisCenter[0] - otherCenter[0], thisCenter[1] = otherCenter[1] };
            if (Utility::DotProduct(d, translationAxis) < 0)
                translationAxis = { -translationAxis[0], -translationAxis[1]};
        }
    }

    Collision coll(colliding, willCollide, translationAxis[0], translationAxis[1]);
    return coll;
}

void Collider::Render() {
    #ifdef _COLLISION_DEBUG
    {
        int ULx, ULy;
        int BLx, BLy;
        int URx, URy;
        int BRx, BRy;
        int x = GetX() + (GetWidth() / 2);
        int y = GetY() + (GetHeight() / 2);
        int Width = GetWidth();
        int Height = GetHeight();
        float A = GetAngle() * M_PI / 180;

        ULx = x + ( Width / 2 ) * std::cos(A) - ( Height / 2 ) * std::sin(A);
        ULy = y + ( Height / 2 ) * std::cos(A)  + ( Width / 2 ) * std::sin(A);
        URx = x - ( Width / 2 ) * std::cos(A) - ( Height / 2 ) * std::sin(A);
        URy = y + ( Height / 2 ) * std::cos(A) - ( Width / 2 ) * std::sin(A);

        BLx = x + ( Width / 2 ) * std::cos(A) + ( Height / 2 ) * std::sin(A);
        BLy = y - ( Height / 2 ) * std::cos(A)  + ( Width / 2 ) * std::sin(A);
        BRx = x - ( Width / 2 ) * std::cos(A) + ( Height / 2 ) * std::sin(A);
        BRy = y - ( Height / 2 ) * std::cos(A)  - ( Width / 2 ) * std::sin(A);

        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderDrawLine(mRenderer, ULx, ULy, URx, URy);
        SDL_RenderDrawLine(mRenderer, BLx, BLy, BRx, BRy);
        SDL_RenderDrawLine(mRenderer, ULx, ULy, BLx, BLy);
        SDL_RenderDrawLine(mRenderer, URx, URy, BRx, BRy);
    }
    #endif // _COLLISION_DEBUG
}

const std::vector<std::vector<float>> Collider::GetPoints() const {
    int ULx, ULy;
    int BLx, BLy;
    int URx, URy;
    int BRx, BRy;
    ULx = ULy = BLx = BLy = URx = URy = BRx = BRy = 0;
    int x = GetX() + (GetWidth() / 2);
    int y = GetY() + (GetHeight() / 2);
    int Width = GetWidth();
    int Height = GetHeight();
    float A = GetAngle() * M_PI / 180;

    if (A == 0) {
        ULx = x;
        ULy = y;
        URx = x + Width;
        URy = y;
        BLx = x;
        BLy = y + Height;
        BRx = x + Width;
        BRy = y + Height;
    } else {

        ULx = x + ( Width / 2 ) * std::cos(A) - ( Height / 2 ) * std::sin(A);
        ULy = y + ( Height / 2 ) * std::cos(A)  + ( Width / 2 ) * std::sin(A);
        URx = x - ( Width / 2 ) * std::cos(A) - ( Height / 2 ) * std::sin(A);
        URy = y + ( Height / 2 ) * std::cos(A) - ( Width / 2 ) * std::sin(A);

        BLx = x + ( Width / 2 ) * std::cos(A) + ( Height / 2 ) * std::sin(A);
        BLy = y - ( Height / 2 ) * std::cos(A)  + ( Width / 2 ) * std::sin(A);
        BRx = x - ( Width / 2 ) * std::cos(A) + ( Height / 2 ) * std::sin(A);
        BRy = y - ( Height / 2 ) * std::cos(A)  - ( Width / 2 ) * std::sin(A);
    }

    std::vector<float> UL(ULx, ULy);
    std::vector<float> UR(URx, URy);
    std::vector<float> BL(BLx, BLy);
    std::vector<float> BR(BRx, BRy);
    std::vector<std::vector<float>> ret;
    ret.push_back(UL);
    ret.push_back(UR);
    ret.push_back(BL);
    ret.push_back(BR);
    return ret;
}
