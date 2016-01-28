#include "RenderableObject.hpp"

int RenderableObject::next = 0;

RenderableObject::RenderableObject(const std::string& filename, int width, int height, float x, float y, float a, SDL_Renderer* ren) :
    GameObject(width, height, x, y, a),
    mTexture(Utility::LoadTexture(ren, filename)),
    mRenderer(ren)
{
}

RenderableObject::RenderableObject(const RenderableObject& other) :
    GameObject(other.GetWidth(), other.GetHeight(), other.GetX(), other.GetY(), other.GetAngle() ),
    mTexture(other.GetTexture()),
    mRenderer(ren)
{

}

RenderableObject::~RenderableObject() {
    SDL_DestroyTexture(mTexture);
}

RenderableObject& RenderableObject::operator=(const RenderableObject& other) {
    width = other.GetWidth();
    height = other.GetHeight();
    x = other.GetX();
    y = other.GetY();
    angle = other.GetAngle();
    mTexture = other.GetTexture();
    mRenderer = ren;
    return *this;
}

SDL_Texture* RenderableObject::GetTexture() const {
    return mTexture;
}

void RenderableObject::Render() {
    SDL_Rect destRect;

    destRect.x = x - (width / 2);
    destRect.y = y - (height / 2);
    destRect.h = height;
    destRect.w = width;

    SDL_RenderCopy(mRenderer, mTexture, NULL, &destRect);
}
