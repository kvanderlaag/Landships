#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <numeric>
#include <cmath>
#include <cstring>

#include "Vector2D.hpp"
#include "Rectangle.hpp"
#include "Defines.hpp"

extern std::string basePath;

class Collider;

class Utility {
public:
    static SDL_Texture* LoadTexture(SDL_Renderer* ren, const std::string& filename);
    static SDL_Texture* RenderText(const std::string& message, const std::string& fontFile,
                                   SDL_Color color, int fontSize, SDL_Renderer* ren);
    static void ProjectRectangle(const Vector2D& axis, const Rectangle& r, float& fMin, float& fMax);
    static float IntervalDistance(float minA, float maxA, float minB, float maxB);

    static Mix_Music* LoadMusic(const std::string& filename);
    static Mix_Chunk* LoadSound(const std::string& filename);
    static void PlayMusic(Mix_Music* m);
    static void PlaySound(Mix_Chunk* m);
    static void PlaySound(Mix_Chunk* m, const int volume);
    static void FireRumble(SDL_Haptic* h);
    static void DieRumble(SDL_Haptic* h);
private:

};

#endif
