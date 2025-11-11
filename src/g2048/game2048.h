#ifndef GAME2048_H
#define GAME2048_H

#include "app.h"

class MergeTileG : public Game {
public:
    typedef struct Tile {
        SDL_FPoint  loc;
        int         val;
        TTF_Text    *ttf_txt;
        SDL_FPoint  txt_loc;
        bool        movable;
            Tile();
            ~Tile();
            Tile(const Tile &other) = default;
            Tile &operator=(const Tile &other) = default;
            Tile(Tile &&other) noexcept;
            Tile &operator=(Tile &&other) noexcept;
        void reset_tile();
    } Button;

    static const int WIND_WIDTH{450};
    static const int WIND_HEIGHT{450};
    static const int BORDER_THICK{10};
    static const int TILE_SIZE{100};
    static const int SPEED{2};

    ~MergeTileG() override;
    bool init(SDL_Renderer *renderer, const std::string &running_dir);
    void update() override;
    void render(SDL_Renderer *renderer) override;
    void restart() override;
    void on_keydown(const SDL_Keycode &code, const SDL_Keymod &mod) override;
private:
    TTF_TextEngine  *text_engine;
    TTF_Font        *font;
    Vector2D<MergeTileG::Tile> tiles;
    SDL_Point       direction;
    bool            need_add_tile;

    bool move_and_merge_tile();
    bool create_random_tile();
};

#endif