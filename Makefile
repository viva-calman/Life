TARGET=life
LIBS = -L/usr/lib -lSDL
IDIR = -I/usr/include/SDL
CC = g++ -O3 

all:
	$(CC) -c life.c -o life.o $(IDIR)
	$(CC) -o $(TARGET) life.o $(LIBS)
	strip $(TARGET)
clean:
	rm -f ./*.o $(TARGET) core
