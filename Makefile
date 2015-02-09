
SRC = a.c
MAIN = test

$(MAIN): $(SRC)
	$(CC) $(^) -o $(@)
	./$(@)

clean:
	rm -f $(MAIN)

