#ifndef APP_H
#define APP_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <filesystem>
#include "SDL3/SDL.h"
#include "SDL3/SDL_ttf.h"

template <typename T>
void free_ptr(T **ptr) {
    if (*ptr) {
        delete *ptr;
        *ptr = nullptr;
    }
}

template <typename T>
void free_arr_ptr(T **ptr) {
    if (*ptr) {
        delete [] *ptr;
        *ptr = nullptr;
    }
}

typedef struct AudioData {
    SDL_AudioSpec   spec;
    Uint8           *buffer;
    Uint32          length;
    void set_default() {
        spec = {SDL_AUDIO_UNKNOWN, 0, 0};
        if (buffer)
            SDL_free(buffer);

        buffer = nullptr;
        length = 0;
    }
} AudioData;

class Game {
    protected:
        SDL_AudioStream *aud_stream = nullptr;
        std::vector<AudioData> audios;
    public:
        Game() = default;
        virtual ~Game() {
            for (int i = 0; i < audios.size(); i++)
            audios[i].set_default();            
        }
        Game(const Game &other) = delete;
        Game(Game &&other) noexcept = delete;
        Game &operator=(const Game &other) = delete;
        Game &operator=(Game &other) noexcept = delete;
    virtual bool init(SDL_Renderer *renderer, const std::string &running_dir) {return true;}
            void set_audio_stream(SDL_AudioStream *aud_stream) {this->aud_stream = aud_stream;}
            void play_audio(const int &audio_idx) {
                if (!aud_stream || audios.empty() || !audios[audio_idx].buffer)
                    return;
                SDL_PutAudioStreamData(aud_stream, audios[audio_idx].buffer, audios[audio_idx].length);
            }
    virtual void update() {}
    virtual void render(SDL_Renderer *renderer) {}
    virtual void restart() {}
    virtual void on_keydown(const SDL_Keycode &code) {}
};

namespace ColorRGB {
    const SDL_FColor BLACK  = {0, 0, 0, 1};
    const SDL_FColor WHITE  = {1, 1, 1, 1};
    const SDL_FColor RED    = {1, 0, 0, 1};
    const SDL_FColor GREEN  = {0, 1, 0, 1};
    const SDL_FColor BLUE   = {0, 0, 1, 1};
    const SDL_FColor MEDIUM_GRAY    = {171/255.F, 171/255.F, 171/255.F, 1};
    const SDL_FColor LIGHT_GRAY     = {228/255.F, 228/255.F, 228/255.F, 1};
    const SDL_FColor DARK_GRAY      = {114/255.F, 114/255.F, 114/255.F, 1};
    
};

const int GRID_SIZE{25};
const int GRID_COLS{32};
const int GRID_ROWS{24};
const std::string APP_DIR_NAME{"mini_games"};
const std::string WICKED_MOUSE_FONT{"fonts/wicked_mouse.ttf"};
const std::string HIT_SOUND{"sounds/hit.wav"};
const std::string OBJ_FLYING_SOUND{"sounds/obj_flying_fast.wav"};
const SDL_AudioSpec AUD_SPEC_WAV{SDL_AUDIO_S16LE, 2, 44100};
#endif