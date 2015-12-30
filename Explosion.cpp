#include "Explosion.hpp"

Explosion::Explosion(float x, float y, SDL_Renderer* ren) :
    RenderableObject("Explosion.png", 16, 16, x, y, 0, ren),
    mTicks(0),
    mFrame(0)
{

}

void Explosion::Update(uint32_t ticks) {
    mTicks += ticks;
    if (mTicks / ticksPerFrame > 0) {
        mFrame++;
        mTicks = 0;
    }
    if (mFrame > framesToLive)
        dead = true;
}

void Explosion::Render() {

    SDL_Rect srcRect, dstRect;

    srcRect.x = mFrame * 16;
    srcRect.y = 0;
    srcRect.w = width;
    srcRect.h = height;

    dstRect.x = x - 8;
    dstRect.y = y - 8;
    dstRect.w = width;
    dstRect.h = height;

    SDL_RenderCopy(mRenderer, mTexture, &srcRect, &dstRect);
}

