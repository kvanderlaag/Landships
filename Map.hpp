#ifndef MAP_H
#define MAP_H

#include "Tile.hpp"
#include "Collider.hpp"
#include "RenderableObject.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <vector>

class Map : public RenderableObject
{
    public:
        Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren);
        virtual ~Map();
        void Render();
        std::vector<Collider>& GetColliders();
    protected:
    private:
        int tiles[40][30];
        SDL_Renderer* mRenderer;
        SDL_Texture* mTexture;
        std::vector<Collider> mvColliders;
};

#endif // MAP_H
