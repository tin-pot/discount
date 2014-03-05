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

#define IN_ENCODING_MASK \
	(MKD_IN_LATIN1 | MKD_IN_UTF8)
	
#define OUT_ENCODING_MASK \
	(MKD_OUT_ASCII | MKD_OUT_LATIN1 | MKD_IN_UTF8)
	
#define SETBITS(var, val, mask) ((var) = (var) & ~(mask) | (val))

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
    mkd_flag_t flags = DEFAULT_FLAGS;
    
    int strict = 0;
    const char *charset = "UTF-8";
    const char *doctyp = "-//W3C//DTD HTML 4.01 Transitional//EN"; 
    const char *docdtd = "http://www.w3.org/TR/html4/loose.dtd";
    
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
	    case 'T':
		SETBITS(flags, MKD_TOC, MKD_TOC);
		break;
	    default: 
		fprintf(stderr, "usage: %s [-S | -I] [-A | -L | -U ] [-l] "
			"[-css URL] [-header text] "
			"[-footer text] [ source [dest] ]\n",       pgm);
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
	char *filearg;
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
	break;

    default:
	fprintf(stderr, "usage: %s [opts] source [dest]\n", pgm);
	exit(1);
    }

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
	doctyp, 
	docdtd ? "\n\t\t\t\"" : "",
	docdtd ? docdtd : "",
	docdtd ? "\"" : "",
	markdown_version);
	
	
/*	docdecl[doctype],
	docdtd[doctype] ? "\n\t\t\t\"" : "",
	docdtd[doctype] ? docdtd[doctype] : "",
	docdtd[doctype] ? "\"" : "", */
    
    fprintf(output,"  <meta http-equiv=\"Content-Type\"\n"
		   "        content=\"text/html; charset=%s\">\n",
		   charset);

    for ( i=0; i < S(css); i++ )
	fprintf(output, "  <link rel=\"stylesheet\"\n"
			"        type=\"text/css\"\n"
			"        href=\"%s\">\n", T(css)[i]);

    if ( h ) {
	fprintf(output,"  <title>");
	mkd_generateline(h, (int)strlen(h), output, flags);
	fprintf(output, "</title>\n");
    }
    for ( i=0; i < S(headers); i++ )
	fprintf(output, "  %s\n", T(headers)[i]);
    fprintf(output, "</head>\n"
		    "<body>\n");

    if (flags & MKD_TOC)
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
