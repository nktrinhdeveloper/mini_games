#ifndef MATCHTHREE_H
#define MATCHTHREE_H

#include "app.h"
class Match3G : public Game {
public:
    enum ItemShape {
        TRIANGLE = 1,
        SQUARE,
        ROTATED_SQUARE,
        PENTAGON,
        HEXAGON,
        CIRCLE,
        DIAMOND
    };
    static const int GRID_SIZE{75};
    static const int PADDING{10};
    static const int NB_ITEMS{7};
    static constexpr float SPEED{1.F};

    ~Match3G() override = default;
    bool init(SDL_Renderer *renderer, const std::string &running_dir) override;
    void update() override;
    void render(SDL_Renderer *renderer) override;
    void restart() override;
    void on_keydown(const SDL_Keycode &code) override;
    void on_mouse_motion(const int &mousex, const int &mousey) override;
    void on_mouse_down(const int &mouse) override;
    void on_mouse_up(const int &mouse) override;
private:
    enum GState {
        NONE_STATE,
        SWAPPING    = 1,
        MATCHING    = 1 << 2,
        REMOVING    = 1 << 3,
        REVERTING   = 1 << 4,
        FILLING     = 1 << 5,
    };
    Vector2D<Polygon> items;
    std::vector<std::pair<int, int>> matches;
    std::pair<int, int>   selected;
    std::pair<int, int>   targeted;
    SDL_Point   direction;
    float       offs;
    bool        mouse_selected;
    int         state;

    void swap_select_items();
    bool check_match();
    void remove_matches();
    bool filling_removed_matches();
};

#endif