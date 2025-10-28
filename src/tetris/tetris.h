#ifndef TETRIS_H
#define TETRIS_H

#include "app.h"

typedef struct TetrisBlock {
    static const int nb_rects{4};
    SDL_FRect   rects[nb_rects];
    int         shape;
    int         angle; // radian, only 0 and pi/2
    SDL_FColor  color;
} TetrisBlock;

class TetrisG : public Game {
    private:
        enum BlockShape {
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
        static const int    AREA_COL_UPPERB{13};
        static const int    AREA_COLS{AREA_COL_UPPERB - AREA_COL_LOWERB};
        int                     col_pos[TetrisBlock::nb_rects];
        int                     r_pos[2];
        SDL_FRect               area[GRID_ROWS][AREA_COLS];
        TetrisBlock             *curr_block;
        std::queue<TetrisBlock> blocks_qu;
        
        void create_I_shape_block();
        void create_O_shape_block();
        void create_S_shape_block();
        void create_Z_shape_block();
        void create_L_shape_block();
        void create_J_shape_block();
        void create_T_shape_block();
        void fill_block_queue();
    public:
        TetrisG();
        ~TetrisG() override = default;
        void update() override;
        void render(SDL_Renderer *renderer) override;
        void restart() override;
        void on_keydown(const SDL_Keycode &code) override;
};

#endif