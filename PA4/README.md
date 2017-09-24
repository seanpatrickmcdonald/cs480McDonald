PA4 Engine, and other code, and Dependency ReadMe writen by Daniel Lopez, Kurt Andersen, Dustin Barnes, and others?
Modified by Sean McDonald

##This Project will display a coffin esque box, with red walls and orange base

# Dependencies, Building, and Running

## Dependency Instructions

For both of the operating systems to run this project installation of these three programs are required GLEW, GLM, and SDL2.

This project uses OpenGL 3.3.

## Building and Running

To build this project there are two options. One is to use CMake, the second option is to use the provided Makefile which is used as usual.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.

### CMake Instructions

The building of the project is done using CMake, installation with apt-get or brew may be necessary. Currently shaders must reside in a separate folder in the source directory, i.e. not in the build folder. See Command-Line Argument/Shaders for more.

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

### Makefile Instructions

The makefile works as expected and must be updated with new files added in.

# Command-Line Arguments

To pass Shaders use respective argument, followed by the string without quotation marks.
Otherwise, the default shaders will be loaded, so do NOT remove the shaders in the shaders folder, unless you have replacement shaders and pass them in.

-v vertex shader <br />
-f fragment shader <br />
 <br />
 <br />
 