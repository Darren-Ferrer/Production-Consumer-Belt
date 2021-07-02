mizzo: main.o io.o
	g++ main.o io.o -o mizzo -lpthread -pthread

mizzo.o: main.cpp producer.h consumer.h queue.h semaphore.h
	g++ -c -g main.cpp

io.o: io.c io.h
	g++ -c -g io.c

clean:
	rm *.o mizzo