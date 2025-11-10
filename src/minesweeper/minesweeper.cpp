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

bool MineSweeperG::init(SDL_Renderer *renderer, const std::string &app_dir_str) {
    std::filesystem::path font_dir = std::filesystem::path(app_dir_str) / AppConst::WICKED_MOUSE_FONT;
    if (!(font = TTF_OpenFont(font_dir.string().c_str(), 14))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to open font\nError: %s\n", SDL_GetError());
        return false;
    } else if (!(text_engine = TTF_CreateRendererTextEngine(renderer))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text engine\nError: %s\n", SDL_GetError());
        return false;
    }

    hover_btn = {-1, -1};
    nb_safe = 0;
    for (int r = 0; r < AppConst::GRID_ROWS; r++) {
        for (int q = 0; q < AppConst::GRID_COLS; q++) {
            btns[r][q].rect = {(float)(q * AppConst::GRID_SIZE) + 1, (float)(r * AppConst::GRID_SIZE) + 1, (float)AppConst::GRID_SIZE - 2, (float)AppConst::GRID_SIZE - 2};
            btns[r][q].hover = false;
            btns[r][q].color = ColorRGB::MEDIUM_GRAY;
            btns[r][q].state = ButtonSt::ACTIVE;
            btns[r][q].text.text_obj = TTF_CreateText(text_engine, font, "", 0);
            btns[r][q].text.text = "";
            btns[r][q].count = 0;
        }
    }
    create_random_boom();
    return true;
}

void MineSweeperG::create_random_boom() {
    int boomrate = 10 + SDL_rand(10);
    int total_btns = AppConst::GRID_ROWS * AppConst::GRID_COLS;
    int nb_boom = total_btns * boomrate / 100;
    nb_safe = total_btns - nb_boom;
    int r, q;
    while (nb_boom) {
        r = SDL_rand(AppConst::GRID_ROWS);
        q = SDL_rand(AppConst::GRID_COLS);
        if (btns[r][q].count < 0)
            continue;
        
        btns[r][q].count = -1;
        count_boom(r, q);
        nb_boom--;
    }
}

void MineSweeperG::count_boom(const int &boomr, const int& boomq) {
    for (int r = boomr - 1; r <= boomr + 1; r++) {
        if (r < 0 || r >= AppConst::GRID_ROWS)
            continue;

        for (int q = boomq - 1; q <= boomq + 1; q++) {
            if (q < 0 || q >= AppConst::GRID_COLS || (r == boomr && q == boomq) || btns[r][q].count < 0)
                continue;
            
            btns[r][q].count++;
        }
    }
}

void MineSweeperG::cleanup() {
    for (int r = 0; r < AppConst::GRID_ROWS; r++) {
        for (int q = 0; q < AppConst::GRID_COLS; q++) {
            TTF_DestroyText(btns[r][q].text.text_obj);
            btns[r][q].text.text_obj = nullptr;
        }
    }
}

void MineSweeperG::update() {}

void MineSweeperG::render(SDL_Renderer *renderer) {
    for (int r = 0; r < AppConst::GRID_ROWS; r++) {
        for (int q = 0; q < AppConst::GRID_COLS; q++) {
            SDL_SetRenderDrawColorFloat(renderer, btns[r][q].color.r, btns[r][q].color.g, btns[r][q].color.b, btns[r][q].color.a);
            SDL_RenderFillRect(renderer, &btns[r][q].rect);
            if (!btns[r][q].text.text.empty())
                TTF_DrawRendererText(btns[r][q].text.text_obj, btns[r][q].text.loc.x, btns[r][q].text.loc.y);
        }
    }
}

void MineSweeperG::restart() {
    for (int r = 0; r < AppConst::GRID_ROWS; r++) {
        for (int q = 0; q < AppConst::GRID_COLS; q++) { 
            btns[r][q].hover = false;
            btns[r][q].color = ColorRGB::MEDIUM_GRAY;
            btns[r][q].state = ButtonSt::ACTIVE;
            btns[r][q].text.text = "";
            btns[r][q].count = 0;            
        }
    }
    
    create_random_boom();
}

void MineSweeperG::on_mouse_motion(const int &mousex, const int &mousey) {
    int r, q;
    q = (mousex - 1) / AppConst::GRID_SIZE;
    r = (mousey - 1) / AppConst::GRID_SIZE;
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

static void set_btn_display_text(Button &btn, const bool &end = false) {
    int text_w, text_h;
    if (end && btn.count < 0 && btn.text.text == "O") {
        TTF_SetTextColorFloat(btn.text.text_obj, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
        return;
    }

    btn.text.text = btn.count < 0 ? "X" : btn.count >= 1000 ? "O" : btn.count == 0 ? "" : std::to_string(btn.count);
    TTF_SetTextString(btn.text.text_obj, btn.text.text.c_str(), btn.text.text.length());    
    TTF_GetTextSize(btn.text.text_obj, &text_w, &text_h);
    btn.text.loc.x = btn.rect.x + ((btn.rect.w - text_w) / 2);
    btn.text.loc.y = btn.rect.y + ((btn.rect.h - text_h) / 2);
    if (btn.count < 0 || btn.count >= 1000)
        TTF_SetTextColorFloat(btn.text.text_obj, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
    else
        TTF_SetTextColorFloat(btn.text.text_obj, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
}

void MineSweeperG::on_mouse_down(const int &mouse_button) {
    if (hover_btn.y < 0 && hover_btn.x < 0) {
        return;
    }

    if (mouse_button == SDL_BUTTON_LEFT) {
        if (btns[hover_btn.y][hover_btn.x].count < 0 || (btns[hover_btn.y][hover_btn.x].count >= 1000 && btns[hover_btn.y][hover_btn.x].count - 1001 < 0)) {
            end_game();
            return;
        }
    
        nb_safe--;
        open_button(hover_btn.y, hover_btn.x);
        if (btns[hover_btn.y][hover_btn.x].count == 0)
            open_neighbor(hover_btn.y, hover_btn.x);
    
        hover_btn = {-1, -1};
    } else if (mouse_button == SDL_BUTTON_RIGHT) {
        note_button(hover_btn.y, hover_btn.x);
    }

}

void MineSweeperG::end_game() {
    for (int r = 0; r < AppConst::GRID_ROWS; r++) {
        for (int q = 0; q < AppConst::GRID_COLS; q++) {
            if (!btns[r][q].text.text.empty() && btns[r][q].count < 1000)
                continue;

            open_button(r, q, true);
        }
    }

    hover_btn = {-1, -1};
}

void MineSweeperG::open_button(const int &r, const int &q, const bool &all) {
    if (btns[r][q].count >= 1000)
        btns[r][q].count -= 1001;
    btns[r][q].state = 0;
    btns[r][q].hover = false;
    btns[r][q].color = ColorRGB::DARK_GRAY;
    set_btn_display_text(btns[r][q], all);
    if (nb_safe == 0) {
        std::cout << "win" << std::endl;
    }
}

void MineSweeperG::open_neighbor(const int &startr, const int &startq) {
    for (int r = startr - 1; r <= startr + 1; r++) {
        if (r < 0 || r >= AppConst::GRID_ROWS)
            continue;

        for (int q = startq - 1; q <= startq + 1; q++) {
            if (q < 0 || q >= AppConst::GRID_COLS || btns[r][q].state == ButtonSt::DISABLE)
                continue;
 
            nb_safe--;
            open_button(r, q);
            if (btns[r][q].count == 0)
                open_neighbor(r, q);            
        }
    }
}

void MineSweeperG::note_button(const int &r , const int &q) {
    btns[r][q].count += 1001;
    set_btn_display_text(btns[r][q]);
}