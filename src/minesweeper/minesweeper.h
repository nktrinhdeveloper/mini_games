#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "app.h"

typedef struct TextObj {
    TTF_Text    *text_obj;
    std::string text;
    SDL_FPoint  loc;
} TextObj;

typedef struct Button {
    SDL_FRect   rect;
    SDL_FColor  color;
    int         state;
    bool        hover;
    TextObj     text;
    int         count;
} Button;

class MineSweeperG : public Game {
    private:
        enum ButtonSt {
            DISABLE     = 0,
            ACTIVE      = 1,
        };

        TTF_Font        *font;
        TTF_TextEngine  *text_engine;
        Button          btns[AppConst::GRID_ROWS][AppConst::GRID_COLS];
        SDL_Point       hover_btn;
        int             nb_safe;

        void count_boom(const int &boomr, const int& boomq);
        void create_random_boom();
        void cleanup();
        void end_game();
        void open_neighbor(const int &startr, const int &startq);
        void open_button(const int &r, const int &q, const bool &all = false);
        void note_button(const int &r, const int &q);
    public:
        ~MineSweeperG() override;
        bool init(SDL_Renderer *renderer, const std::string &running_dir) override;
        void update() override;
        void render(SDL_Renderer *renderer) override;
        void restart() override;
        void on_mouse_motion(const int &mousex, const int &mousey) override;
        void on_mouse_down(const int &mouse) override;

};

#endif