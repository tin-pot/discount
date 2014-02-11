#ifndef POSC_H_INCLUDED
#define POSC_H_INCLUDED

#include "posc_hdrdir.h"

#define POSC_XSTR(s) #s
#define POSC_STR(s) POSC_XSTR(s)
#ifdef _WIN32
# define POSC_XCONC(s1, s2) POSC_STR(s1 ## \\ ## s2)
#else
# define POSC_XCONC(s1, s2) POSC_STR(s1 ## / ## s2)
#endif
#define POSC_CONC(s1,s2) POSC_XCONC(s1,s2)

#define POSC_HEADER(h) POSC_CONC(POSC_STDDIR, h)

/* 
   A post-it comment: _MSC_VER identifies MS Visual C/C++.

   Visual Studio |   VC   |    _MSC_VER
   --------------|--------|--------------
   .NET 2003     |   7.1  |      1310
        2005     |   8.0  |      1400
        2008     |   9.0  |      1500
        2010     |  10.0  |      1600
   --------------|--------|---------------

   _WIN32 identifies Windows platform (any version, even x86_64).
   __GNUC__ identifies GNU compiler and preprocessor.
*/

#ifdef _MSC_VER
#pragma comment( lib , "posc.lib" )
#endif

#endif /* POSC_H_INCLUDED */

