

CC=i586-mingw32msvc-g++
LD=i586-mingw32msvc-g++
RM=rm
CFLAGS=-O2 -save-temps -Wall -g -mwindows


INCLUDES+=-I./
INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/Common

VPATH+=./
VPATH+=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)

LIBS=



all: Main.exe

clean:
	-$(RM) *.o
	-$(RM) *.ii
	-$(RM) *.s
	-$(RM) Main


Main.exe: $(OBJS)
	$(LD) -o Output/$@   $(addprefix Output/, $^)  $(LIBS)


.cpp.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@



