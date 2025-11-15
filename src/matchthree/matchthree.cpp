#include "matchthree.h"

namespace {
    Vector2D<int> get_special_shape_pattern(const int &shape, SDL_Point *center_point) {
        switch (shape) {
        case Match3G::DIAMOND:
            if (center_point) {
                center_point->x = 2;
                center_point->y = 2;
            }
            return {
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0}
            };
        case Match3G::BOMB:
            if (center_point) {
                center_point->x = 3;
                center_point->y = 3;
            }
            return {
                {0, 0, 0, 1, 0, 0, 0},
                {0, 1, 1, 0, 1, 1, 0},
                {0, 1, 0, 0, 0, 1, 0},
                {1, 0, 0, 0, 0, 0, 1},
                {0, 1, 0, 0, 0, 1, 0},
                {0, 1, 1, 0, 1, 1, 0},
                {0, 0, 0, 1, 0, 0, 0},
            };
        case Match3G::PLANE:
            if (center_point) {
                center_point->x = 2;
                center_point->y = 1;
            }
            return {
                {0, 0, 0, 1, 0, 0, 0},
                {1, 0, 0, 0, 0, 0, 1},
                {0, 0, 1, 0, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0, 0, 0},
            };
        case Match3G::STAR:
            if (center_point) {
                center_point->x = 3;
                center_point->y = 3;
            }
            return {
                {0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0},
                {1, 0, 1, 0, 1, 0, 1},
                {0, 0, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0, 0, 0},
                {0, 1, 0, 0, 0, 1, 0},
            };
        }
        return {};
    }

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
        case Match3G::PLANE:
        case Match3G::STAR:
        case Match3G::BOMB:
            ret = ColorRGB::MEDIUM_GRAY;
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
        Vector2D<int> special_shape_pattern;

        if (poly.shape < Match3G::DIAMOND) {
            center.x = (col * Match3G::GRID_SIZE) + Match3G::PADDING + (item_size / 2);
            center.y = ((row - 1) * Match3G::GRID_SIZE) + Match3G::PADDING + (item_size / 2);
            poly.create_regular_polygon(segments, center, radius, color, rotate_angle);
        } else {
            special_shape_pattern =  get_special_shape_pattern(poly.shape, (SDL_Point*)(&center));
            poly.create_irregular_polygon(SDL_FRect{(float)(col * Match3G::GRID_SIZE) + Match3G::PADDING,
                                                    (float)((row - 1) * Match3G::GRID_SIZE) + Match3G::PADDING,
                                                    item_size, item_size},
                                                    color, special_shape_pattern, (SDL_Point&)(center));
        }
    }

    void create_item(Polygon &poly, const int &row, const int &col, const int &shape) {    
        poly.shape = shape;
        float       item_size       = Match3G::GRID_SIZE - Match3G::PADDING;
        float       radius          = item_size / 2;
        SDL_FColor  color           = get_color_of_shape(poly.shape);
        int         segments        = get_regular_poly_segments(poly.shape);
        float       rotate_angle    = poly.shape == Match3G::SQUARE ? AppConst::PI / 4 : -AppConst::PI / 2;
        SDL_FPoint  center;     
        Vector2D<int> special_shape_pattern;

        if (poly.shape < Match3G::DIAMOND) {
            center.x = (col * Match3G::GRID_SIZE) + Match3G::PADDING + (item_size / 2);
            center.y = ((row - 1) * Match3G::GRID_SIZE) + Match3G::PADDING + (item_size / 2);
            poly.create_regular_polygon(segments, center, radius, color, rotate_angle);
        } else {
            special_shape_pattern =  get_special_shape_pattern(poly.shape, (SDL_Point*)(&center));
            poly.create_irregular_polygon(SDL_FRect{(float)(col * Match3G::GRID_SIZE) + Match3G::PADDING,
                                                    (float)((row - 1) * Match3G::GRID_SIZE) + Match3G::PADDING,
                                                    item_size, item_size},
                                                    color, special_shape_pattern, (SDL_Point&)(center));
        }
    
    }

    bool add_item_to_check_list(std::vector<std::pair<int, int>> &check_list, const std::pair<int, int> &pair) {
        for (int i = 0; i < check_list.size(); i++) {
            if (check_list[i].second == pair.second && check_list[i].first >= pair.first)
                return false;
            if (check_list[i].second == pair.second && check_list[i].first < pair.first) {
                check_list[i].first = pair.first;
                return true;
            }
        }

        check_list.push_back(pair);
        return true;
    }

    void get_neighbor_on_horz(std::vector<std::pair<int, int>> &neighbor_row, std::pair<int, int> *track_pos,
                                const Vector2D<int> &items, const std::pair<int, int> &pos) {
        for (int rev = 0; rev < 2; rev++) {
            int q, limit, k;
            if (!rev) {
                q = pos.second + 1;
                limit = items[pos.first].size();
                k = 1;
            } else {
                q = pos.second - 1;
                limit = 1;
                k  = -1;
            }

            for (; q * k < limit; q += k) {
                if (items[pos.first][q] != items[pos.first][pos.second])
                    break;

                if (!rev)
                    neighbor_row.emplace_back(pos.first, q);
                else {
                    neighbor_row.insert(neighbor_row.begin(), {pos.first, q});
                    if (track_pos)
                        track_pos->second++;
                }
            }
        }
    }

    void get_neighbor_on_vert(Vector2D<std::pair<int, int>> &neighbor, std::pair<int, int> &track_pos,
                                const Vector2D<int> &items, const std::pair<int, int> &pos) {
        get_neighbor_on_horz(neighbor[0], &track_pos, items, pos);
        for (int rev = 0; rev < 2; rev++) {
            int r, limit, k;
            if (!rev) {
                r = pos.first + 1;
                limit = items.size();
                k = 1;
            } else {
                r = pos.first - 1;
                limit = 0;
                k = -1;
            }

            for (; r * k < limit; r += k) {
                if (items[pos.first][pos.second] != items[r][pos.second])
                    break;
                
                if (!rev) {
                    neighbor.emplace_back(std::vector<std::pair<int, int>>{std::pair<int, int>(r, pos.second)});
                    get_neighbor_on_horz(neighbor.back(), nullptr, items, {r, pos.second});
                } else {
                    neighbor.insert(neighbor.begin(), {std::vector<std::pair<int, int>>{std::pair<int, int>(r, pos.second)}});
                    track_pos.first++;
                    get_neighbor_on_horz(neighbor.front(), nullptr, items, {r, pos.second});
                }
            }
        }
    }

    bool is_match_O_shape(const std::pair<int, int> &track_pos, Vector2D<std::pair<int, int>> &neighbor, Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        if (neighbor.size() < 2 || neighbor[track_pos.first].size() < 2)
            return false;
        
        bool match = false;
        for (int r = track_pos.first - 1; r <= track_pos.first + 1; r += 2) {
                if (r < 0 || r >= neighbor.size())
                    continue;
            for (int q = track_pos.second - 1; q <= track_pos.second + 1; q += 2) {
                if (q < 0 || q >= neighbor[r].size())
                    continue;
                
                if (neighbor[r][q].second < neighbor[track_pos.first][track_pos.second].second || neighbor[r][q].second > neighbor[track_pos.first][track_pos.second].second) {
                    match = true;
                    break;
                }
            }
            if (match)
                break;
        }

        if (match) {
            Match3G::Matcher *matcher = &matchers.emplace_back();
            matcher->shape = Match3G::MatchShape::O_SHAPE;
            matcher->check_pos = neighbor[track_pos.first][track_pos.second];
            for (std::vector<std::pair<int, int>> &row : neighbor) {
                for (std::pair<int, int> &col : row) {
                    items[col.first][col.second] = -1;
                    matcher->matches.push_back(col);
                } 
            }
        }
        return match;
    }
        
    bool is_match_vert_I_shape(const std::pair<int, int> &track_pos, Vector2D<std::pair<int, int>> &neighbor, Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        if (neighbor.size() < 3)
            return false;

        std::pair<int, int> *selected = &neighbor[track_pos.first][track_pos.second];
        Match3G::Matcher *matcher = &matchers.emplace_back();
        matcher->shape = Match3G::MatchShape::I_SHAPE_VERT;
        matcher->check_pos = neighbor[track_pos.first][track_pos.second];
        for (std::vector<std::pair<int, int>> &row : neighbor) {
            for (std::pair<int, int> &col : row) {
                if (col.second != selected->second)
                    continue;

                matcher->matches.push_back(col);
                items[col.first][col.second] = -1;
            }
        }
        return true;
    }
        
    bool is_match_horz_I_shape(const std::pair<int, int> &track_pos, Vector2D<std::pair<int, int>> &neighbor, Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        if (neighbor[track_pos.first].size() < 3)
            return false;

        Match3G::Matcher *matcher = &matchers.emplace_back();
        matcher->shape = Match3G::MatchShape::I_SHAPE_HORZ;
        matcher->check_pos = neighbor[track_pos.first][track_pos.second];
        matcher->matches.swap(neighbor[track_pos.first]);
        for (const std::pair<int, int> &match : matcher->matches) {
            items[match.first][match.second] = -1;
        }
        return true;
    }
        
    bool is_match_T_shape(const std::pair<int, int> &track_pos, Vector2D<std::pair<int, int>> &neighbor, Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        if (neighbor.size() < 3 || neighbor[track_pos.first].size() < 3)
            return false;

        Match3G::Matcher *matcher = &matchers.emplace_back();
        matcher->shape = Match3G::MatchShape::T_SHAPE;
        matcher->check_pos = neighbor[track_pos.first][track_pos.second];
        for (int r = 0; r < neighbor.size(); r++) {
            for (int q = 0; q < neighbor[r].size(); q++) {
                if (r != track_pos.first &&
                (neighbor[r][q].second < neighbor[track_pos.first][track_pos.second].second || neighbor[r][q].second > neighbor[track_pos.first][track_pos.second].second))
                    continue;
                matcher->matches.push_back(neighbor[r][q]);
                items[neighbor[r][q].first][neighbor[r][q].second] = -1;
            }
        }
        return true;
    }
    
    bool check_match_pattern(std::vector<Match3G::Matcher> &matchers, Vector2D<int> &items, const std::pair<int, int> &pos) {
        Vector2D<std::pair<int, int>> neighbor = {{pos}};
        std::pair<int, int> track_pos = {0, 0};
        get_neighbor_on_vert(neighbor, track_pos, items, pos);
        if (is_match_T_shape(track_pos, neighbor, items, matchers))
            return true;
        else if (is_match_O_shape(track_pos, neighbor, items, matchers))
            return true;
        if (is_match_vert_I_shape(track_pos, neighbor, items, matchers))
            return true;
        if (is_match_horz_I_shape(track_pos, neighbor, items, matchers))
            return true;
        return false;
    }

    SDL_Point select_items_to_swap(std::vector<std::pair<int, int>> &check_list, const std::pair<int, int> &hovering) {
        if (check_list[0].first < 0 && check_list[0].second < 0) {
            check_list[0] = hovering;
        } else if (check_list[0].first != hovering.first || check_list[0].second != hovering.second) {
            int x = SDL_abs(hovering.second - check_list[0].second);
            int y = SDL_abs(hovering.first - check_list[0].first);
            if ( x > 1 ||  y > 1 || x == y) {
                check_list[0] = hovering;
            } else {
                check_list[1] = hovering;
                return {hovering.second - check_list[0].second, hovering.first - check_list[0].first};
            }
        }

        return {0, 0};
    }

    void show_hovering_on_keypress(std::pair<int, int> &hovering) {
        if (hovering.first < 0 && hovering.second < 0)
            hovering = {1, 0};
    }

    Vector2D<int> get_2d_shape_grid(const Vector2D<Polygon> &items) {
        Vector2D<int> ret(items.size(), std::vector<int>(items[0].size()));
        for (int r = 0; r < items.size(); r++) {
            for (int q = 0; q < items[r].size(); q++) {
                ret[r][q] = items[r][q].shape;
            }
        }
        return ret;
    }

    void get_explode_items(const std::pair<int, int> &pos, std::vector<Match3G::Matcher> &matchers) {
        Match3G::Matcher *matcher = &matchers.emplace_back();
        matcher->shape = -1;
        for (int r = pos.first - 2; r <= pos.first + 2; r++) {
            if (r < 1 || r >= 9)
                continue;

            for (int q = pos.second - 2; q <= pos.second + 2; q++) {
                if (q < 0 || q >= 8)
                    continue;
                matcher->matches.emplace_back(r, q);
            }
        }
    }

    void get_shotdown_items(const std::pair<int, int> &pos, const Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        Match3G::Matcher *matcher = &matchers.emplace_back();
        matcher->shape = -1;
        matcher->matches.emplace_back(pos);
        if (pos.first - 1 >= 1)
            matcher->matches.emplace_back(pos.first - 1, pos.second);
        if (pos.first + 1 < 9)
            matcher->matches.emplace_back(pos.first + 1, pos.second);
        if (pos.second - 1 >= 0)
            matcher->matches.emplace_back(pos.first, pos.second - 1);
        if (pos.second + 1 < 8)
            matcher->matches.emplace_back(pos.first, pos.second + 1);

        int random_shape = Match3G::PLANE;
        int r, q;
        while (random_shape > Match3G::DIAMOND) {
            r = SDL_rand(8) + 1;
            q = SDL_rand(8);
            bool already_have = false;
            for (const std::pair<int, int> &pair : matcher->matches) {
                if (pair.first == r && pair.second == q) {
                    random_shape = Match3G::DIAMOND + 1;
                    already_have = true;
                    break;
                }
            }
            if (already_have)
                continue;
            random_shape = items[r][q];
        }
        matcher->matches.emplace_back(r, q);
    }

    void get_all_sameshape_items(const std::vector<std::pair<int, int>> &check_list, const Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        Match3G::Matcher *matcher = &matchers.emplace_back();
        matcher->shape = -1;
        int shape;
        for (const std::pair<int, int> &pos : check_list) {
            if (items[pos.first][pos.second] == Match3G::STAR)
                matcher->matches.emplace_back(pos);
            else
                shape = items[pos.first][pos.second];
        }

        for (int r = 1; r < items.size(); r++) {
            for (int q = 0; q < items[r].size(); q++) {
                if (items[r][q] != shape)
                    continue;
                matcher->matches.emplace_back(r, q);
            }
        }
    }

    bool check_special_items(std::vector<std::pair<int, int>> &check_list, Vector2D<int> &items, std::vector<Match3G::Matcher> &matchers) {
        for (int i = 0; i < check_list.size(); i++) {
            switch (items[check_list[i].first][check_list[i].second]) {
                case Match3G::BOMB:
                    get_explode_items(check_list[i], matchers);
                    break;
                case Match3G::PLANE:
                    get_shotdown_items(check_list[i], items, matchers);
                    break;
                case Match3G::STAR:
                    get_all_sameshape_items(check_list, items, matchers);
                    break;
            }
        }
        return !matchers.empty();
    }

    bool on_check_match(std::vector<std::pair<int, int>> &check_list, Vector2D<int> &items,
                        std::vector<Match3G::Matcher> &matchers, const bool &recheck = false) {

        if (!recheck && check_special_items(check_list, items, matchers))
            return true;
        
        bool done_checking = false;
        bool need_to_fill = false;
        while (!done_checking) {
            done_checking = true;
            for (std::pair<int, int> &pair : check_list) {
                if (pair.first < 1) {
                    done_checking *= true;
                    continue;
                }

                if (!need_to_fill && items[pair.first][pair.second] > 0 && items[pair.first][pair.second] <= Match3G::DIAMOND)
                    check_match_pattern(matchers, items, pair);

                if (recheck) {
                    pair.first -= 1;
                    done_checking *= pair.first < 1;
                }
            }
        }
        return !matchers.empty();
    }

    void get_holes_in_col(const Vector2D<Polygon> &items, std::vector<std::pair<int, int>> &dst, const std::vector<std::pair<int, int>> &checklist) {
        for (const std::pair<int, int> &pair : checklist) {
            for (int r = pair.first; r > 0; r--) {
                if (!items[r][pair.second].shape) {
                    dst.emplace_back(r, pair.second);
                    break;
                }
            }
        }
    }
}

static void create_grid_for_test(Vector2D<Polygon> &items) {
    Polygon q, w, e, r;
    e.shape = 0;
    q.shape = Match3G::TRIANGLE;
    w.shape = Match3G::SQUARE;
    r.shape = Match3G::ROTATED_SQUARE;
    items = {
    //   0  1  2  3  4  5  6  7  
        {e, e, e, e, e, e, e, e}, // 0
        {e, e, e, e, q, e, e, e}, // 1
        {e, e, e, e, q, e, e, e}, // 2
        {e, e, e, e, r, q, e, e}, // 3
        {e, e, e, e, q, e, e, e}, // 4
        {e, e, e, e, q, e, e, e}, // 5
        {e, e, e, e, e, e, e, e}, // 6
        {e, e, e, e, e, e, e, e}, // 7
        {e, e, e, e, e, e, e, e}, // 8
    };
    for (int r = 0; r < items.size(); r++) {
        for (int q = 0; q < items[r].size(); q++) {
            if (!items[r][q].shape) {
                create_random_item(items[r][q], r, q);
                continue;
            }
            create_item(items[r][q], r, q, items[r][q].shape);
        }
    }
    
}

bool Match3G::init(SDL_Renderer *renderer, const std::string &running_dir) {
    items = Vector2D<Polygon>(9, std::vector<Polygon>(8));
    for (int r = 0; r < items.size(); r++) {
        for (int q = 0; q < items[r].size(); q++) {
            create_random_item(items[r][q], r, q);
        }
    }
    // create_grid_for_test(items);
    mouse_selected = false;
    hovering = {-1, -1};
    direction = {0, 0};
    offs = 0;
    state = NONE_STATE;
    check_list.resize(2);
    check_list[0] = {-1, -1};
    check_list[1] = {-1, -1};
    return true;
}

void Match3G::update() {
    if (state & SWAPPING || state & REVERTING)
        swap_select_items();
    
    if (state & MATCHING)
        check_match();

    if (state & FILLING) {
        std::vector<std::pair<int, int>> check_for_holes;
        filling_removed_matches(check_for_holes);
        if (offs <= 0)
            offs = GRID_SIZE;

        get_holes_in_col(items, fill_list, check_for_holes);
        if (fill_list.empty())
            state = MATCHING;
    }
}

void Match3G::swap_select_items() {
    Polygon *selected_poly = &items[check_list[0].first][check_list[0].second];
    Polygon *targeted_poly = &items[check_list[1].first][check_list[1].second];
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
        std::swap(items[check_list[0].first][check_list[0].second], items[check_list[1].first][check_list[1].second]);
        std::swap(check_list[0], check_list[1]);
        if (state & REVERTING) {
            direction = {0, 0};
            state = NONE_STATE;
            check_list[0] = {-1, -1};
            check_list[1] = {-1, -1};
        } else {
            state = MATCHING;
        }
    }
}

void Match3G::check_match() {
    bool re_check = (!direction.x && !direction.y);
    Vector2D<int> items_grid = get_2d_shape_grid(items);
    std::vector<Matcher> matchers;
    if (!on_check_match(check_list, items_grid, matchers, re_check)) {
        if (!direction.x && !direction.y) {
            state = NONE_STATE;
            check_list.erase(check_list.begin() + 2, check_list.end());
            check_list[0] = {-1, -1};
            check_list[1] = {-1, -1};
            return;
        }

        direction.x *= -1;
        direction.y *= -1;
        offs = GRID_SIZE;
        state = REVERTING;
    } else {
        offs = GRID_SIZE;
        direction = {0, 0};
        check_list.clear();
        state = FILLING;
        remove_matches(matchers);
    }    
}

void Match3G::remove_matches(const std::vector<Matcher> &matchers) {
    for (const Matcher &matcher : matchers) {
        for (const std::pair<int, int> &match : matcher.matches) {
            items[match.first][match.second].reset_default();
            add_item_to_check_list(check_list, match);
        }
    
        switch (matcher.shape){
        case O_SHAPE:
            create_item(items[matcher.check_pos.first][matcher.check_pos.second], matcher.check_pos.first, matcher.check_pos.second, PLANE);
            break;
        case T_SHAPE:
            create_item(items[matcher.check_pos.first][matcher.check_pos.second], matcher.check_pos.first, matcher.check_pos.second, BOMB);
            break;
        case I_SHAPE_HORZ:
        case I_SHAPE_VERT:
            if (matcher.matches.size() >= 5)
                create_item(items[matcher.check_pos.first][matcher.check_pos.second], matcher.check_pos.first, matcher.check_pos.second, STAR);
            break;
        }
    }
    for (int i = 0; i < check_list.size(); i++) {
        if (items[check_list[i].first][check_list[i].second].shape)
            get_holes_in_col(items, fill_list, {{check_list[i]}});
        else
            fill_list.push_back(check_list[i]);
    }
}

bool Match3G::filling_removed_matches(std::vector<std::pair<int, int>> &check_for_holes) {
    float speed = offs - speed <= 0 ? offs : SPEED;
    offs -= SPEED;
    int ret = 1;

    for (std::vector<std::pair<int, int>>::iterator pair = fill_list.begin(); pair != fill_list.end();){
        if (!items[pair->first][pair->second].shape) {
            for (int r = pair->first - 1; r >= 0; r--) {
                if (!items[r][pair->second].shape)
                    continue;
                for (SDL_Vertex &vert : items[r][pair->second].vertices)
                    vert.position.y += speed;
    
                if (offs > 0)
                    continue;

                std::swap(items[r][pair->second], items[r + 1][pair->second]);
                if (r == 0)
                    create_random_item(items[r][pair->second], r, pair->second);

            }
        }
        if (items[pair->first][pair->second].shape) {
            check_for_holes.emplace_back(pair->first, pair->second);
            fill_list.erase(pair);
        }else
            pair++;
    }
    return fill_list.empty();
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

    SDL_FRect rect{0, 0, 0, 0};
    if (state != MATCHING && state != FILLING) {
        if (check_list[1].first >= 0 && check_list[1].second >= 0) {
            float x = SDL_min(check_list[0].second, check_list[1].second);
            float y = SDL_min(check_list[0].first, check_list[1].first);
            float w = direction.x ? (float) 2 * GRID_SIZE - PADDING + 2 : (float) GRID_SIZE - PADDING + 2;
            float h = direction.y ? (float) 2 * GRID_SIZE - PADDING + 2 : (float) GRID_SIZE - PADDING + 2;
            rect = {(x * GRID_SIZE) + PADDING - 1,
                    ((y - 1) * GRID_SIZE) + PADDING - 1,
                    w, h};
        } else if (check_list[0].first >= 0 && check_list[0].second >= 0) {
            rect = {(float) (check_list[0].second * GRID_SIZE) + PADDING - 1,
                    (float) ((check_list[0].first - 1) * GRID_SIZE) + PADDING - 1,
                    (float) GRID_SIZE - PADDING + 2,
                    (float) GRID_SIZE - PADDING + 2};
        }

        if (rect.w && rect.h) {
            SDL_SetRenderDrawColorFloat(renderer, ColorRGB::RED.r, ColorRGB::RED.g, ColorRGB::RED.b, ColorRGB::RED.a);
            SDL_RenderRect(renderer, &rect);
        }
    }

    if (!state) {
        rect = {(float) (hovering.second * GRID_SIZE) + PADDING,
                (float) ((hovering.first - 1) * GRID_SIZE) + PADDING,
                (float) GRID_SIZE - PADDING, (float) GRID_SIZE - PADDING};
        SDL_SetRenderDrawColorFloat(renderer, ColorRGB::WHITE.r, ColorRGB::WHITE.g, ColorRGB::WHITE.b, ColorRGB::WHITE.a);
        SDL_RenderRect(renderer, &rect);
    }
}

void Match3G::restart() {
    create_grid_for_test(items);
    // items.clear();
    // items = Vector2D<Polygon>(9, std::vector<Polygon>(8));
    // for (int r = 0; r < items.size(); r++) {
    //     for (int q = 0; q < items[r].size(); q++) {
    //         create_random_item(items[r][q], r, q);
    //     }
    // }
    offs            = 0;
    direction       = {0, 0};
    mouse_selected  = false;
    state           = NONE_STATE;
    hovering        = {-1, -1};
    check_list.erase(check_list.begin() + 2, check_list.end());
    check_list[0]      = {-1, -1};
    check_list[1]      = {-1, -1};
}

void Match3G::on_keydown(const SDL_Keycode &code, const SDL_Keymod &mod) {
    if (state)
        return;

    switch (code) {
    case SDLK_UP:
        show_hovering_on_keypress(hovering);
        if (!key_ctrl_swap(mod, SDL_Point{0, -1}))
            hovering.first = SDL_max(1, hovering.first - 1);
        break;
    case SDLK_DOWN:
        show_hovering_on_keypress(hovering);
        if (!key_ctrl_swap(mod, SDL_Point{0, 1}))
            hovering.first = SDL_min(items.size() - 1, hovering.first + 1);
        break;
    case SDLK_RIGHT:
        show_hovering_on_keypress(hovering);
        if (!key_ctrl_swap(mod, SDL_Point{1, 0}))
            hovering.second = SDL_min(items[0].size() - 1, hovering.second + 1);
        break;
    case SDLK_LEFT:
        show_hovering_on_keypress(hovering);
        if (!key_ctrl_swap(mod, SDL_Point{-1, 0}))
            hovering.second = SDL_max(0, hovering.second - 1);
        break;
    case SDLK_SPACE:
        show_hovering_on_keypress(hovering);
        direction = select_items_to_swap(check_list, hovering);
        if (direction.x || direction.y) {
            state = SWAPPING;
            offs = GRID_SIZE;
        }
        break;
    }
}

bool Match3G::key_ctrl_swap(const SDL_Keymod &mod, const SDL_Point &direction) {
    if (mod & SDL_KMOD_CTRL) {
        if (hovering.first + direction.y < 1 || hovering.first + direction.y >= items.size() ||
        hovering.second + direction.x < 0 || hovering.second + direction.x >= items[0].size())
            return false;
        this->state = SWAPPING;
        this->offs = GRID_SIZE;
        this->direction = direction;
        this->check_list[0] = hovering;
        this->check_list[1] = {hovering.first + direction.y, hovering.second + direction.x};
        return true;
    }
    return false;
}

void Match3G::on_mouse_motion(const int &mousex, const int &mousey) {
    if (state)
        return;

    int col = ((mousex - PADDING) / GRID_SIZE);
    int row = ((mousey - PADDING) / GRID_SIZE) + 1;
    if (col < items[0].size()) {
        hovering.first = row;
        hovering.second = col;
    }

    if (mouse_selected) {
        if (col < check_list[0].second && check_list[0].second - 1 >= 0)
            direction = {-1, 0};
        else if (col > check_list[0].second && check_list[0].second + 1 < items[0].size())
            direction = {1, 0};
        else if (row < check_list[0].first && check_list[0].first - 1 >= 0)
            direction = {0, -1};
        else if (row > check_list[0].first && check_list[0].first + 1 < items.size())
            direction = {0, 1};
        else 
            return;

        offs = GRID_SIZE;
        state = SWAPPING;
        check_list[1] = {check_list[0].first + direction.y, check_list[0].second + direction.x};
    }
}

void Match3G::on_mouse_down(const int &mouse, const int &mousex, const int &mousey) {
    if (mouse != SDL_BUTTON_LEFT || state || (mousex - PADDING) / GRID_SIZE >= items[0].size())
        return;

    mouse_selected = true;
    direction = select_items_to_swap(check_list, hovering);
    if (direction.x || direction.y) {
        state = SWAPPING;
        offs = GRID_SIZE;
    }
}

void Match3G::on_mouse_up(const int &mouse) {
    if (mouse !=SDL_BUTTON_LEFT)
        return;
    mouse_selected = false;
}
