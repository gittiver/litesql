#ifndef _compatibility_hpp
#define _compatibility_hpp
#include "config.h"
#ifndef HAVE_STRTOLL
long long int strtoll(const char *nptr, char **endptr, int base);
#endif
#ifndef HAVE_STRTOF
float strtof (const char *nptr, char **endptr);
#endif
#endif
