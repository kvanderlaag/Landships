#ifndef MAP_H
#define MAP_H

#include "Tile.hpp"
#include "Collider.hpp"
#include "RenderableObject.hpp"
#include "Vector2D.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <vector>
#include <cstring>

#define EMPTY   0x00
#define P1START 0x01
#define P2START 0x02
#define P3START 0x03
#define P4START 0x04
#define BLOCK   0x05

class Map : public RenderableObject
{
    public:
        Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren);
        virtual ~Map();
        void Render();
        std::vector<Collider>& GetColliders();
        const Vector2D& GetStartPos(const int pn) const;
        void Update(uint32_t ticks) {};
    protected:
    private:
        Vector2D StartPos[4];
        unsigned char tiles[30][40];
        std::vector<Collider> mvColliders;
};

#endif // MAP_H
