#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Utility.hpp"

#include <string>
#include <sstream>
#include <fstream>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define EMPTY   0x00
#define P1START 0x01
#define P2START 0x02
#define P3START 0x03
#define P4START 0x04
#define BLOCK   0x05

using namespace std;

int SaveAs(SDL_Renderer* ren, unsigned char tiles[30][40]);

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cout << "Failed to initialize SDL. (Code: " << SDL_GetError() << ")" << std::endl;
        exit(1);
    }
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cout << "Failed to initialize SDL_Image. (Code: " << SDL_GetError() << ")" << std::endl;
        SDL_Quit();
        exit(1);
    }
    if (TTF_Init() != 0) {
        std::cout << "Failed to initialize SDL_ttf. (Code: " << SDL_GetError() << ")" << std::endl;
        SDL_Quit();
        IMG_Quit();
        exit(1);
    }

    uint32_t rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        amask = 0xff000000;
        bmask = 0x00ff0000;
        gmask = 0x0000ff00;
        rmask = 0x000000ff;
    #endif // SDL_BIG_ENDIAN

    SDL_Window* win = SDL_CreateWindow("Tanks Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
    SDL_ShowCursor(0);

    SDL_Surface* surf = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 0x20, 0x20, 0x05, 0xFF));

    SDL_Texture* bgTex = SDL_CreateTextureFromSurface(ren, surf);

    SDL_FreeSurface(surf);

    SDL_Texture* wallTex = Utility::LoadTexture(ren, "WallTiles.png");

    int tileCount;

    SDL_QueryTexture(wallTex, NULL, NULL, &tileCount, NULL);

    tileCount /= 8;

    bool running = true;

    int mousex, mousey;
    int mousextile, mouseytile;
    mousex = mousey = -1;
    mousextile = mouseytile = -1;
    bool mouseheld = false;

    int tiletype = EMPTY;

    unsigned char tiles[30][40];
    for (int row = 0; row < 30; row++) {
        for (int col = 0; col < 40; col++) {
            if (row == 0 || row == 29) {
                tiles[row][col] = BLOCK;
            } else if (col == 0 || col == 39) {
                tiles[row][col] = BLOCK;
            } else {
                tiles[row][col] = EMPTY;
            }
        }
    }

    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                mousex = e.motion.x;
                mousey = e.motion.y;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                mouseheld = true;
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                mouseheld = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (tiletype < tileCount) {
                        tiletype++;
                    }
                    break;
                case SDLK_DOWN:
                    if (tiletype > EMPTY) {
                        tiletype--;
                    }
                    break;
                case SDLK_s:
                    if (SaveAs(ren, tiles) == -1) {
                        running = false;
                    }
                    break;
                case SDLK_END:
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
            }
        }

        if (mouseheld) {
            if (mousextile != 0 && mousextile != 39 && mouseytile != 0 && mouseytile != 29) {
                    if (tiletype >= P1START && tiletype <= P4START) {
                        for (int row = 0; row < 30; row++) {
                            for (int col = 0; col < 40; col++) {
                                if (tiles[row][col] == tiletype)
                                    tiles[row][col] = EMPTY;
                            }
                        }
                    }
                    tiles[mouseytile][mousextile] = tiletype;
                }
        }

        SDL_Rect* select = NULL;
        if (mousex >= 0 && mousey >= 0) {
            mousextile = mousex / 8;
            mouseytile = mousey / 8;
            select = (SDL_Rect*) malloc(sizeof(SDL_Rect));
            select->h = 8;
            select->w = 8;
            select->y = mouseytile * 8;
            select->x = mousextile * 8;

        } else {
            mouseytile = mousextile = -1;
        }

        SDL_RenderClear(ren);

        SDL_RenderCopy(ren, bgTex, NULL, NULL);

        // Draw tiles as laid out
        for (int row = 0; row < 30; row++) {
            for (int col = 0; col < 40; col++) {
                if (tiles[row][col] > EMPTY) {
                    SDL_Rect srcRect, dstRect;
                    srcRect.x = (tiles[row][col] - 1) * 8;
                    srcRect.y = 0;
                    srcRect.w = 8;
                    srcRect.h = 8;

                    dstRect.x = col * 8;
                    dstRect.y = row * 8;
                    dstRect.w = 8;
                    dstRect.h = 8;

                    SDL_RenderCopy(ren, wallTex, &srcRect, &dstRect);
                }
            }
        }

        if (select != NULL) {
            if (tiletype != EMPTY) {
                SDL_Rect srcRect, dstRect;
                srcRect.x = (tiletype - 1) * 8;
                srcRect.y = 0;
                srcRect.w = 8;
                srcRect.h = 8;

                dstRect.x = mousextile * 8;
                dstRect.y = mouseytile * 8;
                dstRect.w = 8;
                dstRect.h = 8;

                SDL_RenderCopy(ren, wallTex, &srcRect, &dstRect);
            } else {
                SDL_Surface* blanktile = SDL_CreateRGBSurface(0, 8, 8, 32, rmask, gmask, bmask, amask);
                SDL_FillRect(blanktile, NULL, SDL_MapRGB(blanktile->format, 0x20, 0x20, 0x05));
                SDL_Texture* blanktex = SDL_CreateTextureFromSurface(ren, blanktile);


                SDL_Rect srcRect, dstRect;
                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.w = 8;
                srcRect.h = 8;

                dstRect.x = mousextile * 8;
                dstRect.y = mouseytile * 8;
                dstRect.w = 8;
                dstRect.h = 8;

                SDL_RenderCopy(ren, blanktex, &srcRect, &dstRect);
                SDL_FreeSurface(blanktile);
                SDL_DestroyTexture(blanktex);

            }
            SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(ren, select);
            free(select);
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(bgTex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    return 0;
}

int SaveAs(SDL_Renderer* ren, unsigned char tiles[30][40]) {
    bool running = true;

    std::string saveFileName;

    std::stringstream outputStringStream;

    while (running) {

        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return -1;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_END:
                    return 0;
                    break;
                case SDLK_a:
                    saveFileName += 'a';
                    break;
                case SDLK_b:
                    saveFileName += 'b';
                    break;
                case SDLK_c:
                    saveFileName += 'c';
                    break;
                case SDLK_d:
                    saveFileName += 'd';
                    break;
                case SDLK_e:
                    saveFileName += 'e';
                    break;
                case SDLK_f:
                    saveFileName += 'f';
                    break;
                case SDLK_g:
                    saveFileName += 'g';
                    break;
                case SDLK_h:
                    saveFileName += 'h';
                    break;
                case SDLK_i:
                    saveFileName += 'i';
                    break;
                case SDLK_j:
                    saveFileName += 'j';
                    break;
                case SDLK_k:
                    saveFileName += 'k';
                    break;
                case SDLK_l:
                    saveFileName += 'l';
                    break;
                case SDLK_m:
                    saveFileName += 'm';
                    break;
                case SDLK_n:
                    saveFileName += 'n';
                    break;
                case SDLK_o:
                    saveFileName += 'o';
                    break;
                case SDLK_p:
                    saveFileName += 'p';
                    break;
                case SDLK_q:
                    saveFileName += 'q';
                    break;
                case SDLK_r:
                    saveFileName += 'r';
                    break;
                case SDLK_s:
                    saveFileName += 's';
                    break;
                case SDLK_t:
                    saveFileName += 't';
                    break;
                case SDLK_u:
                    saveFileName += 'u';
                    break;
                case SDLK_v:
                    saveFileName += 'v';
                    break;
                case SDLK_w:
                    saveFileName += 'w';
                    break;
                case SDLK_x:
                    saveFileName += 'x';
                    break;
                case SDLK_y:
                    saveFileName += 'y';
                    break;
                case SDLK_z:
                    saveFileName += 'z';
                    break;
                case SDLK_1:
                    saveFileName += '1';
                    break;
                case SDLK_2:
                    saveFileName += '2';
                    break;
                case SDLK_3:
                    saveFileName += '3';
                    break;
                case SDLK_4:
                    saveFileName += '4';
                    break;
                case SDLK_5:
                    saveFileName += '5';
                    break;
                case SDLK_6:
                    saveFileName += '6';
                    break;
                case SDLK_7:
                    saveFileName += '7';
                    break;
                case SDLK_8:
                    saveFileName += '8';
                    break;
                case SDLK_9:
                    saveFileName += '9';
                    break;
                case SDLK_0:
                    saveFileName += '0';
                    break;
                case SDLK_UNDERSCORE:
                    saveFileName += '_';
                    break;
                case SDLK_RETURN:
                    running = false;
                    break;
                case SDLK_BACKSPACE:
                    std::string saveFileNameNew;
                    if (saveFileName.length() > 0) {
                        for (size_t i = 0; i < saveFileName.length() - 1; i++) {
                            saveFileNameNew += saveFileName[i];
                        }
                    }
                    saveFileName = saveFileNameNew;
                    break;
                }

            }
        }


        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(ren);

        outputStringStream.str("");
        outputStringStream << "Save file as: ";
        outputStringStream << saveFileName.c_str();

        SDL_Color c = {0xFF, 0xFF, 0xFF, 0xFF};
        SDL_Texture* saveText = Utility::RenderText(outputStringStream.str(), "sample.ttf", c, 10, ren);

        int saveTextWidth, saveTextHeight;

        SDL_QueryTexture(saveText, NULL, NULL, &saveTextWidth, &saveTextHeight);

        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = saveTextWidth;
        srcRect.h = saveTextHeight;

        SDL_RenderCopy(ren, saveText, &srcRect, &srcRect);
        SDL_RenderPresent(ren);


    }

    if (saveFileName.length() > 0) {
        std::string basePath = SDL_GetBasePath();
        basePath += saveFileName + ".d";
        std::ofstream outFile(basePath, std::ofstream::out);
        for (int row = 0; row < 30; row++) {
            for (int col = 0; col < 40; col++) {
                outFile << tiles[row][col];
            }
        }

    } else {
        running = true;
        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(ren);
        SDL_Color c = {0xFF, 0xFF, 0xFF, 0xFF};
        SDL_Texture* saveText = Utility::RenderText("Invalid file name! (Press Enter to continue.)", "sample.ttf", c, 10, ren);

        int saveTextWidth, saveTextHeight;

        SDL_QueryTexture(saveText, NULL, NULL, &saveTextWidth, &saveTextHeight);

        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = saveTextWidth;
        srcRect.h = saveTextHeight;

        SDL_RenderCopy(ren, saveText, &srcRect, &srcRect);
        SDL_RenderPresent(ren);

        while (running) {
            SDL_Event e;

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        running = false;
                    }
                }
            }
        }
    }
    return 0;
}
