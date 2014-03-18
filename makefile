all : my_shell

my_shell :	vprocess.o utilities.o internals.o
		g++ -o my_shell vprocess.c utilities.c internals.c
vprocess.o :	vprocess.c pHeader.h
		g++ -c vprocess.c
utilities.o :	utilities.c pHeader.h
		g++ -c utilities.c
internals.o : 	internals.c pHeader.h
		g++ -c internals.c
clean :		
		rm -rf *.o   
