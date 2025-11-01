#ifndef TETRIS_H
#define TETRIS_H

#include "app.h"

typedef struct Tetromino {
    static const int        nb_blocks{4};
    static constexpr float  SPEED{3.f};
    static constexpr float  FALL_SPEED{0.5f};
    SDL_FPoint  blocks[nb_blocks];
    int         shape;
    float       horz_off;
    float       angle; // radian
    int         anchor_pos;
    SDL_FColor  color;
} Tetromino;

class TetrisG : public Game {
    public:
        enum TetroShape {
                NONE_SHAPE,
                I_SHAPE,
                O_SHAPE,
                S_SHAPE,
                Z_SHAPE,
                L_SHAPE,
                J_SHAPE,
                T_SHAPE
            };
        static const int    AREA_COL_LOWERB{1};
        static const int    AREA_COLS{24};
        TetrisG();
        ~TetrisG() override = default;
        void update() override;
        void render(SDL_Renderer *renderer) override;
        void restart() override;
        void on_keydown(const SDL_Keycode &code) override;
    private:
        int         area[GRID_ROWS][AREA_COLS];
        int         top_high;
        Tetromino   *curr_tetro;
        std::queue<Tetromino> tetros_qu;
        
        void fill_block_queue();
        void move_curr_tetro();
};

#endif