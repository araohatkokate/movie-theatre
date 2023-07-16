#Araohat Kokate 1001829841 Coding Assignment 7

SRC1 = Code7_1001829841.c
SRC2 = MovieTheaterLib.c
SRC3 = QueueLib.c
SRC4 = BSTLib.c
SRC5 = ListLib.c
SRC6 = StackLib.c
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
OBJ3 = $(SRC3:.c=.o)
OBJ4 = $(SRC4:.c=.o)
OBJ5 = $(SRC5:.c=.o)
OBJ6 = $(SRC6:.c=.o)

EXE = $(SRC1:.c=.e)

CFLAGS = -g

all : $(EXE)
 
$(EXE): $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6)
	gcc $(CFLAGS) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) -o $(EXE)

$(OBJ1) : $(SRC1)
	gcc -c $(CFLAGS) $(SRC1)

$(OBJ2) : $(SRC2)
	gcc -c $(CFLAGS) $(SRC2)
	
$(OBJ3) : $(SRC3)
	gcc -c $(CFLAGS) $(SRC3)
	
$(OBJ4) : $(SRC4)
	gcc -c $(CFLAGS) $(SRC4)
	
$(OBJ5) : $(SRC5)
	gcc -c $(CFLAGS) $(SRC5)
	
$(OBJ6) : $(SRC6)
	gcc -c $(CFLAGS) $(SRC6)
