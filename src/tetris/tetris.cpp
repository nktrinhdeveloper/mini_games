#include "tetris.h"

TetrisG::TetrisG() : Game() {
    // for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < AREA_COLS; q++) {
            area[GRID_ROWS - 1][q] = 1;
        }
    // }
    curr_tetro = nullptr;
}

void TetrisG::update() {
    if(tetros_qu.empty())
        create_T_shape_block();

    if (!curr_tetro) {
        curr_tetro = &tetros_qu.front();
        curr_tetro->horz_off = 0;
    }
    move_curr_tetro();
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

    for (int q = 0; q < TetrisG::AREA_COLS; q++) {
        rect.w = GRID_SIZE;
        rect.h = GRID_SIZE;
        rect.x = (q + TetrisG::AREA_COL_LOWERB) * GRID_SIZE;
        rect.y = 3 * GRID_SIZE;
        SDL_RenderFillRect(renderer, &rect);
    }
}

static void tetro_move_horz(Tetromino *curr_tetro) {
    int k = curr_tetro->horz_off / SDL_fabsf(curr_tetro->horz_off);
    bool at_boundry = curr_tetro->blocks[0].x + (k * TetrisG::SPEED) < TetrisG::AREA_COL_LOWERB * GRID_SIZE ||
                    curr_tetro->blocks[Tetromino::nb_blocks - 1].x + (k * TetrisG::SPEED) > (TetrisG::AREA_COLS - 1) * GRID_SIZE;
                
    curr_tetro->horz_off = at_boundry || k * (curr_tetro->horz_off - (k * TetrisG::SPEED)) < 0 ? 0 : curr_tetro->horz_off - (k * TetrisG::SPEED);
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (at_boundry || curr_tetro->horz_off == 0) {
            int posx = SDL_roundf((curr_tetro->blocks[i].x - 1) / GRID_SIZE);
            curr_tetro->blocks[i].x = (posx * GRID_SIZE) + 1;
        } else {
            curr_tetro->blocks[i].x += k * TetrisG::SPEED;
        }
    }
}

void TetrisG::move_curr_tetro() {
    if (!curr_tetro || curr_tetro->horz_off == 0)
        return;

    if (curr_tetro->horz_off)
        tetro_move_horz(curr_tetro);
}


void TetrisG::restart() {}

void TetrisG::on_keydown(const SDL_Keycode &code) {
    if (!curr_tetro || curr_tetro->horz_off)
        return;

    switch (code){
    case SDLK_LEFT:
        curr_tetro->horz_off = -GRID_SIZE;
        break;
    case SDLK_RIGHT:
        curr_tetro->horz_off = GRID_SIZE;
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