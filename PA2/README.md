PA2 Engine, and other code, and Dependency ReadMe writen by Daniel Lopez, Kurt Andersen, Dustin Barnes, and others?
Modified by Sean McDonald

# Dependencies, Building, and Running

## Dependency Instructions

For both of the operating systems to run this project installation of these three programs are required GLEW, GLM, and SDL2.

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at Using OpenGL 2.7

### Ubuntu/Linux

sudo apt-get install libglew-dev libglm-dev libsdl2-dev

### Mac OSX

Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.

brew install glew glm sdl2
## Building and Running

To build this project there are two options. One is to use CMake which makes including new libraries easier, and handles new files added automatically to the src and include directory. CMake is a small new learning curve but makes things easier in the future. The second option is to use the provided Makefile which is used as usual.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.

### CMake Instructions

The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the add_custom_target function place

COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
mkdir build
cd build
cmake ..
make
./Tutorial
Makefile Instructions

The makefile works as expected and must be updated with new files added in.

# Command-Line Arguments

To pass Shaders use respective argument, followed by the string without quotation marks.
Otherwise, the default shaders will be loaded, so do NOT remove the shaders in the shaders folder, unless
you have replacement shaders and pass them in.

-v vertex shader <br />
-f fragment shader <br />
 <br />
 <br />
 
# Input
To modify orbit or rotation press the indicated key

a     - reverses rotation <br />
q     - pauses rotation <br />
d     - reverses orbit <br />
e     - pauses orbit <br />

mouse buttons: <br />
Left  - reverses rotation <br />
Right - reverses orbit <br />
