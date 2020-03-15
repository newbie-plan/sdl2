CFLAG=-I /usr/local/sdl2/include
LDFLAG=-L /usr/local/sdl2/lib -lSDL2

sdl:sdl.c
	gcc -g $< -o $@ ${CFLAG} ${LDFLAG}
