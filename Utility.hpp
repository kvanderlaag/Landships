#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "Utility.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class Utility {
public:
    static SDL_Texture* LoadTexture(SDL_Renderer* ren, const std::string& filename);
private:

};

#endif
