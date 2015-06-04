CC = gcc
LDFLAGS = -lncurses
CFLAGS = -Wall -Werror -Wextra -g -std=c99 -c
DEFINES = -D_POSIX_SOURCE
SOURCES = main.c main_utils.c visual.c visual_utils.c decompress.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = viscpr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
