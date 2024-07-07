CXX := clang

all:
	mkdir -p build
	$(CXX) -std=c++23 -o build/flow -g -ggdb src/*.cpp -Wall -Wextra -lstdc++
