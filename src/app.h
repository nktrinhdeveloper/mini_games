#ifndef APP_H
#define APP_H

#include <iostream>
#include <vector>
#include "SDL3/SDL.h"

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
    virtual void update() {}
    virtual void render(SDL_Renderer *renderer) {}
};

namespace ColorRGB {
    const SDL_FColor BLACK  = {0, 0, 0, 1};
    const SDL_FColor WHITE  = {1, 1, 1, 1};
    const SDL_FColor RED    = {1, 0, 0, 1};
    const SDL_FColor GREEN  = {0, 1, 0, 1};
    const SDL_FColor BLUE   = {0, 0, 1, 1};
};

#endif