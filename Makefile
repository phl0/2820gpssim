PRG            = 2820gpssim
OBJ            = 2820gpssim.o
MCU_TARGET     = attiny2313
PROGRAMMER     = usbasp
AVRDUDE_TARGET = t2313
#F_CPU          = 20000000

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
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) \
	 -U flash:w:$(PRG).hex

