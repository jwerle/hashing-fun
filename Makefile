
SRC = $(wildcard *.c)
TARGETS = $(SRC:%.c=%)
CFLAGS += -std=c99 -pedantic -Wall

all: $(TARGETS)

$(TARGETS): $(SRC)
	$(CC) $(@:%=%.c) -o $(@)
	./$(@:.c=)

clean:
	rm -f $(TARGETS)

