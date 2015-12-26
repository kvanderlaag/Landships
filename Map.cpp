#include "Map.hpp"

//#define _COLLISION_DEBUG

Map::Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren) :
    RenderableObject(texturefile, 320, 240, 0, 0, 0, ren)

{
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 40; ++j) {
            tiles[j][i] = -1;
        }
    }
    std::ifstream infile(filename, std::ifstream::in);
    int row = 0;
    int col = 0;
    while (infile.good()) {
        char tile = infile.get();
        if (tile == '\n') {
            row++;
            col = 0;
        } else if  (tile == ' ') {
            tiles[col][row] = -1;
            col++;
        } else {
            mvColliders.push_back(Collider(8, 8, col * 8 + 4, row * 8 + 4, 0, this));
            if (tile == '0') {
                tiles[col][row] = 0;
                col++;
            } else if (tile == '1') {
                tiles[col][row] = 1;
                col++;
            } else if (tile == '2') {
                tiles[col][row] = 2;
                col++;
            } else if (tile == '3') {
                tiles[col][row] = 3;
                col++;
            } else if (tile == '4') {
                tiles[col][row] = 4;
                col++;
            } else if (tile == '5') {
                tiles[col][row] = 5;
                col++;
            } else if (tile == '6') {
                tiles[col][row] = 6;
                col++;
            } else if (tile == '7') {
                tiles[col][row] = 7;
                col++;
            } else if (tile == '8') {
                tiles[col][row] = 8;
                col++;
            }
        }
    }
}

Map::~Map()
{
    //dtor
}

void Map::Render() {
    for (int row = 0; row < 30; row++) {
        for (int col = 0; col < 40; col++) {

                if (tiles[col][row] == -1) {
                    continue;
                }

                int offset = tiles[col][row] * 8;

                SDL_Rect srcRect, dstRect;
                srcRect.x = offset;
                srcRect.y = 0;
                srcRect.w = 8;
                srcRect.h = 8;

                dstRect.x = col * 8;
                dstRect.y = row * 8;
                dstRect.w = 8;
                dstRect.h = 8;

                SDL_RenderCopy(mRenderer, mTexture, &srcRect, &dstRect);

                /* Render all colliders
                for (Collider& c : mvColliders) {
                    std::vector<Point> colliderPoints = c.GetPoints();
                    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    SDL_RenderDrawLine(mRenderer, colliderPoints[0].x, colliderPoints[0].y, colliderPoints[1].x, colliderPoints[1].y);
                    SDL_RenderDrawLine(mRenderer, colliderPoints[1].x, colliderPoints[1].y, colliderPoints[2].x, colliderPoints[2].y);
                    SDL_RenderDrawLine(mRenderer, colliderPoints[2].x, colliderPoints[2].y, colliderPoints[3].x, colliderPoints[3].y);
                    SDL_RenderDrawLine(mRenderer, colliderPoints[3].x, colliderPoints[3].y, colliderPoints[0].x, colliderPoints[0].y);
                }
                /* End of collider rendering */

        }
    }
}

std::vector<Collider>& Map::GetColliders() {
    return mvColliders;
}
