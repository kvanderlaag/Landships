#ifndef MAP_H
#define MAP_H

#include "Tile.hpp"
#include "Collider.hpp"
#include "RenderableObject.hpp"
#include "DestructibleBlock.hpp"
#include "Vector2D.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <random>

#define EMPTY   0x00
#define P1START 0x01
#define P2START 0x02
#define P3START 0x03
#define P4START 0x04
#define DSTRBLK 0x05
#define DSTRBNC 0x06
#define BLOCK   0x07

class DestructibleBlock;

extern std::default_random_engine generator;

class Map : public RenderableObject
{
    public:
        Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren);
        virtual ~Map();
        void Render();
        std::vector<Collider>& GetColliders();
        std::vector<DestructibleBlock*> GetDestructibleBlocks();
        const Vector2D& GetStartPos(const int pn) const;
        void Update(uint32_t ticks) {};
        const unsigned int GetTileAt(const int row, const int col) const;
        const bool LoadSuccess() const;
    protected:
    private:
        std::vector<Collider> CombineColliders(unsigned char (&tiles)[30][40]) const;
        Vector2D StartPos[4];
        unsigned char tiles[30][40];
        std::vector<Collider> mvColliders;
        std::vector<DestructibleBlock*> mvDestructibleBlocks;
        bool mLoadSuccess;
};

#endif // MAP_H
