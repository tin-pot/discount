/*
 * Pre-digested configuration header for MSVC on Win32.
 */

#ifndef __AC_MARKDOWN_D
#define __AC_MARKDOWN_D 1

#ifndef _MSC_VER
#error Use this header with MSVC only.
#endif

#define OS_WIN32 1

/* discount feature macros - we want it all! */
#define WITH_ID_ANCHOR 1
#define WITH_FENCED_CODE 1
#define WITH_GITHUB_TAGS 1
#define USE_DISCOUNT_DL 1
#define USE_EXTRA_DL 1

/* No inline in MSC. */
#define inline 

/* Beware conflicts with <Windows.h> */
#ifndef WINVER
#define DWORD unsigned long
#define WORD  unsigned short
#define BYTE  unsigned char
#endif

#define HAVE_PWD_H 0
#define HAVE_GETPWUID 0
#define HAVE_SRANDOM 0
#define INITRNG(x) srand((unsigned int)x)
#define HAVE_BZERO 0
#define HAVE_RANDOM 0
#define COINTOSS() (rand()&1)
#define HAVE_STRCASECMP  1  /* Faked in posc/strings.h */
#define HAVE_STRNCASECMP 1  /* Faked in posc/strings.h */
#define HAVE_FCHDIR 0
#define TABSTOP 8
#define HAVE_MALLOC_H    0

#endif /* __AC_MARKDOWN_D */