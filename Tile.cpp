#include "Tile.hpp"

Tile::Tile(const std::string& filename, SDL_Renderer* ren) :
    mtexture(Utility::LoadTexture(ren, filename)),
    mRenderer(ren)
{
    int w;
    SDL_QueryTexture(mtexture, NULL, NULL, &w, NULL);
}

Tile::~Tile() {

}

const bool Tile::IsPassable(int index) const {
    bool pass = false;
    if (passable.find(index) != passable.end()) {
        pass = passable.find(index)->second;
    }
    return pass;
}

void Tile::Render(SDL_Renderer* ren) {

}
