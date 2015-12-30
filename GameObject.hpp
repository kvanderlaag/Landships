#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <cstdint>

class GameObject {
public:
    GameObject(int w, int h, float x, float y, float a);
    virtual ~GameObject();

    virtual void Move() {};
    virtual void Rotate() {};
    virtual void Update(uint32_t ticks) = 0;

    const float GetX() const;
    const float GetY() const;
    const float GetAngle() const;

    virtual void SetX(float newx);
    virtual void SetY(float newy);
    virtual void SetAngle(float a);

    const bool IsDead() const;
    void Die();


protected:
    float x, y;
    float angle;
    int width, height;
    bool dead;
};

#endif // _GAMEOBJECT_H
