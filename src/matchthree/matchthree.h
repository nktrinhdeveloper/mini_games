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
        DIAMOND,
        BOMB,
        PLANE,
        STAR
    };
    enum MatchShape {
        O_SHAPE = 1,
        T_SHAPE,
        I_SHAPE_VERT,
        I_SHAPE_HORZ
    };
    typedef struct Matcher {
        std::vector<std::pair<int, int>> matches;
        int shape;
        std::pair<int, int> check_pos;
    } Matcher;
    static const int GRID_SIZE{75};
    static const int PADDING{10};
    static const int NB_ITEMS{7};
    static constexpr float SPEED{1.F};

    ~Match3G() override = default;
    bool init(SDL_Renderer *renderer, const std::string &running_dir) override;
    void update() override;
    void render(SDL_Renderer *renderer) override;
    void restart() override;
    void on_keydown(const SDL_Keycode &code, const SDL_Keymod &mod) override;
    void on_mouse_motion(const int &mousex, const int &mousey) override;
    void on_mouse_down(const int &mouse, const int &mousex, const int &mousey) override;
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
    // std::pair<int, int> check_list[2];
    std::vector<std::pair<int, int>> check_list;
    std::pair<int, int> hovering;
    SDL_Point   direction;
    float       offs;
    bool        mouse_selected;
    int         state;

    void swap_select_items();
    void check_match();
    void remove_matches(const std::vector<Matcher> &matchers);
    bool filling_removed_matches();
    bool key_ctrl_swap(const SDL_Keymod &mod, const SDL_Point &direction);
};

#endif