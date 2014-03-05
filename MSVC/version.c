/*
 * version.c -- Version of markdown for MSVC / Win32.
 */

#include "config.h"

#define XSTR(s) #s
#define STR(s) XSTR(s)

char markdown_version[] = "2.1.7"
/*
 * This is "tin-pot/discount" on _github_.
 */
		"/<http://github.com/tin-pot/discount>"

#if TABSTOP != 4
                " TAB=" STR(TABSTOP)
#endif
#if defined(DEBUG) || USE_AMALLOC 
		" DEBUG"
#endif
#if USE_DISCOUNT_DL
# if USE_EXTRA_DL
		" DL=BOTH"
# else
		" DL=DISCOUNT"
# endif
#elif USE_EXTRA_DL
		" DL=EXTRA"
#else
		" DL=NONE"
#endif
#if WITH_ID_ANCHOR
		" ID-ANCHOR"
#endif
#if WITH_GITHUB_TAGS
		" GITHUB-TAGS"
#endif
#if WITH_FENCED_CODE
		" FENCED-CODE"
#endif
/*
 * <tin-pot@gmx.net> 2014-03-05:
 *
 * Report additional feature macros (cf "config.h").
 */
#if WITH_ENCODINGS
		" ENCODINGS"
#endif
#if WITH_DOCTYPES
		" DOCTYPES"
#endif
#if WITH_HTML_OBJECT
		" HTML_OBJECT"
#endif
#if WITH_TINPOT
		" TIN-POT"
#endif
#if WITH_TCL_WIKI
		" TCL-WIKI"
#endif
		;
