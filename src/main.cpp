#include "SDL3/SDL_main.h"
#include "application.h"

#define ARRAY_ELEMS(x) (sizeof(x) / sizeof(x[0]))
const struct CmdMap {
    int         cmd_code;
    std::string cmd;
} cmd_map[] {
    {MiniGame::SNAKE, "snake"},
    {MiniGame::QUIT, "quit"}
};

std::string to_lower_string(const std::string &str);
bool get_game_from_input(const std::string &str, int &game);
void show_entry();

int main(int args, char *arc[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to init sdl\nError: ", SDL_GetError());
        return EXIT_FAILURE;
    }

    std::string usr_input;
    int         game = MiniGame::NO_GAME;
    Application app;
    show_entry();
    while (game) {
        usr_input = "";
        std::cout << "Enter game: ";
        std::getline(std::cin >> std::ws, usr_input);
        if (!get_game_from_input(to_lower_string(usr_input), game))
            continue;

        if (!app.was_app_init() && !app.init())
            return EXIT_FAILURE;

        app.start((MiniGame)game);
        show_entry();
    }

    app.close();
    return EXIT_SUCCESS;
}

void show_entry() {
    std::string txt = "Game:\n";
    for (int i = 0; i < ARRAY_ELEMS(cmd_map); i++) {
        txt += "\t"+ cmd_map[i].cmd + "\n";
    }
    std::cout << txt;
}

std::string to_lower_string(const std::string &str) {
    std::string ret;
    for (int i = 0; i < str.length(); i++) {
        ret += tolower(str[i]);
    }
    return ret;
}

bool get_game_from_input(const std::string &str, int &game) {
    if (str.empty())
        return false;

    for (int i = 0; i < ARRAY_ELEMS(cmd_map); i++) {
        if (cmd_map[i].cmd == str) {
            game = cmd_map[i].cmd_code;
            return cmd_map[i].cmd_code != MiniGame::QUIT;
        }
    }

    game = MiniGame::NO_GAME;
    SDL_LogError(SDL_LOG_CATEGORY_INPUT, "invalid game\n");
    return false;
}

