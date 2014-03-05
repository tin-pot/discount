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

/*
 * <tin-pot@gmx.net> 2014-03-05:
 *
 * More `WITH_`* feature macros for this variant of `discount`.
 */

/*
 * Miscellaneous small tweaks and fixes.
 */
#define WITH_TINPOT 1
/*
 * Implement input/output encodings.
 */
#define WITH_ENCODINGS 1
/*
 * Implement output of 
 * - "HTML 4.01" (ie "Strict" W3C HTML) and
 * - "HTML" (ie ISO/IEC 14445:2000 HTML)
 * document types in additon to "HTML 4.01 Transitional".
 * (And replace the XML `/>` with `>` when generating HTML.)
 */
#define WITH_DOCTYPES 1
/*
 * Implement the `?[title](uri =WxH)` notation for a HTML <object>.
 */
#define WITH_HTML_OBJECT 1
/*
 * What this `WITH_TCL_WIKI` does is 
 *   - "in-house" and internal,
 *   - undocumented,
 *   - not what you want.
 * Don't use it, and don't complain if you *do*!
 */
#ifndef WITH_TCL_WIKI
#define WITH_TCL_WIKI 0
#endif

/*
 * The Visual C++ "C" compiler has a `__inline` keyword implemented
 * in Visual Studio 2008 and later, see
 * <http://msdn.microsoft.com/de-de/library/cx3b23a3%28v=vs.90%29.aspx>
 */
#if _MSC_VER >= 1500 /* Compiler Ver. 15, Visual Studio 2008 */
#define inline __inline
#else
#define inline 
#endif

/*
 * Beware of conflicts with <Windows.h>, which typedef's these names.
 */
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
