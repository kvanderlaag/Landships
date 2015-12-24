#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Utility.hpp"
#include "Map.hpp"
#include "Collider.hpp"

#include <memory>

#include <cmath>

class Player {
public:
    Player(const std::string& filename, SDL_Renderer* ren, const Map& m);
    ~Player();
    const float GetX() const;
    const float GetY() const;

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

    void CheckCollision(const Collider& other);

    void Move(int maxX, int maxY, uint32_t ticks);

private:
    float mAngle;
    float mX, mY, mXvel, mYvel;
    float mForwardVel;
    float mRotationVel;

    float mTurretAngle;
    float mTurretRotationVel;

    std::unique_ptr<Collider> mCollider;

    int mWidth, mHeight;
    SDL_Texture* mtexture;
    SDL_Renderer* mRenderer;

    const Map* parentMap;

};

#endif // _PLAYER_H_
