CC = gcc
CFLAGS = -Wall

#generazione exe
esame: compito.o grafi.o coda.o
	$(CC) $(CFLAGS) -o esame $^

#generazione file oggetto
compito.o: compito.c grafi.h coda.h node.h
	$(CC) $(CFLAGS) -c $^

grafi.o: grafi.c grafi.h
	$(CC) $(CFLAGS) -c $^

coda.o: coda.c coda.h
	$(CC) $(CFLAGS) -c $^