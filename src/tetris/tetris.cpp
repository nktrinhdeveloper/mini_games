#include "tetris.h"

static void correct_horz_position_after_rotate(Tetromino *tetro, const bool &add) {
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (add)
            tetro->blocks[i].x += GRID_SIZE;
        else
            tetro->blocks[i].x -= GRID_SIZE;
    }

    if (tetro->blocks[tetro->minx_pos].x <= TetrisG::AREA_COL_LOWERB * GRID_SIZE)
        correct_horz_position_after_rotate(tetro, true);
    else if (tetro->blocks[tetro->maxx_pos].x >= (TetrisG::AREA_COLS + TetrisG::AREA_COL_LOWERB) * GRID_SIZE)
        correct_horz_position_after_rotate(tetro, false);

}

static void check_minmax_horz(Tetromino *tetro, const int &idx, float &minx, float &maxx) {
    if (tetro->blocks[idx].x < minx) {
        minx = tetro->blocks[idx].x;
        tetro->minx_pos = idx;
    }

    if (tetro->blocks[idx].x > maxx) {
        maxx = tetro->blocks[idx].x;
        tetro->maxx_pos = idx;
    }    
}

static void create_I_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::I_SHAPE;
    tetro->anchor_pos = 1;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int q = 0; q < 4; q++) {
            tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro->blocks[i].y = 1;
            check_minmax_horz(tetro, i, minx, maxx);
            i++;
    }
}

static void create_O_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::O_SHAPE;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 2; q++) {
            tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro->blocks[i].y = (r * GRID_SIZE) + 1;
            check_minmax_horz(tetro, i, minx, maxx);
            i++;
        }
    }
}

static void create_S_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::S_SHAPE;
    tetro->anchor_pos = 3;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q + r > 0 && q + r < 3) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }
}

static void create_Z_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::Z_SHAPE;
    tetro->anchor_pos = 2;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q - r >= 0 && q - r < 2) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }    
}

static void create_L_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::L_SHAPE;
    tetro->anchor_pos = 3;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 0 && q == 2) || r == 1 ) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }    
}

static void create_J_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::J_SHAPE;
    tetro->anchor_pos = 2;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 2) || r == 0) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }    
}

static void create_T_shape_block(std::queue<Tetromino> &tetros_qu) {
    Tetromino *tetro = &tetros_qu.emplace();
    tetro->shape = TetrisG::TetroShape::T_SHAPE;
    tetro->anchor_pos = 1;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 1) || r == 0) {
                tetro->blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro->blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }
}

static void tetro_move_horz(Tetromino *curr_tetro) {
    int k = curr_tetro->horz_off / SDL_fabsf(curr_tetro->horz_off);
    bool at_boundry = curr_tetro->blocks[curr_tetro->minx_pos].x + (k * Tetromino::SPEED) < TetrisG::AREA_COL_LOWERB * GRID_SIZE ||
                    curr_tetro->blocks[curr_tetro->maxx_pos].x + (k * Tetromino::SPEED) > TetrisG::AREA_COLS * GRID_SIZE;
                
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

static bool check_and_remove_full_row(int (&area)[GRID_ROWS][TetrisG::AREA_COLS], const int &row) {
    for (int q = 0; q < TetrisG::AREA_COLS; q++) {
        if (!area[row][q])
            return false;
    }

    for (int q = 0; q < TetrisG::AREA_COLS; q++) {
        area[row][q] = 0;
    }

    return true;
}

static bool tetro_move_vert(Tetromino *curr_tetro, int (&area)[GRID_ROWS][TetrisG::AREA_COLS], int &top_high) {
    bool add_to_area = false;
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        float row = (curr_tetro->blocks[i].y - 1) / GRID_SIZE;
        float extra = (float) (Tetromino::FALL_SPEED + GRID_SIZE - 1) / GRID_SIZE;
        float col = ((curr_tetro->blocks[i].x - 1) / GRID_SIZE) - TetrisG::AREA_COL_LOWERB;

        if (!add_to_area && (add_to_area = row + extra >= GRID_ROWS || area[(int) (row + extra)][(int) col])) {
            for (int rev_i = i - 1; rev_i >= 0; rev_i--) {
                int rev_row = (curr_tetro->blocks[rev_i].y - 1) / GRID_SIZE;
                int rev_col = ((curr_tetro->blocks[rev_i].x - 1) / GRID_SIZE) - TetrisG::AREA_COL_LOWERB;
                if (check_and_remove_full_row(area, rev_row)) {
                    top_high = top_high + 1 >= GRID_ROWS ? GRID_ROWS - 1 : top_high + 1;
                    continue;
                }

                area[rev_row][rev_col] = 1;
                printf("row: %d, col: %d\n", rev_row, rev_col);
                if (top_high > rev_row)
                    top_high = rev_row;
            }
        }

        if (add_to_area){
            if (check_and_remove_full_row(area, row)) {
                top_high = top_high + 1 >= GRID_ROWS ? GRID_ROWS - 1 : top_high + 1;
                continue;
            }
            printf("row: %d, col: %d\n", (int) row, (int) col);
            area[(int) row][(int) col] = 1;
            if (top_high > row)
                top_high = row;
        } else 
            curr_tetro->blocks[i].y += Tetromino::FALL_SPEED;
    }

    if (add_to_area)
        printf("-------------------\n");
    return add_to_area;
}

static void rotate_tetro(Tetromino *tetro, const bool &clockwise) {
    if (tetro->shape == TetrisG::TetroShape::O_SHAPE)
        return;

    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (i == tetro->anchor_pos) {
            check_minmax_horz(tetro, i, minx, maxx);
            continue;
        }
        
        float distanceX = tetro->blocks[i].x - tetro->blocks[tetro->anchor_pos].x;
        float distanceY = tetro->blocks[i].y - tetro->blocks[tetro->anchor_pos].y;
        int k = clockwise ? 1 : -1;
        tetro->blocks[i].x = tetro->blocks[tetro->anchor_pos].x - (distanceY * k);
        tetro->blocks[i].y = tetro->blocks[tetro->anchor_pos].y + (distanceX * k);
        check_minmax_horz(tetro, i, minx, maxx);
    }

    if (tetro->blocks[tetro->minx_pos].x <= TetrisG::AREA_COL_LOWERB * GRID_SIZE)
        correct_horz_position_after_rotate(tetro, true);
    else if (tetro->blocks[tetro->maxx_pos].x >= (TetrisG::AREA_COLS + TetrisG::AREA_COL_LOWERB) * GRID_SIZE)
        correct_horz_position_after_rotate(tetro, false);

}

TetrisG::TetrisG() : Game() {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < AREA_COLS; q++) {
            area[r][q] = 0;
        }
    }
    top_high = GRID_ROWS - 1;
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

static void add_boundry_cols(std::vector<SDL_FRect> &rects) {
    SDL_FRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = GRID_SIZE;
    rect.h = GRID_ROWS * GRID_ROWS;
    rects.push_back(rect);
    rect.x = (TetrisG::AREA_COLS + TetrisG::AREA_COL_LOWERB) * GRID_SIZE;
    rects.push_back(rect);    
}

void TetrisG::render(SDL_Renderer *renderer) {
    SDL_FRect rect;
    std::vector<SDL_FRect> rects;
    rect.w = GRID_SIZE - 1;
    rect.h = GRID_SIZE - 1;
    add_boundry_cols(rects);
    if (curr_tetro) {
        for (int i = 0; i < Tetromino::nb_blocks; i++) {
            if (i == curr_tetro->minx_pos) {
                SDL_SetRenderDrawColorFloat(renderer, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
                rect.x = curr_tetro->blocks[i].x;
                rect.y = curr_tetro->blocks[i].y;
                SDL_RenderFillRect(renderer, &rect);
                continue;
            } else if (i == curr_tetro->maxx_pos) {
                SDL_SetRenderDrawColorFloat(renderer, ColorRGB::BLUE.r, ColorRGB::BLUE.g, ColorRGB::BLUE.b, ColorRGB::BLUE.a);
                rect.x = curr_tetro->blocks[i].x;
                rect.y = curr_tetro->blocks[i].y;
                SDL_RenderFillRect(renderer, &rect);
                continue;
            } else if (i == curr_tetro->anchor_pos) {
                SDL_SetRenderDrawColorFloat(renderer, ColorRGB::WHITE.r, ColorRGB::WHITE.g, ColorRGB::WHITE.b, ColorRGB::WHITE.a);
                rect.x = curr_tetro->blocks[i].x;
                rect.y = curr_tetro->blocks[i].y;
                SDL_RenderFillRect(renderer, &rect);
                continue;
            }
            rect.x = curr_tetro->blocks[i].x;
            rect.y = curr_tetro->blocks[i].y;
            rects.push_back(rect);
        }
    }

    for (int r = GRID_ROWS - 1; r >= 0; r--) {
        if (r < top_high)
            break;

        for (int q = 0; q < AREA_COLS; q++) {
            if (!area[r][q])
                continue;

            rect.x = ((q + AREA_COL_LOWERB) * GRID_SIZE) + 1;
            rect.y = (r * GRID_SIZE) + 1;
            rects.push_back(rect);
        }
    }

    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    SDL_RenderFillRects(renderer, rects.data(), rects.size());
}

void TetrisG::move_curr_tetro() {
    if (!curr_tetro)
        return;

    if (curr_tetro->horz_off)
        tetro_move_horz(curr_tetro);
    
    if (tetro_move_vert(curr_tetro, area, top_high)) {
        curr_tetro = nullptr;
        tetros_qu.pop();
    }
}

void TetrisG::restart() {}

void TetrisG::on_keydown(const SDL_Keycode &code) {
    if (!curr_tetro)
        return;

    switch (code){
    case SDLK_LEFT:
        if (!curr_tetro->horz_off)
            curr_tetro->horz_off = -GRID_SIZE;
        break;
    case SDLK_RIGHT:
        if (!curr_tetro->horz_off)
            curr_tetro->horz_off = GRID_SIZE;
        break;
    case SDLK_UP:
        rotate_tetro(curr_tetro, false);
        break;
    case SDLK_DOWN:
        rotate_tetro(curr_tetro, true);
        break;
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