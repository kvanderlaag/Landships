#ifndef MAP_H
#define MAP_H

#include "Collider.hpp"
#include "RenderableObject.hpp"
#include "DestructibleBlock.hpp"
#include "Vector2D.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <random>

class DestructibleBlock;

extern std::default_random_engine generator;
extern std::string basePath;

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

        static const char EMPTY   = 0x00;
        static const char P1START = 0x01;
        static const char P2START = 0x02;
        static const char P3START = 0x03;
        static const char P4START = 0x04;
        static const char DSTRBLK = 0x05;
        static const char DSTRBNC = 0x06;
        static const char BLOCK   = 0x07;
        std::vector<Collider> CombineColliders(unsigned char (&tiles)[30][40]) const;
        Vector2D StartPos[4];
        unsigned char tiles[30][40];
        std::vector<Collider> mvColliders;
        std::vector<DestructibleBlock*> mvDestructibleBlocks;
        bool mLoadSuccess;
};

#endif // MAP_H
