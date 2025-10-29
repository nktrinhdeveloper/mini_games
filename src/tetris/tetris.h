#ifndef TETRIS_H
#define TETRIS_H

#include "app.h"

typedef struct Tetromino {
    static const int nb_blocks{4};
    SDL_FPoint  blocks[nb_blocks];
    int         shape;
    float       width;
    float       height;
    float       horz_off;
    float       angle; // radian
    int         anchor_pos;
    SDL_FColor  color;
} Tetromino;

class TetrisG : public Game {
    private:
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
        static const int    AREA_COL_UPPERB{25};
        static const int    AREA_COLS{AREA_COL_UPPERB - AREA_COL_LOWERB};
        int         area[GRID_ROWS][AREA_COLS];
        bool        need_update_area;
        Tetromino   *curr_tetro;
        std::queue<Tetromino> tetros_qu;
        std::vector<SDL_FRect> rendered_rect;
        
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