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
        Button          btns[GRID_ROWS][GRID_COLS];
        SDL_Point       hover_btn;

        void count_boom(const int &boomr, const int& boomq);
        void create_random_boom();
        void cleanup();
    public:
        ~MineSweeperG() override;
        bool init(SDL_Renderer *renderer) override;
        void update() override;
        void render(SDL_Renderer *renderer) override;
        void restart() override;
        void on_hover(const int &mousex, const int &mousey);
        void on_click();
};

#endif