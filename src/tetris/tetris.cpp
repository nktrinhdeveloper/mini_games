#include "tetris.h"

static void set_tetro_default(Tetromino &tetro) {
    for (int i = 0; i < Tetromino::nb_blocks; i++)
        tetro.blocks[i] = {0, 0};
    
    tetro.shape = TetrisG::TetroShape::NONE_SHAPE;
    tetro.horz_off = 0;
    tetro.anchor_pos = 0;
    tetro.color = ColorRGB::BLACK;
    tetro.minx_pos = 0;
    tetro.maxx_pos = 0;
}

static bool correct_horz_position_after_rotate(Tetromino &tetro,  const int (&area)[GRID_ROWS][TetrisG::AREA_COLS]) {
    int col, row;
    bool add;

    if (tetro.blocks[tetro.minx_pos].x <= TetrisG::AREA_COL_LOWERB * GRID_SIZE)
        add = true;
    else if (tetro.blocks[tetro.maxx_pos].x >= (TetrisG::AREA_COLS + TetrisG::AREA_COL_LOWERB) * GRID_SIZE)
        add = false;
    else 
        return true;

    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (add)
            tetro.blocks[i].x += GRID_SIZE;
        else
            tetro.blocks[i].x -= GRID_SIZE;

        col = (tetro.blocks[i].x - 1) / GRID_SIZE - TetrisG::AREA_COL_LOWERB;
        row = (tetro.blocks[i].y - 1) / GRID_SIZE;
        if (area[row][col]) {
            return false;
        }
    }

    return correct_horz_position_after_rotate(tetro, area);
}

static void check_minmax_horz(Tetromino &tetro, const int &idx, float &minx, float &maxx) {
    if (tetro.blocks[idx].x < minx) {
        minx = tetro.blocks[idx].x;
        tetro.minx_pos = idx;
    }

    if (tetro.blocks[idx].x > maxx) {
        maxx = tetro.blocks[idx].x;
        tetro.maxx_pos = idx;
    }
}

static void create_I_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::I_SHAPE;
    tetro.anchor_pos = 1;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int q = 0; q < 4; q++) {
            tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro.blocks[i].y = 1;
            check_minmax_horz(tetro, i, minx, maxx);
            i++;
    }
}

static void create_O_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::O_SHAPE;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 2; q++) {
            tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
            tetro.blocks[i].y = (r * GRID_SIZE) + 1;
            check_minmax_horz(tetro, i, minx, maxx);
            i++;
        }
    }
}

static void create_S_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::S_SHAPE;
    tetro.anchor_pos = 3;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q + r > 0 && q + r < 3) {
                tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro.blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }
}

static void create_Z_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::Z_SHAPE;
    tetro.anchor_pos = 2;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if (q - r >= 0 && q - r < 2) {
                tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro.blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }    
}

static void create_L_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::L_SHAPE;
    tetro.anchor_pos = 3;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 0 && q == 2) || r == 1 ) {
                tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro.blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }    
}

static void create_J_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::J_SHAPE;
    tetro.anchor_pos = 2;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 2) || r == 0) {
                tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro.blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }    
}

static void create_T_shape_block(Tetromino &tetro) {
    tetro.shape = TetrisG::TetroShape::T_SHAPE;
    tetro.anchor_pos = 1;
    float minx, maxx;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    int i = 0;
    for (int r = 0; r < 2; r++) {
        for (int q = 0; q < 3; q++) {
            if ((r == 1 && q == 1) || r == 0) {
                tetro.blocks[i].x = (((TetrisG::AREA_COLS / 2) - 2 + TetrisG::AREA_COL_LOWERB + q) * GRID_SIZE) + 1;
                tetro.blocks[i].y = (r * GRID_SIZE) + 1;
                check_minmax_horz(tetro, i, minx, maxx);
                i++;
            }
        }
    }
}

static void tetro_move_horz(Tetromino &curr_tetro, const int (&area)[GRID_ROWS][TetrisG::AREA_COLS]) {
    int k = curr_tetro.horz_off / SDL_fabsf(curr_tetro.horz_off);
    bool at_boundry = curr_tetro.blocks[curr_tetro.minx_pos].x + (k * Tetromino::SPEED) < TetrisG::AREA_COL_LOWERB * GRID_SIZE ||
                    curr_tetro.blocks[curr_tetro.maxx_pos].x + (k * Tetromino::SPEED) > TetrisG::AREA_COLS * GRID_SIZE;
                
    curr_tetro.horz_off = at_boundry || k * (curr_tetro.horz_off - (k * Tetromino::SPEED)) < 0 ? 0 : curr_tetro.horz_off - (k * Tetromino::SPEED);
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        int col, row;
        col = SDL_roundf((curr_tetro.blocks[i].x - 1) / GRID_SIZE);
        row = (curr_tetro.blocks[i].y - 1) / GRID_SIZE;
        if (curr_tetro.horz_off && (area[row][col - TetrisG::AREA_COL_LOWERB + k] || area[row + 1][col - TetrisG::AREA_COL_LOWERB + k])) {
            for (int rev_i = i - 1; rev_i >= 0; rev_i--) {
                curr_tetro.blocks[rev_i].x -= k * Tetromino::SPEED;
                int rev_vol = SDL_roundf((curr_tetro.blocks[rev_i].x - 1) / GRID_SIZE);
                int rev_row = (curr_tetro.blocks[rev_i].y - 1) / GRID_SIZE;
                curr_tetro.blocks[rev_i].x = (rev_vol * GRID_SIZE) + 1;
            }
        
            curr_tetro.horz_off = 0;
        }

        if (at_boundry || !curr_tetro.horz_off) {
            curr_tetro.blocks[i].x = (col * GRID_SIZE) + 1;
        } else {
            curr_tetro.blocks[i].x += k * Tetromino::SPEED;
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

static void drop_content(int (&area)[GRID_ROWS][TetrisG::AREA_COLS], int &top_high, const int &count, const int &drop_start) {
    if (count == 0) {
        return;
    }

    for (int r = drop_start - 1; r >= top_high; r--) {
        for (int q = 0; q < TetrisG::AREA_COLS; q++) {
            if (!area[r][q])
                continue;
            
            area[r + count][q] = 1;
            area[r][q] = 0;            
        }
    }
    top_high += count;
}

static void drop_area_content_one_line(int (&area)[GRID_ROWS][TetrisG::AREA_COLS], int &top_high, int *full_rows) {
    int drop_start = -1;
    int count = 0;
    int contiguous = -1;
    std::sort(full_rows, full_rows + Tetromino::nb_blocks);
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (drop_start < 0) {
            drop_start = full_rows[i];
            contiguous = drop_start;
            continue;
        }

        count += 1;
        if (contiguous + 1 == full_rows[i]){
            contiguous += 1;
        } else {
            drop_content(area, top_high, count, drop_start);
            count = 0;
            drop_start = full_rows[i];
            contiguous = drop_start;
        }
    }
    if (drop_start > 0)
        count += 1;
    drop_content(area, top_high, count, drop_start);
}

static bool tetro_move_vert(Tetromino &curr_tetro, int (&area)[GRID_ROWS][TetrisG::AREA_COLS], int &top_high, const bool &commit) {
    bool add_to_area = false;
    int full_row[Tetromino::nb_blocks] = {-1, -1, -1, -1};
    int count_full_line = 0;
    float fall_speed = commit ? Tetromino::FALL_SPEED * 20 : Tetromino::FALL_SPEED;
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        float row = commit ? SDL_roundf((curr_tetro.blocks[i].y - 1) / GRID_SIZE) : (curr_tetro.blocks[i].y - 1) / GRID_SIZE;
        float extra = (float) (fall_speed + GRID_SIZE - 1) / GRID_SIZE;
        float col = ((curr_tetro.blocks[i].x - 1) / GRID_SIZE) - TetrisG::AREA_COL_LOWERB;

        if (!add_to_area && (add_to_area = row + extra >= GRID_ROWS || area[(int) (row + extra)][(int) col])) {
            for (int rev_i = i; rev_i >= 0; rev_i--) {
                int rev_row = commit ? SDL_roundf((curr_tetro.blocks[rev_i].y - 1) / GRID_SIZE) : (curr_tetro.blocks[rev_i].y - 1) / GRID_SIZE;
                int rev_col = ((curr_tetro.blocks[rev_i].x - 1) / GRID_SIZE) - TetrisG::AREA_COL_LOWERB;
                
                area[rev_row][rev_col] = 1;
                if (top_high > rev_row)
                    top_high = rev_row;

                if (check_and_remove_full_row(area, rev_row)) {
                    full_row[rev_i] = rev_row;
                }
            }
            continue;
        }

        if (add_to_area){
            area[(int) row][(int) col] = 1;
            if (top_high > row)
                top_high = row;
            
            if (check_and_remove_full_row(area, row)) {
                full_row[i] = row;
            }
            
        } else 
            curr_tetro.blocks[i].y += fall_speed;
    }

    drop_area_content_one_line(area, top_high, full_row);
    return add_to_area;
}

static void rotate_tetro(Tetromino &tetro, const bool &clockwise, const int (&area)[GRID_ROWS][TetrisG::AREA_COLS]) {
    if (tetro.shape == TetrisG::TetroShape::O_SHAPE)
        return;

    float minx, maxx, distanceX, distanceY;
    Tetromino temp = tetro;
    int row, col, k;
    minx = SDL_MAX_SINT16;
    maxx = 0;
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        if (i == temp.anchor_pos) {
            check_minmax_horz(temp, i, minx, maxx);
            continue;
        }
        
        distanceX = temp.blocks[i].x - temp.blocks[temp.anchor_pos].x;
        distanceY = temp.blocks[i].y - temp.blocks[temp.anchor_pos].y;
        k = clockwise ? 1 : -1;
        temp.blocks[i].x = temp.blocks[temp.anchor_pos].x - (distanceY * k);
        temp.blocks[i].y = temp.blocks[temp.anchor_pos].y + (distanceX * k);
        col = (temp.blocks[i].x - 1) / GRID_SIZE - TetrisG::AREA_COL_LOWERB;
        row = (temp.blocks[i].y - 1) / GRID_SIZE;
        if (area[row][col])
            return;

        check_minmax_horz(temp, i, minx, maxx);
    }

    if (correct_horz_position_after_rotate(temp, area))
        tetro = temp;
}

Tetromino::Tetromino(Tetromino &&other) noexcept {
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        this->blocks[i] = other.blocks[i];
        other.blocks[i] = {0, 0};
    }

    this->shape = other.shape;
    this->horz_off = other.horz_off;
    this->anchor_pos = other.anchor_pos;
    this->color = other.color;
    this->minx_pos = other.minx_pos;
    this->maxx_pos = other.maxx_pos;

    other.shape = TetrisG::TetroShape::NONE_SHAPE;
    other.horz_off = 0;
    other.anchor_pos = 0;
    other.color = ColorRGB::BLACK;
    other.minx_pos = 0;
    other.maxx_pos = 0;    
}   

Tetromino &Tetromino::operator=(Tetromino &&other) noexcept {
    if (this != &other) {
        for (int i = 0; i < Tetromino::nb_blocks; i++) {
            this->blocks[i] = other.blocks[i];
            other.blocks[i] = {0, 0};
        }

        this->shape = other.shape;
        this->horz_off = other.horz_off;
        this->anchor_pos = other.anchor_pos;
        this->color = other.color;
        this->minx_pos = other.minx_pos;
        this->maxx_pos = other.maxx_pos;

        other.shape = TetrisG::TetroShape::NONE_SHAPE;
        other.horz_off = 0;
        other.anchor_pos = 0;
        other.color = ColorRGB::BLACK;
        other.minx_pos = 0;
        other.maxx_pos = 0;    
    }
    return *this;
}

TetrisG::TetrisG() : Game() {
    top_high = GRID_ROWS - 10;
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < AREA_COLS; q++) {
            if (r >= top_high && (q < 5 || q > 10)) 
                area[r][q] = 1;
            else
                area[r][q] = 0;
        }
    }
    set_tetro_default(curr_tetro);
    set_tetro_default(next_tetro);
    commit_drop = false;
}

void TetrisG::update() {
    create_random_tetro();
    move_curr_tetro();
}

static void add_boundry_cols(SDL_Renderer *renderer) {
    SDL_FRect rects[2];
    rects[0].x = 1;
    rects[0].y = 0;
    rects[0].w = GRID_SIZE - 1;
    rects[0].h = GRID_ROWS * GRID_ROWS + GRID_SIZE;
    rects[1].x = (TetrisG::AREA_COLS + TetrisG::AREA_COL_LOWERB) * GRID_SIZE + 1;
    rects[1].y = 0;
    rects[1].w = GRID_SIZE - 1;
    rects[1].h = GRID_ROWS * GRID_ROWS + GRID_SIZE;
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::MEDIUM_GRAY.r, ColorRGB::MEDIUM_GRAY.g, ColorRGB::MEDIUM_GRAY.b, ColorRGB::MEDIUM_GRAY.a);
    SDL_RenderFillRects(renderer, rects, 2);
}

void TetrisG::render(SDL_Renderer *renderer) {
    SDL_FRect rect;
    std::vector<SDL_FRect> rects;
    std::vector<SDL_FRect> tetro_rects;
    rect.w = GRID_SIZE - 1;
    rect.h = GRID_SIZE - 1;
    add_boundry_cols(renderer);
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        rect.x = curr_tetro.blocks[i].x;
        rect.y = curr_tetro.blocks[i].y;
        tetro_rects.push_back(rect);
    }

    float offsetx, offsety;
    offsetx = (TetrisG::AREA_COLS + TetrisG::AREA_COL_LOWERB + 2) * GRID_SIZE - next_tetro.blocks[next_tetro.minx_pos].x + 1;
    offsety = 2 * GRID_SIZE - next_tetro.blocks[next_tetro.minx_pos].y + 1;
    for (int i = 0; i < Tetromino::nb_blocks; i++) {
        rect.x = next_tetro.blocks[i].x + offsetx;
        rect.y = next_tetro.blocks[i].y + offsety;
        tetro_rects.push_back(rect);
    }

    for (int r = GRID_ROWS - 1; r >= top_high; r--) {
        for (int q = 0; q < AREA_COLS; q++) {
            if (!area[r][q])
                continue;

            rect.x = ((q + AREA_COL_LOWERB) * GRID_SIZE) + 1;
            rect.y = (r * GRID_SIZE) + 1;
            rects.push_back(rect);
        }
    }

    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::LIGHT_GRAY.r, ColorRGB::LIGHT_GRAY.g, ColorRGB::LIGHT_GRAY.b, ColorRGB::LIGHT_GRAY.a);
    SDL_RenderFillRects(renderer, tetro_rects.data(), tetro_rects.size());

    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::DARK_GRAY.r, ColorRGB::DARK_GRAY.g, ColorRGB::DARK_GRAY.b, ColorRGB::DARK_GRAY.a);
    SDL_RenderFillRects(renderer, rects.data(), rects.size());
}

void TetrisG::move_curr_tetro() {
    if (curr_tetro.horz_off && !commit_drop)
        tetro_move_horz(curr_tetro, area);
    
    if (tetro_move_vert(curr_tetro, area, top_high, commit_drop)) {
        curr_tetro = std::move(next_tetro);
        commit_drop = false;
    }
}

void TetrisG::restart() {
    for (int r = GRID_ROWS - 1; r >= top_high; r--) {
        for (int q = 0; q < AREA_COLS; q++) {
            area[r][q] = 0;
        }
    }
    set_tetro_default(curr_tetro);
    set_tetro_default(next_tetro);
}

void TetrisG::on_keydown(const SDL_Keycode &code) {
    switch (code){
    case SDLK_LEFT:
        if (!curr_tetro.horz_off)
            curr_tetro.horz_off = -GRID_SIZE;
        break;
    case SDLK_RIGHT:
        if (!curr_tetro.horz_off)
            curr_tetro.horz_off = GRID_SIZE;
        break;
    case SDLK_UP:
        if (!commit_drop)
            rotate_tetro(curr_tetro, false, area);

        break;
    case SDLK_DOWN:
        if (!commit_drop)
            rotate_tetro(curr_tetro, true, area);
    
        break;
    case SDLK_SPACE:
        commit_drop = true;
        break;
    }
}

void TetrisG::create_random_tetro() {
    if (next_tetro.shape)
        return;

    next_tetro.shape = SDL_rand(7) + 1;
    switch (next_tetro.shape) {
    case TetroShape::I_SHAPE:
        create_I_shape_block(next_tetro);
        break;
    case TetroShape::O_SHAPE:
        create_O_shape_block(next_tetro);
        break;
    case TetroShape::S_SHAPE:
        create_S_shape_block(next_tetro);
        break;
    case TetroShape::Z_SHAPE:
        create_Z_shape_block(next_tetro);
        break;
    case TetroShape::L_SHAPE:
        create_L_shape_block(next_tetro);
        break;
    case TetroShape::J_SHAPE:
        create_J_shape_block(next_tetro);
        break;
    case TetroShape::T_SHAPE:
        create_T_shape_block(next_tetro);
        break;    
    }

    if (!curr_tetro.shape)
        curr_tetro = std::move(next_tetro);
}