
all: a

a: a.c
	gcc a.c -o a -lGL -lglut -lGLU
