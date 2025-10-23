#include "application.h"

Application::Application() {
    window = nullptr;
    renderer = nullptr;
    running = false;
    game = nullptr;
}

bool Application::init() {
    if (!(window = SDL_CreateWindow("MiniGame", 800, 600, SDL_WINDOW_HIDDEN))) {
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
    running = true;
    game = new SnakeG();
    mainloop();
    free_ptr(&game);
}
    
void Application::close() {
    if (window) 
        SDL_DestroyWindow(window);
    if (renderer)
        SDL_DestroyRenderer(renderer);

    free_ptr(&game);
    
    game = nullptr;
    window = nullptr;
    renderer = nullptr;
    SDL_Quit();
}

void Application::mainloop() {
    Uint64 now, tpf;
    Uint64 last_ticks = SDL_GetTicks();
    SDL_ShowWindow(window);
    SDL_RaiseWindow(window);
    while (running) {
        event_listening();

        game->update();
        game->render(renderer);

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
            if(evt.key.key == SDLK_ESCAPE) {
                running = false;
                SDL_HideWindow(window);
            }
            break;
        }
    }
}


