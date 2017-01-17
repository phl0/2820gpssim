PRG            = 2820gpssim
OBJ            = 2820gpssim.o
MCU_TARGET     = attiny2313
PROGRAMMER     = usbasp        # or stk500 etc.
AVRDUDE_TARGET = t2313
PORT           = /dev/ttyUSB0
#F_CPU          = 20000000
HFUSE          = 0xdf
LFUSE          = 0xe4

OPTIMIZE       = -Os

DEFS           =
LIBS           =

CC             = avr-gcc

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf hex

hex:  $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) *.o *.map *.elf *.hex

program:
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) \
	 -U flash:w:$(PRG).hex

fuses:
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) \
		-U hfuse:w:$(HFUSE):m \
		-U lfuse:w:$(LFUSE):m
