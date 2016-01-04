#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <random>
#include "RenderableObject.hpp"
#include "Collider.hpp"

extern std::default_random_engine generator;

class Container : public RenderableObject {
public:
    Container(const float x, const float y, SDL_Renderer* ren);
    virtual ~Container() {};

    void Update(uint32_t ticks) {};
    void Render();

    enum Contents {
        powerupBullets = 1,
        powerupBounce = 2,
        powerupSpeed = 3
    };

    const int GetContents() const;
    const Collider& GetCollider() const;
private:
    Contents mContents;
    Collider mCollider;
};

#endif // _CONTAINER_H
