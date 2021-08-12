CC=gcc
OUT=bin/gltest
all:
	rm -rf $(OUT)
	$(CC) $(pkg-config --cflags glfw3 glew) -g -o $(OUT) gltest.c -lGLEW -lglfw3 -ldl -lm -lGL -lGLU -lX11 -lpthread $(pkg-config --static --libs glfw3 glew X11)

