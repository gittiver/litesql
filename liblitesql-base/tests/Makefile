CFLAGS += -Wall -ggdb  -I../include `xml2-config --cflags` -ansi
LDFLAGS += `xml2-config --libs` -L../src -llitesql-base
OBJS=string.o
all: string

string: $(OBJS) string.o
	$(CC) $^ -o $@  $(LDFLAGS) 

clean:
	rm -f *.o test
# DO NOT DELETE
