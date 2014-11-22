#!/bin/sh

CC=g++

algorithm: 
	$(CC) $(OBJECTS) stencil.cpp -lm -lGL  -lGLU -lglut -lGLEW -lsfml-window -lsfml-system  -o stencil 

clean:
	rm stencil

