#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.hpp"
#include "RenderableObject.hpp"
#include "Utility.hpp"
#include "Map.hpp"
#include "Collider.hpp"

#include <memory>

#include <cmath>

class Player : public RenderableObject {
public:
    Player(const std::string& filename, SDL_Renderer* ren);
    ~Player();

    void SetX(float newx);
    void SetY(float newy);

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

    const Collider& GetCollider() const;
    void CheckCollision(const Collider& other, uint32_t ticks);

    void Update(uint32_t ticks);

private:
    const int PLAYER_SIZE = 16;
    const float maxX = 320;
    const float maxY = 240;
    float mAngle;
    float mX, mY, mXvel, mYvel;
    float mForwardVel;
    float mRotationVel;

    float mTurretAngle;
    float mTurretRotationVel;

    int mWidth, mHeight;
    SDL_Texture* mtexture;
    SDL_Renderer* mRenderer;

    Collider mCollider;

};

#endif // _PLAYER_H_
