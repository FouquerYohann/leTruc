
GCC=gcc
GPP=g++

FLAGS=-Wall -g -D DEBUG
#FLAGS=-Wall -g -fdiagnostics-color=auto -D DEBUG
#FLAGS=-Wall -g -fdiagnostics-color=auto
GCCFLAGS=  -g -pthread -lrt
CPPFLAGS= 
LIBS= -lm

# User definitions must be here

#executable
EXEC = serveur

#.h
INCS = serveur.h \
		thread.h \
		parser.h \
		mime.h \
		log.h

#.c
SOURCES = serveur.c \
		thread.c  \
		parser.c \
		mime.c \
		log.c

OBJS = $(SOURCES:.c=.o)


# Building the world
all: $(EXEC) 

$(EXEC): $(INCS) $(OBJS) 
	$(GCC) $(GCCFLAGS) $(OBJS) $(LIBS) -o $(EXEC) 


.SUFFIXES:
.SUFFIXES: .c .cc .o

.cc.o:
	$(GPP) $(FLAGS) -c $<

.c.o:
	$(GCC) $(FLAGS) -c $<


# Clean up
clean:
	rm -f *~ .*~ \#*\# *.o *.tmp* Chaine*.txt *.png *.plt Reseau*.txt
	rm -f $(EXEC) 

# Dependencies
depend: 

	$(GCC) -M $(CPPFLAGS) $(SOURCES) > .depend
include .depend



