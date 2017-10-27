## PA7 Sean McDonald and Bruce Martin


# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials), [ASSIMP] sudo apt-get install libassimp-dev

This project uses OpenGL 3.3.


## Makefile

This program is built using makefile. Create a new directory, copy the makefile into the directory, and run "make && ./Tutorial"

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
There are 2 main views, Scaled and Detailed. Scaled is a view of the System from above, where Detailed is a close up of each planet.  
    
Scaled View is useful for viewing differences in Orbit Radiuses and Speeds.  
Detailed View is useful for viewing Textures, Scales, and Rotation Speeds. The Camera is set a fixed (x, 0, z) away from the planet,  
and thus, rotations and scales will be accurate.

## Global Inputs
Tab         = Toggle between Scaled and Detailed views.   
Letter O    = Toggle Orbit Path Rendering  
Number Keys = Change speed proportionally, e.g. 1 = 1x, 2 = 2x, 3 = 3x, etc.  
Space       = Pause all motion. Note: While paused no other actions are processed. (Can only unpause)  

## Scaled View
The Scaled View has two modes: System and Inner Planets.   
Up   = Change to System View  
Down = Change to Inner Planets View  

## Detailed View
### Detailed View is a center on the target planet, with fixed translational offset. So, rotations will be equal to their actual values.
Left/Right Arrow = Change target planet, e.g. Right moves from Sun->Mercury->Venus, Left moves from Mercury->Sun->Pluto  
