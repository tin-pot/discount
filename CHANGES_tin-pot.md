
Changes in this distribution of `discount`
==========================================

1  Overview
-----------

For the most part, the "[_tin-pot/discount_][gh-tp]" sources are
identical to those in David Parsons' "[_orc/discount_][gh-orc]"
repository (after merging his repository into "_tin-pot/discount_" from
time to time).

Building upon David Parsons' work, this distribution has two goals:

- Allow an "out-of-the-box" build on _Windows_ using _Visual C++_ (or
  possibly any _Standard C_ compiler),

- add some features that I deemed useful, mostly visible as new options
  for `mkd2html`.

To this end, this distribution differs from "_discount/orc_" in these
respects:

1. Project files for _Visual C++ 2003.NET_ and _Visual Studio 2008_ are
   provided in the `MSVC/` directory, as well as

2. files (in the `MSVC/posc/` directory) that allow building the rather
   "*UNIX*ish" `discount` package on _Windows_---using (the _C_ part
   of) any version or edition[^1] of _Visual C++_ without additional
   requirements.

Some new features are also available if the associated "feature macro"
in `"config.h"` is defined to evaluate to non-zero.

If none of these macros is "switched on", the resulting programs should
be functionally equivalent to those from the "_orc/discount_" code base.


### 1.1  The `WITH_ENCODINGS` feature

Enables processing of input and generating of output text in any of the
encodings:

1. US-ASCII,
2. ISO 8859-1,
3. UTF-8.

In `mkd2html`, the options `-l`, `-u`, `-A`, `-L`, `-U` select these
encodings.


### 1.2  The `WITH_DOCTYPES` feature

Enables `mkd2html` to generate HTML with any of the document types:

1. _ISO 15445:2000_,
2. _HTML 4.01_ ("Strict"),
3. _HTML 4.01 Transitional_.

In `mkd2html`, the options `-I` and `-S` select the first two of these
document types, _Transitional_ is the default.

When the output is "ISO HTML" (`-I`):

 - don't write `width` and `height` attributes for images and objects;

 - don't write a `type="a"` attribute for "alphabetic" `<OL>` lists,
   but emit a `class="alf"` attribute for "alphabetic" lists, and
   `class="num"` otherwise;

 - generate a `summary` attribute in each `<table>` (this attribute is
   required in "ISO HTML").


### 1.3  The `WITH_HTML_OBJECT` feature

Introduces a new syntax, modeled after the existing _Markdown_ notation

    ![title](uri =WxH)

for `<IMG>` elements, that does create an `<OBJECT>` element, so that
any external object can be embedded in the generated HTML document. The
syntax for this is

    ?[title](uri =WxH)

where the MIME type of the object is derived from the URI's suffix.


### 1.4  Common changes

If any of the mentioned "feature macros" is enabled, there are also some
small changes implied:

1. An option `-T` for `mkd2html` to generate a Table of Contents, and
   place it at the beginning of the document (*This "feature" is coarse
   and preliminary!*).

2. When generating (any style of) HTML output, no XML `<... />` tags are
   generated for empty elements (like `<BR>`, `<HR>`), instead they are
   written as `<...>`, as HTML requires.


2  Changes to `discount` source files
-------------------------------------

- `generate.c`: **ADD:** New code checks `MKD_XML` to suppress "`/>`"
  in HTML; new *linkytype* `objt` and related `IS_MIME_URL` value. New
  value `OBJ` in additon to `IMG` for the `image` parameter of various
  functions.

- `generate.c`: **ADD:** Check `MKD_ISO` and supress/emit some
  attributes in "ISO mode", output `summary` attribute in `<table>`.

- `generate.c`: **ADD:** Process "Object" syntax into `<object>`
  elements.

- `markdown.h`: **ADD:** Definition of new `MKD_`\* bit mask values.
  **Important:** must match definition in `mkdio.h.in`!

- `mkd2html.c`: **ADD:** Options for the new features.

- `mkd2html.c`: **ADD:** Enable footnotes by default, and generate them.

- `mkd2hmtl.c`: **ADD:** Generate TOC if requested by `-T`.


3  Changes to `discount` template files
---------------------------------------

The configuration script `configure.sh` generates four files:

1. `config.h` from user options and platform diagnostics.
2. `Makefile` from the template `Makefile.in`.
3. `version.c` from the template `version.c.in`.
4. `mkdio.h` from the template `mkdio.h.in`>

Required changes to these generated files are effected by changing the
corresponding template file:

- `version.c.in`: **ADD:** A suffix added to the version identification
  shown in the `markdown -V` output, to differentiate it from the
  version information shown by an *original* `markdown` built from the
  "_orc/discount_" repository.

- `mkdio.h.in`: **ADD:** Define new flag bit-values `MKD_`\*.
  **Important:** They must match the definition in `markdown.h`!


4  Files of `discount` intentionally *not* changed
--------------------------------------------------

- `VERSION`: This should only contain a numeric version identification.
  The version information output by `markdown -V` is composed in
  `version.c.in`. The current version is `2.1.7`.


5  Added files and directories
------------------------------

In directory `MSVC/`: All files in this directory are new, written by
Martin Hofmann `<tin-pot@gmx.net>`, except for these files:

 - `ansidecl.h`, `ansidecl_1996.h`, and `ansidecl_2001.h` are *Copyright
   (c) 1991--2001 Free Software Foundation*.

 - `getopt.c` and `getopt.h` are *Copyright (c) 2000--2003 Mark K. Kim*.

 - `inttypes.h`, `stdint.h` are *Copyright (c) 2006 Alexander Chemeris*.

These files contain their own license conditions, see the contents of
`MSVC/posc/`.

[^1]:I only did try out _2003.NET_, and _2008_ yet, but I see no reason otherwise. 
[gh-orc]:https://github.com/Orc/discount/
[gh-tp]:https://github.com/tin-pot/discount/
[dp]:http://www.pell.portland.or.us/~orc/Code/discount/
[co]:https://github.com/Orc/discount/commit/c1904052041dafc085791e6f06340dbd1472536b

