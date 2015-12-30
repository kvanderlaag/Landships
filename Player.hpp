#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.hpp"
#include "RenderableObject.hpp"
#include "Utility.hpp"
#include "Map.hpp"
#include "Collider.hpp"
#include "Bullet.hpp"

#include <memory>

#include <cmath>

class Bullet;

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

private:
    const int PLAYER_SIZE = 16;
    const int MOVE_SPEED = 50;
    const int ROTATE_SPEED = 90;
    const float maxX = 320;
    const float maxY = 240;
    float xvel, yvel;
    float mForwardVel;
    float mRotationVel;

    float mTurretAngle;
    float mTurretRotationVel;

    Collider mCollider;
    int mMaxBullets, mBullets, mMaxBounce;

};

#endif // _PLAYER_H_
