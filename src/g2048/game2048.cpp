#include "game2048.h"

namespace {
    void set_tile_text(MergeTileG::Tile &tile) {
        std::string val_txt = std::to_string(tile.val);
        int txt_w, txt_h;
        TTF_SetTextString(tile.ttf_txt, val_txt.c_str(), val_txt.length());
        TTF_SetTextColorFloat(tile.ttf_txt, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
        TTF_GetTextSize(tile.ttf_txt, &txt_w, &txt_h);
        tile.txt_loc.x = tile.loc.x + ((MergeTileG::TILE_SIZE - txt_w) / 2);
        tile.txt_loc.y = tile.loc.y + ((MergeTileG::TILE_SIZE - txt_h) / 2);
    }

    void prepare_vars_for_loop(const Vector2D<MergeTileG::Tile> &tiles,
                                const SDL_Point &direction,
                                int &begin_r, int &limit_r, int &kr,
                                int &begin_q, int &limit_q, int &kq) {
        if (direction.x) {
            begin_q = direction.x > 0 ? tiles[0].size() - 1 : 0;
            limit_q = direction.x > 0 ? 1 : tiles[0].size();
            kq = -direction.x; 
            kr = 1;
            begin_r = 0;
            limit_r = tiles.size();
        } else if (direction.y) {
            begin_r = direction.y > 0 ? tiles.size() - 1 : 0;
            limit_r = direction.y > 0 ? 1 : tiles.size(); 
            kr = -direction.y;        
            kq = 1; 
            begin_q = 0;
            limit_q = tiles[0].size();
        }
    }

    Tribool update_tile(Vector2D<MergeTileG::Tile> &tiles, const int &r, const int &q) {
        float update_r, update_q;
        update_q = (tiles[r][q].loc.x - MergeTileG::BORDER_THICK) / (MergeTileG::TILE_SIZE + MergeTileG::BORDER_THICK);
        update_r = (tiles[r][q].loc.y - MergeTileG::BORDER_THICK) / (MergeTileG::TILE_SIZE + MergeTileG::BORDER_THICK);
        if (!(SDL_abs(r - update_r) && SDL_fmodf(update_r, 1) < 0.1f) && !(SDL_abs(q - update_q) && SDL_fmodf(update_q, 1) < 0.1f)) {
            return Tribool::FAILED;
        }

        if (tiles[update_r][update_q].val) {
            tiles[update_r][update_q].val *= 2;
            tiles[update_r][update_q].movable = false;
            set_tile_text(tiles[update_r][update_q]);
            tiles[r][q].reset_tile();
            return Tribool::SUCCESSED;
        } else 
            tiles[update_r][update_q] = std::move(tiles[r][q]);

        return Tribool::NORMAL;
    }
}

MergeTileG::Tile::Tile() {
    loc = {0, 0};
    val = 0;
    ttf_txt = nullptr;
    txt_loc = {0, 0};
    movable = false;
}

MergeTileG::Tile::~Tile() {
    if (ttf_txt)
        TTF_DestroyText(ttf_txt);
}

MergeTileG::Tile::Tile(Tile &&other) noexcept {
    loc = std::move(other.loc);
    val = other.val;
    ttf_txt = other.ttf_txt;
    txt_loc = std::move(other.txt_loc);
    movable = false;

    other.loc = {0, 0};
    other.val = 0;
    other.ttf_txt = nullptr;
    other.txt_loc = {0, 0};
    other.movable = false;
}

MergeTileG::Tile &MergeTileG::Tile::operator=(Tile &&other) noexcept {
    if (this != &other) {
        loc = std::move(other.loc);
        val = other.val;
        ttf_txt = other.ttf_txt;
        txt_loc = std::move(other.txt_loc);
        movable = false;

        other.loc = {0, 0};
        other.val = 0;
        other.ttf_txt = nullptr;
        other.txt_loc = {0, 0};
        other.movable = false;
    }
    return *this;
}

void MergeTileG::Tile::reset_tile() {
    loc = {0, 0};
    val = 0;
    if (ttf_txt)
        TTF_DestroyText(ttf_txt);
    ttf_txt = nullptr;
    txt_loc = {0, 0};
    movable = false;
}

MergeTileG::~MergeTileG() {
    tiles.clear();
    if (text_engine)
        TTF_DestroyRendererTextEngine(text_engine);
    if (font)
        TTF_CloseFont(font);
}

bool MergeTileG::init(SDL_Renderer *renderer, const std::string &running_dir) {
    std::string font_dir = running_dir + "/" + WICKED_MOUSE_FONT;
    if (!(font = TTF_OpenFont(font_dir.c_str(), 28))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to open font\nError: %s\n", SDL_GetError());
        return false;
    } else if (!(text_engine = TTF_CreateRendererTextEngine(renderer))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to create text engine\nError: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_SetWindowSize(SDL_GetRenderWindow(renderer), MergeTileG::WIND_WIDTH, MergeTileG::WIND_HEIGHT);
    tiles = Vector2D<MergeTileG::Tile>(4, std::vector<MergeTileG::Tile>(4));
    for (int i = 0; i < 2; i++) {
        create_random_tile();
    }

    direction = {0, 0};
    need_add_tile = false;
    return true;
}

bool MergeTileG::create_random_tile() {
    int val = SDL_rand(10) == 0 ? 4 : 2;
    int r, q;
    bool valid_pos = false;
    int count = 0;
    while (!valid_pos) {
        if (count >= 16)
            return false;
        r = SDL_rand(4);
        q = SDL_rand(4);
        valid_pos = !tiles[r][q].val;
        count++;
    }

    tiles[r][q].loc.y = (float) ((r * (MergeTileG::TILE_SIZE + MergeTileG::BORDER_THICK)) + MergeTileG::BORDER_THICK);
    tiles[r][q].loc.x = (float) ((q * (MergeTileG::TILE_SIZE + MergeTileG::BORDER_THICK)) + MergeTileG::BORDER_THICK);
    tiles[r][q].val = val;
    tiles[r][q].ttf_txt = TTF_CreateText(text_engine, font, "", 0);
    set_tile_text(tiles[r][q]);
    return true;
}

void MergeTileG::update() {
    if (!alive)
        return;

    if ((direction.x || direction.y) && move_and_merge_tile() && need_add_tile) {
        alive = create_random_tile();
        need_add_tile = false;
    }
}

bool MergeTileG::move_and_merge_tile() {
    bool  moving = false;
    bool need_create = false;
    int limit_r, begin_r, limit_q, begin_q, kr, kq;
    prepare_vars_for_loop(tiles, direction, begin_r, limit_r, kr, begin_q, limit_q, kq);
    for (int r = begin_r; r * kr < limit_r; r += kr) {
        if (r + direction.y < 0 || r + direction.y >= tiles.size())
            continue;
        

        for (int q = begin_q; q * kq < limit_q; q += kq) {
            if (q + direction.x < 0 || q + direction.x >= tiles.size() || !tiles[r][q].val ||
                (tiles[r + direction.y][q + direction.x].val &&
                tiles[r][q].val != tiles[r + direction.y][q + direction.x].val &&
                !tiles[r + direction.y][q + direction.x].movable))
                continue;

            moving |= true;
            if (direction.x) {
                tiles[r][q].loc.x += direction.x * SPEED;
                tiles[r][q].txt_loc.x += direction.x * SPEED;
                tiles[r][q].movable = true;
            } else if (direction.y) {
                tiles[r][q].loc.y += direction.y * SPEED;
                tiles[r][q].txt_loc.y += direction.y * SPEED;
                tiles[r][q].movable = true;
            }

            update_tile(tiles, r, q);
            need_add_tile = true;
        }
    }

    if (!moving)
        direction = {0, 0};

    return !moving;
}

void MergeTileG::render(SDL_Renderer *renderer) {
    // SDL_SetRenderDrawColorFloat(renderer, 1, 1, 1, 1);
    // SDL_RenderClear(renderer);
    // std::vector<SDL_FRect> rects;
    SDL_FRect rect;
    rect.w = (float) (MergeTileG::TILE_SIZE);
    rect.h = (float) (MergeTileG::TILE_SIZE);
    // SDL_SetRenderDrawColorFloat(renderer, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
    // for (int r = 0; r < tiles.size(); r++) {
    //     if (r != 1)
    //         continue;
    //     for (int q = 0; q < tiles.size(); q++) {                
    //         rect.y = (float) ((r * (MergeTileG::TILE_SIZE + MergeTileG::BORDER_THICK)) + MergeTileG::BORDER_THICK);
    //         rect.x = (float) ((q * (MergeTileG::TILE_SIZE + MergeTileG::BORDER_THICK)) + MergeTileG::BORDER_THICK);
    //         rects.push_back(rect);
    //     }
    // }
    // SDL_RenderFillRects(renderer, rects.data(), rects.size());

    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::MEDIUM_GRAY.r, ColorRGB::MEDIUM_GRAY.g, ColorRGB::MEDIUM_GRAY.b, ColorRGB::MEDIUM_GRAY.a);
    for (int r = 0; r < tiles.size(); r++) {
        for (int q = 0; q < tiles.size(); q++) {
            if (!tiles[r][q].val)
                continue;
            rect.x = tiles[r][q].loc.x;
            rect.y = tiles[r][q].loc.y;
            SDL_RenderFillRect(renderer, &rect);
            TTF_DrawRendererText(tiles[r][q].ttf_txt, tiles[r][q].txt_loc.x, tiles[r][q].txt_loc.y);
        }
    }
}

void MergeTileG::restart() {}

void MergeTileG::on_keydown(const SDL_Keycode &code) {
    if (direction.x || direction.y)
        return;
    
    switch (code) {
    case SDLK_UP:
        direction = {0, -1};
        break;
    case SDLK_DOWN:
        direction = {0, 1};
        break;
    case SDLK_LEFT:
        direction = {-1, 0};
        break;
    case SDLK_RIGHT:
        direction = {1, 0};
        break;
    default:
        direction = {0, 0};
        return;
    }
}
