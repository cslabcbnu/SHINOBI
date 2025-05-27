CC = gcc
CFLAGS = -Wall
SRCDIR = src
BINDIR = bin

TARGETS = $(BINDIR)/SHINOBI_BASE $(BINDIR)/SHINOBI_MT

all: $(TARGETS)

$(BINDIR)/SHINOBI_BASE: $(SRCDIR)/shinobi_base.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BINDIR)/SHINOBI_MT: $(SRCDIR)/shinobi_mt.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(BINDIR)/SHINOBI_BASE $(BINDIR)/SHINOBI_MT

