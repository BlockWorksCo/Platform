

CC=g++
LD=g++
RM=rm
CFLAGS=-O2 -save-temps -Wall -g 


INCLUDES+=-I./ -I$(ROCKETBLOX)/Platform/$(PLATFORM)/ -I$(ROCKETBLOX)/Platform
VPATH=./ $(ROCKETBLOX)/Platform/$(PLATFORM)

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



