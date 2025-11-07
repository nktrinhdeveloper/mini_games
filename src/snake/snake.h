#ifndef SNAKE_H
#define SNAKE_H

#include "app.h"

typedef struct SnakePart {
    SDL_FRect   rect;
    SDL_FPoint  offs;
} SnakePart;

class SnakeG : public Game{
    private:
        enum SnakeState{
            DEAD = -1,
            HUNT = 0,
            GROW = 1
        };
        const float SPEED{0.5f};
        std::vector<SDL_FRect>  snake;
        SDL_Point   direction;
        SDL_Point   next_direction;
        SDL_FRect   prey;

        std::vector<SDL_FRect> get_snake_render_rect() const;
        void controller();
        void move();
        void create_random_prey();
        void change_direction();
        int  check_collision(const bool &horz, const bool &pos);
        void add_tail(const bool &horz, const bool &pos);
    public:
        ~SnakeG() override = default;
        bool init(SDL_Renderer *renderer, const std::string &running_dir) override;
        void update() override;
        void render(SDL_Renderer *renderer) override;
        void restart() override;
};

#endif