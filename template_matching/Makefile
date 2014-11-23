#!/bin/sh

CC=g++

algorithm: 
	$(CC) $(OBJECTS) `pkg-config --cflags opencv` -o matching mathcing_opencv.cpp `pkg-config --libs opencv` 

clean:
	rm matching

