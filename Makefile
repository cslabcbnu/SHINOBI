CC = gcc
CFLAGS = -Wall -O2
TARGET = SHINOBI
SRCS = main.c
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
