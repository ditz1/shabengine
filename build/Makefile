default:
	gcc -Wall -Wextra -o game.exe ../src/main.c -I../include/ -L../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

mac: 
	clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -I ../include/ ../lib/libraylibmac.a ../src/main.c -o shabgame

windows:
	gcc -o shabgame.exe ../src/main.c -I../include/ -L../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

clean:
	del /Q game.exe
	
