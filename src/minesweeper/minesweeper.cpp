#include "minesweeper/minesweeper.h"

MineSweeperG::~MineSweeperG() {
    cleanup();
    if (text_engine)
        TTF_DestroyRendererTextEngine(text_engine);
    if (font)
        TTF_CloseFont(font);

    text_engine = nullptr;
    font = nullptr;
}

bool MineSweeperG::init(SDL_Renderer *renderer) {
    if (!(font = TTF_OpenFont("C:\\Users\\Admin\\Desktop\\projects\\mini_games\\fonts\\wicked_mouse.ttf", 14))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to open font\nError: %s\n", SDL_GetError());
        return false;
    } else if (!(text_engine = TTF_CreateRendererTextEngine(renderer))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text engine\nError: %s\n", SDL_GetError());
        return false;
    }

    hover_btn = {-1, -1};
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < GRID_COLS; q++) {
            btns[r][q].rect = {(float)(q * GRID_SIZE) + 1, (float)(r * GRID_SIZE) + 1, (float)GRID_SIZE - 2, (float)GRID_SIZE - 2};
            btns[r][q].hover = false;
            btns[r][q].color = ColorRGB::MEDIUM_GRAY;
            btns[r][q].state = ButtonSt::ACTIVE;
            btns[r][q].text.text_obj = TTF_CreateText(text_engine, font, "", 0);
            btns[r][q].count = 0;
        }
    }
    create_random_boom();
    return true;
}

void MineSweeperG::create_random_boom() {
    int boomrate = 10 + SDL_rand(10);
    int nb_boom = GRID_ROWS * GRID_COLS * boomrate / 100;
    int r, q;
    while (nb_boom) {
        r = SDL_rand(GRID_ROWS);
        q = SDL_rand(GRID_COLS);
        if (btns[r][q].count < 0)
            continue;
        
        btns[r][q].count = -1;
        count_boom(r, q);
        nb_boom--;
    }
}

void MineSweeperG::count_boom(const int &boomr, const int& boomq) {
    for (int r = boomr - 1; r <= boomr + 1; r++) {
        if (r < 0 || r >= GRID_ROWS)
            continue;

        for (int q = boomq - 1; q <= boomq + 1; q++) {
            if (q < 0 || q >= GRID_COLS || (r == boomr && q == boomq) || btns[r][q].count < 0)
                continue;
            
            btns[r][q].count++;
        }
    }
}

void MineSweeperG::cleanup() {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < GRID_COLS; q++) {
            TTF_DestroyText(btns[r][q].text.text_obj);
            btns[r][q].text.text_obj = nullptr;
        }
    }
}

void MineSweeperG::update() {}

static void set_btn_display_text(Button &btn) {
    int text_w, text_h;
    btn.text.text = btn.count < 0 ? "X" : std::to_string(btn.count);
    TTF_SetTextString(btn.text.text_obj, btn.text.text.c_str(), btn.text.text.length());
    TTF_GetTextSize(btn.text.text_obj, &text_w, &text_h);
    btn.text.loc.x = btn.rect.x + ((btn.rect.w - text_w) / 2);
    btn.text.loc.y = btn.rect.y + ((btn.rect.h - text_h) / 2);
    if (btn.count < 0)
        TTF_SetTextColorFloat(btn.text.text_obj, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
    else 
        TTF_SetTextColorFloat(btn.text.text_obj, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
}

void MineSweeperG::render(SDL_Renderer *renderer) {
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int q = 0; q < GRID_COLS; q++) {
            SDL_SetRenderDrawColorFloat(renderer, btns[r][q].color.r, btns[r][q].color.g, btns[r][q].color.b, btns[r][q].color.a);
            SDL_RenderFillRect(renderer, &btns[r][q].rect);
            if (btns[r][q].text.text.length() == 0)
                set_btn_display_text(btns[r][q]);
            TTF_DrawRendererText(btns[r][q].text.text_obj, btns[r][q].text.loc.x, btns[r][q].text.loc.y);
        }
    }
}

void MineSweeperG::restart() {}

void MineSweeperG::on_hover(const int &mousex, const int &mousey) {
    int r, q;
    q = (mousex - 1) / GRID_SIZE;
    r = (mousey - 1) / GRID_SIZE;
    if (btns[r][q].state == ButtonSt::DISABLE) {
        if (hover_btn.x >= 0 && hover_btn.y >= 0) {
            btns[hover_btn.y][hover_btn.x].hover = false;
            btns[hover_btn.y][hover_btn.x].color = ColorRGB::MEDIUM_GRAY;
        }
        hover_btn = {-1, -1};
        return;
    } else if (!btns[r][q].hover) {
        btns[r][q].hover = true;
        btns[r][q].color = ColorRGB::LIGHT_GRAY;
        if (hover_btn.x >= 0 && hover_btn.y >= 0 && (hover_btn.x != q || hover_btn.y != r)) {
            btns[hover_btn.y][hover_btn.x].hover = false;
            btns[hover_btn.y][hover_btn.x].color = ColorRGB::MEDIUM_GRAY;
        }
        hover_btn = {q, r};
    }
}

void MineSweeperG::on_click() {
    btns[hover_btn.y][hover_btn.x].state = ButtonSt::DISABLE;
    btns[hover_btn.y][hover_btn.x].hover = false;
    btns[hover_btn.y][hover_btn.x].color = ColorRGB::DARK_GRAY;
    hover_btn = {-1, -1};
}

