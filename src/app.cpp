#include "app.h"

void Clock::start_clock() {
    last_ticks = SDL_GetTicks();
    tpf = 0;
}
void Clock::ticking() {
    Uint64 now = SDL_GetTicks();
    tpf = now - last_ticks;
    last_ticks = now;
    if (tpf < Clock::DEFAULT_TPF)
        SDL_Delay(Clock::DEFAULT_TPF - tpf);
}
const Uint32 &Clock::get_tpf() const {return tpf;}

void AudioData::set_default() {
    spec = {SDL_AUDIO_UNKNOWN, 0, 0};
    if (buffer)
        SDL_free(buffer);

    buffer = nullptr;
    length = 0;
}

void Polygon::create_regular_polygon(const int &segments, const SDL_FPoint &center, const float &radius, const SDL_FColor &color, const float &roate_angle) {
    if (!segments || !radius)
        return;

    float angle = (2 * AppConst::PI) / segments;
    vertices.resize(segments + 1);
    vertices[0].position = center;
    vertices[0].color = color;
    indices.push_back(0);
    for (int i = 1; i <= segments; i++) {
        if (i > 2)
            indices.insert(indices.end(), {0, i - 1});

        indices.push_back(i);
        if (i == segments)
            indices.insert(indices.end(), {0, i, 1});

        float omega = ((i - 1) * angle) + roate_angle;
        vertices[i].position.x = center.x + SDL_floorf((SDL_cosf(omega) * radius));
        vertices[i].position.y = center.y + SDL_floorf((SDL_sinf(omega) * radius));
        vertices[i].color = color;
    }
}

void Polygon::create_irregular_polygon(const SDL_FRect &dst, const SDL_FColor &color, const Vector2D<int> &pattern) {
    if (pattern.empty())
        return;

    float grid_w, grid_h;
    grid_h = dst.h / pattern.size();
    grid_w = dst.w / pattern[0].size();
    int index = 0;
    for (int r = 0; r < pattern.size(); r++) {
        for (int q = 0; q < pattern[r].size(); q++) {
            if (!pattern[r][q])
                continue;

            if (index > 2)
                indices.insert(indices.end(), {index - 2, index - 1});
            indices.push_back(index++);
            SDL_Vertex *vert = &vertices.emplace_back();
            vert->position.x = dst.x + (grid_w / 2) + (grid_w * q);
            vert->position.y = dst.y + (grid_h / 2) + (grid_h * r);
            vert->color = color;
        }
    }
}

void Polygon::reset_default() {
    shape = 0;
    std::vector<SDL_Vertex>().swap(vertices);
    std::vector<int>().swap(indices);
}


Game::Game() : aud_stream(nullptr), clock(nullptr), alive(true) {}
      
Game::~Game() {
    for (int i = 0; i < audios.size(); i++)
    audios[i].set_default();            
}

void Game::set_audio_stream(SDL_AudioStream *aud_stream) {this->aud_stream = aud_stream;}

void Game::set_clock(Clock *clock) {this->clock = clock;}

void Game::play_audio(const int &audio_idx) {
    if (!aud_stream || audios.empty() || !audios[audio_idx].buffer)
        return;
    SDL_PutAudioStreamData(aud_stream, audios[audio_idx].buffer, audios[audio_idx].length);
}
