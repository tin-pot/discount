/** findhdrdir.c *******************************************************

Description
===========

 Uses the compiler to find out where the standard libraries are.

 Definitions placed in `posc_hdrdir.h`:

 - POSC_STDDIR:  Points to the directory where `stdio.h` was
                 found.

 - POSC_XXX_HDR: For each header xxx.h of ISO 9899:1990, a macro
                 containing the full pathname for that header.

 - POSC_YYY_HDR: For each header yyy.h given as an argument to
                 this command, a macro containing the full pathname
                 for that header is added.



Licence
=======

Copyright (c) 2012, Martin Hofmann <mrtnhfmnn@gmail.com>.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define POSC_ERR_NAME "posc_findhdrdir.err"
#define POSC_HDR_NAME "posc_hdrdir.h"
#define FILE_C_NAME   "posc_tmp.c"
#define FILE_I_NAME   "posc_tmp.i"

#define STD_HEADER_NAME "stdio.h"

#define LINEMAX 2048

#if defined(__INTERIX)
#define CC "cc -E"
#elif defined(_MSC_VER)
#define CC "CL /nologo /E"
#define PATHMAX _MAX_PATH
#elif defined(__GNUC__)
#define CC "cc -E"
#define PATHMAX PATH_MAX
#else
#error Unknown compiler.
#endif

#ifdef _WIN32
#define ALTSEP '\\'
#else
#define ALTSEP '/'
#endif

#define PANIC(args)         \
    do {                    \
        printf args;        \
        exit(EXIT_FAILURE); \
    } while (0)

#define TRACE(args) /* printf args */
    
static const char *const stdheader[] = {
    "assert.h",
    "ctype.h",
    "errno.h",
    "float.h",
    "limits.h",
    "locale.h",
    "math.h",
    "setjmp.h",
    "signal.h",
    "stdarg.h",
    "stddef.h",
    "stdio.h",
    "stdlib.h",
    "string.h",
    "time.h",
    NULL
};

void create_src(const char *name, const char *header)
{
    FILE *fp;

    errno = 0;
    if ((fp = fopen(name, "w")) == NULL)
        PANIC(("Can't open '%s': %s\n", name, strerror(errno)));

    fprintf(fp, "#include <%s>\n", header);
    fclose(fp);
    if (errno != 0)
        PANIC(("Can't write '%s': %s\n", name, strerror(errno)));
}

void run_cc(const char *cname, const char *iname)
{
    char cmdline[500];
    int result;

    if ((result = system(NULL)) == 0)
        PANIC(("No command interpreter: %s\n", strerror(errno)));

    sprintf(cmdline, "%s %s >%s 2>%s", CC, cname, iname, 
                POSC_ERR_NAME);
    result = system(cmdline);

    if (result != 0)
        PANIC(("Command failed (see stderr in '%s'): '%s'\n", 
                        POSC_ERR_NAME, cmdline));
}

char *getincludeline(const char *filename, const char *header)
{
    FILE *fp;
    static char line[LINEMAX+1];

    if ((fp = fopen(filename, "r")) == NULL)
        PANIC(("Can't open '%s': %s\n", filename, strerror(errno)));

    errno = 0;
    while (fgets(line, LINEMAX, fp) != NULL) {
        if (strstr(line, header)) {
            fclose(fp);
            return line;
        }
    }

    if (feof(fp)) {
        fclose(fp);
        return NULL;
    }

    if (ferror(fp) || errno != 0)
        PANIC(("Error reading '%s': %s\n", filename, strerror(errno)));
    else
        PANIC(("Neither feof() nor ferror() for '%s'!\n", filename));
}

char *pathname(char *path, int cut)
{
    char *dst, *src, *comp;

    comp = dst = src = path;

    while (*src != '\0') {
        char ch;

        *dst++ = (ch = *src++);
        if (ch == '/' || ch == ALTSEP) {
            comp = dst-1;
            while ((ch = *src) == '/' || ch == ALTSEP)
                src++;
        }
    }
    if (cut)
        *comp = '\0';
    else
        *dst = '\0';
    return path;
}

char *findhdrloc(const char *header)
{
    char *includeline;
    const char *cname = FILE_C_NAME;
    const char *iname = FILE_I_NAME;

    TRACE(("Temp source: %s\n", cname));
    create_src(cname, header);

    TRACE(("Expanded source: %s\n", iname));
    run_cc(cname, iname);

    includeline = getincludeline(iname, header);
    if (includeline == NULL)
        PANIC(("No include line for %s in '%s'!\n", header, iname));
    else {
        char *start, *end;

        start = strchr(includeline, '\"');
        if (start == NULL) 
            PANIC(("Malformed line: '%s'\n", includeline));

        end = strchr(++start, '\"');
        if (end == NULL)
            PANIC(("Malformed line: '%s'\n", includeline));
        *end = '\0';

        remove(cname);
        remove(iname);

        return start;
    }
}

void writehdrdir(FILE *fp, const char *hdr, const char *value)
{
    if (hdr == NULL) 
        fprintf(fp, "#define POSC_STDDIR %s\n", value);
    else {
        int i;
        unsigned char ch;

        fprintf(fp, "#define POSC_");
        for (i = 0; (ch = (unsigned char)hdr[i]) != '\0' && isalnum(ch); ++i)
            fprintf(fp, "%c", toupper(ch));
        fprintf(fp, "_HDR \"%s\"\n", value);
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    const char *dir;
    const char *header = STD_HEADER_NAME;
    int i;

    if ((fp = fopen(POSC_HDR_NAME, "w")) == NULL) 
        PANIC(("Can't open '%s' for writing: %s\n", POSC_HDR_NAME, strerror(errno)));

    dir = pathname(findhdrloc(header), 1);
    printf("'%s'\n", dir);
    writehdrdir(fp, NULL, dir);

    for (i = 0; stdheader[i] != NULL; ++i) {
        const char *loc;

        header = stdheader[i];
        printf("%s: ", header);
        loc = pathname(findhdrloc(header), 0);
        printf("'%s'\n", loc);
        writehdrdir(fp, header, loc);
    }

    for (i = 1; i < argc; ++i) {
        const char *loc;

        header = argv[i];
        printf("%s: ", header);
        loc = pathname(findhdrloc(header), 0);
        printf("'%s'\n", loc);
        writehdrdir(fp, header, loc);
    }

    fclose(fp);
    remove("posc_findhdrdir.err");

    return EXIT_SUCCESS;
}
