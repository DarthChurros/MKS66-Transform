ifdef MEM
	DATA = -g
	VG = valgrind --leak-check=full
endif

OBJECTS= main.o draw.o display.o matrix.o parser.o
CFLAGS= -Wall
LDFLAGS= -lm
CC= gcc

run: all
	$(VG) ./main script $(ARGS)

demo: all
	$(VG) ./main my_script $(ARGS)

all: $(OBJECTS)
	$(CC) $(DATA) -o main $(OBJECTS) $(LDFLAGS)

main.o: main.c display.h draw.h ml6.h matrix.h parser.h
	$(CC) $(DATA) -c main.c

draw.o: draw.c draw.h display.h ml6.h matrix.h
	$(CC) $(DATA) $(CFLAGS) -c draw.c

dsiplay.o: display.c display.h ml6.h matrix.h
	$(CC) $(DATA) $(CFLAGS) -c display.c

matrix.o: matrix.c matrix.h
	$(CC) $(DATA) $(CFLAGS) -c matrix.c

parser.o: parser.c parser.h matrix.h draw.h display.h ml6.h
	$(CC) $(DATA) $(CFLAGS) -c parser.c

clean:
	rm *.o *~
