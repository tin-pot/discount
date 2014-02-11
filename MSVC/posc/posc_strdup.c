#include "string.h"
#include <stdlib.h>

char *posc_strdup(const char *s)
{
    size_t size = strlen(s) + 1;
    char *dup = malloc(size);
    if (dup != NULL)
        memcpy(dup, s, size);
    return dup;
}
