wish: wish.c
	$(CC) $(CFLAGS) $(CLIBS) $(CINCS) -o $@ $^

.PHONY: clean
clean:
	$(RM) *~ wish
