#include "GameObject.hpp"

GameObject::GameObject(float w, float h, float x, float y, float a) :
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

void GameObject::SetX(float newx) {
    x = newx;
}

void GameObject::SetY(float newy) {
    y = newy;
}
