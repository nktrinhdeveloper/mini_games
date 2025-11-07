#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H

#include "app.h"

class FlappyBird : public Game {
    public:
        FlappyBird() = default;
        ~FlappyBird() override = default;
        bool init(SDL_Renderer *renderer, const std::string &running_dir) override;
        void update() override;
        void render(SDL_Renderer *renderer) override;
        void restart() override;
        void on_keydown(const SDL_Keycode &code) override;
    private:
        static constexpr float HORZ_SPEED{0.5f};
        static constexpr float VERT_SPEED{4.f};
        enum FlappySound {
            FLAP
        };
        SDL_FRect               bird;
        std::vector<SDL_FRect>  map;
        int                     pipe_create_idx;
        int                     min_pipe_idx;
        float                   b_vert_velo;

        bool load_sound();
        void move_world();
        void bird_fall();
        bool check_bird_collision();
};

#endif