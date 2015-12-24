#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include <cmath>
#include <algorithm>

class Vector2D
{
private:
    float x, y;
    float magnitude;
public:
    Vector2D();
    Vector2D(float x, float y);
    Vector2D(const Vector2D& other);
    virtual ~Vector2D();

    Vector2D& operator=(Vector2D other);

    Vector2D operator+=(const Vector2D& other);
    Vector2D operator-=(const Vector2D& other);
    Vector2D operator*=(const float other);
    Vector2D operator*=(const Vector2D& other);
    Vector2D operator/=(const float other);

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(const float other) const;
    Vector2D operator*(const Vector2D& other) const;
    Vector2D operator/(const float other) const;

    const bool operator==(const Vector2D& rhs) const;
    const bool operator!=(const Vector2D& rhs) const { return !(*this==rhs); }

    void SetX(float newx);
    void SetY(float newy);

    const float GetX() const;
    const float GetY() const;
    const float GetMagnitude() const;

    const Vector2D Normalized() const;

    void Scale(float scalefactor);
    float Dot(const Vector2D& other);

};

#endif // _VECTOR2D_H
