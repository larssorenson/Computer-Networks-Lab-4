CC		=gcc
DEBUG		=-U Debug
FLAGS		=-g -Wall -std=c99 $(DEBUG)
EXECUTABLE	=fsserver
SOURCES		=$(EXECUTABLE).c
HEADERS		=fsserver.h
OBJECTS		=$(SOURCES:.c=.o)
RM		=rm -rf

.SUFFIXES:
.SUFFIXES: .c .o

all: $(EXECUTABLE) $(SOURCES) $(HEADERS)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $@
	$(RM) *.o

.c.o:
	$(CC) $(FLAGS) -c $< -o $@
	
clean:
	$(RM) fsserver fsclient *.o *.core core *~
	
