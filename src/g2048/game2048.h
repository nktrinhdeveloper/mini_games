#ifndef GAME2048_H
#define GAME2048_H

#include "app.h"

class Game2048 : public Game {
    public:
    ~Game2048() override = default;
    bool init(SDL_Renderer *renderer, const std::string &running_dir) override {return true;}
    void update() override {}
    void render(SDL_Renderer *renderer) override {}
    void restart() override {}
    void on_keydown(const SDL_Keycode &code) override {}
};

#endif