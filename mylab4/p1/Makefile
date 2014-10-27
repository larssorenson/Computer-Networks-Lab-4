CC		=gcc
DEBUG		=-U Debug
FLAGS		=-g -Wall -std=c99 $(DEBUG) $(EXEC) $(TEST)
EXECUTABLE	=fsserver
SOURCES		=$(EXECUTABLE).c printUsage.c parseAndCheckPort.c mallocAndCheck.c bindUDPSocket.c numberFromString.c
HEADERS		=fs.h
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
	
