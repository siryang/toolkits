
SOURCE=../code/*.cpp
LIBS=-lpthread

all:$(SOURCE)
	g++ -g -c $(SOURCE)
	g++ *.o $(LIBS) -o thread_pool.out 
	rm *.o
.PHONY:all

