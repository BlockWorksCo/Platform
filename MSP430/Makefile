

CC=msp430-g++
LD=msp430-g++

CFLAGS=-mmcu=msp430g2553 -O2 -Wall -g -nostdlib
LDFLAGS=-mmcu=msp430g2553 -g  -L./ -Wl,-Map=Output/Main.map -nostdlib


INCLUDES=-I./

VPATH=./

LIBS=



all: Main

clean:
	-$(RM) Output/*


Main: $(OBJS)
	$(LD) $(LDFLAGS) -o Output/$@   $(addprefix Output/, $^)  $(LIBS)


.cpp.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@

