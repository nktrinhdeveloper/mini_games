#include "snake/snake.h"

SnakeG::SnakeG() : Game() {
        next_offs = {GRID_SIZE, 0};
        for (int i = 0;  i < 3; i++) {
            SnakePart *part = &snake.emplace_back();
            part->rect = {(float)(GRID_SIZE * (5 - i)) + 1,(float)(GRID_SIZE * 5) + 1,(float)(GRID_SIZE - 2), (float)(GRID_SIZE - 2)};
            part->offs = {0, 0};
        }
        create_random_prey();
}

void SnakeG::create_random_prey() {
    int x = 0;
    int y = 0;
    bool regen = true;
    while (regen) {
        regen = false;
        x = SDL_rand(GRID_COLS);
        y = SDL_rand(GRID_ROWS);
        for (const SnakePart &part : snake) {
            regen |= part.rect.x / GRID_SIZE == x || part.rect.y / GRID_SIZE == y;
        }
    }
    prey = {(float)(GRID_SIZE * x) + 5, (float)(GRID_SIZE * y) + 5, (float)(GRID_SIZE - 10), (float)(GRID_SIZE - 10)};
}

void SnakeG::update() {
    controller();
    move();
}

void SnakeG::controller() {
    const bool *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_UP] && next_offs.y <= 0) {
        next_offs = {0, -GRID_SIZE};
    } else if (keystate[SDL_SCANCODE_DOWN] && next_offs.y >= 0) {
        next_offs = {0, GRID_SIZE};
    } else if (keystate[SDL_SCANCODE_LEFT] && next_offs.x <= 0) {
        next_offs = {-GRID_SIZE, 0};
    } else if (keystate[SDL_SCANCODE_RIGHT] && next_offs.x >= 0) {
        next_offs = {GRID_SIZE, 0};
    }

    if ((SDL_fabsf(snake[0].offs.x) == 1 || SDL_fabsf(snake[0].offs.y) == 1) && change_direction())
        corners.emplace_back(SDL_FRect{snake[0].rect.x + snake[0].offs.x, snake[0].rect.y + snake[0].offs.y, snake[0].rect.w, snake[0].rect.h});

    if (snake[0].offs.x != 0 || snake[0].offs.y != 0) 
        return;

    snake[0].offs = next_offs;
    for (int i = 1; i < snake.size(); i++) {
        snake[i].offs.x = snake[i - 1].rect.x - snake[i].rect.x; 
        snake[i].offs.y = snake[i - 1].rect.y - snake[i].rect.y; 
    }
}

bool SnakeG::change_direction() {
    return (snake[0].offs.x != 0 && next_offs.y != 0) || (snake[0].offs.y != 0 && next_offs.x != 0); 
}

void SnakeG::move() {
    for (int i = 0; i < snake.size(); i++) {
        if (snake[i].offs.x > 0) {
            snake[i].rect.x += 1;
            snake[i].offs.x = snake[i].offs.x - 1 < 0 ? 0 : snake[i].offs.x - 1;
        } else if (snake[i].offs.x < 0) {
            snake[i].rect.x -= 1;
            snake[i].offs.x = snake[i].offs.x + 1 > 0 ? 0 : snake[i].offs.x + 1;
        } else if (snake[i].offs.y > 0) {
            snake[i].rect.y += 1;
            snake[i].offs.y = snake[i].offs.y - 1 < 0 ? 0 : snake[i].offs.y - 1;
        } else if (snake[i].offs.y < 0) {
            snake[i].rect.y -= 1;
            snake[i].offs.y = snake[i].offs.y + 1 > 0 ? 0 : snake[i].offs.y + 1;
        }
    }

    if (!corners.empty() && snake[snake.size() - 1].rect.x == corners[0].x && snake[snake.size() - 1].rect.y == corners[0].y) {
        corners.erase(corners.begin());
    }
}

void show_grid(SDL_Renderer *renderer) {
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

void SnakeG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
    SDL_RenderClear(renderer);

    show_grid(renderer);
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
    for (const SnakePart &part : snake) {
        SDL_RenderFillRect(renderer, &part.rect);
    }

    for (const SDL_FRect &rect : corners) {
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    SDL_RenderFillRect(renderer, &prey);

    SDL_RenderPresent(renderer);
}
