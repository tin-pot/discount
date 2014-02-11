#ifndef POSC_STDIO_H_INCLUDED
#define POSC_STDIO_H_INCLUDED
#include "posc.h"

#include POSC_STDIO_HDR

#ifdef _MSC_VER

/* Silence the most annoying warning of all. */
#pragma warning( disable: 4996 )

#ifdef __cplusplus
extern "C" {
#endif

int posc_snprintf(char *, size_t, const char *, ...);
int posc_vsnprintf(char *, size_t, const char *, va_list);

#ifdef __cplusplus
}
#endif

#define snprintf  posc_snprintf
#define vsnprintf posc_vsnprintf

#endif /* _MSC_VER */

#endif /* POSC_STDIO_H_INCLUDED */

