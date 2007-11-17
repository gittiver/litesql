CFLAGS += -Wall -ggdb  -I../include `xml2-config --cflags` -ansi
LDFLAGS += `xml2-config --libs` -L../src -llitesql-base
all: string split

string: string.o
	$(CC) $^ -o $@  $(LDFLAGS) 

split: split.o
	$(CC) $^ -o $@  $(LDFLAGS) 

clean:
	rm -f *.o test
# DO NOT DELETE
