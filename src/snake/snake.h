#ifndef SNAKE_H
#define SNAKE_H

#include "app.h"

class SnakeG : public Game{
    private:
        std::vector<SDL_FRect>  snake;
        SDL_FRect               prey;
    
    public:
        ~SnakeG() override = default;
        void update() override;
        void render(SDL_Renderer *renderer) override;
};

#endif