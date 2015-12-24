#ifndef MAP_H
#define MAP_H

#include "Tile.hpp"
#include "Collider.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <vector>

class Map
{
    public:
        Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren);
        virtual ~Map();
        void Render();
        const std::vector<Collider*> GetColliders() const;
    protected:
    private:
        std::vector<Collider*> mColliders;
        int tiles[40][30];
        SDL_Renderer* mRenderer;
        SDL_Texture* mTexture;
};

#endif // MAP_H
