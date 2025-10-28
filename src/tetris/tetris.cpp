#include "tetris.h"

TetrisG::TetrisG() : Game() {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < AREA_COLS; q++) {
            area[r][q] = {0, 0, 0, 0};
        }
    }
    curr_block = nullptr;
     // calc column position relate to window
    int mid_pos_left = ((AREA_COLS / 2) - 1) + AREA_COL_LOWERB;
    col_pos[0] = mid_pos_left - 1;
    col_pos[1] = mid_pos_left;
    col_pos[2] = mid_pos_left + 1;
    col_pos[3] = mid_pos_left + 2;

    r_pos[0] = 1;
    r_pos[1] = 2;
}

void TetrisG::update() {
    if(blocks_qu.empty())
        create_S_shape_block();

    if (!curr_block)
        curr_block = &blocks_qu.front();

}
        
void TetrisG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    for (int i = 0; i < TetrisBlock::nb_rects; i++) {
        SDL_RenderFillRect(renderer, &curr_block->rects[i]);
    }
}
        
void TetrisG::restart() {}

void TetrisG::on_keydown(const SDL_Keycode &code) {}

void TetrisG::create_I_shape_block() {
    TetrisBlock *tet_block = &blocks_qu.emplace();
    tet_block->shape = BlockShape::I_SHAPE;
    tet_block->angle = 0;
    for (int i = 0; i < TetrisBlock::nb_rects; i++) {
        tet_block->rects[i].x = (col_pos[i] * GRID_SIZE) + 0.5f;
        tet_block->rects[i].y = (r_pos[0] * GRID_SIZE) + 0.5f;
        tet_block->rects[i].w = GRID_SIZE - 1;
        tet_block->rects[i].h = GRID_SIZE - 1;
    }
}

void TetrisG::create_O_shape_block() {
    TetrisBlock *block = &blocks_qu.emplace();
    block->shape = BlockShape::O_SHAPE;
    block->angle = 0;
    int i = 0;
    for (int q = 0; q < TetrisBlock::nb_rects / 2; q++) {
        for (int r = 0; r < 2; r++) {
            block->rects[i].x = (col_pos[q + 1] * GRID_SIZE) + 0.5f;
            block->rects[i].y = (r_pos[r] * GRID_SIZE) + 0.5f;
            block->rects[i].w = GRID_SIZE - 1;
            block->rects[i].h = GRID_SIZE - 1;
            i++;
        }
    }
}

void TetrisG::create_S_shape_block() {
    TetrisBlock *block = &blocks_qu.emplace();
    block->shape = BlockShape::S_SHAPE;
    block->angle = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 2; q >= 1 ; q--) {
            block->rects[i].x = (col_pos[q - r] * GRID_SIZE) + 0.5f;
            block->rects[i].y = (r_pos[r] * GRID_SIZE) + 0.5f;
            block->rects[i].w = GRID_SIZE - 1;
            block->rects[i].h = GRID_SIZE - 1;
            i++;
        }
    }
}

void TetrisG::create_Z_shape_block() {
    TetrisBlock *block = &blocks_qu.emplace();
    block->shape = BlockShape::S_SHAPE;
    block->angle = 0;
    int i = 0;
    for (int q = 0; q < TetrisBlock::nb_rects / 2; q++) {
        for (int r = 0; r < 2; r++) {
            block->rects[i].x = (col_pos[q + r] * GRID_SIZE) + 0.5f;
            block->rects[i].y = (r_pos[r] * GRID_SIZE) + 0.5f;
            block->rects[i].w = GRID_SIZE - 1;
            block->rects[i].h = GRID_SIZE - 1;
            i++;
        }
    }
}

void TetrisG::create_L_shape_block() {}

void TetrisG::create_J_shape_block() {}

void TetrisG::create_T_shape_block() {}

void TetrisG::fill_block_queue() {
    int shape = 0;
    while (blocks_qu.size() < 4) {
        shape = SDL_rand(7) + 1;
        switch (shape) {
        case BlockShape::I_SHAPE:
            create_I_shape_block();
            break;
        case BlockShape::O_SHAPE:
            create_O_shape_block();
            break;
        case BlockShape::S_SHAPE:
            create_S_shape_block();
            break;
        case BlockShape::Z_SHAPE:
            create_Z_shape_block();
            break;
        case BlockShape::L_SHAPE:
            create_L_shape_block();
            break;
        case BlockShape::J_SHAPE:
            create_J_shape_block();
            break;
        case BlockShape::T_SHAPE:
            create_T_shape_block();
            break;    
        }
    }
}