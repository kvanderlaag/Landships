#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "SDL2/SDL.h"
#include "Collision.hpp"
#include "Utility.hpp"
#include <cmath>
#include <cfloat>
#include <vector>

class Collider {
private:
    int mWidth, mHeight;
    float mX, mY;
    float mAngle;
    SDL_Renderer* mRenderer;
public:
    Collider(int width, int height, float x, float y, float a, SDL_Renderer* ren);
    virtual ~Collider();

    const int GetWidth() const;
    const int GetHeight() const;
    const float GetAngle() const;
    const float GetX() const;
    const float GetY() const;

    const std::vector<std::vector<float>> GetPoints() const;

    void Render();

    void SetWidth(int newwidth);
    void SetHeight(int newheight);
    void SetAngle(float newangle);
    void SetX(float newx);
    void SetY(float newy);

    const Collision IsColliding(const Collider& other, std::vector<float> velocity) const;
};


#endif // _COLLIDER_H
