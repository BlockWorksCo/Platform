

CC=avr-g++
LD=avr-g++

CFLAGS=-O2 -Wall -g -nostdlib -D__AVR_ATmega328P__
LDFLAGS=-g  -L./ -Wl,-Map=Output/Main.map -nostdlib

ARDUINO_BASE=/usr/share/arduino

INCLUDES+=-I./
INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/Common
INCLUDES+=-I$(ARDUINO_BASE)/hardware/arduino/cores/arduino
INCLUDES+=-I$(ARDUINO_BASE)/hardware/arduino/variants/standard
INCLUDES+=-I$(ARDUINO_BASE)/libraries/SPI

VPATH+=./
VPATH+=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)
VPATH+=$(ARDUINO_BASE)/hardware/arduino/variants/standard
VPATH+=$(ARDUINO_BASE)/libraries/SPI
VPATH+=$(ARDUINO_BASE)/hardware/arduino/cores/arduino

LIBS+=



all: Main

clean:
	-$(RM) Output/*


Main: $(OBJS)
	$(LD) $(LDFLAGS) -o Output/$@   $(addprefix Output/, $^)  $(LIBS)


.cpp.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@


.c.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@
