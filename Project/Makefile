CXX := g++
INC := -I./include
OUT := ./build/main

run:
	$(CXX) $(INC) ./src/main.cpp -o $(OUT) -lGL -ldl -lm $(shell pkg-config --libs glfw3)
	./$(OUT)

clean:
	rm -f $(OUT)
