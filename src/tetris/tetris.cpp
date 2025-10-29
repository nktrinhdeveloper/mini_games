#include "tetris.h"

TetrisG::TetrisG() : Game() {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < AREA_COLS; q++) {
            area[r][q] = 0;
        }
    }
    curr_tetro = nullptr;
    need_update_area = true;
}

void TetrisG::update() {
    if(tetros_qu.empty())
        create_T_shape_block();

    if (!curr_tetro) {
        curr_tetro = &tetros_qu.front();
        curr_tetro->horz_off = 0;
    }
}

void TetrisG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    SDL_FRect rect;
    if (curr_tetro) {
        rect.w = curr_tetro->width;
        rect.h = curr_tetro->height;
        for (int i = 0; i < Tetromino::nb_blocks; i++) {
            rect.x = curr_tetro->blocks[i].x;
            rect.y = curr_tetro->blocks[i].y;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
        
void TetrisG::restart() {}

void TetrisG::on_keydown(const SDL_Keycode &code) {
    if (!curr_tetro)
        return;

    switch (code){
    case SDLK_LEFT:
        curr_tetro->horz_off += -GRID_SIZE;
        break;
    case SDLK_RIGHT:
        curr_tetro->horz_off += GRID_SIZE;
        break;
    }
}

void TetrisG::create_I_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::I_SHAPE;
    int i = 0;
    for (int q = 0; q < 4; q++) {
            tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro->blocks[i].y = 1;
            i++;
    }
}

void TetrisG::create_O_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::O_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 2; q++) {
            tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro->blocks[i].y = (r * GRID_SIZE) + 1;
            i++;
        }
    }
}

void TetrisG::create_S_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::S_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q + r > 0 && q + r < 3) {
                tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }
}

void TetrisG::create_Z_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::Z_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q - r >= 0 && q - r < 2) {
                tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }    
}

void TetrisG::create_L_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::L_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 0 && q == 2) || r == 1 ) {
                tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }    
}

void TetrisG::create_J_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::J_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 2) || r == 0) {
                tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }    
}

void TetrisG::create_T_shape_block() {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->width = GRID_SIZE - 1;
    tetro->height = GRID_SIZE - 1;
    tetro->shape = TetroShape::T_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 1) || r == 0) {
                tetro->blocks[i].x = (((AREA_COLS / 2) - 2 + AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }
}

void TetrisG::fill_block_queue() {
    int shape = 0;
    while (tetros_qu.size() < 4) {
        shape = SDL_rand(7) + 1;
        switch (shape) {
        case TetroShape::I_SHAPE:
            create_I_shape_block();
            break;
        case TetroShape::O_SHAPE:
            create_O_shape_block();
            break;
        case TetroShape::S_SHAPE:
            create_S_shape_block();
            break;
        case TetroShape::Z_SHAPE:
            create_Z_shape_block();
            break;
        case TetroShape::L_SHAPE:
            create_L_shape_block();
            break;
        case TetroShape::J_SHAPE:
            create_J_shape_block();
            break;
        case TetroShape::T_SHAPE:
            create_T_shape_block();
            break;    
        }
    }
}