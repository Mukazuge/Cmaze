all: main execute

main: 
	gcc -g -o bin\juego.exe -I../SDL2/i686-w64-mingw32/include/ src/main.c -Wall -L../SDL2/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2

execute:
	bin\juego.exe
