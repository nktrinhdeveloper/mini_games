#ifndef APPLICATION_H
#define APPLICATION_H

#include "snake/snake.h"

typedef enum MiniGame {
    QUIT,
    NO_GAME,
    SNAKE
} MiniGame;

class Application {
    private:
        SDL_Window      *window;
        SDL_Renderer    *renderer;
        bool            running;
        Game            *game;

        void event_listening();
        void mainloop();
    public:
            Application();
            ~Application() = default;
            Application(const Application &other) = delete;
            Application(Application &&other) noexcept = delete;
            Application &operator=(const Application &other) = delete;
            Application &operator=(Application &&other) noexcept = delete;
        bool init();
        bool was_app_init();
        void start(const MiniGame &code);
        void close();
};

#endif