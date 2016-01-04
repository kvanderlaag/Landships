#include "Rectangle.hpp"

Rectangle::Rectangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4) :
    p1(Point(p1.x, p1.y)),
    p2(Point(p2.x, p2.y)),
    p3(Point(p3.x, p3.y)),
    p4(Point(p4.x, p4.y)),
    center(Point(p1.x + ((p3.x - p1.x) / 2), p1.y + ((p3.y - p1.y) / 2))),
    angle(std::acos((p1.y - p4.y) / (p1.x - p4.x))),
    width(  sqrt( (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) ) ),
    height( sqrt( (p3.x - p1.x) * (p3.x - p1.x) + (p3.y - p1.y) * (p3.y - p1.y) ) )
{
    CalculatePoints();
}

Rectangle::Rectangle(const float w, const float h, const float x, const float y, const float a) :
    center(Point(x, y)),
    angle(a),
    width(w),
    height(h)
{
    CalculatePoints();
}

Rectangle::~Rectangle() {

}

void Rectangle::CalculateCenter() {
    center.x = p1.x + ((p3.x - p1.x) / 2);
    center.y = p1.y + ((p3.y - p1.y) / 2);
}

void Rectangle::CalculateAngle() {
    // slope p4 -> p1
    angle = std::acos((p1.y - p4.y) / (p1.x - p4.x));
}

void Rectangle::CalculatePoints() {
    p1.x = center.x - (width/2);
    p1.y = center.y - (height/2);
    p2.x = center.x + (width/2);
    p2.y = center.y - (height/2);
    p3.x = center.x + (width/2);
    p3.y = center.y + (height/2);
    p4.x = center.x - (width/2);
    p4.y = center.y + (height/2);

    if (angle != 0) {
        float a = angle * M_PI / 180;
        float fSin, fCos;
        fSin = std::sin(a);
        fCos = std::cos(a);

        // Translate
        float tempX = p1.x - center.x;
        float tempY = p1.y - center.y;

        // now apply rotation
        float rotatedX = tempX*fCos - tempY*fSin;
        float rotatedY = tempX*fSin + tempY*fCos;

        // translate back
        p1.x = rotatedX + center.x;
        p1.y = rotatedY + center.y;

        // Translate
        tempX = p2.x - center.x;
        tempY = p2.y - center.y;

        // now apply rotation
        rotatedX = tempX*fCos - tempY*fSin;
        rotatedY = tempX*fSin + tempY*fCos;

        // translate back
        p2.x = rotatedX + center.x;
        p2.y = rotatedY + center.y;


        // Translate
        tempX = p3.x - center.x;
        tempY = p3.y - center.y;

        // now apply rotation
        rotatedX = tempX*fCos - tempY*fSin;
        rotatedY = tempX*fSin + tempY*fCos;

        // translate back
        p3.x = rotatedX + center.x;
        p3.y = rotatedY + center.y;


        // Translate
        tempX = p4.x - center.x;
        tempY = p4.y - center.y;

        // now apply rotation
        rotatedX = tempX*fCos - tempY*fSin;
        rotatedY = tempX*fSin + tempY*fCos;

        // translate back
        p4.x = rotatedX + center.x;
        p4.y = rotatedY + center.y;
    }
}

const Vector2D Rectangle::GetUpNormal() const {
    float a = angle * M_PI / 180;
    float vx = 0 * std::cos(a) - (-1) * std::sin(a);
    float vy = 0 * std::sin(a) + (-1) * std::cos(a);
    return Vector2D(vx, vy);
}

const Vector2D Rectangle::GetLeftNormal() const {
    float a = angle * M_PI / 180;
    float vx = (-1) * std::cos(a) - 0 * std::sin(a);
    float vy = (-1) * std::sin(a) + 0 * std::cos(a);
    return Vector2D(vx, vy);
}

const Point Rectangle::GetCenter() const {
    return Point(center.x, center.y);
}

const float Rectangle::GetAngle() const {
    return angle;
}

void Rectangle::Rotate(const float rotation) {
    angle += rotation;
    if (angle < 0) {
        while (angle < 0)
            angle += 360;
    } else if (angle > 360) {
        while (angle > 360)
            angle -= 360;
    }


    float a = rotation * M_PI / 180;
    float fSin, fCos;
    fSin = std::sin(a);
    fCos = std::cos(a);

    // Translate
    float tempX = p1.x - center.x;
    float tempY = p1.y - center.y;

    // now apply rotation
    float rotatedX = tempX*fCos - tempY*fSin;
    float rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p1.x = rotatedX + center.x;
    p1.y = rotatedY + center.y;

    // Translate
    tempX = p2.x - center.x;
    tempY = p2.y - center.y;

    // now apply rotation
    rotatedX = tempX*fCos - tempY*fSin;
    rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p2.x = rotatedX + center.x;
    p2.y = rotatedY + center.y;


    // Translate
    tempX = p3.x - center.x;
    tempY = p3.y - center.y;

    // now apply rotation
    rotatedX = tempX*fCos - tempY*fSin;
    rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p3.x = rotatedX + center.x;
    p3.y = rotatedY + center.y;


    // Translate
    tempX = p4.x - center.x;
    tempY = p4.y - center.y;

    // now apply rotation
    rotatedX = tempX*fCos - tempY*fSin;
    rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p4.x = rotatedX + center.x;
    p4.y = rotatedY + center.y;
    //CalculatePoints();
}

void Rectangle::SetAngle(const float a) {
    float rotation = (a - angle) * M_PI / 180;
    angle = a;
    if (angle < 0) {
        while (angle < 0)
            angle += 360;
    } if (angle > 360) {
        while (angle > 360)
            angle -= 360;
    }

    float fSin, fCos;
    fSin = std::sin(rotation);
    fCos = std::cos(rotation);

    // Translate
    float tempX = p1.x - center.x;
    float tempY = p1.y - center.y;

    // now apply rotation
    float rotatedX = tempX*fCos - tempY*fSin;
    float rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p1.x = rotatedX + center.x;
    p1.y = rotatedY + center.y;

    // Translate
    tempX = p2.x - center.x;
    tempY = p2.y - center.y;

    // now apply rotation
    rotatedX = tempX*fCos - tempY*fSin;
    rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p2.x = rotatedX + center.x;
    p2.y = rotatedY + center.y;


    // Translate
    tempX = p3.x - center.x;
    tempY = p3.y - center.y;

    // now apply rotation
    rotatedX = tempX*fCos - tempY*fSin;
    rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p3.x = rotatedX + center.x;
    p3.y = rotatedY + center.y;


    // Translate
    tempX = p4.x - center.x;
    tempY = p4.y - center.y;

    // now apply rotation
    rotatedX = tempX*fCos - tempY*fSin;
    rotatedY = tempX*fSin + tempY*fCos;

    // translate back
    p4.x = rotatedX + center.x;
    p4.y = rotatedY + center.y;
    //CalculatePoints();
}

void Rectangle::Move(const float newx, const float newy) {
    float xTranslate, yTranslate;

    xTranslate = (newx - center.x);
    yTranslate = (newy - center.y);

    p1.x += xTranslate;
    p1.y += yTranslate;

    p2.x += xTranslate;
    p2.y += yTranslate;

    p3.x += xTranslate;
    p3.y += yTranslate;

    p4.x += xTranslate;
    p4.y += yTranslate;

    center.x = newx;
    center.y = newy;


    //CalculatePoints();
}

void Rectangle::Resize(const float w, const float h) {
    width = w;
    height = h;
    CalculatePoints();
}

const std::vector<Point> Rectangle::GetPoints() const {
    return std::vector<Point> { Point(p1.x, p1.y), Point(p2.x, p2.y), Point(p3.x, p3.y), Point(p4.x, p4.y) };
}
