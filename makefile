CC = g++
CFLAGS = -lGL -lGLU -lglut
all:	HW3
HW3:	Curve.cpp
	$(CC) -o $@ $< $(CFLAGS)
clean:
	rm -rf *o HW3