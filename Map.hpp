#ifndef MAP_H
#define MAP_H

#include "Tile.hpp"
#include <SDL2/SDL.h>
#include <fstream>

class Map
{
    public:
        Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren);
        virtual ~Map();
        void Render();
    protected:
    private:
        int tiles[40][30];
        SDL_Renderer* mRenderer;
        SDL_Texture* mTexture;
};

#endif // MAP_H
