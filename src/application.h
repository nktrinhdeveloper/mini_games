#ifndef APPLICATION_H
#define APPLICATION_H

#include "snake/snake.h"
#include "minesweeper/minesweeper.h"
#include "tetris/tetris.h"

typedef enum MiniGame {
    QUIT,
    NO_GAME,
    SNAKE,
    MINESWEEPER,
    TETRIS
} MiniGame;

class Application {
    private:
        SDL_Window      *window;
        SDL_Renderer    *renderer;
        bool            running;
        Game            *game;
        int             game_code;

        void event_listening();
        void mainloop();
        void create_game(const MiniGame &code);
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