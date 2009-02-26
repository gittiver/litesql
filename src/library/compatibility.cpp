#include "compatibility.hpp"
#ifndef HAVE_STRTOLL
long long int strtoll(const char *nptr, char **endptr, int base) {
    return strtol(nptr, endptr, base);
}
#endif
#ifndef HAVE_STRTOF
float strtof (const char *nptr, char **endptr) {
#ifdef HAVE_ATOF
    return atof(nptr);
#endif
}
#endif

