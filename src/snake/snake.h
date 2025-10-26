#ifndef SNAKE_H
#define SNAKE_H

#include "app.h"

typedef struct SnakePart {
    SDL_FRect   rect;
    SDL_FPoint  offs;
} SnakePart;

class SnakeG : public Game{
    private:
        const float SPEED{0.5f};
        std::vector<SDL_FRect>  snake;
        SDL_Point   direction;
        SDL_Point   next_direction;
        SDL_FRect   prey;
        bool        alive;

        std::vector<SDL_FRect> get_snake_render_rect() const;
        void controller();
        void move();
        void create_random_prey();
        void change_direction();
        int  check_collision(const bool &horz, const bool &pos);
        void add_tail();
    public:
        SnakeG();
        ~SnakeG() override = default;
        void update() override;
        void render(SDL_Renderer *renderer) override;
};

#endif