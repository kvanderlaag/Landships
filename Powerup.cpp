#include "Powerup.hpp"

Powerup::Powerup(const float x, const float y, SDL_Renderer* ren, int type) :
    RenderableObject("Powerups.png", 16, 16, x, y, 0, ren),
    mType(powerupBullets),
    mCollider(Collider(16, 16, x, y, 0, this)),
    mFrame(0),
    mTicksElapsed(0)
{
    int texWidth, texHeight;
    SDL_QueryTexture(mTexture, NULL, NULL, &texWidth, &texHeight);

    mAnimationFrames = texWidth / width;

    switch (type) {
    case 1:
        mType = powerupBullets;
        break;
    case 2:
        mType = powerupBounce;
        break;
    case 3:
        mType = powerupSpeed;
        break;
    }
}

Powerup::~Powerup() {

}

void Powerup::Render() {
    SDL_Rect srcRect, dstRect;

    srcRect.x = mFrame * width;
    srcRect.y = (mType - 1) * height;
    srcRect.w = width;
    srcRect.h = height;

    dstRect.x = x - (width / 2);
    dstRect.y = y - (height / 2);
    dstRect.w = width;
    dstRect.h = height;

    SDL_RenderCopy(mRenderer, mTexture, &srcRect, &dstRect);
}

void Powerup::Update(uint32_t ticks) {
    mTicksElapsed += ticks;
    if (mTicksElapsed >= ticksPerFrame) {
        mFrame++;
        mTicksElapsed = 0;
    }
    if (mFrame > mAnimationFrames) {
        mFrame = 0;
    }
}

void Powerup::Apply(Player& p) {
    switch (mType) {
        case powerupBullets:
            Utility::PlaySound(sfxPowerupBullet[gRndTiles]);
            if (p.GetMaxBullets() < maxBullets) {
                p.IncreaseMaxBullets();
            }
            break;
        case powerupBounce:
            Utility::PlaySound(sfxPowerupBounce[gRndTiles]);
            if (p.GetMaxBounce() < maxBounces) {
                p.IncreaseMaxBounce();
            }
            break;
        case powerupSpeed:
            Utility::PlaySound(sfxPowerupSpeed[gRndTiles]);
            if (p.GetMaxSpeed() < maxSpeed) {
                p.IncreaseMaxSpeed();
            }
            break;
    }
    Die();
}

const Collider& Powerup::GetCollider() const {
    return mCollider;
}
