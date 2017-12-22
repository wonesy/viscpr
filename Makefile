CC = gcc
LDFLAGS = -lncurses
CFLAGS = -Wall -Werror -Wextra -g -std=c99 -c
DEFINES = -D_POSIX_SOURCE
SOURCES = $(wildcard *c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = viscpr

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -largp
endif

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
