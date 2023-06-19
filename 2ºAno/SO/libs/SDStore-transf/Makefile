CC = gcc
CFLAGS = -Wall -g

OBJFOLDER= ../../obj/SDStore-transf
BINFOLDER= ../../bin/SDStore-transf

_BINFILES = nop gcompress gdecompress bcompress bdecompress encrypt decrypt 
BINFILES = $(patsubst %,$(BINFOLDER)/%,$(_BINFILES))


all: folders $(BINFILES)

folders: 
	mkdir -p $(OBJFOLDER) $(BINFOLDER)

$(OBJFOLDER)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BINFOLDER)/%: $(OBJFOLDER)/%.o
	$(CC) $(CFLAGS) $^ -o $@

.PRECIOUS: $(OBJFOLDER)/%.o

clean: 
	rm -rf $(OBJFOLDER) $(BINFOLDER)

