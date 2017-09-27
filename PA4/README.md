PA4 Engine, and other code, and Dependency ReadMe writen by Daniel Lopez, Kurt Andersen, Dustin Barnes, and others?
Modified by Sean McDonald

### This program uses Coffin.obj in /assets. It will load this asset, so do not remove it. The coffin rotates and has red walls, with an orange base.

# Dependencies, Building, and Running

## Dependency Instructions

For both of the operating systems to run this project installation of these three programs are required GLEW, GLM, and SDL2.

This project uses OpenGL 3.3.

## Building and Running

To build this project use Cmake

### CMake Instructions

The building of the project is done using CMake, installation with apt-get or brew may be necessary. Currently shaders must reside in a separate folder in the source directory, i.e. not in the build folder. See Command-Line Argument/Shaders for more.

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

# Command-Line Arguments

To pass Shaders use respective argument, followed by the string without quotation marks.
Otherwise, the default shaders will be loaded, so do NOT remove the shaders in the shaders folder, unless you have replacement shaders and pass them in.

-v vertex shader <br />
-f fragment shader <br />
 <br />
 <br />
 
