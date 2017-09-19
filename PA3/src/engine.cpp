
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(int argc, char **argv)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argc, argv))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  m_pauseState = false;

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update and render the graphics
    if (!m_pauseState)
    {
    	m_graphics->Update(m_DT);    	
    }
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_a)
    {
    	Object::FlipRotation(m_graphics->getCube());
    }

    if (m_event.key.keysym.sym == SDLK_q)
    {
      Object::ToggleRotationPause(m_graphics->getCube());
    }

    if (m_event.key.keysym.sym == SDLK_d)
    {
    	Object::FlipOrbit(m_graphics->getCube());
    }

    if (m_event.key.keysym.sym == SDLK_e)
    {
      Object::ToggleOrbitPause(m_graphics->getCube());
    }

    if (m_event.key.keysym.sym == SDLK_LEFT)
    {
      Object::SetOrbitDirection(m_graphics->getCube(), -1);
    }

    if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
      Object::SetOrbitDirection(m_graphics->getCube(), 1);
    }

    if (m_event.key.keysym.sym == SDLK_DOWN)
    {
      Object::SetRotationDirection(m_graphics->getCube(), -1);
    }

    if (m_event.key.keysym.sym == SDLK_UP)
    {
      Object::SetRotationDirection(m_graphics->getCube(), 1);
    }
    /*
    if (m_event.key.keysym.sym == SDLK_SPACE)
    {
      m_pauseState ^= true;
    }
    */
  }

  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    if (m_event.button.button == 1)
    {
      Object::FlipRotation(m_graphics->getCube());
    }

    if (m_event.button.button == 3)
    {
      Object::FlipOrbit(m_graphics->getCube());
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
