#ifndef _TILE_H_
#define _TILE_H_

#include "Utility.hpp"
#include <map>

class Tile {
public:
    Tile(const std::string& filename, SDL_Renderer* ren);
    ~Tile();
    const int GetX() const;
    const int GetY() const;
    const bool IsPassable(int index) const;

    void Render(SDL_Renderer* ren);
private:
    int offset;
    SDL_Renderer* mRenderer;
    SDL_Texture* mtexture;
    int mXtile, mYtile;
    std::map<int, bool> passable;
};

#endif
