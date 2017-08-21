CROSS := arm-linux-gnueabi

ifneq ($(CROSS),)
CROSS_PREFIX := $(CROSS)-
endif

CC = $(CROSS_PREFIX)gcc
#CFLAGS = -Wall -Wextra -O2
CFLAGS = -Wall -O2 -D__STANDALONE__
LFLAGS =
TARGET = ir_sensor

all: $(TARGET)

$(TARGET): ir_sensor.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) *~ *.o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

