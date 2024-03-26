MCU = atmega328p
DMCU = m328p
DEVICE = 

F_CPU = 16000000UL

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER = arduino

IDIR = ./include
AVR_CORE = 
AVR_PINS = 

CFLAGS = -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I$(AVR_CORE) -I$(AVR_PINS) -I$(IDIR)

TARGET = ledstrip_controller
SRC = ./src/ledstrip.c ./src/light_ws2812.c

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ 

deploy: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(DMCU) -b 57600 -P $(DEVICE) -U flash:w:$<

clean:
	rm -f $(TARGET).hex $(TARGET).elf
