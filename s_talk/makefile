all: s-talk

s-talk: s-talk.o sendrecieve.o list.o inputoutput.o mutex_head.o
	gcc -o s-talk s-talk.o list.o sendrecieve.o inputoutput.o mutex_head.o -lpthread

s-talk.o: s-talk.c
	gcc -c s-talk.c

inputoutput.o: inputoutput.c
	gcc -c inputoutput.c 

sendrecieve.o: sendrecieve.c 
	gcc -c sendrecieve.c

mutex_head.o : mutex_head.c
	gcc -c mutex_head.c
clean:
	rm -f s-talk s-talk.o inputoutput.o sendrecieve.o mutex_head.o 
