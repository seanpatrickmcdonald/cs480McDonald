#ifndef GUI_H
#define GUI_J

#include "graphics_headers.h"
#include "imgui.h"
#include <iostream>

class GuiHandle
{
public:
  GuiHandle();
  ~GuiHandle();

  bool GuiInit();
  void NewFrame();

private:
  
};

#endif
