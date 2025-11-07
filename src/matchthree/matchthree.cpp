#include "matchthree.h"

namespace {
    void create_triangle();
}

bool Match3G::init(SDL_Renderer *renderer, const std::string &running_dir) {
    items_grid = Vector2D<int>(8, std::vector<int>(8));
    return true;
}
    
void Match3G::update() {}

void Match3G::render(SDL_Renderer *renderer) {
    SDL_FRect rect{0, 0, 600, 600};
    SDL_SetRenderDrawColorFloat(renderer, 1, 1, 1, 1);
    SDL_RenderRect(renderer, &rect);
}

void Match3G::restart() {}

void Match3G::on_keydown(const SDL_Keycode &code) {}
