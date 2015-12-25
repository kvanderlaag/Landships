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

void Utility::ProjectRectangle(const Vector2D& axis, const Rectangle& r, float& fMin, float& fMax) {
    const std::vector<Point>& points = r.GetPoints();
    Vector2D v(points[0].x, points[0].y);
    float dotProduct = axis.Dot(v);
    fMin = dotProduct;
    fMax = dotProduct;
    for (size_t i = 0; i < points.size(); ++i) {
        v.SetX(points[i].x);
        v.SetY(points[i].y);
        dotProduct = v.Dot(axis);
        if (dotProduct < fMin) {
            fMin = dotProduct;
        } else {
            if (dotProduct > fMax) {
                fMax = dotProduct;
            }
        }
    }
}

float Utility::IntervalDistance(float minA, float maxA, float minB, float maxB) {
    if (minA < minB) {
        return minB - maxA;
    } else {
        return minA - maxB;
    }
}
