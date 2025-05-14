CC = gcc
CFLAGS = -Wall -O2
TARGETS = SHINOBI_BASE SHINOBI_ALT
all: $(TARGETS)

SHINOBI_BASE: shinobi_base.c
	$(CC) $(CFLAGS) -o $@ $<

#SHINOBI_ALT: shinobi_alt.c
#	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
