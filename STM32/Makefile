


CPPC=arm-none-eabi-g++
CC=arm-none-eabi-g++
LD=arm-none-eabi-g++
OBJCOPY=arm-none-eabi-objcopy 
RM=rm


CFLAGS+= -g -Ofast -ftree-loop-optimize -funroll-loops -Wall -std=c++11 -T$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/stm32_ram.ld -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4 -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard -nostdlib -fno-rtti -fno-exceptions -DSTM32F40XX -DUSE_STDPERIPH_DRIVER
LDFLAGS+=$(CFLAGS) -D__FPU_PRESENT -lc 




ifneq ($(CMSIS),OWN)
	INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include
	INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc
	INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Include
	INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Project/STM32F4xx_StdPeriph_Examples/CortexM/MPU

	VPATH+=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)
	VPATH+=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src
	
	OBJS+=system_stm32f4xx.o
	OBJS+=startup_stm32f4xx.o
	OBJS+=syscalls.o
	OBJS+=Support.o
	OBJS+=stm32f4xx_usart.o
	OBJS+=stm32f4xx_rcc.o
	OBJS+=stm32f4xx_gpio.o
	OBJS+=stm32f4xx_dac.o
	OBJS+=stm32f4xx_tim.o
	OBJS+=misc.o
endif

INCLUDES+=-I./
INCLUDES+=-I$(BLOCKWORKS_PLATFORM_BASE)/Common

VPATH+=./

LIBS+=



all: Main

clean:
	-$(RM) Output/*


Main: $(OBJS)
	$(LD) $(LDFLAGS) -o Output/$@   $(addprefix Output/, $^)  $(LIBS)
	$(OBJCOPY) -O binary Output/$@ Output/$@.bin


.cpp.o:
	-@mkdir Output
	$(CPPC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@

.s.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o Output/$@



