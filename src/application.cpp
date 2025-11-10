#include "application.h"

Application::Application() {
    window      = nullptr;
    renderer    = nullptr;
    running     = false;
    game        = nullptr;
    game_code   = MiniGame::NO_GAME;
    aud_stream  = nullptr;
}

bool Application::init(const std::string &dir) {
    if (!(window = SDL_CreateWindow("MiniGame", AppConst::GRID_SIZE * AppConst::GRID_COLS, AppConst::GRID_SIZE * AppConst::GRID_ROWS, SDL_WINDOW_HIDDEN))) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to create window\nError: %s\n", SDL_GetError());
        return false;
    } else if (!(renderer = SDL_CreateRenderer(window, nullptr))) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to create renderer\nError: %s\n", SDL_GetError());
        return false;
    } else if (!(aud_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &AppConst::AUD_SPEC_WAV, nullptr, nullptr))) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to open device stream\nError: %s\n", SDL_GetError());
    }

    running_dir = dir;
    SDL_SetWindowPosition(window, 0, 20);
    clock.start_clock();
    return true;
}

bool Application::was_app_init() {
    return window && renderer;
}

void Application::start(const MiniGame &code) {
    create_game(code);
    mainloop();
    on_close_game();
}

void Application::on_close_game() {
    int wind_w, wind_h;
    free_ptr(&game);
    game_code = MiniGame::NO_GAME;
    SDL_GetWindowSize(window, &wind_w, &wind_h);
    if (wind_w != AppConst::GRID_SIZE * AppConst::GRID_COLS || wind_h != AppConst::GRID_SIZE * AppConst::GRID_ROWS)
        SDL_SetWindowSize(window, AppConst::GRID_SIZE * AppConst::GRID_COLS, AppConst::GRID_SIZE * AppConst::GRID_ROWS);
    if (aud_stream) {
        SDL_ClearAudioStream(aud_stream);
        SDL_PauseAudioStreamDevice(aud_stream);
    }
}

void Application::create_game(const MiniGame &code) {
    running = true;
    game_code = code;
    switch (code){
        case MiniGame::MATCH3:
            game = new Match3G();
            break;
        default:
            running = false;
            return;
    }

    if (aud_stream) {
        game->set_audio_stream(aud_stream);
        SDL_ResumeAudioStreamDevice(aud_stream);
    }
    game->set_clock(&clock);
    if (!game->init(renderer, running_dir))
        running = false;
}
    
void Application::close() {
    if (window) 
        SDL_DestroyWindow(window);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (aud_stream)
        SDL_DestroyAudioStream(aud_stream);

    free_ptr(&game);
    
    window = nullptr;
    renderer = nullptr;
    aud_stream = nullptr;
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
    
    for (int r = 1; r < wind_h / AppConst::GRID_SIZE; r++) {
        SDL_RenderLine(renderer, 0, r * (float) AppConst::GRID_SIZE, (float) wind_w, r * (float) AppConst::GRID_SIZE);
    }
    for (int q = 1; q < wind_w / AppConst::GRID_SIZE; q++) {
        SDL_RenderLine(renderer, q * (float) AppConst::GRID_SIZE, 0, q * (float) AppConst::GRID_SIZE, (float)wind_h);
    }
}

void Application::mainloop() {
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

        clock.ticking();
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
            game->on_mouse_motion(evt.button.x, evt.button.y);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            game->on_mouse_down(evt.button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            game->on_mouse_up(evt.button.button);
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

