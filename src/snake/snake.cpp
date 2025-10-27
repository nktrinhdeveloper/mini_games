#include "snake/snake.h"

static SDL_FRect get_rect_from(const SDL_FRect &p1, const SDL_FRect &p2) {
    SDL_FRect rect;
    if (p1.y == p2.y) {
        rect.x = p1.x >= p2.x ? p2.x : p1.x;
        rect.y = p2.y;
        rect.w = SDL_abs(p2.x - p1.x) + p2.w;
        rect.h = p2.h;
    } else if (p1.x == p2.x) {
        rect.x = p2.x;
        rect.y = p1.y >= p2.y ? p2.y : p1.y;
        rect.w = p2.w;
        rect.h = SDL_abs(p2.y - p1.y) + p2.h;
    }
    return rect;
}

static bool has_intersect_snake(const SDL_FRect &rect, const std::vector<SDL_FRect> &snake) {
    SDL_FRect snake_rect;
    for (int i = 1; i < snake.size(); i++) {
        snake_rect = get_rect_from(snake[i], snake[i - 1]);
        if (SDL_HasRectIntersectionFloat(&rect, &snake_rect))
            return true;
    }
    return false;
}

static bool has_intersect_snake(const SDL_FPoint &p1, const std::vector<SDL_FRect> &snake) {
    SDL_FRect snake_rect;
    for (int i = 1; i < snake.size(); i++) {
        snake_rect = get_rect_from(snake[i], snake[i - 1]);
        if (i - 1 == 0) {
            snake_rect.x += 2;
            snake_rect.y += 2;
            snake_rect.w -= 4;
            snake_rect.h -= 4;
        }
        if (SDL_PointInRectFloat(&p1, &snake_rect))
            return true;
    }

    return false;
}

SnakeG::SnakeG() : Game() {
    alive = true;
    direction = {1, 0};
    next_direction = direction;
    snake.resize(2);
    snake.at(0) = {(float)(5 * GRID_SIZE) + 1, (float)(5 * GRID_SIZE) + 1, (float)GRID_SIZE - 1, (float)GRID_SIZE - 1};
    snake.at(1) = {(float)(3 * GRID_SIZE) + 1, (float)(5 * GRID_SIZE) + 1, (float)GRID_SIZE - 1, (float)GRID_SIZE - 1};
    create_random_prey();
}

void SnakeG::create_random_prey() {
    int x = 0;
    int y = 0;
    bool regen = true;
    SDL_FRect rect;
    while (regen) {
        regen = false;
        x = SDL_rand(GRID_COLS);
        y = SDL_rand(GRID_ROWS);
        prey = {(float)(GRID_SIZE * x) + 5, (float)(GRID_SIZE * y) + 5, (float)(GRID_SIZE - 10), (float)(GRID_SIZE - 10)};
        regen = has_intersect_snake(prey, snake);
    }    
}

void SnakeG::restart() {
    alive = true;
    direction = {1, 0};
    next_direction = direction;
    snake.resize(2);
    snake.at(0) = {(float)(5 * GRID_SIZE) + 1, (float)(5 * GRID_SIZE) + 1, (float)GRID_SIZE - 1, (float)GRID_SIZE - 1};
    snake.at(1) = {(float)(3 * GRID_SIZE) + 1, (float)(5 * GRID_SIZE) + 1, (float)GRID_SIZE - 1, (float)GRID_SIZE - 1};
    create_random_prey();
}


void SnakeG::update() {
    if (alive) {
        controller();
        move();
    }
}

void SnakeG::controller() {
    const bool *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_UP] && direction.y <= 0) {
        next_direction = {0, -1};
    } else if (keystate[SDL_SCANCODE_DOWN] && direction.y >= 0) {
        next_direction = {0, 1};
    } else if (keystate[SDL_SCANCODE_LEFT] && direction.x <= 0) {
        next_direction = {-1, 0};
    } else if (keystate[SDL_SCANCODE_RIGHT] && direction.x >= 0) {
        next_direction = {1, 0};
    }

    if (next_direction.x == direction.x && next_direction.y == direction.y)
        return;

    change_direction();
}

void SnakeG::change_direction() {
    float x, y;
    // snakex = (x * GRID_SIZE) + 1, snakey = (y * GRID_SIZE) + 1
    x = (snake[0].x - 1) / GRID_SIZE;
    y = (snake[0].y - 1) / GRID_SIZE;
    if (SDL_fmodf(x, 1) == 0 && SDL_fmodf(y, 1) == 0) {
        direction = next_direction;
        snake[0].x = (x * GRID_SIZE) + 1;
        snake[0].y = (y * GRID_SIZE) + 1;
        snake.insert(snake.begin() + 1, snake[0]);
    }
}

void SnakeG::move() {
        int ret = HUNT;
        if (direction.x) {
            snake[0].x += direction.x * SPEED;
        } else if (direction.y) {
            snake[0].y += direction.y * SPEED;
        }
        
        ret = check_collision(direction.x, direction.x > 0 || direction.y > 0);
        if (ret == DEAD) {
            alive = false;
            return;
        }

        SDL_FRect *tail = &snake[snake.size() - 1];
        SDL_FRect *nexto_tail = &snake[snake.size() - 2];
        bool pos = true;
        if (tail->y == nexto_tail->y) {
            pos = nexto_tail->x > tail->x;
            if (ret == GROW)
                add_tail(true, pos);
            
            tail->x = pos ? tail->x + SPEED : tail->x - SPEED;
            if (tail->x == nexto_tail->x)
                snake.pop_back();
        } else if (tail->x == nexto_tail->x) {
            pos = nexto_tail->y > tail->y;
            if (ret == GROW)
                add_tail(false, pos);
            tail->y = pos ? tail->y + SPEED : tail->y - SPEED;
            if (tail->y == nexto_tail->y)
                snake.pop_back();
        }
}

int SnakeG::check_collision(const bool &horz, const bool &pos) {
    if (SDL_HasRectIntersectionFloat(&prey, &snake[0])) {
        create_random_prey();
        return GROW;
    }

    SDL_FPoint a;
    if (horz && pos) {
        a = {snake[0].x + snake[0].w, snake[0].y + (snake[0].h / 2)};
        if (a.x > GRID_SIZE * GRID_COLS || has_intersect_snake(a, snake))
            return DEAD;
    } else if (horz && !pos) {
        a = {snake[0].x, snake[0].y + (snake[0].h / 2)};
        if (a.x < 0 || has_intersect_snake(a, snake))
            return DEAD;
    } else if (!horz && pos) {
        a = {snake[0].x + (snake[0].w / 2), snake[0].y + snake[0].h};
        if (a.y > GRID_SIZE * GRID_ROWS || has_intersect_snake(a, snake))
            return DEAD;
    } else if (!horz && !pos) {
        a = {snake[0].x + (snake[0].w / 2), snake[0].y};
        if (a.y < 0 || has_intersect_snake(a, snake))
            return DEAD;
    }

    return HUNT;
}

void SnakeG::add_tail(const bool &horz, const bool &pos) {
    SDL_FRect *tail = &snake[snake.size() - 1];
    if (horz && pos) {
        tail->x -= GRID_SIZE;
    } else if (horz && !pos) {
        tail->x += GRID_SIZE;
    } else if (!horz && pos) {
        tail->y -= GRID_SIZE;
    } else if (!horz && pos) {
        tail->y += GRID_SIZE;
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

std::vector<SDL_FRect> SnakeG::get_snake_render_rect() const{
    std::vector<SDL_FRect> ret;
    ret.push_back(snake[0]);
    for (int i = 1; i < snake.size(); i++) {
        ret.emplace_back(std::move(get_rect_from(snake[i], snake[i - 1])));
    }
    return ret;
}

void SnakeG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
    SDL_RenderClear(renderer);

    // show_grid(renderer);
    std::vector<SDL_FRect> render_snake = get_snake_render_rect();
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    for (int i = 1; i < render_snake.size(); i++) {
        SDL_RenderFillRect(renderer, &render_snake[i]);
    }
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
    SDL_RenderFillRect(renderer, &render_snake[0]);

    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::GREEN.r, ColorRGB::GREEN.g, ColorRGB::GREEN.b, ColorRGB::GREEN.a);
    SDL_RenderFillRect(renderer, &prey);

    SDL_RenderPresent(renderer);
}
