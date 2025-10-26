#include "snake/snake.h"

SnakeG::SnakeG() : Game() {
    direction = {1, 0};
    snake.resize(2);
    snake.at(0) = {(float)(5 * GRID_SIZE) + 1, (float)(5 * GRID_SIZE) + 1, (float)GRID_SIZE - 1, (float)GRID_SIZE - 1};
    snake.at(1) = {(float)(3 * GRID_SIZE) + 1, (float)(5 * GRID_SIZE) + 1, (float)GRID_SIZE - 1, (float)GRID_SIZE - 1};
}

void SnakeG::create_random_prey() {
//     int x = 0;
//     int y = 0;
//     bool regen = true;
//     while (regen) {
//         regen = false;
//         x = SDL_rand(GRID_COLS);
//         y = SDL_rand(GRID_ROWS);
//         prey = {(float)(GRID_SIZE * x) + 5, (float)(GRID_SIZE * y) + 5, (float)(GRID_SIZE - 10), (float)(GRID_SIZE - 10)};
//         for (const SnakePart &part : snake) {
//             regen = SDL_HasRectIntersectionFloat(&prey, &part.rect);
//             if (regen)
//                 break;
//         }
//     }
}

void SnakeG::update() {
    controller();
    move();
}

void SnakeG::controller() {
    const bool *keystate = SDL_GetKeyboardState(nullptr);
    SDL_Point curnt_dirct = direction;
    if (keystate[SDL_SCANCODE_UP] && direction.y <= 0) {
        direction = {0, -1};
    } else if (keystate[SDL_SCANCODE_DOWN] && direction.y >= 0) {
        direction = {0, 1};
    } else if (keystate[SDL_SCANCODE_LEFT] && direction.x <= 0) {
        direction = {-1, 0};
    } else if (keystate[SDL_SCANCODE_RIGHT] && direction.x >= 0) {
        direction = {1, 0};
    }

    if (curnt_dirct.x == direction.x && curnt_dirct.y == direction.y)
        return;

    change_direction();
}

void SnakeG::change_direction() {
    int x, y;
    // snakex = (x * GRID_SIZE) + 1, snakey = (y * GRID_SIZE) + 1
    x = SDL_roundf((snake[0].x - 1) / GRID_SIZE);
    y = SDL_roundf((snake[0].y - 1) / GRID_SIZE);
    snake[0].x = (x * GRID_SIZE) + 1;
    snake[0].y = (y * GRID_SIZE) + 1;
    snake.insert(snake.begin() + 1, snake[0]);
}

void SnakeG::move() {
        if (direction.x) {
            snake[0].x += direction.x * SPEED;
        } else if (direction.y) {
            snake[0].y += direction.y * SPEED;
        }

        SDL_FRect *tail = &snake[snake.size() - 1];
        SDL_FRect *nexto_tail = &snake[snake.size() - 2];   
        if (tail->y == nexto_tail->y) {
            tail->x = nexto_tail->x > tail->x ? tail->x + SPEED : tail->x - SPEED;
            if (tail->x == nexto_tail->x)
                snake.pop_back();
        } else if (tail->x == nexto_tail->x) {
            tail->y = nexto_tail->y > tail->y ? tail->y + SPEED : tail->y - SPEED;
            if (tail->y == nexto_tail->y)
                snake.pop_back();
        }
}

int SnakeG::check_collision(const bool &horz, const bool &pos) {return false;}

void SnakeG::add_tail() {}

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
    SDL_FRect rect;
    ret.push_back(snake[0]);
    for (int i = 1; i < snake.size(); i++) {
        // snakex = (x * GRID_SIZE) + 1, snakey = (y * GRID_SIZE) + 1
        if (snake[i - 1].y == snake[i].y) {
            rect.x = snake[i - 1].x >= snake[i].x ? snake[i].x : snake[i - 1].x;
            rect.y = snake[i].y;
            rect.w = SDL_abs(snake[i].x - snake[i - 1].x) + snake[i].w;
            rect.h = snake[i].h;
        } else if (snake[i - 1].x == snake[i].x) {
            rect.x = snake[i].x;
            rect.y = snake[i - 1].y >= snake[i].y ? snake[i].y : snake[i - 1].y;
            rect.w = snake[i].w;
            rect.h = SDL_abs(snake[i].y - snake[i-1].y) + snake[i].h;
        }
    
        ret.emplace_back(std::move(rect));
    }
    return ret;
}

void SnakeG::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColorFloat(renderer, ColorRGB::BLACK.r, ColorRGB::BLACK.g, ColorRGB::BLACK.b, ColorRGB::BLACK.a);
    SDL_RenderClear(renderer);

    show_grid(renderer);
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
