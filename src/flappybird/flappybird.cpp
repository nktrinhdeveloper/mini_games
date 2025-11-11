#include "flappybird.h"
namespace {
    void create_random_pipe(std::vector<SDL_FRect> &dst_vec, int &pipe_create_idx, const bool &init = false) {
        const int area_free = AppConst::GRID_ROWS - 4;
        const int distance = 12;
        const int gap = 6;
        int first_height = 0;
        int second_height = 0;
        int locx = (AppConst::GRID_COLS + 1) * AppConst::GRID_SIZE;
        if (!init){
            locx = dst_vec[pipe_create_idx - 1].x + dst_vec[pipe_create_idx - 1].w + distance * AppConst::GRID_SIZE;
            if (pipe_create_idx == dst_vec.size())
                pipe_create_idx = 2;
        }

        first_height = SDL_rand(area_free / 2);
        second_height = area_free - first_height - gap;
        dst_vec[pipe_create_idx++] = {(float) locx, (float) AppConst::GRID_SIZE * 2, (float) 3 * AppConst::GRID_SIZE, (float) first_height * AppConst::GRID_SIZE};
        dst_vec[pipe_create_idx++] = {(float) locx, (float) (first_height + gap + 2) * AppConst::GRID_SIZE, (float) 3 * AppConst::GRID_SIZE, (float) second_height * AppConst::GRID_SIZE};
    }
};


bool FlappyBird::init(SDL_Renderer *renderer, const std::string &running_dir) {
    bool init = true;
    map.resize(8);
    map[0] = {0, 0, AppConst::GRID_SIZE * AppConst::GRID_COLS, AppConst::GRID_SIZE * 2};
    map[1] = {0, (AppConst::GRID_ROWS - 2) * AppConst::GRID_SIZE, AppConst::GRID_SIZE * AppConst::GRID_COLS, AppConst::GRID_SIZE * 2};
    pipe_create_idx = 2;
    min_pipe_idx = 2;
    while (pipe_create_idx < map.size()) {
        create_random_pipe(map, pipe_create_idx, init);
        init = false;
    }

    bird.x = (AppConst::GRID_COLS / 2) * AppConst::GRID_SIZE;
    bird.y = (3) * AppConst::GRID_SIZE;
    bird.w = AppConst::GRID_SIZE;
    bird.h = AppConst::GRID_SIZE;
    b_vert_velo = 0;

    std::map<int, std::string> sounds_dir = {
        {FlappySound::FLAP, running_dir + "/" + AppConst::HIT_SOUND}
    };
    audios.resize(sounds_dir.size());
    for (const std::pair<int, std::string> pair : sounds_dir) {
        if (!SDL_LoadWAV(pair.second.c_str(), &audios[pair.first].spec, &audios[pair.first].buffer, &audios[pair.first].length))
            audios[pair.first].set_default();
    }
    return true;
}

void FlappyBird::update() {
    if (!alive)
        return;
    move_world();
    bird_fall();
    alive = !check_bird_collision();
}

void FlappyBird::move_world() {
    for (int i = 2; i < map.size(); i++)
        map[i].x -= HORZ_SPEED;
    
    if (map[min_pipe_idx].x + map[min_pipe_idx].w < 0) {
        create_random_pipe(map, pipe_create_idx);
        min_pipe_idx = min_pipe_idx + 2 >= map.size() ? 2 : min_pipe_idx + 2;
    }
}

void FlappyBird::bird_fall() {
    float tpf = clock->get_tpf() / 100.f;
    bird.y += (b_vert_velo * tpf) + ((VERT_SPEED * tpf * tpf) / 2);
    b_vert_velo += VERT_SPEED * tpf;
}

bool FlappyBird::check_bird_collision() {
    for (int i = 0; i < map.size(); i++) {
        if (SDL_HasRectIntersectionFloat(&bird, &map[i]))
            return true;
    }
    return false;
}

void FlappyBird::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    SDL_RenderFillRect(renderer, &bird);
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
    SDL_RenderFillRects(renderer, map.data(), map.size());
}
        
void FlappyBird::restart() {
    bool init = true;
    pipe_create_idx = 2;
    min_pipe_idx = 2;
    while (pipe_create_idx < map.size()) {
        create_random_pipe(map, pipe_create_idx, init);
        init = false;
    }

    bird.x = (AppConst::GRID_COLS / 2) * AppConst::GRID_SIZE;
    bird.y = (3) * AppConst::GRID_SIZE;
    b_vert_velo = 0;
    alive = true;
}
        
void FlappyBird::on_keydown(const SDL_Keycode &code, const SDL_Keymod &mod) {
    switch (code){
    case SDLK_UP:
        b_vert_velo = -VERT_SPEED * 5;
        play_audio(FlappySound::FLAP);
        break;    
    }
}

