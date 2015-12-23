#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Utility.hpp"

#include <cmath>

class Player {
public:
    Player(const std::string& filename, SDL_Renderer* ren);
    ~Player();
    const int GetX() const;
    const int GetY() const;

    void SetX(int newx);
    void SetY(int newy);

    const float GetXVel() const;
    const float GetYVel() const;

    const float GetRotationVel() const;
    void SetRotationVel(float newvel);

    const float GetTurretRotationVel() const;
    void SetTurretRotationVel(float newvel);

    const float GetAngle() const;

    void SetXVel(float newvel);
    void SetYVel(float newvel);

    void SetForwardVel(float newvel);
    const float GetForwardVel() const;

    SDL_Texture* GetTexture();

    void Render();
    void Move(int maxX, int maxY);

private:
    float mAngle;
    float mX, mY, mXvel, mYvel;
    float mForwardVel;
    float mRotationVel;

    float mTurretAngle;
    float mTurretRotationVel;

    int mWidth, mHeight;
    SDL_Texture* mtexture;
    SDL_Renderer* mRenderer;

};

#endif // _PLAYER_H_
