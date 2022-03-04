## Use EXE to represent the name of the program that we're building.
EXE     = client

## SRC is the list of .c files we need to build our program..
SRC     = client.c UI_library.c
CC              = gcc
CC_OPTIONS      = -g -Wall -pedantic -std=c99
LIBS            = -lm -lSDL2_ttf -lSDL2 -lpthread

## Define OBJ, which symbolizes all the .o files that the compiler needs
##      to create and then link together in order to create the program.
## Define OBJ, which symbolizes all the .o files that the compiler needs
##      to create and then link together in order to create the program.
bin:
	$(CC) $(SRC) $(CC_OPTIONS) -o $(EXE) $(LIBS)  $(EXE2)

bin1:
	gcc server.c playerManagement.c board_library.c  -g -Wall  -lpthread  -o server


c:
	./$(EXE) 127.0.0.1

gdb:
	gdb -ex=r --args ./$(EXE) 

clean:
	rm -f $(EXE) *.o a.out core


depend:         Makefile $(SRC)
	makedepend $(INCLUDES) $(SRC)
