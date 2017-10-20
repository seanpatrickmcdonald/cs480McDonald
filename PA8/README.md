## PA5 Sean McDonald and Bruce Martin

# Building and Running
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
