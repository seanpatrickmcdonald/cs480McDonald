#ifndef GUI_H
#define GUI_H

#include "graphics_headers.h"
#include "imgui.h"
#include <iostream>
#include <SDL2/SDL.h>

class GuiHandle
{
public:
  GuiHandle();
  ~GuiHandle();

  bool GuiInit(SDL_Window*);
  void NewFrame(SDL_Window*);

private:
  
};

#endif
