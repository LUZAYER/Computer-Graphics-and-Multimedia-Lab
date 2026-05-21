WIN_CXX := x86_64-w64-mingw32-g++
LINUX_CXX := g++

SRC := ./src/main.cpp ./src/glad.c ./src/shader.cpp ./src/renderer.cpp \
       ./src/player.cpp ./src/obstacle.cpp ./src/game.cpp
INC := -I./include
OUT := ./build

win:
	$(WIN_CXX) -fdiagnostics-color=always \
	$(INC) $(SRC) \
	-o $(OUT)/main.exe \
	-static-libstdc++ -static-libgcc \
	-Llib -lglfw3 -lopengl32 -lgdi32
	@echo "Built Windows binary: main.exe"

linux:
	$(LINUX_CXX) -fdiagnostics-color=always $(INC) $(SRC) \
	-o $(OUT)/main \
	-lGL -ldl -lm $(shell pkg-config --static --libs glfw3)
	@echo "Built Linux binary: main"

run: linux
	./build/main

clean:
	rm -f $(OUT)/main $(OUT)/main.exe
