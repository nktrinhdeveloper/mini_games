#include "matchthree.h"

namespace {
    SDL_FColor get_color_of_shape(const int &shape) {
        SDL_FColor ret;
        switch (shape) {
        case Match3G::TRIANGLE:
            ret = ColorRGB::RED;
            break;
        case Match3G::SQUARE:
            ret = ColorRGB::ORANGE;
            break;
        case Match3G::ROTATED_SQUARE:
            ret = ColorRGB::YELLOW;
            break;
        case Match3G::PENTAGON:
            ret = ColorRGB::GREEN;
            break;
        case Match3G::HEXAGON:
            ret = ColorRGB::BLUE;
            break;
        case Match3G::CIRCLE:
            ret = ColorRGB::INDIGO;
            break;
        case Match3G::DIAMOND:
            ret = ColorRGB::VIOLET;
            break;
        }
        return ret;

    }

    int get_regular_poly_segments(const int &shape) {
        int ret = 0;
        switch (shape) {
        case Match3G::TRIANGLE:
            ret = 3;
            break;
        case Match3G::SQUARE:
            ret = 4;
            break;
        case Match3G::ROTATED_SQUARE:
            ret = 4;
            break;
        case Match3G::PENTAGON:
            ret = 5;
            break;
        case Match3G::HEXAGON:
            ret = 6;
            break;
        case Match3G::CIRCLE:
            ret = 10;
            break;
        }
        return ret;
    }

    void create_random_item(Polygon &poly, const int &row, const int &col) {
        if (poly.shape) {
            int shape = SDL_rand(Match3G::NB_ITEMS) + 1;
            while (shape == poly.shape)
                shape = SDL_rand(Match3G::NB_ITEMS) + 1;

            poly.reset_default();
            poly.shape = shape;
        } else {
            poly.shape   = SDL_rand(Match3G::NB_ITEMS) + 1;
        }

        float       item_size       = Match3G::GRID_SIZE - Match3G::PADDING;
        float       radius          = item_size / 2;
        SDL_FColor  color           = get_color_of_shape(poly.shape);
        int         segments        = get_regular_poly_segments(poly.shape);
        float       rotate_angle    = poly.shape == Match3G::SQUARE ? AppConst::PI / 4 : -AppConst::PI / 2;
        SDL_FPoint  center;     
        center.x = (col * Match3G::GRID_SIZE) + Match3G::PADDING + (item_size / 2);
        center.y = ((row - 1) * Match3G::GRID_SIZE) + Match3G::PADDING + (item_size / 2);

        if (poly.shape != Match3G::DIAMOND) {
            poly.create_regular_polygon(segments, center, radius, color, rotate_angle);
        } else {
            poly.create_irregular_polygon(SDL_FRect{center.x - (item_size / 2),
                                                    center.y - (item_size / 2),
                                                    item_size, item_size},
                                                    color,
                                                    Vector2D<int>{{0, 1, 0, 1, 0},
                                                                  {1, 0, 0, 0, 1},
                                                                  {0, 0, 0, 0, 0},
                                                                  {0, 0, 0, 0, 0},
                                                                  {0, 0, 1, 0, 0}});   
        }
    }

    bool get_match_on_horz(std::vector<std::pair<int, int>> &dst, const Vector2D<Polygon> &items, const int &row, const int &col) {
        for (int rev = 0; rev < 2; rev++) {
            int q, limit, k;
            if (!rev) {
                q = col + 1;
                limit = items[row].size();
                k = 1;
            } else {
                q = col - 1;
                limit = 1;
                k  = -1;
            }

            for (; q * k < limit; q += k) {
                if (items[row][q].shape != items[row][col].shape)
                    break;

                dst.emplace_back(std::pair<int, int>{row, q});
            }
        }
        return dst.size() >= 3;
    }

    bool get_match_on_vert(std::vector<std::pair<int, int>> &dst, const Vector2D<Polygon> &items, const int &row, const int &col) {
        for (int rev = 0; rev < 2; rev++) {
            int r, limit, k;
            if (!rev) {
                r = row + 1;
                limit = items.size();
                k = 1;
            } else {
                r = row - 1;
                limit = 0;
                k = -1;
            }

            for (; r * k < limit; r += k) {
                if (items[row][col].shape != items[r][col].shape)
                    break;
                
                dst.emplace_back(std::pair<int, int>{r, col});
            }
        }
        
        return dst.size() >= 3;
    }

}

bool Match3G::init(SDL_Renderer *renderer, const std::string &running_dir) {
    items = Vector2D<Polygon>(9, std::vector<Polygon>(8));
    for (int r = 0; r < items.size(); r++) {
        for (int q = 0; q < items[r].size(); q++) {
            create_random_item(items[r][q], r, q);
        }
    }
    mouse_selected = false;
    selected = {-1, -1};
    targeted = {-1, -1};
    direction = {0, 0};
    offs = 0;
    state = NONE_STATE;
    return true;
}

void Match3G::update() {
    if (state & SWAPPING || state & REVERTING)
        swap_select_items();
    
    if (state & MATCHING) {
        if (!check_match()) {
            matches.clear();
            if (!direction.x && !direction.y) {
                state = NONE_STATE;
                return;
            }

            direction.x *= -1;
            direction.y *= -1;
            offs = GRID_SIZE;
            state = REVERTING;
            return;
        } else {
            offs = GRID_SIZE;
            direction = {0, 0};
            selected = {-1, -1};
            targeted = {-1, -1};
            state = FILLING;
            remove_matches();
        }    
    }

    if (state & FILLING) {
        if (filling_removed_matches()) {
            state = MATCHING;
        } else if (offs <= 0)
            offs = GRID_SIZE;
    }
}

void Match3G::swap_select_items() {
    Polygon *selected_poly = &items[selected.first][selected.second];
    Polygon *targeted_poly = &items[targeted.first][targeted.second];
    int limit = SDL_max(selected_poly->vertices.size(), targeted_poly->vertices.size());
    float speed = 0;
    speed = offs - SPEED <= 0 ? offs : SPEED;
    offs -= SPEED;
    for (int i = 0; i < limit; i++) {
        if (i < selected_poly->vertices.size()) {
                selected_poly->vertices[i].position.x += direction.x * speed;
                selected_poly->vertices[i].position.y += direction.y * speed;
        }

        if (i < targeted_poly->vertices.size()) {
                targeted_poly->vertices[i].position.x -= direction.x * speed;
                targeted_poly->vertices[i].position.y -= direction.y * speed;
        }
    }

    if (offs <= 0) {
        std::swap(items[selected.first][selected.second], items[targeted.first][targeted.second]);
        std::swap(selected, targeted);
        if (state & REVERTING) {
            direction = {0, 0};
            state = NONE_STATE;
        } else {
            matches.push_back(selected);
            matches.push_back(targeted);
            state = MATCHING;
        }
    }
}

bool Match3G::check_match() {
    std::vector<std::pair<int, int>> temp(1);
    int minrow = items.size();
    int endrow = 1;
    int row_step = SDL_max(SDL_abs(matches.front().first - matches.back().first), 1);
    while (minrow >= endrow) {
        for (std::pair<int, int> &pair : matches) {
            temp[0] = {pair.first, pair.second};
            if (get_match_on_horz(temp, items, pair.first, pair.second)) {
                temp.swap(matches);
                return true;
            } else 
                temp.erase(temp.begin() + 1, temp.end());

            if (get_match_on_vert(temp, items, pair.first, pair.second)) {
                temp.swap(matches);
                return true;
            } else 
                temp.erase(temp.begin() + 1, temp.end());
            
            if (!direction.x && !direction.y) {
                minrow = SDL_min(pair.first, minrow);
                if (pair.first - 1 < 1)
                    minrow = 0;
                else
                    pair.first -= row_step;
            } else {
                endrow = minrow + 1;
            }
        }
    }
    return false;
}

void Match3G::remove_matches() {
    std::sort(matches.begin(), matches.end(),
    [](const std::pair<int, int> &p1, const std::pair<int, int> &p2){
        if (p1.first == p2.first)
            return p1.second > p2.second;
        return p1.first > p2.first;
    });

    for (int i = 0; i < matches.size(); i++) {
        items[matches[i].first][matches[i].second].reset_default();
    }
}

bool Match3G::filling_removed_matches() {
    float speed = offs - speed <= 0 ? offs : SPEED;
    offs -= SPEED;
    for (std::vector<std::pair<int, int>>::iterator pair = matches.begin(); pair != matches.end(); pair++){
        if (items[pair->first][pair->second].shape || !items[pair->first - 1][pair->second].shape) {
            continue;
        }

        for (int r = pair->first - 1; r >= 0; r--) {
            for (SDL_Vertex &vert : items[r][pair->second].vertices)
                vert.position.y += speed;

            if (offs <= 0) {
                std::swap(items[r][pair->second], items[r + 1][pair->second]);
                if (r == 0)
                    create_random_item(items[r][pair->second], r, pair->second);
            }
        }
        // ret &= items[pair->first][pair->second].shape;
    }
    return items[matches.front().first][matches.front().second].shape;
}

void Match3G::render(SDL_Renderer *renderer) {
    for (int r = state & FILLING ? 0 : 1; r < items.size(); r++) {
        for (int q = 0; q < items[r].size(); q++) {
            if (!items[r][q].shape)
                continue;
            SDL_RenderGeometry(renderer, nullptr,
                                items[r][q].vertices.data(), items[r][q].vertices.size(),
                                items[r][q].indices.data(), items[r][q].indices.size());
        }
    }

    if (!state) {
        SDL_FRect rect{(float) (selected.second * GRID_SIZE) + PADDING, (float) ((selected.first - 1) * GRID_SIZE) + PADDING, (float) GRID_SIZE - PADDING, (float) GRID_SIZE - PADDING};
        SDL_SetRenderDrawColorFloat(renderer, ColorRGB::WHITE.r, ColorRGB::WHITE.g, ColorRGB::WHITE.b, ColorRGB::WHITE.a);
        SDL_RenderRect(renderer, &rect);
    }
}

void Match3G::restart() {
    items.clear();
    items = Vector2D<Polygon>(9, std::vector<Polygon>(8));
    for (int r = 0; r < items.size(); r++) {
        for (int q = 0; q < items[r].size(); q++) {
            create_random_item(items[r][q], r, q);
        }
    }
    mouse_selected = false;
    selected = {0, 0};
    direction = {0, 0};
    offs = 0;
    state = NONE_STATE;
}

void Match3G::on_keydown(const SDL_Keycode &code) {}

void Match3G::on_mouse_motion(const int &mousex, const int &mousey) {
    if (state)
        return;

    float col;
    float row;
    col = ((mousex - PADDING) / GRID_SIZE);
    row = ((mousey - PADDING) / GRID_SIZE) + 1;
    if (mouse_selected) {
        if (col < selected.second && selected.second - 1 >= 0)
            direction = {-1, 0};
        else if (col > selected.second && selected.second + 1 < items[0].size())
            direction = {1, 0};
        else if (row < selected.first && selected.first - 1 >= 0)
            direction = {0, -1};
        else if (row > selected.first && selected.first + 1 < items.size())
            direction = {0, 1};
        else 
            return;

        offs = GRID_SIZE;
        targeted = {selected.first + direction.y, selected.second + direction.x};
        state = SWAPPING;
        return;
    }

    if (col < items[0].size()) {
        selected.first = row;
        selected.second = col;
    }
}

void Match3G::on_mouse_down(const int &mouse) {
    if (mouse != SDL_BUTTON_LEFT)
        return;
    mouse_selected = true;
}

void Match3G::on_mouse_up(const int &mouse) {
    if (mouse !=SDL_BUTTON_LEFT)
        return;
    mouse_selected = false;
}
