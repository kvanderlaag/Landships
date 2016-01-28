#include "GameObject.hpp"

GameObject::GameObject(int w, int h, float x, float y, float a) :
    x(x),
    y(y),
    angle(a),
    width(w),
    height(h),
    dead(false)
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

const int GameObject::GetWidth() const {
    return width;
}

const int GameObject::GetHeight() const {
    return height;
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

const bool GameObject::IsDead() const {
     return dead;
}

void GameObject::Die() {
    dead = true;
}
