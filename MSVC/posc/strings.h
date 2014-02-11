#ifndef POSC_STRINGS_H_INCLUDED
#define POSC_STRINGS_H_INCLUDED
#include "posc.h"

#include POSC_HEADER(string.h)

#if 0 /* Traditional <strings.h> content that would belong here. */
int    bcmp(const void *, const void *, size_t); /* LEGACY, see memcmp */
void   bcopy(const void *, void *, size_t); /* LEGACY, see memcpy, memmove */
void   bzero(void *, size_t); /* LEGACY, see memset */
int    ffs(int);
char  *index(const char *, int); /* LEGACY, see strchr */
char  *rindex(const char *, int); /* LEGACY, see strrchr */
int    strcasecmp(const char *, const char *);
int    strncasecmp(const char *, const char *, size_t);
#endif

#ifdef _MSC_VER
#define strcasecmp  _stricmp 
#define strncasecmp _strnicmp 
#endif

#endif /* POSC_STRINGS_H_INCLUDED */
