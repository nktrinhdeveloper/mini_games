#include "SDL3/SDL_main.h"
#include "application.h"

#define ARRAY_ELEMS(x) (sizeof(x) / sizeof(x[0]))
const struct CmdMap {
    int         cmd_code;
    std::string cmd;
} cmd_map[] {
    {MiniGame::MATCH3, "match3"},
    {MiniGame::QUIT, "quit"}
};

std::string to_lower_string(const std::string &str);
bool get_game_from_input(const std::string &str, int &game);
void show_entry();
std::filesystem::path get_running_dir(const std::string &str);

int main(int args, char *argc[]) {
    std::filesystem::path running_dir = get_running_dir(argc[0]);
    if (running_dir.empty()) {
        std::cout << "cannot find running dir" << std::endl;
        return EXIT_FAILURE;
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to init sdl\nError: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } else if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "failed to init sdl ttf\nError: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    std::string usr_input;
    int         game = MiniGame::NO_GAME;
    Application app;
    show_entry();
    // while (game) {
        usr_input = "";
        std::cout << "Enter game: ";
        std::getline(std::cin >> std::ws, usr_input);
        if (!get_game_from_input(to_lower_string(usr_input), game))
            return EXIT_FAILURE;
            // continue;

        if (!app.was_app_init() && !app.init(running_dir.string()))
            return EXIT_FAILURE;

        app.start((MiniGame)game);
        // show_entry();
    // }

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

std::filesystem::path get_running_dir(const std::string &str) {
    int count = 0;
    std::filesystem::path path(str);
    while (path != path.root_path() || count != 50) {
        count++;
        if (path.filename().string() != AppConst::APP_DIR_NAME)
            path = path.parent_path();
        else 
            return path;
    }
    return {};
}
