#ifndef _DESTRUCTIBLEBLOCK_H
#define _DESTRUCTIBLEBLOCK_H

#include <random>

#include "Collider.hpp"
#include "Powerup.hpp"
#include "RenderableObject.hpp"
#include "Defines.hpp"

extern std::default_random_engine generator;

class DestructibleBlock : public RenderableObject {
public:
    DestructibleBlock(const float x, const float y, SDL_Renderer* ren);
    virtual ~DestructibleBlock() {};

    int Damage();

    const int GetContents() const;

private:
    int mHealth;
    Collider mCollider;
    int mContents;
};
#endif // _DESTRUCTIBLEBLOCK_H
