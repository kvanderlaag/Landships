#include "Map.hpp"

//#define _COLLISION_DEBUG

Map::Map(const std::string& filename, const std::string& texturefile, SDL_Renderer* ren) :
    RenderableObject(texturefile, 320, 240, 0, 0, 0, ren),
    StartPos({Vector2D(0, 0), Vector2D(0, 0), Vector2D(0, 0), Vector2D(0, 0)})
{
    std::string path = SDL_GetBasePath();
    path += filename;
    std::ifstream inFile(path, std::ifstream::in);

    int row = 0;
    int col = 0;

    if (!inFile.good()) {
        exit(2);
    }

    std::string levelStr;
    {
        char c;
        while (inFile.get(c)) {
            levelStr += c;
        }
    }
    if (levelStr.length() != 40 * 30) {
        exit(2);
    }

    for (char c : levelStr) {
        if (c == EMPTY) {
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
        } else {
            mvColliders.push_back(Collider(8, 8, col * 8 + 4, row * 8 + 4, 0, this));
            tiles[row][col] = c;
        }
        if (++col == 40) {
            row++;
            col = 0;
        }
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
