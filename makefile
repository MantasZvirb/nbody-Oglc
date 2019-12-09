Files = Entry.c Physics.c Physics.h Render.c Render.h Utils.c Utils.h MatrixMath.c MatrixMath.h Defines.h
Libs = -lSDL2 -lGL -lm -lGLU
Flags = -Ofast -s -w
Command = $(Files) $(Libs) $(Flags)
all:
	gcc $(Command)
	rm -f *.gch