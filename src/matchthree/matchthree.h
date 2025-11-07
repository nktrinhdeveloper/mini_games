#ifndef MATCHTHREE_H
#define MATCHTHREE_H

#include "app.h"
class Match3G : public Game {
public:
    enum ItemShape {
        TRIANGLE = 1,
        SQUARE,
        ROTATED_SQUARE,
        HEXAGON,
        ROTATE_HEXAGON,
        CIRCLE
    };
    static const int GRID_SIZE{75};
    static const int PADDING{3};

    ~Match3G() override = default;
    bool init(SDL_Renderer *renderer, const std::string &running_dir) override;
    void update() override;
    void render(SDL_Renderer *renderer) override;
    void restart() override;
    void on_keydown(const SDL_Keycode &code) override;
private:
    Vector2D<int> items_grid;
};

#endif