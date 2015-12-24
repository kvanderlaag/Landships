#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "Point.hpp"
#include "Vector2D.hpp"
#include <vector>

class Rectangle {
private:
    Point p1, p2, p3, p4, center;
    float angle;
    float width, height;

    void CalculateCenter();
    void CalculateAngle();
    void CalculatePoints();
public:

    Rectangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
    Rectangle(const float x, const float y, const float w, const float h, const float a);

    virtual ~Rectangle();

    const Vector2D GetUpNormal() const;
    const Vector2D GetLeftNormal() const;

    const float GetAngle() const;

    void Rotate(const float rotation);
    void SetAngle(const float a);
    void Move(const float x, const float y);
    void Resize(const float w, const float h);

    const std::vector<Point> GetPoints() const;
};
#endif // _RECTANGLE_H
