#include "snake/snake.h"

void SnakeG::update() {
    if (snake.empty()) {
        SDL_FRect *rect = &snake.emplace_back();
        rect->x = 25;
        rect->y = 25;
        rect->w = 25;
        rect->h = 25;
    }
}

void show_grid(SDL_Renderer *renderer) {
    int wind_w, wind_h;
    SDL_FPoint a, b;
    SDL_GetWindowSize(SDL_GetRenderWindow(renderer), &wind_w, &wind_h);
    SDL_SetRenderDrawColorFloat(renderer,
                                ColorRGB::WHITE.r,
                                ColorRGB::WHITE.g,
                                ColorRGB::WHITE.b,
                                ColorRGB::WHITE.a);
    
    for (int r = 1; r < wind_h / 25; r++) {
        SDL_RenderLine(renderer, 0, r * 25.f, (float) wind_w, r * 25.f);
    }
    for (int q = 1; q < wind_w / 25; q++) {
        SDL_RenderLine(renderer, q * 25.f, 0, q * 25.f, (float)wind_h);
    }
}

void SnakeG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer,
                                ColorRGB::BLACK.r,
                                ColorRGB::BLACK.g,
                                ColorRGB::BLACK.b,
                                ColorRGB::BLACK.a);
    SDL_RenderClear(renderer);

    show_grid(renderer);
    SDL_SetRenderDrawColorFloat(renderer,
                                ColorRGB::RED.r,
                                ColorRGB::RED.g,
                                ColorRGB::RED.b,
                                ColorRGB::RED.a);
    SDL_RenderFillRect(renderer, &snake.at(0));
    SDL_RenderPresent(renderer);
}
