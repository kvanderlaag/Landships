#include "DestructibleBlock.hpp"

DestructibleBlock::DestructibleBlock(const float x, const float y, SDL_Renderer* ren) :
    RenderableObject("DestructibleBlock.png", 16, 16, x, y, 0, ren),
    mHealth(3),
    mCollider(Collider(16, 16, x, y, 0, this)),
    mContents(-1)
{
    std::uniform_int_distribution<int> powerupDist(0, 6);

    int contents = powerupDist(generator);
    if (contents == 1) {
        contents = Powerup::PowerupType::powerupBullets;
    } else if (contents == 2) {
        contents = Powerup::PowerupType::powerupBounce;
    } else if (contents == 3) {
        contents = Powerup::PowerupType::powerupSpeed;
    }
}
