## PA10 Sean McDonald and Bruce Martin

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), [ASSIMP](http://assimp.org/lib_html/install.html), and [BULLET](http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Installation)

This project uses OpenGL 3.3. This project also uses [stbi_image](https://github.com/nothings/stb/blob/master/stb_image.h) for image loading, [JSON for Modern C++](https://github.com/nlohmann/json) for JSON config loading, and [ImGui](https://github.com/ocornut/imgui) for the Gui, but neither require installation on your part.

## Makefile

### This program is built using makefile. Create a new directory, copy the makefile into the directory, and run "make && ./Tutorial"

```bash
mkdir build
cp makefile build/makefile
cd build
make && ./Tutorial
```


# CommandLine Arguments
## Shaders
### Enter the appropriate command, followed by the filename of the shader to load. Can load any of the following in any order.

-v = vertex shader    
-f = fragment shader    

```bash
./Tutorial -v "vertex shader name" -f "fragment shader name"
```
# Input
### Plunger
Space or Button - Launch Ball

### Flippers 
A or Left Arrow  - Hit Left Flipper
D or Right Arrow - Hit Right Flipper

### Buggy Ball Reset
R - Reset ball position/status

### Shader Change - Shader starts at per fragment and will move between it and the per vertex
P - Toggle current Shader Type
