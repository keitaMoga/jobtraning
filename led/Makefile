CROSS := arm-linux-gnueabi

ifneq ($(CROSS),)
CROSS_PREFIX := $(CROSS)-
endif

CC = $(CROSS_PREFIX)gcc
#CFLAGS = -Wall -Wextra -O2
CFLAGS = -Wall -O2 -D__STANDALONE__
LFLAGS =
TARGET = led

all: $(TARGET)

$(TARGET):led.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) *~ *.o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

