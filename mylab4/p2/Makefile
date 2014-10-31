CC		=gcc
DEBUG		=-U Debug
FLAGS		=-g -Wall $(DEBUG) $(EXEC) $(TEST)
EXECUTABLE	=mytalk
SOURCES		=$(EXECUTABLE).c printUsage.c parseAndCheckPort.c mallocAndCheck.c bindUDPSocket.c setChildHandler.c handleChild.c numberFromString.c setPacketHandler.c handlePacket.c noResponse.c setAlarm.c setAlarmHandler.c
HEADERS		=mytalk.h
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
	$(RM) mytalk *.o *.core core *~
	
