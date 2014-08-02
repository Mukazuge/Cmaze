ifdef SystemRoot
	BIN = bin\juego.exe
	LIBS = -L../SDL2/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2
	INC = -I../SDL2/i686-w64-mingw32/include/
	DEL = del bin\*.exe
	FixPath = $(subst /,\,$1)
else
	ifeq ($(shell uname), Linux)
		RM = rm -f
		FixPath = $1
	else
		BIN = juego.app
		LIBS = -framework SDL2 -F /Library/Frameworks -F ~/Library/Frameworks/
		INC = -I ~/Library/Frameworks/SDL2.framework/Headers
		DEL = rm -rf juego.app*
	endif
endif

ALL: CLEAN BUILD

CLEAN:
	$(DEL)

BUILD: 
	echo gcc -g -o $(BIN) $(INC) src/main.c -Wall $(LIBS)
	gcc -g -o $(BIN) $(INC) src/main.c -Wall $(LIBS)

EXECUTE:
	$(BIN)	
