#ifndef POSC_STDARG_H_INCLUDED
#define POSC_STDARG_H_INCLUDED
#include "posc.h"

#include POSC_STDARG_HDR

/*
 * VS2003 (at least) has no va_copy - we can get away with this.
 *
 * cf. http://stackoverflow.com/questions/558223/va-copy-porting-to-visual-c
 */
#ifdef _MSC_VER
# define va_copy(lhs, rhs) ((lhs) = (rhs))
#endif /* _MSC_VER */

#endif /* POSC_STDARG_H_INCLUDED */
