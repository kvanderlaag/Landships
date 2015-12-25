#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

#include "GameObject.hpp"

#include <SDL2/SDL.h>

class RenderableObject : public GameObject {
protected:
    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;
public:
    RenderableObject(float width, float height, float x, float y, float a, SDL_Renderer* ren);
    virtual ~RenderableObject();

    virtual void Render() {};
};
#endif // _RENDERABLEOBJECT_H
