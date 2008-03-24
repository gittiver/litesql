#include "litesql.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
int lsqlSnprintf(char* str, size_t size, const char* format, ...) {
    int used = 0;
    va_list ap;
    char* orig = str;
    memset(orig, 0, size);
    va_start(ap, format);
    while (used < size - 1) {
        char ch = *format++;
        if (ch == '\0')
            break;
        if (ch == '%') {
            char buf[16];
            const char* s2;
            const lsqlString* ls;
            int bufsize;
            ch = *format++;
            if (ch == '\0')
                break;

            switch(ch) {

                case 'd':
                    sprintf(buf, "%d", va_arg(ap, int));
                    bufsize = strlen(buf);
                    if (used + bufsize > size - 1) {
                        bufsize = (size - 1) - used;
                        buf[bufsize] = '\0';
                    }
                    strcpy(str, buf);
                    str += bufsize;
                    used += bufsize;
                    break;
                case 's':
                    s2 = va_arg(ap, const char*);
                    bufsize = strlen(s2);
                    if (used + bufsize > size - 1)
                        bufsize = (size - 1) - used;

                    strncpy(str, s2, bufsize);
                    str += bufsize;
                    used += bufsize;
                    break;
                case 'S':
                    ls = va_arg(ap, const lsqlString*);
                    s2 = lsqlStringPtr(ls);
                    bufsize = strlen(s2);
                    if (used + bufsize > size - 1)
                        bufsize = (size - 1) - used;
                    strncat(str, s2, bufsize);
                    str += bufsize;
                    used += bufsize;
                    break;
                case '%':
                    *str++ = '%';
                    used++;
                    break;
            }
        } else {
            *str++ = ch;
            used++;
        }
    }    
    *str = '\0';
    used++;

    va_end(ap);
    return used;
}
