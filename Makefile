
SRC = $(wildcard *.c)
TARGETS = $(SRC:%.c=%)

$(TARGETS): $(SRC)
	$(CC) $(^) -o $(@)
	./$(@)

clean:
	rm -f $(TARGETS)

