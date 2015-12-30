#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "RenderableObject.hpp"

class Explosion : public RenderableObject {
private:
    const uint32_t framesToLive = 5;
    const uint32_t ticksPerFrame = 50;
    uint32_t mTicks;
    unsigned char mFrame;

public:
    Explosion(float x, float y, SDL_Renderer* ren);
    virtual ~Explosion() {};

    void Update(uint32_t ticks);
    void Render();

};

#endif // _EXPLOSION_H
