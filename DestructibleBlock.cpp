#include "DestructibleBlock.hpp"

DestructibleBlock::DestructibleBlock(const float x, const float y, SDL_Renderer* ren, Map* parent, const bool bounce) :
    RenderableObject("DestructibleBlock.png", 16, 16, x, y, 0, ren),
    mParent(*parent),
    mHealth(3),
    mCollider(Collider(16, 16, x, y, 0, this)),
    mContents(-1),
    mDieFrame(0),
    mDieAnimationFrames(3),
    mAnimTicks(0),
    mBounce(bounce)
{
    std::uniform_int_distribution<int> powerupDist(0, 6);

    int contents = powerupDist(generator);
    if (contents == 1) {
        mContents = Powerup::PowerupType::powerupBullets;
    } else if (contents == 2) {
        mContents = Powerup::PowerupType::powerupBounce;
    } else if (contents == 3) {
        mContents = Powerup::PowerupType::powerupSpeed;
    }
}

DestructibleBlock::~DestructibleBlock() {
    //std::cout << "Destroying destructible block." << std::endl;
}

int DestructibleBlock::Damage() {
    if (mHealth-- == 0)
        return 1;
    else
        return 0;
}

void DestructibleBlock::Update(uint32_t ticks) {
    if (mHealth <= 0) {
        mAnimTicks += ticks;
        if (mAnimTicks >= ticksPerDieFrame) {
            mAnimTicks = 0;
            mDieFrame++;
            if (mDieFrame >= mDieAnimationFrames) {
                Die();
            }
        }
    }
}

void DestructibleBlock::Render() {
    SDL_Rect srcRect;
    srcRect.w = 16;
    srcRect.h = 16;

    //srcRect.y = 0;
    srcRect.y = 16 * gRndTiles;
    if (mBounce)
        srcRect.y += 3 * 16;

    if (mHealth == 3) {
        srcRect.x = 0;
    } else if (mHealth == 2) {
        srcRect.x = 16;
    } else if (mHealth == 1) {
        srcRect.x = 32;
    } else if (mHealth <= 0) {
        srcRect.x = 48 + (mDieFrame * 16);
    }

    SDL_Rect dstRect;

    dstRect.x = x - 8;
    dstRect.y = y - 8;
    dstRect.w = 16;
    dstRect.h = 16;

    SDL_RenderCopy(mRenderer, mTexture, &srcRect, &dstRect);

}

const Collider& DestructibleBlock::GetCollider() const {
    return mCollider;
}

const int DestructibleBlock::GetContents() const {
    return mContents;
}

const int DestructibleBlock::Health() const {
    return mHealth;
}

const bool DestructibleBlock::Bounce() const {
    return mBounce;
}
