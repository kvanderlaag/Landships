#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>

#include "Player.hpp"
#include "Tile.hpp"
#include "Map.hpp"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_MOVE 1
#define MAX_ROTATE 2

int main(int argc, char** argv) {

    uint32_t ticks = SDL_GetTicks();
    uint32_t time_elapsed = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);

    int playerx, playery;
    playerx = (SCREEN_WIDTH / 2) - 8;
    playery = (SCREEN_HEIGHT / 2) - 8;

    SDL_Window* win = SDL_CreateWindow("Balls", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
    SDL_ShowCursor(0);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

    Player p("Tank.png", ren);
    p.SetX(playerx);
    p.SetY(playery);

    std::string basePath = SDL_GetBasePath();
    basePath += "map.d";
    Map m(basePath, "WallTiles.png", ren);

    if (p.GetTexture() == nullptr) {
        std::cout << "Error loading player sprite: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_DestroyRenderer(ren);
        SDL_Quit();
        return 1;
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

    bool running = true;

    while (running == true) {

        time_elapsed += SDL_GetTicks() - ticks;
        ticks = SDL_GetTicks();
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_PAUSE:
                case SDLK_END:
                    running = false;
                    break;
                case SDLK_LEFT:
                    p.SetRotationVel(-MAX_ROTATE);
                    break;
                case SDLK_RIGHT:
                    p.SetRotationVel(MAX_ROTATE);
                    break;
                case SDLK_a:
                    p.SetTurretRotationVel(-MAX_ROTATE);
                    break;
                case SDLK_d:
                    p.SetTurretRotationVel(MAX_ROTATE);
                    break;
                case SDLK_UP:
                    p.SetForwardVel(MAX_MOVE);
                    break;
                case SDLK_DOWN:
                    p.SetForwardVel(-MAX_MOVE);
                    break;
                }

            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    if (p.GetRotationVel() < 0)
                        p.SetRotationVel(0);
                    break;
                case SDLK_RIGHT:
                    if (p.GetRotationVel() > 0)
                        p.SetRotationVel(0);
                    break;
                case SDLK_a:
                    if (p.GetTurretRotationVel() < 0)
                        p.SetTurretRotationVel(0);
                    break;
                case SDLK_d:
                    if (p.GetTurretRotationVel() > 0)
                        p.SetTurretRotationVel(0);
                    break;
                case SDLK_UP:
                    if (p.GetForwardVel() > 0)
                        p.SetForwardVel(0);
                    break;
                case SDLK_DOWN:
                    if (p.GetForwardVel() < 0)
                        p.SetForwardVel(0);
                    break;
                }

            }
        }


        /*if (cycle) {
            red--;
        } else {
            red++;
        }*/

        p.Move(SCREEN_WIDTH, SCREEN_HEIGHT);

        /*if (red == 0 || red == 0xFF) {
            cycle = !cycle;
        }*/

        if (time_elapsed >= (1000/60)) {

            SDL_Surface* surf = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);

            SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0x20, 0x20, 0x05));

            SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);

            SDL_FreeSurface(surf);

            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, tex, NULL, NULL);
            m.Render();
            p.Render();
            SDL_RenderPresent(ren);
        }
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
