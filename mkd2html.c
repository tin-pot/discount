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

#define DEFAULT_FLAGS (MKD_EXTRA_FOOTNOTE)
/* | MKD_IN_LATIN1 | MKD_OUT_LATIN1) */

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


main(argc, argv)
char **argv;
{
    char *h;
    char *source = 0, *dest = 0;
    MMIOT *mmiot;
    int i;
    FILE *input, *output; 
    STRING(char*) css, headers, footers;
    mkd_flag_t outenc = MKD_OUT_UTF8;
    mkd_flag_t inenc  = MKD_IN_UTF8;
    mkd_flag_t flags = DEFAULT_FLAGS;
    int toc = 0;
    const char *charset;
    enum { Transitional, Strict, Iso } doctype = Transitional;
    const char *const docdecl[] = {
	"-//W3C//DTD HTML 4.01 Transitional//EN", 
	"-//W3C//DTD HTML 4.01//EN", 
	"ISO/IEC 15445:2000//DTD HTML//EN"
    };
    const char *const docdtd[] = {
	"http://www.w3.org/TR/html4/loose.dtd",
	"http://www.w3.org/TR/html4/strict.dtd",
	NULL
    };


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
	else if ( argv[1][0] == '-' ) {
	    char ch, *opt = argv[1];
	    
	    while ((ch = *++opt) != '\0') switch (ch) {
	    case 'S': doctype = Strict; continue;
	    case 'I': doctype = Iso;    continue;
	    case 'A': outenc = MKD_OUT_ASCII; continue;
	    case 'L': outenc = MKD_OUT_LATIN1; continue;
	    case 'U': outenc = MKD_OUT_UTF8; continue;
	    case 'l': inenc  = MKD_IN_LATIN1; continue;
	    case 'T': toc = 1; continue;
	    default: 
		fprintf(stderr, 
		"usage: %s [-S | -I] [-A | -L | -U ] [-l] "
		"[-css URL] [-header text] [-footer text] [ source [dest] ]\n",
		 pgm);
		exit(1);
	    }
	    argc -= 1;
	    argv += 1;
	}
	else
	    break;
    }

    switch ( argc ) {
	char *p, *dot;
    case 1:
	input = stdin;
	output = stdout;
	break;
    case 2:
    case 3:
	dest   = malloc(strlen(argv[argc-1]) + 6);
	source = malloc(strlen(argv[1]) + 6);

	if ( !(source && dest) )
	    fail("out of memory allocating name buffers");

	strcpy(source, argv[1]);
	if (( p = strrchr(source, '/') ))
	    p = source;
	else
	    ++p;

	if ( (input = fopen(source, "r")) == 0 ) {
	    strcat(source, ".text");
	    if ( (input = fopen(source, "r")) == 0 )
		fail("can't open either %s or %s", argv[1], source);
	}
	strcpy(dest, source);

	if (( dot = strrchr(dest, '.') ))
	    *dot = 0;
	strcat(dest, ".html");

	if ( (output = fopen(dest, "w")) == 0 )
	    fail("can't write to %s", dest);
	break;

    default:
	fprintf(stderr,
		"usage: %s [-S | -I] [-A | -L | -U ] [-l] [-T] "
		"[-css URL] [-header text] [-footer text] [ source [dest] ]\n",
		 pgm);
	exit(1);
    }

    /*
     * Set up flags.
     */
    switch (outenc) {
    case MKD_OUT_ASCII:  charset = "US-ASCII";
			 flags |= MKD_OUT_ASCII;
			 break;
    case MKD_OUT_LATIN1: charset = "ISO-8859-1";
			 flags |= MKD_OUT_LATIN1;
			 break;
    case MKD_OUT_UTF8: default:
			 charset = "UTF-8";
			 flags |= MKD_OUT_UTF8;
			 break;
    }
    
    switch (inenc) {
    case MKD_IN_LATIN1:  flags |= MKD_IN_LATIN1;
                         break;
    case MKD_IN_UTF8: default:
                         flags |= MKD_IN_UTF8;
                         break;
    }
    
    if (toc)
	flags |= MKD_TOC;
	
    if ( (mmiot = mkd_in(input, flags)) == 0 )
	fail("can't read %s", source ? source : "stdin");

    if ( !mkd_compile(mmiot, flags) )
	fail("couldn't compile input");


    h = mkd_doc_title(mmiot);

    /* print a header */

    fprintf(output,
	"<!doctype html public\t\"%s\"%s%s%s>\n"
	"<html>\n"
	"<head>\n"
	"  <meta name=\"GENERATOR\" content=\"mkd2html %s\">\n",
	docdecl[doctype],
	docdtd[doctype] ? "\n\t\t\t\"" : "",
	docdtd[doctype] ? docdtd[doctype] : "",
	docdtd[doctype] ? "\"" : "",
	markdown_version);
    
    fprintf(output,"  <meta http-equiv=\"Content-Type\"\n"
		   "        content=\"text/html; charset=%s\">\n",
		   charset);

    for ( i=0; i < S(css); i++ )
	fprintf(output, "  <link rel=\"stylesheet\"\n"
			"        type=\"text/css\"\n"
			"        href=\"%s\">\n", T(css)[i]);

    if ( h ) {
	fprintf(output,"  <title>");
	mkd_generateline(h, strlen(h), output, flags);
	fprintf(output, "</title>\n");
    }
    for ( i=0; i < S(headers); i++ )
	fprintf(output, "  %s\n", T(headers)[i]);
    fprintf(output, "</head>\n"
		    "<body>\n");

    if (toc)
        mkd_generatetoc(mmiot, output);
        
    /* print the compiled body */
    mkd_generatehtml(mmiot, output);

    for ( i=0; i < S(footers); i++ )
	fprintf(output, "%s\n", T(footers)[i]);
    
    fprintf(output, "</body>\n"
		    "</html>\n");
    
    mkd_cleanup(mmiot);
    exit(0);
}
