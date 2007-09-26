CFLAGS += -Wall -ggdb  -I../include `xml2-config --cflags` -ansi
LDFLAGS += `xml2-config --libs`
OBJS=memory.o xml.o string.o
all: test

test: $(OBJS) test.o
	$(CC) $^ -o $@  $(LDFLAGS)

clean:
	rm -f *.o test
# DO NOT DELETE

memory.o: /usr/include/malloc.h /usr/include/features.h
memory.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
string.o: /usr/include/string.h /usr/include/features.h
string.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
test.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
test.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
test.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
test.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
test.o: /usr/include/bits/wchar.h /usr/include/gconv.h
test.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
xml.o: /usr/include/string.h /usr/include/features.h /usr/include/sys/cdefs.h
xml.o: /usr/include/gnu/stubs.h
