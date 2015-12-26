#include "RenderableObject.hpp"

int RenderableObject::next = 0;

RenderableObject::RenderableObject(const std::string& filename, int width, int height, float x, float y, float a, SDL_Renderer* ren) :
    GameObject(width, height, x, y, a),
    mTexture(Utility::LoadTexture(ren, filename)),
    mRenderer(ren)
{
}

RenderableObject::~RenderableObject() {
    SDL_DestroyTexture(mTexture);
}

SDL_Texture* RenderableObject::GetTexture() {
    return mTexture;
}
