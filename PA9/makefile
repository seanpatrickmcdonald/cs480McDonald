# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=@g++
	LIBS=-lSDL2 -lGLEW -lGL -lassimp -lBulletDynamics -lBulletSoftBody -lLinearMath -lBulletCollision
# Mac
else
	CC=clang++
	LIBS=-lSDL2 -framework OpenGL -framework Cocoa -lGLEW -stdlib=libc++
endif

# Paths, use this to set Include directories, or library directories; uncomment in all: if used
#PATHI=-I/usr/local/include/ 
#PATHL=-L/usr/local/lib/

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

#SRC DIR
SRC_DIR=../src

#CUR_DIR
CUR_DIR = $(shell pwd)

#OBJ FILES
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES   = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRC_FILES))

# Point to includes of local directories
INCLUDES=-I../include -I/usr/include/bullet

#Color Variables
LNK_COLOR   = \033[0;36m
OBJ_COLOR   = \033[0;32m
NO_COLOR    = \033[m


all: $(OBJ_FILES)
	@printf "$(LNK_COLOR)Linking executable Tutorial$(NO_COLOR)"
	$(CC) $(CXXFLAGS) -o Tutorial $^ $(LIBS) 
	@printf "\rBuilt Target Tutorial                                 \n"

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "$(OBJ_COLOR)Building $@ $(NO_COLOR)"
	$(CC) $(CXXFLAGS) -c -o $@ $< $(INCLUDES) 
	@printf "\r$(OBJ_COLOR)Built $@                                   \n"


clean:
	-@if rm ../src/*.o Tutorial imgui.ini 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi

print:
	echo $(O_FILES)