

CC=arm-linux-gnueabihf-g++
LD=arm-linux-gnueabihf-g++
RM=rm
CFLAGS=-O3 -std=c++11  -save-temps -Wall -g 


INCLUDES+=-I./
INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/Common

VPATH+=./
VPATH+=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)

LIBS=-lpthread



all: Main

clean:
	-$(RM) *.o
	-$(RM) *.ii
	-$(RM) *.s
	-$(RM) Main


Main: $(OBJS)
	$(LD) -g -o Output/$@   $(addprefix Output/, $^)  $(LIBS)


.cpp.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@



