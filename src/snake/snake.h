#ifndef SNAKE_H
#define SNAKE_H

#include "app.h"

typedef struct SnakePart {
    SDL_FRect   rect;
    SDL_FPoint  offs;
} SnakePart;

class SnakeG : public Game{
    private:
        const int SNAKEPART_SIZE{GRID_SIZE - 2};
        const float SPEED{1};
        std::vector<SnakePart>  snake;
        std::vector<SDL_FRect>  corners;
        SDL_FPoint  corner;
        SDL_FRect   prey;
        SDL_FPoint  next_offs;

        void controller();
        void move();
        void create_random_prey();
        bool change_direction();
    public:
        SnakeG();
        ~SnakeG() override = default;
        void update() override;
        void render(SDL_Renderer *renderer) override;
};

#endif