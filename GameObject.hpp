#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <cstdint>

class GameObject {
public:
    GameObject(int w, int h, float x, float y, float a);
    virtual ~GameObject();

    virtual void Move() {};
    virtual void Rotate(float rotation) {};
    virtual void Update(uint32_t ticks) = 0;

    const int GetWidth() const;
    const int GetHeight() const;

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
    bool mPassable;
    bool mStopsShots;
};

#endif // _GAMEOBJECT_H
