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
/* include <unistd.h> */
# endif
#endif

#include <unistd.h> /* getopt() */
#include "pgm_options.h"

#include <stdarg.h>

#include "mkdio.h"
#include "cstring.h"
#include "amalloc.h"

char *pgm = "mkd2html";

#ifndef HAVE_BASENAME
char *
basename(char *path)
{
    char *p;

    if (( p = strrchr(path, '/') ))
	return 1+p;
    return path;
}
#endif

void
complain(char *fmt, ...)
{
    va_list ptr;

    va_start(ptr, fmt);
    fprintf(stderr, "%s: ", pgm);
    vfprintf(stderr, fmt, ptr);
    fputc('\n', stderr);
    va_end(ptr);
    fflush(stderr);
}

void
fail(char *fmt, ...)
{
    va_list ptr;

    va_start(ptr,fmt);
    fprintf(stderr, "%s: ", pgm);
    vfprintf(stderr, fmt, ptr);
    fputc('\n', stderr);
    va_end(ptr);
    exit(1);
}


int
main(argc, argv)
char **argv;
{
    char *h;
    MMIOT *mmiot;
    int i;
    FILE *input, *output; 
    char *source;
    STRING(char*) css, headers, footers;

    int opt;
    mkd_flag_t flags = 0;
    int debug = 0;
    int toc = 0;
    int version = 0;
    int with_html5 = 0;
    int use_mkd_line = 0;
    char *extra_footnote_prefix = 0;
    char *urlflags = NULL;
    char *text = NULL;
    char *urlbase = NULL;

    int res = 0;

    CREATE(css);
    CREATE(headers);
    CREATE(footers);
    pgm = basename(argv[0]);

    /* tin-pot 2012-01-02: Use getopt() for args. */
    opterr = 1;
    input = output = NULL;

    while ( (opt=getopt(argc, argv, "d5TVb:C:c:h:f:F:o:s:")) != EOF ) {
            
	switch (opt) {
	case 'd':   debug = 1;
		    break;
	case '5':   with_html5 = 1;
		    break;
	case 'T':   toc = 1;
		    break;
	case 'V':   version++;
		    break;
	case 'b':   urlbase = optarg;
		    break;
	case 'C':   extra_footnote_prefix = optarg;
		    break;
#if 0
	case 'E':   urlflags = optarg;
		    break;
#endif
	case 'F':   if ( strcmp(optarg, "?") == 0 ) {
			show_flags(0);
			exit(0);
		    }
		    else
			flags = strtol(optarg, 0, 0);
		    break;
	case 'f':   /* check for -footer */
                    if (strcmp(optarg, "ooter") == 0) {
                        if (argv[optind] == NULL) 
                            fail("--footer argument missing.\n");

                        EXPAND(footers) = argv[optind++];
                        break;
                    }
                    if ( strcmp(optarg, "?") == 0 ) {
			show_flags(1);
			exit(0);
		    }
		    else if ( !set_flag(&flags, optarg) )
			complain("unknown option <%s>", optarg);
		    break;
	case 'o':   if (output != NULL) {
			complain("Too many -o options");
			exit(1);
		    }
                    if ((output = fopen(optarg, "w")) == NULL) {
			perror(optarg);
			exit(1);
		    }
		    break;
	case 's':   text = optarg;
		    break;
        case 'c':
        case 'h':   /* check for -css or -header */
                    if (opt == 'c' && strcmp(optarg, "ss") == 0) {
                        if (argv[optind] == NULL) 
                            fail("-css argument missing.\n");

                        EXPAND(css) = argv[optind++];
                        break;
                    } else if (opt == 'h' &&
                               strcmp(optarg, "eader") == 0) {
                        if (argv[optind] == NULL) 
                            fail("-header argument missing.\n");

                        EXPAND(headers) = argv[optind++];
                        break;
                    }
                    /* FALLTHROUGH */
	default:    fprintf(stderr, "usage: %s [-d5TV] [-b url-base] "
                        "[-C ...] "
                        "[-css ...] [-header ...] [-footer ...] "
                        "[-F bitmap] [-f {+-}flags] "
			"[-o ofile] [-s text] "
			"[file]\n", pgm);
                    exit(1);
	}
    }

    if ( version ) {
	printf("%s: discount %s%s", pgm, markdown_version,
				  with_html5 ? " +html5":"");
	if ( version > 1 )
	    mkd_flags_are(stdout, flags, 0);
	putchar('\n');
	exit(0);
    }

    argc -= optind;
    argv += optind;

    source = NULL;
    if (argc > 0) {
	char *dot, *dest;
	
	if ((source = malloc(strlen(argv[0]) + 6)) == NULL)
            fail("out of memory");
        strcpy(source, argv[0]);
        if ((input = fopen(source, "r")) == NULL) {
            strcat(source, ".text");
            if ((input = fopen(source, "r")) == NULL)
                fail("can't open either %s or %s", argv[0], source);
        }

        if (output == NULL) {
    	    if ((dest = malloc(strlen(argv[0]) + 6)) == NULL)
    	        fail("out of memory");

    	    strcpy(dest, source);

    	    if ((dot = strrchr(dest, '.')) != NULL)
    	        *dot = '\0';
    	    strcat(dest, ".html");

    	    if  ((output = fopen(dest, "w")) == NULL)
    	        fail("can't write to %s", dest);
            free(dest);
        }
    } else {
	input = stdin;
        if (output == NULL) /* no -o ofile given */
	    output = stdout;
    }

    /* end tin-pot */

    if ( (mmiot = mkd_in(input, flags)) == 0 )
	fail("can't read %s", source ? source : "stdin");

    /* Prepare compile. */

    if ( with_html5 )
	mkd_with_html5_tags();
    if (urlbase)
        mkd_basename(mmiot, urlbase);
    if (extra_footnote_prefix)
        mkd_ref_prefix(mmiot, extra_footnote_prefix);
    if (debug) {
        res = mkd_dump(mmiot, stdout, 0, argc ? basename(argv[0]) : "stdin");
        exit(res);
    }

    if ( !mkd_compile(mmiot, flags) )
	fail("couldn't compile input");


    h = mkd_doc_title(mmiot);

    /* print a header */

    fprintf(output,
	"<!doctype html public \"-//W3C//DTD HTML 4.0 Transitional //EN\">\n"
	"<html>\n"
	"<head>\n"
	"  <meta name=\"GENERATOR\" content=\"mkd2html %s\">\n", markdown_version);

    fprintf(output,"  <meta http-equiv=\"Content-Type\"\n"
		   "        content=\"text/html; charset-us-ascii\">");

    for ( i=0; i < S(css); i++ )
	fprintf(output, "  <link rel=\"stylesheet\"\n"
			"        type=\"text/css\"\n"
			"        href=\"%s\" />\n", T(css)[i]);

    if ( h ) {
	fprintf(output,"  <title>");
	mkd_generateline(h, (int)strlen(h), output, flags);
	fprintf(output, "</title>\n");
    }
    for ( i=0; i < S(headers); i++ )
	fprintf(output, "  %s\n", T(headers)[i]);
    fprintf(output, "</head>\n"
		    "<body>\n");

    /* print the compiled body */

    if (toc)
        mkd_generatetoc(mmiot, stdout);

    mkd_generatehtml(mmiot, output);

    for ( i=0; i < S(footers); i++ )
	fprintf(output, "%s\n", T(footers)[i]);
    
    fprintf(output, "</body>\n"
		    "</html>\n");
    
    mkd_cleanup(mmiot);
    exit(0);
}
