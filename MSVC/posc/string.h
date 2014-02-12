#ifndef POSC_STRING_H_INCLUDED
#define POSC_STRING_H_INCLUDED
#include "posc.h"

#include POSC_STRING_HDR

/*
 * This is only here because `disount` happily uses
 * strcasecmp() without including <strings.h>. So we 
 * let it come in along with the <string.h> content.
 */
#ifdef _MSC_VER
#ifdef __cplusplus
extern "C" {
#endif
char *posc_strdup(const char *);
#ifdef __cplusplus
}
#endif

#define strcasecmp  _stricmp 
#define strncasecmp _strnicmp 
#if 0 /* MSVC has a strdup() */
#define strdup posc_strdup
#endif

#endif

#endif /* POSC_STRING_H_INCLUDED */
