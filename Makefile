CC = gcc
LDFLAGS = 
CFLAGS = -Wall -Werror -Wextra -g -std=c99 -c
SOURCES = main.c main_utils.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = viscpr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
