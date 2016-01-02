#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.hpp"
#include "RenderableObject.hpp"
#include "Utility.hpp"
#include "Map.hpp"
#include "Collider.hpp"
#include <SDL2/SDL_mixer.h>
#include "Bullet.hpp"

#include <memory>

#include <cmath>

extern Mix_Chunk* sfxFire;

class Bullet;

class Player : public RenderableObject {
public:
    Player(const std::string& filename, int id, SDL_Renderer* ren);
    ~Player();

    void SetX(float newx);
    void SetY(float newy);

    const float GetXVel() const;
    const float GetYVel() const;

    const float GetRotationVel() const;
    void SetRotationVel(float newvel);

    const float GetTurretRotationVel() const;
    void SetTurretRotationVel(float newvel);

    const float GetTurretAngle() const;
    void SetTurretAngle(const float newangle);

    const float GetAngle() const;

    const Vector2D GetVelocity() const;

    void SetXVel(float newvel);
    void SetYVel(float newvel);

    void SetForwardVel(float newvel);
    const float GetForwardVel() const;

    void Render();

    Bullet* Fire();

    const Collider& GetCollider() const;
    void CheckCollision(const Collider& other, uint32_t ticks);

    void Update(uint32_t ticks);

    const std::vector<Bullet>& GetBullets();

    void DestroyBullet();

    const int GetMaxBullets() const;
    const int GetBullets() const;
    const int GetMaxBounce() const;

    const int GetID() const;
    const int GetScore() const;
    void AddScore(const int mod);

    const bool FireHeld() const;
    void FireIsHeld(const bool val);

    const bool JoyMove() const;
    void SetJoyMove(const bool val);

    const bool JoyRotate() const;
    void SetJoyRotate(const bool val);

    const bool JoyTurret() const;
    void SetJoyTurret(const bool val);

    const bool IsInvincible() const;
    void Invincible();

private:

    int mID;
    int score;
    const int PLAYER_SIZE = 16;
    const int MOVE_SPEED = 50;
    const int ROTATE_SPEED = 90;
    const int INVINCIBLE_TICKS = 2000;
    const int TICKS_PER_FLASH = 100;
    const float maxX = 320;
    const float maxY = 240;
    Vector2D mVelocity;
    float mForwardVel;
    float mRotationVel;

    float mTurretAngle;
    float mTurretRotationVel;

    Collider mCollider;
    int mMaxBullets, mBullets, mMaxBounce;

    bool mFireHeld;
    bool mJoyMove, mJoyRotate, mJoyTurret;
    int32_t mInvincible, mFlashTicks;
    bool mInvisible;
};

#endif // _PLAYER_H_
