WIN_CXX := x86_64-w64-mingw32-g++
LINUX_CXX := g++

SRC := ./src/main.cpp ./src/glad.c
INC := -I./include
OUT := ./build

win:
	$(WIN_CXX) -fdiagnostics-color=always \
	$(INC) $(SRC) \
	-o $(OUT)/main.exe \
	-static-libstdc++ -static-libgcc \
	-Llib -lglfw3 -lopengl32 -lgdi32
	@echo "Built Windows binary: main.exe"
	@command -v wine >/dev/null 2>&1 && wine $(OUT)/main.exe || \
		echo "Wine not installed, skipping run"

linux:
	g++ -fdiagnostics-color=always -I./include ./src/main.cpp ./src/glad.c \
	-o ./build/main \
	-lGL -ldl $(shell pkg-config --static --libs glfw3)

