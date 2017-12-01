
#include "engine.h"


unsigned int Engine::score = 0;

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

    left_flipper_hit = false;
    right_flipper_hit = false;

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    LeftFlipperUpdate();
    RightFlipperUpdate();

    // Update and render the graphics
    m_graphics->Update(m_DT);

    glm::vec4 vertex(0.0, 0.0, 0.0, 1.0);
    glm::vec3 sphere_center = glm::vec3(m_graphics->getPhysicsManager()->GetModelMatrixAtIndex(0) * vertex);
    //std::cout << "Center x: " << sphere_center.x << " y: " << sphere_center.y << " z: " << sphere_center.z << std::endl;

    if (sphere_center.x < 0.3 && sphere_center.z > 0.86)
    {
      if (num_balls == 0)
      {
        //m_graphics->Render(score, num_balls, true);
        num_balls = 3;
        score = 0;
      }

      else
      {
        num_balls--;
      }

      m_graphics->getPhysicsManager()->ResetBall();
      m_graphics->m_plunger->triggeringPlunger = true;
      m_graphics->m_plunger->displacement = 1;
    }


    m_graphics->Render(score, num_balls);

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
    if (m_event.key.keysym.sym == SDLK_p)
    {
      m_graphics->ToggleShader();
    }

    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_r)
    {
      m_graphics->getPhysicsManager()->ResetBall();
      m_graphics->m_plunger->triggeringPlunger = true;
      m_graphics->m_plunger->displacement = 1;
    }

    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_SPACE)
    {
      m_graphics->m_plunger->triggeringPlunger = false;
      m_graphics->getPhysicsManager()->ApplyForceAtIndex(btVector3(0, 0, -.18), 0);
    }


  }

  
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);

  if (keystate == NULL)
  {
      std::cout << "keystate null" << std::endl;
  }

  if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])
  {
    right_flipper_hit = true;
    //RightFlipperHit();
  }  

  if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT])
  {
    left_flipper_hit = true;
    //LeftFlipperHit();
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

void Engine::LeftFlipperHit()
{
  left_flipper_angle += 0.05;
  m_graphics->getPhysicsManager()->RotateFlipper(6, left_flipper_angle);
}

void Engine::RightFlipperHit()
{
  right_flipper_angle += -0.05;
  m_graphics->getPhysicsManager()->RotateFlipper(9, right_flipper_angle);  
}

void Engine::LeftFlipperUpdate()
{
  if (left_flipper_hit && left_flipper_dt == 0)
  {  
    left_flipper_dt++;
  }

  if (left_flipper_dt > 0 && left_flipper_dt < 6 && left_flipper_angle < 0.9)
  {
    left_flipper_angle += 0.25;
    left_flipper_dt++;
  }

  else
  {
    if (left_flipper_angle > 0 && !left_flipper_hit)
      left_flipper_angle += -0.05;

    left_flipper_dt = 0;
  }

  m_graphics->getPhysicsManager()->RotateFlipper(1, left_flipper_angle);


}

void Engine::RightFlipperUpdate()
{
  if (right_flipper_hit && right_flipper_dt == 0)
  {  
    right_flipper_dt++;
  }

  if (right_flipper_dt > 0 && right_flipper_dt < 6 && right_flipper_angle > -0.9)
  {
    right_flipper_angle += -0.25;
    right_flipper_dt++;
  }

  else
  {
    if (right_flipper_angle < 0 && !right_flipper_hit)
      right_flipper_angle += 0.05;

    right_flipper_dt = 0;
  }

  m_graphics->getPhysicsManager()->RotateFlipper(2, right_flipper_angle);
}