.SUFFIXES:
.SUFFIXES: .c .cpp

CC = gcc
GCC = g++

.c:
	$(CC) -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@

.cpp:
	$(GCC) -O2 `pkg-config --cflags opencv4` $< -o $@ `pkg-config --libs opencv4`


