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
