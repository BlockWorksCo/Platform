

CC=avr-g++
LD=avr-g++
OBJCOPY=avr-objcopy


CFLAGS=-O3 -std=c++11 -Wall -g -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105
LDFLAGS=-g  -L./ -Wl,-Map=Output/Main.map,--gc-sections  -mmcu=atmega328p 

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


OBJS+=SPI.o
OBJS+=wiring_digital.o
OBJS+=wiring.o




all: Main

clean:
	-$(RM) Output/*


Main: $(OBJS)
	$(LD) $(LDFLAGS) -o Output/$@   $(addprefix Output/, $^)  $(LIBS)
	#$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 C:\Users\STEVE_~1\AppData\Local\Temp\build2086817999978773033.tmp\Max7219.cpp.elf C:\Users\STEVE_~1\AppData\Local\Temp\build2086817999978773033.tmp\Max7219.cpp.eep 
	$(OBJCOPY) -O ihex -R .eeprom Output/$@ Output/$@.hex



.cpp.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@


.c.o:
	-@mkdir Output
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@
