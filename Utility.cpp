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

SDL_Texture* Utility::RenderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* ren) {
    TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);

    if (font == nullptr) {
        std::cout << "Error opening font file: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr) {
        TTF_CloseFont(font);
        std::cout << "Error rendering text: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    if (tex == nullptr) {
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(surf);

    return tex;
}

void Utility::ProjectCollider(const Collider& col, const std::vector<float>& axis, float& pMin, float& pMax) {
    std::vector<std::vector<float>> points = col.GetPoints();

    float dotProduct = DotProduct(axis, points[0]);

    pMin = dotProduct;
    pMax = dotProduct;
    for (size_t i = 0; i < points.size(); ++i) {
        dotProduct = DotProduct(points[i], axis);
        if (dotProduct < pMin)
            pMin = dotProduct;
        else if (dotProduct > pMax)
            pMax = dotProduct;
    }
}

float Utility::DotProduct(const std::vector<float>& first, const std::vector<float>& second) {
    if (first.size() == second.size()) {
        return std::inner_product(first.begin(), first.end(), second.begin(), 0);
    } else {
        return FLT_MAX;
    }
}

float Utility::IntervalDistance(const float minA, const float maxA, const float minB, const float maxB) {
    if (minA < minB) {
        return minB - maxA;
    } else {
        return minA - maxB;
    }
}

void Utility::NormalizeVector2D(std::vector<float>& vec) {
    if (vec.size() < 2) {
        return;
    }

    float magnitude = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);

    vec[0] /= magnitude;
    vec[1] /= magnitude;
}
