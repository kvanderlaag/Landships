#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    virtual void Move();
    virtual void Rotate();


private:
    float x, y;
    float angle;
    float width, height;
};

#endif // _GAMEOBJECT_H
