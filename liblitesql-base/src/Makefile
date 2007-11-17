CFLAGS += -Wall -ggdb  -I../include `xml2-config --cflags` -ansi
LDFLAGS += `xml2-config --libs`
OBJS=memory.o xml.o string.o processxml.o
all: liblitesql-base test 

liblitesql-base: string.o memory.o xml.o processxml.o
	$(AR) -rcv liblitesql-base.a $^

test: $(OBJS) test.o
	$(CC) $^ -o $@  $(LDFLAGS)

clean:
	rm -f *.o test
# DO NOT DELETE
