#include "Container.hpp"


Container::Container(const float x, const float y, SDL_Renderer* ren) :
    RenderableObject("Container.png", 16, 16, x, y, 0, ren),
    mContents(powerupBullets),
    mCollider(Collider(16, 16, x, y, 0, this))
{
    std::uniform_int_distribution<int> dist(powerupBullets, powerupSpeed);
    int rnd = dist(generator);

    switch (rnd) {
        case powerupBullets:
            mContents = powerupBullets;
            break;
        case powerupBounce:
            mContents = powerupBounce;
            break;
        case powerupSpeed:
            mContents = powerupSpeed;
            break;
    }
}

void Container::Render() {
    SDL_Rect dstRect;
    dstRect.x = x - (width/2);
    dstRect.y = y - (height/2);
    dstRect.w = width;
    dstRect.h = height;

    SDL_RenderCopy(mRenderer, mTexture, NULL, &dstRect);
}

const Collider& Container::GetCollider() const {
    return mCollider;
}

const int Container::GetContents() const {
    switch (mContents) {
    case powerupBullets:
        return 1;
    case powerupBounce:
        return 2;
    case powerupSpeed:
        return 3;
    }
    return 1;
}


