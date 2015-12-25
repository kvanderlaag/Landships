#include "RenderableObject.hpp"

RenderableObject::RenderableObject(float width, float height, float x, float y, float a, SDL_Renderer* ren) :
    GameObject(width, height, x, y, a),
    mRenderer(ren)
{

}

RenderableObject::~RenderableObject() {

}
