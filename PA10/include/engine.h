#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"



class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(int argc, char **argv);
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

    static unsigned int score;
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    unsigned int num_balls = 3;

    bool left_flipper_hit = false;
    bool right_flipper_hit = false;
    
    float left_flipper_angle = 0;
    float right_flipper_angle = 0;

    unsigned int left_flipper_dt = 0;
    unsigned int right_flipper_dt = 0;

    void LeftFlipperHit();
    void LeftFlipperUpdate();
    void RightFlipperHit();
    void RightFlipperUpdate();
};



#endif // ENGINE_H
