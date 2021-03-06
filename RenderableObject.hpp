#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

#include "GameObject.hpp"
#include "Utility.hpp"
#include <string>

#include <SDL.h>

extern SDL_Renderer* ren;

class RenderableObject : public GameObject {
protected:
    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;
public:
    static int next;

    RenderableObject(const std::string& filename, int width, int height, float x, float y, float a, SDL_Renderer* ren);
    RenderableObject(const RenderableObject& other);

    RenderableObject& operator=(const RenderableObject& other);

    virtual ~RenderableObject();

    virtual SDL_Texture* GetTexture() const;

    virtual void Render();
};
#endif // _RENDERABLEOBJECT_H
