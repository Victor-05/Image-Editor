CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: testare.c
	$(CC) $(CFLAGS) testare.c -o image_editor

pack:
	zip -FSr 313CA_SerbanVictorGabriel_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
