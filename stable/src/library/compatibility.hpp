#ifndef _compatibility_hpp
#define _compatibility_hpp
#include "config.h"
#ifdef WIN32 // TODO: HAVE_STRTOLL not defined even though strtoll is available on Linux
#ifndef HAVE_STRTOLL
long long int strtoll(const char *nptr, char **endptr, int base);
#endif
#endif
#ifdef WIN32 // TODO: HAVE_STRTOF not defined even though strtof is available on Linux
#ifndef HAVE_STRTOF
float strtof (const char *nptr, char **endptr);
#endif
#endif
#ifndef HAVE_LOCALTIME_R
#include <time.h>
struct tm *localtime_r(const time_t *clock, struct tm *result);
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

#endif
