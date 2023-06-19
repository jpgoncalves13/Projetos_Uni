CC = gcc
CFLAGS1 = -O2 -Wall -gcc
CFLAGS = -Wall -g -I src -lm

OBJFOLDER= obj
BINFOLDER= bin
TMPFOLDER= tmp

_OBJFILES = execTransf fifologic message priorityQueue router routerConnections task
OBJFILES = $(patsubst %,$(OBJFOLDER)/%.o,$(_OBJFILES))


all: folders server client transfs

folders: 
	@echo " "
	@echo "INFO: making folders"
	@mkdir -p $(OBJFOLDER) $(BINFOLDER) $(TMPFOLDER)

transfs:
	@echo " "
	@echo "INFO: building transf"
	@make -C libs/SDStore-transf

server: bin/sdstored

client: bin/sdstore

$(OBJFOLDER)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BINFOLDER)/% : $(OBJFOLDER)/%.o $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ -lm

.PRECIOUS: $(OBJFOLDER)/%.o

clean:
	@echo " "
	@echo "INFO: cleaning"
	rm -rf $(OBJFOLDER) $(BINFOLDER) $(TMPFOLDER)

re: clean all