#include "application.h"

Application::Application() {
    window = nullptr;
    renderer = nullptr;
    running = false;
    game = nullptr;
    game_code = MiniGame::NO_GAME;
}

bool Application::init() {
    if (!(window = SDL_CreateWindow("MiniGame", GRID_SIZE * GRID_COLS, GRID_SIZE * GRID_ROWS, SDL_WINDOW_HIDDEN))) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to create window\nError: ", SDL_GetError());
        return false;
    } else if (!(renderer = SDL_CreateRenderer(window, nullptr))) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to create renderer\nError: ", SDL_GetError());
        return false;
    }
    return true;
}

bool Application::was_app_init() {
    return window && renderer;
}

void Application::start(const MiniGame &code) {
    game_code = code;
    create_game(code);
    mainloop();
    free_ptr(&game);
    game_code = MiniGame::NO_GAME;
}

void Application::create_game(const MiniGame &code) {
    running = true;
    switch (code){
        case MiniGame::SNAKE:
            game = new SnakeG();
            break;
        case MiniGame::MINESWEEPER:
            game = new MineSweeperG();
            break;
        case MiniGame::TETRIS:
            game = new TetrisG();
            break;
        default:
            running = false;
    }

    if (game && !game->init(renderer))
        running = false;
}
    
void Application::close() {
    if (window) 
        SDL_DestroyWindow(window);
    if (renderer)
        SDL_DestroyRenderer(renderer);

    free_ptr(&game);
    
    window = nullptr;
    renderer = nullptr;
    SDL_Quit();
}

void Application::show_grid(SDL_Renderer *renderer) {
    int wind_w, wind_h;
    SDL_FPoint a, b;
    SDL_GetWindowSize(SDL_GetRenderWindow(renderer), &wind_w, &wind_h);
    SDL_SetRenderDrawColorFloat(renderer,
                                ColorRGB::WHITE.r,
                                ColorRGB::WHITE.g,
                                ColorRGB::WHITE.b,
                                ColorRGB::WHITE.a);
    
    for (int r = 1; r < wind_h / GRID_SIZE; r++) {
        SDL_RenderLine(renderer, 0, r * (float) GRID_SIZE, (float) wind_w, r * (float) GRID_SIZE);
    }
    for (int q = 1; q < wind_w / GRID_SIZE; q++) {
        SDL_RenderLine(renderer, q * (float) GRID_SIZE, 0, q * (float) GRID_SIZE, (float)wind_h);
    }
}

void Application::mainloop() {
    Uint64 now, tpf;
    Uint64 last_ticks = SDL_GetTicks();
    SDL_ShowWindow(window);
    SDL_RaiseWindow(window);
    while (running) {
        event_listening();

        game->update();

        SDL_SetRenderDrawColorFloat(renderer, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
        SDL_RenderClear(renderer);

        // show_grid(renderer);

        game->render(renderer);
        SDL_RenderPresent(renderer);

        now = SDL_GetTicks();
        tpf = now - last_ticks;
        last_ticks = now;
        if (tpf < 1000 / 60)
            SDL_Delay((1000/60) - tpf);
    }
}

void Application::event_listening() {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
        case SDL_EVENT_QUIT:
            running = false;
            SDL_HideWindow(window);
            break;
        case SDL_EVENT_KEY_DOWN:
            handle_key_event(evt);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            if (game_code == MiniGame::MINESWEEPER) {
                MineSweeperG *sweeper = dynamic_cast<MineSweeperG *>(game);
                sweeper->on_hover(evt.button.x, evt.button.y);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (game_code == MiniGame::MINESWEEPER) {
                MineSweeperG *sweeper = dynamic_cast<MineSweeperG *>(game);
                sweeper->on_click(evt.button.button);
            }            
            break;
        }
    }
}

void Application::handle_key_event(const SDL_Event &evt) {
    switch (evt.key.key) {
    case SDLK_ESCAPE:
        running = false;
        SDL_HideWindow(window);
        break;
    case SDLK_R:
        game->restart();
        break;
    default:
        game->on_keydown(evt.key.key);
    }
}

