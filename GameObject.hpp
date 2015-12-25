#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <cstdint>

class GameObject {
public:
    GameObject(float w, float h, float x, float y, float a);
    virtual ~GameObject();

    virtual void Move() {};
    virtual void Rotate() {};
    virtual void Update(uint32_t ticks) {};

    const float GetX() const;
    const float GetY() const;

    virtual void SetX(float newx);
    virtual void SetY(float newy);




protected:
    float x, y;
    float angle;
    float width, height;
};

#endif // _GAMEOBJECT_H
