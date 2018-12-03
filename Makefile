OBJS	= cluster.o structures.o lsh.o
OUT	= cluster
CC		= g++
FLAGS	= -c -w -Wall

all: t1

lsh.o: lsh.cpp lsh.h
	$(CC) $(FLAGS) lsh.cpp
structures.o: structures.cpp structures.h lsh.h
	$(CC) $(FLAGS) structures.cpp
cluster.o: cluster.cpp structures.h
	$(CC) $(FLAGS) cluster.cpp
t1: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

# clean up
clean:
	rm -f $(OBJS) $(OUT)
