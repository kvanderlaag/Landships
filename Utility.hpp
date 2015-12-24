#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "Collider.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <numeric>
#include <cmath>

class Collider;

class Utility {
public:
    static SDL_Texture* LoadTexture(SDL_Renderer* ren, const std::string& filename);
    static SDL_Texture* RenderText(const std::string& message, const std::string& fontFile,
                                   SDL_Color color, int fontSize, SDL_Renderer* ren);
    static void ProjectCollider(const Collider&, const std::vector<float>& axis, float& pMin, float& pMax);
    static float DotProduct(const std::vector<float>& first, const std::vector<float>& second);
    static float IntervalDistance(const float minA, const float maxA, const float minB, const float maxB);
    static void NormalizeVector2D(std::vector<float>& vec);
private:

};

#endif
