CROSS := arm-linux-gnueabi

ifneq ($(CROSS),)
CROSS_PREFIX := $(CROSS)-
endif

CC = $(CROSS_PREFIX)gcc
#CFLAGS = -Wall -Wextra -O2
CFLAGS = -Wall -O2
LFLAGS =
TARGET = irmeasure

all: $(TARGET)

$(TARGET): irmeasure.o led.o button.o lcd.o ir_sensor.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) *~ *.o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

