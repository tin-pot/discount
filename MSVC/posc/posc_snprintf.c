/*** posc_snprintf.c ***************************************************

This code was grabbed from a Stack Overflow discussion, and adapted
to cope with the bogus _vsnprintf() of VC7.1.

http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010

***********************************************************************/

#include "stdio.h" /* The POSC extended one, that is. */
#include <stdarg.h> /* The non-extended one will do. */

#ifdef snprintf /* Emulation required? */

int posc_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = posc_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

int posc_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

#ifdef _MSC_VER
    if (size > 0) {
#if _MSC_VER >= 1400
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
#else
        /*
         * VC 7.1: _vsnprintf() does only append a NUL if there is 
         * room for it after the complete formatted string has been 
         * written!
         */
        count = _vsnprintf(str, size, format, ap);
        str[size-1] = '\0';
#endif
    }
    if (count == -1)
        count = _vscprintf(format, ap);
#else
# error This works with MSVC only.
#endif // _MSC_VER

    return count;
}

#endif /* defined(snprintf) */
