

CC=g++-4.8
LD=g++-4.8
RM=rm
CFLAGS=-O3 -save-temps -Wall -g -std=c++11  


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



.c.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@



