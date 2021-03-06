#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.hpp"
#include "RenderableObject.hpp"
#include "Utility.hpp"
#include "Map.hpp"
#include "Collider.hpp"
#include <SDL_mixer.h>
#include "Bullet.hpp"

#include <memory>

#include <cmath>

extern Mix_Chunk* sfxFire;

class Bullet;

class Player : public RenderableObject {
public:
    Player(const std::string& filename, int id, int lives, SDL_Renderer* ren);
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
    const int GetMaxSpeed() const;

    void IncreaseMaxBullets();
    void IncreaseMaxBounce();
    void IncreaseMaxSpeed();

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

    const bool FireReady() const;
    const bool FireReleased() const;
    void FireIsReleased(const bool value);

    void IsMoving(const bool value);
    const bool Moving() const;

    const int GetLives() const;
    void LoseLife();



private:

    int mID;
    int score;
    int lives;
    const int PLAYER_SIZE = 16;
    const int MOVE_SPEED = 50;

    const float MOVESCALE_BASE = 0.7;
    const float MOVESCALE_1 = 0.9;
    const float MOVESCALE_2 = 1.1;

    const int ROTATE_SPEED = 90;
    const int TURRET_ROTATE_SPEED = 110;
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

    bool mFireHeld, mFireReleased;
    bool mJoyMove, mJoyRotate, mJoyTurret;
    int32_t mInvincible, mFlashTicks;
    bool mInvisible;

    int mMaxSpeed;

    bool mFireReady;
    int32_t mFireTicksElapsed;
    const int ticksBetweenShots = 125;

    const int ticksPerAnimationFrame = 100;
    int mAnimationTicksElapsed;
    int mAnimationFrames;
    int mFrame;
    bool mMoving;


};

#endif // _PLAYER_H_
