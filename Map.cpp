#include "Map.hpp"

//#define _COLLISION_DEBUG

Map::Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren) :
    RenderableObject(texturefile, 320, 240, 0, 0, 0, ren),
    StartPos({Vector2D(0, 0), Vector2D(0, 0), Vector2D(0, 0), Vector2D(0, 0)}),
    mLoadSuccess(true)
{
    std::string path = basePath + MAPS_PATH;
    path += filename;
    std::ifstream inFile(path, std::ios::binary);

    if (!inFile.good()) {
        mLoadSuccess = false;
        return;
    }

    {
        char c;
        char DE = 0xDE;
        char AD = 0xAD;
        char BE = 0xBE;
        char EF = 0xEF;

        inFile.get(c);
        //std::cout << c;
        if (c != DE) {
            mLoadSuccess = false;
        }

        inFile.get(c);
        //std::cout << c;
        if (c != AD) {
            mLoadSuccess = false;
        }

        inFile.get(c);
        //std::cout << c;
        if (c != BE) {
            mLoadSuccess = false;
        }

        inFile.get(c);
        //std::cout << c;
        if (c != EF) {
            mLoadSuccess = false;
        }

        //std::cout << std::endl;
        //std::cout << DE << AD << BE << EF << std::endl;

        if (mLoadSuccess == false) {
            return;
        }
    }


    mvColliders.push_back(Collider(320,      8,          160,      4,          0, this));
    mvColliders.push_back(Collider(320,      8,          160,      240 - 4,     0, this));
    mvColliders.push_back(Collider(8,           8 * 28,     4,      240 / 2,          0, this));
    mvColliders.push_back(Collider(8,           8 * 28,     8*40 - 4,   240 / 2,          0, this));

    std::uniform_int_distribution<int> rndTileDist(6, 17);

    for (int row = 0; row < 30; ++row) {
        for (int col = 0; col < 40; ++col) {
            char c;
            if (!inFile.good() ) {
                    mLoadSuccess = false;
                    return;
            }
            inFile.get(c);

            if (row == 0 || row == 29 || col == 0 || col == 39) {
              int tile = rndTileDist(generator);
              if (tile < 12) {
                tile = BLOCK;
              }
              tiles[row][col] = tile;
            } else if (c == EMPTY) {
                tiles[row][col] = c;
            } else if (c == P1START) {
                StartPos[0] = Vector2D(col * 8, row * 8);
                tiles[row][col] = EMPTY;
            } else if (c == P2START) {
                StartPos[1] = Vector2D(col * 8, row * 8);
                tiles[row][col] = EMPTY;
            } else if (c == P3START) {
                StartPos[2] = Vector2D(col * 8, row * 8);
                tiles[row][col] = EMPTY;
            } else if (c == P4START) {
                StartPos[3] = Vector2D(col * 8, row * 8);
                tiles[row][col] = EMPTY;
            } else if (c == DSTRBLK) {
                // Do destructible block stuff
                mvDestructibleBlocks.push_back(new DestructibleBlock(col * 8, row * 8, mRenderer, this, false));
                tiles[row][col] = EMPTY;
            } else if (c == DSTRBNC) {
                mvDestructibleBlocks.push_back(new DestructibleBlock(col * 8, row * 8, mRenderer, this, true));
                tiles[row][col] = EMPTY;
            } else {
                if (row != 0 && row != 29 && col != 0 && col != 39)
                    //mvColliders.push_back(Collider(8, 8, col * 8 + 4, row * 8 + 4, 0, this));
                tiles[row][col] = c;
            }
        }
    }

    std::vector<Collider> combined = CombineColliders(tiles);
    for (Collider coll : combined) {
            mvColliders.push_back(coll);
    }

    inFile.close();
}

Map::~Map()
{
    //dtor
}

void Map::Render() {
    for (int row = 0; row < 30; row++) {
        for (int col = 0; col < 40; col++) {

                if (tiles[row][col] == EMPTY) {
                    continue;
                } else if (tiles[row][col] == P1START || tiles[row][col] == P2START || tiles[row][col] == P3START || tiles[row][col] == P4START) {
                    continue;
                }

                int offset = (tiles[row][col] - 1) * 8;

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

                #ifdef _COLLISION_DEBUG
                /* Render all colliders */
                for (Collider& c : mvColliders) {
                    std::vector<Point> colliderPoints = c.GetPoints();
                    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    SDL_RenderDrawLine(mRenderer, colliderPoints[0].x, colliderPoints[0].y, colliderPoints[1].x, colliderPoints[1].y);
                    SDL_RenderDrawLine(mRenderer, colliderPoints[1].x, colliderPoints[1].y, colliderPoints[2].x, colliderPoints[2].y);
                    SDL_RenderDrawLine(mRenderer, colliderPoints[2].x, colliderPoints[2].y, colliderPoints[3].x, colliderPoints[3].y);
                    SDL_RenderDrawLine(mRenderer, colliderPoints[3].x, colliderPoints[3].y, colliderPoints[0].x, colliderPoints[0].y);
                }
                /* End of collider rendering */
                #endif // _COLLISION_DEBUG

        }
    }
}

std::vector<Collider>& Map::GetColliders() {
    return mvColliders;
}

const Vector2D& Map::GetStartPos(const int pn) const {
    return StartPos[pn - 1];
}

const unsigned int Map::GetTileAt(const int row, const int col) const {
    return tiles[row][col];
}

const bool Map::LoadSuccess() const {
    return mLoadSuccess;
}

std::vector<Collider> Map::CombineColliders(unsigned char (&tiles)[30][40]) const {

    std::vector<Collider> vColliders;

    char tilesColliders[30][40];

    for (int row = 0; row < 30; ++row) {
        for (int col = 0; col < 40; ++col) {
            if (row == 0 || row == 29 || col == 0 || col == 39) {
                tilesColliders[row][col] = 1;
            } else if (tiles[row][col] != EMPTY) {
                tilesColliders[row][col] = -1;
            } else {
                tilesColliders[row][col] = 0;
            }
        }
    }

    int colliderGroup = 2;
    for (int row = 1; row < 29; ++row) {
        for (int col = 1; col < 39; ++col) {
            if (tilesColliders[row][col] == -1) {
                int tempX, tempY;
                int tempH, tempW;

                tempX = col;
                tempY = row;

                tilesColliders[tempY][tempX] = colliderGroup;
                tempH = tempW = 1;

                tempX++;

                while (tilesColliders[row][tempX] == -1) {
                    tilesColliders[row][tempX] = colliderGroup;
                    tempW++;
                    tempX++;
                }

                tempY = row + 1;

                while (tilesColliders[tempY][col] == -1) {
                    bool fullRow = true;
                    for (tempX = col; tempX < col + tempW; tempX++) {
                        if (tilesColliders[tempY][tempX] != -1) {
                            fullRow = false;
                            break;
                        }
                    }
                    if (!fullRow) {
                        break;
                    } else {
                        tempH++;
                        for (tempX = col; tempX < col + tempW; tempX++) {
                            tilesColliders[tempY][tempX] = colliderGroup;
                        }
                        tempY++;
                        tempX = col;
                    }
                }

                vColliders.push_back(Collider(8 * tempW, 8 * tempH, (col * 8) + ( tempW * 4), (row * 8) + (tempH * 4), 0, this));
                colliderGroup++;

            }
        }
    }
    return vColliders;


}

std::vector<DestructibleBlock*> Map::GetDestructibleBlocks() {
    return mvDestructibleBlocks;
}
