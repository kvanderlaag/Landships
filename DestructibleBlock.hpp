#ifndef _DESTRUCTIBLEBLOCK_H
#define _DESTRUCTIBLEBLOCK_H

#include <random>
#include <SDL2/SDL.h>
#include <iostream>

#include "Collider.hpp"
#include "Powerup.hpp"
#include "RenderableObject.hpp"
#include "Defines.hpp"
#include "Map.hpp"

extern std::default_random_engine generator;
extern int gRndTiles;

class Map;

class DestructibleBlock : public RenderableObject {
public:
    DestructibleBlock(const float x, const float y, SDL_Renderer* ren, Map* parent, const bool bounce);
    virtual ~DestructibleBlock();

    int Damage();

    void Render();

    void Update(uint32_t ticks);

    const int Health() const;


    const int GetContents() const;
    const Collider& GetCollider() const;

private:
    Map& mParent;
    int mHealth;
    Collider mCollider;
    int mContents;

    int mDieFrame;
    int mDieAnimationFrames;
    int mAnimTicks;
    bool mBounce;
    const int ticksPerDieFrame = 100;
};
#endif // _DESTRUCTIBLEBLOCK_H
