/*
 * mkd2html:  parse a markdown input file and generate a web page.
 *
 * usage:  mkd2html [options] filename
 *  or     mkd2html [options] < markdown > html
 *
 *  options
 *         -css css-file
 *         -header line-to-add-to-<HEADER>
 *         -footer line-to-add-before-</BODY>
 *
 * example:
 *
 *   mkd2html -cs /~orc/pages.css syntax
 *     ( read syntax OR syntax.text, write syntax.html )
 *
 * ---------------------------------------------------------------------
 *
 * Martin Hofmann <tin-pot@gmx.net> 2014-03-05:
 *
 * If the respective feature macros are defined in "config.h",
 * additional options are available:
 *
 * WITH_ENCODINGS:
 *
 * 	-l -u
 *		Indicate that input is in ISO 8859-1 rsp UTF-8.
 *
 *	-A -L -U
 *		Generate output in US-ASCII, ISO-8859-1, UTF-8.
 *
 * WITH_DOCTYPES:
 *
 *	-S -I
 *		Generate "HTML 4.01" (ie "Strict"),
 *		or "ISO/IEC 14445:200" document type declarations.
 *		The default is "HTML 4.01 Transitional".
 *
 * If *any* "tin-pot/discount" feature macro is set (and the
 * macro WITH_TINPOT_ therefore evaluates to  non-zero, see "config.h"),
 * then
 *
 *  1. are footnotes enabled by default in this program, and
 *
 *  2. are two more options available, namely:
 *
 *	-T
 *		Generate a rough table of contents and place it at
 *		the beginning of the <BODY> element.
 *		(This is a preliminary feature.)
 *	-r str
  		Pass text through in unmodified form if it is delimited
  		by user-defined markup. If the argument string `str` has
  		the form
  
  		    :string1:string2:
  
  		then text `string` enclosed between `string1` and `string2` will
  		be treated as if by `[](raw:string)` - except that it may also
  		contain a closing parenthesis `)` ...
 
		If the argument string `str` has the form
		
		    :string1:string2:out1:out2:

		then the raw text will be enclosed between `out1` and `out2` in
		the HTML output (both `out1` and `out2` may be empty strings).

		You don't have to use a colon `:` to separate the substrings, any
		printable character - given as the first character of the argument
		value - will do, as long it does not occur in your substrings:
		
		    +string1+string2+

		is equivalent to the pattern given above.
  
 * ---------------------------------------------------------------------
 */
/*
 * Copyright (C) 2007 David L Parsons.
 * The redistribution terms are provided in the COPYRIGHT file that must
 * be distributed with this source code.
 */
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef HAVE_BASENAME
# ifdef HAVE_LIBGEN_H
#  include <libgen.h>
# else
#  include <unistd.h>
# endif
#endif
#include <stdarg.h>

#include "mkdio.h"
#include "cstring.h"
#include "amalloc.h"

char *pgm = "mkd2html";

#if WITH_TINPOT_
#define DEFAULT_FLAGS (MKD_EXTRA_FOOTNOTE)
#endif
#if WITH_ENCODINGS
#define IN_ENCODING_MASK \
	(MKD_IN_LATIN1 | MKD_IN_UTF8)
	
#define OUT_ENCODING_MASK \
	(MKD_OUT_ASCII | MKD_OUT_LATIN1 | MKD_IN_UTF8)
#endif
#if WITH_TINPOT_
#define SETBITS(var, val, mask) ((var) = (var) & ~(mask) | (val))
static const char *ASCIIMathDelim = NULL;
#endif

#ifndef HAVE_BASENAME
char *
basename(char *path)
{
    char *p;

    if ( p = strrchr(path, '/') )
	return 1+p;
    return path;
}
#endif

void
fail(char *why, ...)
{
    va_list ptr;

    va_start(ptr,why);
    fprintf(stderr, "%s: ", pgm);
    vfprintf(stderr, why, ptr);
    fputc('\n', stderr);
    va_end(ptr);
    exit(1);
}

void
usage(void)
{
    fprintf(stderr, "usage: %s "
#if WITH_DOCTYPES
	"[-S | -I] "
#endif
#if WITH_ENCODINGS
	"[-A | -L | -U ] [-l] "
#endif
#if WITH_TINPOT_
	"[-T] [-r str] "
#endif
	"[-css URL] [-header text] "
	"[-footer text] [ source [dest] ]\n", pgm);
    exit(1);
}


int
main(argc, argv)
char **argv;
{
    char *h;
    char *source = 0, *dest = 0;
    MMIOT *mmiot;
    int i;
    FILE *input, *output; 
    STRING(char*) css, headers, footers;
#if WITH_TINPOT_
    mkd_flag_t flags = DEFAULT_FLAGS;
#else
    const mkd_flag_t flags = 0;
#endif
#if WITH_ENCODINGS
    const char *charset = "UTF-8";
#endif
#if WITH_DOCTYPES
    const char *doctyp = "-//W3C//DTD HTML 4.01 Transitional//EN"; 
    const char *docdtd = "http://www.w3.org/TR/html4/loose.dtd";
#endif


    CREATE(css);
    CREATE(headers);
    CREATE(footers);
    pgm = basename(argv[0]);

    while ( argc > 1) {
	if ( argc > 2 && strcmp(argv[1], "-css") == 0 ) {
	    EXPAND(css) = argv[2];
	    argc -= 2;
	    argv += 2;
	}
	else if ( argc > 2 && strcmp(argv[1], "-header") == 0 ) {
	    EXPAND(headers) = argv[2];
	    argc -= 2;
	    argv += 2;
	}
	else if ( argc > 2 && strcmp(argv[1], "-footer") == 0 ) {
	    EXPAND(footers) = argv[2];
	    argc -= 2;
	    argv += 2;
	}
#if WITH_TINPOT_
	else if ( argv[1][0] == '-' ) {
	    char ch, *opt = argv[1];
    	    extern int rawarg(char *);
	    
	    while ((ch = *++opt) != '\0') switch (ch) {
#if WITH_DOCTYPES
	    case 'S':
		doctyp=	"-//W3C//DTD HTML 4.01//EN"; 
		docdtd=	"http://www.w3.org/TR/html4/strict.dtd";
		SETBITS(flags, 0, MKD_ISO);
		break;
	    case 'I':
		doctyp = "ISO/IEC 15445:2000//DTD HTML//EN";
		docdtd = NULL;
		SETBITS(flags, MKD_ISO, MKD_ISO);
		break;
#endif /* WITH_DOCTYPES */
#if WITH_ENCODINGS
	    case 'A':
		charset = "US-ASCII";
		SETBITS(flags, MKD_OUT_ASCII, OUT_ENCODING_MASK); 
		break;
	    case 'L':
		charset = "ISO-8859-1";
		SETBITS(flags, MKD_OUT_LATIN1, OUT_ENCODING_MASK); 
		break;
	    case 'U':
		charset = "UTF-8";
		SETBITS(flags, MKD_OUT_UTF8, OUT_ENCODING_MASK);   
		break;
	    case 'l':
		SETBITS(flags, MKD_IN_LATIN1, IN_ENCODING_MASK);
		break;
#endif /* WITH_ENCODINGS */.
	    case 'T':
		SETBITS(flags, MKD_TOC, MKD_TOC);
		break;
	    case 'r':
		if (opt[1] != '\0') {
    		    rawarg(opt+1);
    		    opt += strlen(opt) - 1;
		} else {
		    rawarg(argv[2]);
		    --argc, ++argv;
		}
		break;
	    case 'a':
		{
		    static char arg1[32] = ":`:`:&#xF8F8;:&#xF8F8;:"; // U+F8F8 (PUA)
		    static char arg2[] = ":$$:$$:";
		    static char arg3[] = ":$:$:";
		    static char delim[4];
		    
		    if (opt[1] != '\0') {
			/*
			 * User-defined ASCIIMath delimiter.
			 */
			char ch = opt[1];
			if (ch == '\\') {
			    int n;
			    switch (opt[2]) {
			    case 'u': n = sscanf(opt + 3, "%x", &ch);
				if (n != 1) usage();
				break;
			    default:
				usage();
			    }
			}
			arg1[1] = arg1[3] = ch;
			delim[0] = ch;
			delim[1] = '\0';
			ASCIIMathDelim = delim;
			if (ch == arg1[0]) {
			    const char alt = '!';
			    char *p = strchr(arg1+5, ':');
			    assert(p != NULL);
			    arg1[0] = arg1[2] = arg1[4] = *p = alt;
			}
			opt += strlen(opt) - 1;
		    }
		    rawarg(arg1);
		    rawarg(arg2);
		    rawarg(arg3);
		}
		break;
	    default: 
		usage();
	    }
	    argc -= 1;
	    argv += 1;
	}
	else
	    break;
#endif /* WITH_TINPOT_ */
    }

    output = stdout;
    input = stdin;
    
    if (argc > 1) {
	char *p, *dot;
	char *filearg = argv[1];
	
	/*
	 * Open input from filearg.
	 */
	 
	source = malloc(strlen(filearg) + 6);
	if (source == NULL)
	    fail("out of memory allocating name buffers");

	strcpy(source, filearg);
	if (( p = strrchr(source, '/') ))
	    p = source;
	else
	    ++p;

	if ( (input = fopen(source, "r")) == 0 ) {
	    strcat(source, ".text");
	    if ( (input = fopen(source, "r")) == 0 )
		fail("can't open either %s or %s", argv[1], source);
	}

	if (argc == 2) {
	    filearg = argv[1];
	} else {
	    filearg = argv[argc-1];
	}
	
	/*
	 * Open output from filearg after setting `.html` if required.
	 */

	dest   = malloc(strlen(filearg) + 6);
	if (dest == NULL)
	    fail("out of memory allocating name buffers");
	strcpy(dest, filearg);
	if (( dot = strrchr(dest, '.') ))
	    *dot = 0;
	strcat(dest, ".html");
	
	if ( (output = fopen(dest, "w")) == 0 )
	    fail("can't write to %s", dest);
    }

    if (input == NULL)
	usage();

    if ( (mmiot = mkd_in(input, flags)) == 0 )
	fail("can't read %s", source ? source : "stdin");
    if ( !mkd_compile(mmiot, flags) )
	fail("couldn't compile input");

    h = mkd_doc_title(mmiot);

    /* print a header */

    fprintf(output,
#if WITH_DOCTYPES
	"<!doctype html public\t\"%s\"%s%s%s>\n"
	"<html>\n"
	"<head>\n"
	"  <meta name=\"GENERATOR\" content=\"mkd2html %s\"%s\n",
	doctyp, 
	docdtd ? "\n\t\t\t\"" : "",
	docdtd ? docdtd : "",
	docdtd ? "\"" : "",
	markdown_version,
	(flags & MKD_XML) ? "/>" : ">"
#else
	"<!doctype html public \"-//W3C//DTD HTML 4.0 Transitional //EN\">\n"
	"<html>\n"
	"<head>\n"
	"  <meta name=\"GENERATOR\" content=\"mkd2html %s\">\n",
	markdown_version
#endif
    );

    fprintf(output,"  <meta http-equiv=\"Content-Type\"\n"
#if WITH_ENCODINGS
		   "\tcontent=\"text/html; charset=%s\">\n",
		   charset
#else
		   "\tcontent=\"text/html; charset=UTF-8\">\n"
#endif
    );

    for ( i=0; i < S(css); i++ )
	fprintf(output, "  <link rel=\"stylesheet\"\n"
#if WITH_TINPOT_ /* No `/>` in HTML! */
			"        type=\"text/css\"\n"
			"        href=\"%s\"%s\n",
			T(css)[i], (flags & MKD_XML) ? "/>" : ">"
#else
			"        type=\"text/css\"\n"
			"        href=\"%s\"/>\n", T(css)[i]
#endif
	);

    if ( h ) {
	fprintf(output,"  <title>");
	mkd_generateline(h, (int)strlen(h), output, flags);
	fprintf(output, "</title>\n");
    }
    for ( i=0; i < S(headers); i++ )
	fprintf(output, "  %s\n", T(headers)[i]);
#if 0 && WITH_TINPOT_ /* I can't get `ASCIIMathML.js` to grok this. */
    if (ASCIIMathDelim != NULL) {
	fprintf(output, "  <script type=\"text/javascript\">AMdelimiter1 = \"%s\"; "
	                "AMescape1 = \"\\\\%s\"; </script>\n", ASCIIMathDelim, ASCIIMathDelim);
    }
#endif
    fprintf(output, "</head>\n"
		    "<body>\n");

#if WITH_TINPOT_
    if (flags & MKD_TOC)
	mkd_generatetoc(mmiot, output);
#endif

    /* print the compiled body */

    mkd_generatehtml(mmiot, output);

    for ( i=0; i < S(footers); i++ )
	fprintf(output, "%s\n", T(footers)[i]);

    fprintf(output, "</body>\n"
		    "</html>\n");

    mkd_cleanup(mmiot);
    exit(0);
}
