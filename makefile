ifdef MEM
	DATA = -g
	VG = valgrind --leak-check=full
endif

all: main.o
	gcc $(DATA) -o program main.o

main.o: main.c
	gcc $(DATA) -c main.c

run:
	$(VG) ./program $(ARGS)

clean:
	rm *.o
	rm program
