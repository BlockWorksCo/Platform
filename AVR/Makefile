

CC=avr-g++
LD=avr-g++

CFLAGS=-O2 -Wall -g -nostdlib
LDFLAGS=-g  -L./ -Wl,-Map=Output/Main.map -nostdlib


INCLUDES+=-I./
INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/Common

VPATH+=./
VPATH+=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)

LIBS+=



all: Main

clean:
	-$(RM) Output/*


Main: $(OBJS)
	$(LD) $(LDFLAGS) -o Output/$@   $(addprefix Output/, $^)  $(LIBS)


.cpp.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@

