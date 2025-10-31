#include "tetris.h"

TetrisG::TetrisG() : Game() {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < AREA_COLS; q++) {
            area[r][q] = 0;
        }
    }
    curr_tetro = nullptr;
}

void TetrisG::update() {
    fill_block_queue();

    if (!curr_tetro) {
        curr_tetro = &tetros_qu.front();
        curr_tetro->horz_off = 0;
    }
    move_curr_tetro();
}

void TetrisG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    SDL_FRect rect;
    std::vector<SDL_FRect> rects;
    rect.w = GRID_SIZE - 1;
    rect.h = GRID_SIZE - 1;
    if (curr_tetro) {
        for (int i = 0; i < Tetromino::nb_blocks; i++) {
            rect.x = curr_tetro->blocks[i].x;
            rect.y = curr_tetro->blocks[i].y;
            rects.push_back(rect);
        }
    }

    for (int r = GRID_ROWS - 1; r >= 0; r--) {
        for (int q = 0; q < AREA_COLS; q++) {
            if (!area[r][q])
                continue;

            rect.x = ((q + AREA_COL_LOWERB) * GRID_SIZE) + 1;
            rect.y = (r * GRID_SIZE) + 1;
            rects.push_back(rect);
        }
    }

    SDL_RenderFillRects(renderer, rects.data(), rects.size());
}

static void tetro_move_horz(Tetromino *curr_tetro) {
    int k = curr_tetro->horz_off / SDL_fabsf(curr_tetro->horz_off);
    bool at_boundry = curr_tetro->blocks[0].x + (k * Tetromino::SPEED) < TetrisG::AREA_COL_LOWERB * GRID_SIZE ||
                    curr_tetro->blocks[Tetromino::nb_blocks - 1].x + (k * Tetromino::SPEED) > (TetrisG::AREA_COLS - 1) * GRID_SIZE;
                
    curr_tetro->horz_off = at_boundry || k * (curr_tetro->horz_off - (k * Tetromino::SPEED)) < 0 ? 0 : curr_tetro->horz_off - (k * Tetromino::SPEED);
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (at_boundry || curr_tetro->horz_off == 0) {
            int posx = SDL_roundf((curr_tetro->blocks[i].x - 1) / GRID_SIZE);
            curr_tetro->blocks[i].x = (posx * GRID_SIZE) + 1;
        } else {
            curr_tetro->blocks[i].x += k * Tetromino::SPEED;
        }
    }
}

static bool tetro_move_vert(Tetromino *curr_tetro, int (&area)[GRID_ROWS][TetrisG::AREA_COLS]) {
    bool add_to_area = false;
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        float row = (curr_tetro->blocks[i].y - 1) / GRID_SIZE;
        float extra = (float) (Tetromino::FALL_SPEED + GRID_SIZE - 1) / GRID_SIZE;
        float col = ((curr_tetro->blocks[i].x - 1) / GRID_SIZE) - TetrisG::AREA_COL_LOWERB;

        if (!add_to_area && (add_to_area = row + extra >= GRID_ROWS || area[(int) (row + extra)][(int) col])) {
            area[(int) row][(int) col] = 1;
            for (int rev_i = i - 1; rev_i >= 0; rev_i--) {
                row = (curr_tetro->blocks[rev_i].y - 1) / GRID_SIZE;
                col = ((curr_tetro->blocks[rev_i].x - 1) / GRID_SIZE) - TetrisG::AREA_COL_LOWERB;
                area[(int) row][(int) col] = 1;
            }
            continue;
        }

        if (add_to_area)
            area[(int) row][(int) col] = 1;
        else 
            curr_tetro->blocks[i].y += Tetromino::FALL_SPEED;
    }

    return add_to_area;
}

void TetrisG::move_curr_tetro() {
    if (!curr_tetro)
        return;

    if (curr_tetro->horz_off)
        tetro_move_horz(curr_tetro);
    
    if (tetro_move_vert(curr_tetro, area)) {
        curr_tetro = nullptr;
        tetros_qu.pop();
    }
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

static void create_I_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::I_SHAPE;
    int i = 0;
    for (int q = 0; q < 4; q++) {
            tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro->blocks[i].y = 1;
            i++;
    }
}

static void create_O_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::O_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 2; q++) {
            tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro->blocks[i].y = (r * GRID_SIZE) + 1;
            i++;
        }
    }
}

static void create_S_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::S_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q + r > 0 && q + r < 3) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }
}

static void create_Z_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::Z_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q - r >= 0 && q - r < 2) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }    
}

static void create_L_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::L_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 0 && q == 2) || r == 1 ) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }    
}

static void create_J_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::J_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 2) || r == 0) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }    
}

static void create_T_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::T_SHAPE;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 1) || r == 0) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                i++;
            }
        }
    }
}

void TetrisG::fill_block_queue() {
    while (tetros_qu.size() < 1) {
        int shape = 0;
        shape = SDL_rand(7) + 1;
        switch (shape) {
        case TetroShape::I_SHAPE:
            create_I_shape_block(tetros_qu);
            break;
        case TetroShape::O_SHAPE:
            create_O_shape_block(tetros_qu);
            break;
        case TetroShape::S_SHAPE:
            create_S_shape_block(tetros_qu);
            break;
        case TetroShape::Z_SHAPE:
            create_Z_shape_block(tetros_qu);
            break;
        case TetroShape::L_SHAPE:
            create_L_shape_block(tetros_qu);
            break;
        case TetroShape::J_SHAPE:
            create_J_shape_block(tetros_qu);
            break;
        case TetroShape::T_SHAPE:
            create_T_shape_block(tetros_qu);
            break;    
        }
    }
}