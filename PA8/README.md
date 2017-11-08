## PA8 Sean McDonald and Bruce Martin


# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), [ASSIMP] (sudo apt-get install libassimp-dev), and [BULLET](http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Installation)

This project uses OpenGL 3.3. This project also uses stbi_image for image loading, but requires no installation on your part. 


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
### All movement is done on the box. The sphere will react dynamically to the box, cylinder, and wall. Note: If you press the sphere up against a wall with the box, it can cause it to pop through.
W - Move box in the up direction on the xz plane  
S - Move box in the down direction on the xz plane  
A - Move box in the left direction on the xz plane  
D - Move box in the right direction on the xz plane  
