/*
 * Pre-digested configuration header for MSVC on Win32.
 */

#ifndef __AC_MARKDOWN_D
#define __AC_MARKDOWN_D 1

#ifndef _MSC_VER
#error Use this header with MSVC only.
#endif

#define OS_WIN32 1

/*
 * `discount` feature macros - we want them all!
 */
#ifndef WITH_ID_ANCHOR
#define WITH_ID_ANCHOR 1
#endif
#ifndef WITH_FENCED_CODE
#define WITH_FENCED_CODE 1
#endif
#ifndef WITH_GITHUB_TAGS
#define WITH_GITHUB_TAGS 1
#endif
#ifndef USE_DISCOUNT_DL
#define USE_DISCOUNT_DL 1
#endif
#ifndef USE_EXTRA_DL
#define USE_EXTRA_DL 1
#endif

/*
 * <tin-pot@gmx.net> 2014-03-05:
 *
 * Some `WITH_`* feature macros for this variant of `discount`.
 */

/*
 * Implement input/output encodings.
 */
#ifndef WITH_ENCODINGS
#define WITH_ENCODINGS 1
#endif
/*
 * Implement output of 
 * - "HTML 4.01" (ie "Strict" W3C HTML) and
 * - "HTML" (ie ISO/IEC 14445:2000 HTML)
 * document types in additon to "HTML 4.01 Transitional".
 */
#ifndef WITH_DOCTYPES
#define WITH_DOCTYPES 1
#endif
/*
 * Implement the `?[title](uri =WxH)` notation to specify an <object>.
 */
#ifndef WITH_HTML_OBJECT
#define WITH_HTML_OBJECT 1
#endif
/*
 * What this `WITH_TCL_WIKI` does is 
 *  1. "in-house" and internal,
 *  2. undocumented,
 *  3. not what you want.
 * Don't use it, or don't complain if you *do* ...
 */
#ifndef WITH_TCL_WIKI
#define WITH_TCL_WIKI 0
#endif
/*
 * The WITH_TINPOT_ macro is non-zero iff any of the extensions is used.
 */
#undef WITH_TINPOT_
#define WITH_TINPOT_ (WITH_ENCODINGS   || WITH_DOCTYPES || \
                      WITH_HTML_OBJECT || WITH_TCL_WIKI)

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
