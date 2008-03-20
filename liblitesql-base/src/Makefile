CFLAGS += -Wall -ggdb  -I../include `xml2-config --cflags` -ansi
LDFLAGS += `xml2-config --libs`
OBJS=memory.o xml.o string.o processxml.o split.o internal.o
all: liblitesql-base test 

liblitesql-base: $(OBJS)
	$(AR) -rcv liblitesql-base.a $^

test: $(OBJS) test.o
	$(CC) $^ -o $@  $(LDFLAGS)

clean:
	rm -f *.o test
# DO NOT DELETE

xml.o: processxml.h
