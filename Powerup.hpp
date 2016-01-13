#ifndef _POWERUP_H
#define _POWERUP_H

#include "RenderableObject.hpp"
#include "Collider.hpp"
#include "Player.hpp"

extern Mix_Chunk* sfxPowerupBounce[MAX_TILESET + 1];
extern Mix_Chunk* sfxPowerupBullet[MAX_TILESET + 1];
extern Mix_Chunk* sfxPowerupSpeed[MAX_TILESET + 1];
extern int rndTiles;

class Powerup : public RenderableObject {
public:
    enum PowerupType {
        powerupBullets = 1,
        powerupBounce = 2,
        powerupSpeed = 3
    };

    Powerup(const float x, const float y, SDL_Renderer* ren, int type);
    ~Powerup();


    void Render();
    void Update(uint32_t ticks);

    void Apply(Player& p);

    const Collider& GetCollider() const;


private:
    PowerupType mType;
    Collider mCollider;
    int mAnimationFrames;
    int mFrame;
    int mTicksElapsed;
    const int ticksPerFrame = 100;

    const int maxBounces = 3;
    const int maxBullets = 5;
    const int maxSpeed = 3;


};
#endif // _POWERUP_H
