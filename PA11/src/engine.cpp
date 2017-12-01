
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
  if (m_window)
    delete m_window;
  if (m_graphics)
  	delete m_graphics;

  m_window = nullptr;
  m_graphics = nullptr;
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argc, argv, m_window->GetWindow()))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

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
    m_graphics->getCamera()->Update(m_DT);
    m_graphics->Update(m_DT);
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

  if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }

    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_r)
    {
        m_graphics->getPhysicsManager()->ApplyForceAtIndex(btVector3(0, 0, 0.25), 0);
    }

    if (m_event.key.keysym.sym == SDLK_w)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_Z] = -1;
    }

    if (m_event.key.keysym.sym == SDLK_a)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_X] = -1;
    }

    if (m_event.key.keysym.sym == SDLK_s)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_Z] = 1;
    }

    if (m_event.key.keysym.sym == SDLK_d)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_X] = 1;
    }

    if (m_event.key.keysym.sym == SDLK_q)
    {
        m_graphics->getCamera()->movement[CAMERA_ROTATE][CAMERA_X] = 1;
    }

    if (m_event.key.keysym.sym == SDLK_e)
    {
        m_graphics->getCamera()->movement[CAMERA_ROTATE][CAMERA_X] = -1;
    }
  }

  if (m_event.type == SDL_KEYUP)
  {

    if (m_event.key.keysym.sym == SDLK_w)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_Z] = 0;
    }

    if (m_event.key.keysym.sym == SDLK_a)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_X] = 0;
    }

    if (m_event.key.keysym.sym == SDLK_s)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_Z] = 0;
    }

    if (m_event.key.keysym.sym == SDLK_d)
    {
        m_graphics->getCamera()->movement[CAMERA_TRANSLATE][CAMERA_X] = 0;
    }

    if (m_event.key.keysym.sym == SDLK_q)
    {
        m_graphics->getCamera()->movement[CAMERA_ROTATE][CAMERA_X] = 0;
    }

    if (m_event.key.keysym.sym == SDLK_e)
    {
        m_graphics->getCamera()->movement[CAMERA_ROTATE][CAMERA_X] = 0;
    }
  }
  
  /*  
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);

  if (keystate == NULL)
  {
      std::cout << "keystate null" << std::endl;
  }


  if (keystate[SDL_SCANCODE_W])
  {
    camera_translation += glm::vec3(0, 0, -camera_speed);
  }  

  if (keystate[SDL_SCANCODE_A])
  {
    camera_translation += glm::vec3(-camera_speed, 0, 0);
  }

  if (keystate[SDL_SCANCODE_S])
  {
    camera_translation += glm::vec3(0, 0, camera_speed);
  }

  if (keystate[SDL_SCANCODE_D])
  {
    camera_translation += glm::vec3(camera_speed, 0, 0);
  }
  
  std::cout << camera_translation.x << " " << camera_translation.y << " " << camera_translation.z << std::endl;

  m_graphics->getCamera()->Translate(camera_translation);
  */

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

