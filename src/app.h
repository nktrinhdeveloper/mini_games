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

template <typename T>
using Vector3D = std::vector<std::vector<std::vector<T>>>;

template <typename T>
using Vector2D = std::vector<std::vector<T>>;

typedef struct AudioData {
    SDL_AudioSpec   spec;
    Uint8           *buffer;
    Uint32          length;
    void set_default();
} AudioData;

typedef struct Polygon {
    int                     shape;
    std::vector<SDL_Vertex> vertices;
    std::vector<int>        indices;
    void reset_default();
    void create_regular_polygon(const int &segments, const SDL_FPoint &center, const float &radius, const SDL_FColor &color, const float &roate_angle);
    void create_irregular_polygon(const SDL_FRect &dst, const SDL_FColor &color, const Vector2D<int> &pattern);
} Polygon;

typedef enum Tribool {
    FAILED = -1,
    NORMAL = 0,
    SUCCESSED = 1
} Tribool;

class Clock {
private:
    Uint64 last_ticks;
    Uint32 tpf;
public:
    static const Uint32 DEFAULT_TPF{16};
        Clock() = default;
        ~Clock() = default;
        Clock(const Clock &other) = delete;
        Clock(Clock &&other) noexcept = delete;
        Clock &operator=(const Clock &other) = delete;
        Clock &operator=(Clock &&other) noexcept = delete;
    void start_clock();
    void ticking();
    const Uint32 &get_tpf() const;
};

class Game {
protected:
    SDL_AudioStream *aud_stream;
    Clock           *clock;
    bool            alive;
    std::vector<AudioData> audios;
public:
    Game();
    virtual ~Game();
    Game(const Game &other) = delete;
    Game(Game &&other) noexcept = delete;
    Game &operator=(const Game &other) = delete;
    Game &operator=(Game &other) noexcept = delete;

    void set_audio_stream(SDL_AudioStream *aud_stream);
    void set_clock(Clock *clock);
    void play_audio(const int &audio_idx) ;
    virtual bool init(SDL_Renderer *renderer, const std::string &running_dir) {return true;}
    virtual void update() {}
    virtual void render(SDL_Renderer *renderer) {}
    virtual void restart() {}
    virtual void on_keydown(const SDL_Keycode &code) {}
    virtual void on_mouse_motion(const int &mousex, const int &mousey) {}
    virtual void on_mouse_down(const int &mouse) {}
    virtual void on_mouse_up(const int &mouse) {}
};

namespace ColorRGB {
    const SDL_FColor BLACK  = {0, 0, 0, 1};
    const SDL_FColor WHITE  = {1, 1, 1, 1};
    const SDL_FColor RED    = {1, 0, 0, 1};
    const SDL_FColor ORANGE = {1, 127/255.F, 0, 1};
    const SDL_FColor YELLOW = {1, 1, 0, 1};
    const SDL_FColor GREEN  = {0, 1, 0, 1};
    const SDL_FColor BLUE   = {0, 0, 1, 1};
    const SDL_FColor INDIGO = {75/255.F, 0, 130/255.F, 1};
    const SDL_FColor VIOLET = {148/255.F, 0, 211/255.F, 1};
    const SDL_FColor MEDIUM_GRAY    = {171/255.F, 171/255.F, 171/255.F, 1};
    const SDL_FColor LIGHT_GRAY     = {228/255.F, 228/255.F, 228/255.F, 1};
    const SDL_FColor DARK_GRAY      = {114/255.F, 114/255.F, 114/255.F, 1};
    
};

namespace AppConst{
    const int GRID_SIZE{25};
    const int GRID_COLS{32};
    const int GRID_ROWS{24};
    const float PI{3.14F};
    const std::string APP_DIR_NAME{"mini_games"};
    const std::string WICKED_MOUSE_FONT{"fonts/wicked_mouse.ttf"};
    const std::string HIT_SOUND{"sounds/hit.wav"};
    const std::string OBJ_FLYING_SOUND{"sounds/obj_flying_fast.wav"};
    const SDL_AudioSpec AUD_SPEC_WAV{SDL_AUDIO_S16LE, 2, 44100};
}
#endif