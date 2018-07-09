CC = cc 
CFLAGS = -Isrc

APP = test.elf

all:$(APP)

test.elf:test/test.o
	$(CC) -o $@ $<

clean:
	rm -rf $(APP) ; find . -name *.o | xargs rm -rf