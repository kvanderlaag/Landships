#include "GameObject.hpp"

GameObject::GameObject(int w, int h, float x, float y, float a) :
    x(x),
    y(y),
    angle(a),
    width(w),
    height(h)
{

}

GameObject::~GameObject() {

}

const float GameObject::GetX() const {
    return x;
}

const float GameObject::GetY() const {
    return y;
}

const float GameObject::GetAngle() const {
    return angle;
}

void GameObject::SetX(float newx) {
    x = newx;
}

void GameObject::SetY(float newy) {
    y = newy;
}

void GameObject::SetAngle(float a) {
    angle = a;
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
}
