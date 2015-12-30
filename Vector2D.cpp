#include "Vector2D.hpp"

Vector2D::Vector2D() :
    x(0),
    y(0),
    magnitude(0)
{

}

Vector2D::Vector2D(float x, float y) :
    x(x),
    y(y),
    magnitude(std::sqrt(x * x + y * y))
{
}

Vector2D::Vector2D(const Vector2D& other) :
    x(other.GetX()),
    y(other.GetY()),
    magnitude(other.GetMagnitude())
{

}

Vector2D& Vector2D::operator=(Vector2D other) {
    std::swap(x, other.x);
    std::swap(y, other.y);
    std::swap(magnitude, other.magnitude);
    return *this;
}

Vector2D::~Vector2D() {

}

Vector2D Vector2D::operator+=(const Vector2D& other) {
    x += other.GetX();
    y += other.GetY();
    magnitude = std::sqrt(x * x + y * y);
    return Vector2D(x, y);
}

Vector2D Vector2D::operator-=(const Vector2D& other) {
    x -= other.GetX();
    y -= other.GetY();
    magnitude = std::sqrt(x * x + y * y);
    return Vector2D(x, y);
}

Vector2D Vector2D::operator*=(const float other) {
    x *= other;
    y *= other;
    magnitude = std::sqrt(x * x + y * y);
    return Vector2D(x, y);
}

Vector2D Vector2D::operator*=(const Vector2D& other) {
    x *= other.GetX();
    y *= other.GetY();
    magnitude = std::sqrt(x * x + y * y);
    return Vector2D(x, y);
}

Vector2D Vector2D::operator/=(const float other) {
    x /= other;
    y /= other;
    magnitude = std::sqrt(x * x + y * y);
    return Vector2D(x, y);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.GetX(), y + other.GetY());
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.GetX(), y - other.GetY());
}

Vector2D Vector2D::operator*(const float other) const {
    return Vector2D(x * other, y * other);
}

Vector2D Vector2D::operator*(const Vector2D& other) const {
    return Vector2D(x * other.GetX(), y * other.GetY());
}

Vector2D Vector2D::operator/(const float other) const {
    return Vector2D(x / other, y / other);
}

const bool Vector2D::operator==(const Vector2D& rhs) const {
    return (x == rhs.GetX() && y == rhs.GetY() && magnitude == rhs.GetMagnitude());
}

void Vector2D::SetX(float newx) {
    x = newx;
    magnitude = std::sqrt(x * x + y * y);
}

void Vector2D::SetY(float newy) {
    y = newy;
    magnitude = std::sqrt(x * x + y * y);
}

const float Vector2D::GetX() const {
    return x;
}

const float Vector2D::GetY() const {
    return y;
}

const float Vector2D::GetMagnitude() const {
    return magnitude;
}

const Vector2D Vector2D::Normalized() const {
    return Vector2D(x / magnitude, y / magnitude);

}

void Vector2D::Scale(float scalefactor) {
    x *= scalefactor;
    y *= scalefactor;
    magnitude = std::sqrt(x * x + y * y);
}

const float Vector2D::Dot(const Vector2D& other) const {
    return (x * other.GetX()) + (y * other.GetY());
}

const Vector2D Vector2D::Reflect(const Vector2D& normal) const {
    float dotProduct = 2 * normal.Dot(*this);
    Vector2D n(normal * dotProduct);
    return Vector2D(n - *this) * -1;
}

const float Vector2D::Angle() const {
    float a = std::atan2(y, x) * 180 / M_PI;
    while (a < 0)
        a += 360;
    while (a > 360)
        a -= 360;
    return a;
}
