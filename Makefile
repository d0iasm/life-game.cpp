CC="g++"
CFLAGS ="-std=c++14"

build: main.cpp
	$(CC) $(CFLAGS) -o main.o main.cpp

run: main.cpp
	$(CC) $(CFLAGS) -o main.o main.cpp
	./main.o

clean:
	rm *.o
