#include "Utility.hpp"

SDL_Texture* Utility::LoadTexture(SDL_Renderer* ren, const std::string& filename) {
    char* basePath = SDL_GetBasePath();
    std::string base(basePath);
    base += filename;
    //std::cout << base << std::endl;
    SDL_Texture* tex = IMG_LoadTexture(ren, base.c_str());
    if (tex == nullptr) {
        std::cout << "SDL_LoadTexture error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return tex;

}
