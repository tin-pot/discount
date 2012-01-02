/*
 * Configuration for MSVC on Win32.
 * 
 */
#ifndef __AC_MARKDOWN_D
#define __AC_MARKDOWN_D 1

#define OS_WIN32 1
#define USE_DISCOUNT_DL 1
#define inline /**/
#define DWORD unsigned long
#define WORD unsigned short
#define BYTE unsigned char
#define HAVE_PWD_H 0
#define HAVE_GETPWUID 0
#define HAVE_SRANDOM 0
#define INITRNG(x) srand((unsigned int)x)
#define HAVE_BZERO 0
#define HAVE_RANDOM 0
#define COINTOSS() (rand()&1)
#define HAVE_STRCASECMP 0
#define HAVE_STRNCASECMP 0
#define HAVE_FCHDIR 0
#define TABSTOP 8
#define HAVE_MALLOC_H 0

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define vsnprintf _vsnprintf
#endif

#endif/* __AC_MARKDOWN_D */
