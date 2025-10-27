#ifndef APP_H
#define APP_H

#include <iostream>
#include <vector>
#include "SDL3/SDL.h"
#include "SDL3/SDL_ttf.h"

template <typename T>
void free_ptr(T **ptr) {
    if (*ptr) {
        delete *ptr;
        *ptr = nullptr;
    }
}

template <typename T>
void free_arr_ptr(T **ptr) {
    if (*ptr) {
        delete [] *ptr;
        *ptr = nullptr;
    }
}

class Game {
    public:
        Game() = default;
        virtual ~Game() = default;
        Game(const Game &other) = delete;
        Game(Game &&other) noexcept = delete;
        Game &operator=(const Game &other) = delete;
        Game &operator=(Game &other) noexcept = delete;
    virtual bool init(SDL_Renderer *renderer) {return true;};
    virtual void update() {}
    virtual void render(SDL_Renderer *renderer) {}
    virtual void restart() {}
};

namespace ColorRGB {
    const SDL_FColor BLACK  = {0, 0, 0, 1};
    const SDL_FColor WHITE  = {1, 1, 1, 1};
    const SDL_FColor RED    = {1, 0, 0, 1};
    const SDL_FColor GREEN  = {0, 1, 0, 1};
    const SDL_FColor BLUE   = {0, 0, 1, 1};
    const SDL_FColor MEDIUM_GRAY    = {171/255.F, 171/255.F, 171/255.F, 1};
    const SDL_FColor LIGHT_GRAY     = {228/255.F, 228/255.F, 228/255.F, 1};
    const SDL_FColor DARK_GRAY      = {114/255.F, 114/255.F, 114/255.F, 1};
    
};

const int GRID_SIZE{25};
const int GRID_COLS{32};
const int GRID_ROWS{24};

#endif