#ifndef APPLICATION_H
#define APPLICATION_H

// #include "snake/snake.h"
// #include "minesweeper/minesweeper.h"
// #include "tetris/tetris.h"
// #include "flappybird/flappybird.h"
// #include "g2048/game2048.h"
#include "matchthree/matchthree.h"

typedef enum MiniGame {
    QUIT,
    NO_GAME,
    SNAKE,
    MINESWEEPER,
    TETRIS,
    FLAPPY,
    G2048,
    MATCH3
} MiniGame;

class Application {
    private:
        SDL_Window      *window;
        SDL_Renderer    *renderer;
        Clock           clock;
        bool            running;
        Game            *game;
        int             game_code;
        std::string     running_dir;
        SDL_AudioStream *aud_stream;

        void event_listening();
        void mainloop();
        void create_game(const MiniGame &code);
        void show_grid(SDL_Renderer *renderer);
        void handle_key_event(const SDL_KeyboardEvent &keyboard_evt);
        void on_close_game();
    public:
            Application();
            ~Application() = default;
            Application(const Application &other) = delete;
            Application(Application &&other) noexcept = delete;
            Application &operator=(const Application &other) = delete;
            Application &operator=(Application &&other) noexcept = delete;
        bool init(const std::string &dir);
        bool was_app_init();
        void start(const MiniGame &code);
        void close();
};

#endif