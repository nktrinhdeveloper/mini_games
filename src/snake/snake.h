#ifndef SNAKE_H
#define SNAKE_H

#include "app.h"

typedef struct SnakePart {
    SDL_FRect   rect;
    SDL_FPoint  offs;
} SnakePart;

class SnakeG : public Game{
    private:
        std::vector<SnakePart>  snake;
        SDL_FRect   prey;
        SDL_FPoint  next_offs;

        void controller();
        void move();
        void create_random_prey();
    public:
        SnakeG();
        ~SnakeG() override = default;
        void update() override;
        void render(SDL_Renderer *renderer) override;
};

#endif