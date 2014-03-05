% CHANGES file for 'tin-pot/discount'
% Martin Hofmann <tin-pot@gmx.net>
% 2014-03-05


Changes in this distribution of `discount`
==========================================

1  Overview
-----------

For the most part, the "[_tin-pot/discount_][gh-tp]" sources are
identical to those in David Parsons' "[_orc/discount_][gh-orc]"
repository (after merging his repository into "_tin-pot/discount_" from
time to time).

Building upon David Parsons' work, this distribution has two goals:

- Allow an "out-of-the-box" build on _Windows_ using _Visual C++_ (or
  any _Standard C_ compiler),

- add some features that I deemed useful, mostly visible as new options
  for `mkd2html`.

To this end, this distribution differs from "_discount/orc_" in these
respects:

1. Project files for MS Visual C++ 2003 and 2008 (in `MSVC/`) are
   provided, as well as

2. files (in `MSVC/posc/`) to allow building the rather "*UNIX*ish"
   `discount` package on _Windows_---using (the _C_ part of) any version
   or edition of _Visual C++_ without additional requirements.

3. New options for `mkd2html` (most just set a corresponding flag):
   `-S` and `-I` to choose _HTML 4.01 Strict_ or _ISO 15445:2000_
   as the output _document type_ instead of the default _HTML 4.01
   Transitional_.

4. When the output is "ISO HTML" (`-I`):

   - don't write `width` and `height` attributes for images and objects;

   - don't write a `type="a"` attribute for "alphabetic" `<OL>` lists,
     but emit a `class="alf"` attribute for "alphabetic" lists, and
     `class="num"` else;

   - generate a `summary` attribute in each `<table>` (the attribute is
     required in "ISO HTML").

5. Two new options `-l` and `-u` to choose the input encoding: `ISO-8859-1`
   or `UTF-8`.

6. Three new options `-A`, `-L`, and `-U` to choose the output encoding:
   `US-ASCII`, `ISO-8859-1`, or `UTF-8`.

7. An option `-T` to generate a Table of Contents, and place it at
   the beginning of the document (*This "feature" is coarse and
   preliminary!*).

8. A new syntax modeled after the Markdown notation "`![text](uri
   =WxH)`" for images (`<img>`), which is very similar but generates
   `<object>` elements (with a MIME type derived from the suffix of the
   URI):

       ?[text](uri =WxH)

9. The new flag value `wiki` to produce a variant of output that is
   somewhat easier to post-process by Wiki software.


2  Changes to `discount` source files
-------------------------------------

- `generate.c`: **ADD:** New code checks `MKD_WIKI` and `IS_WIKI`; new
  function `wikiurl()`; new *linkytypes* `wikit` and `objt` and related
  `IS_WIKI` and `IS_MIME_URL` values. Output `summary` attribute in
  `<table>` when generating ISO HTML.

- `generate.c`: **ADD:** Check `MKD_ISO` and supress/emit some
  attributes in "ISO mode".

- `generate.c`: **ADD:** Process "Object" syntax into `<object>`
  elements.

- `markdown.h`: **ADD:** Definition of new `MKD_`\* bit mask values.
  **Important:** must match definition in `mkdio.h.in`!

- `mkd2html.c`: **ADD:** Enable footnotes by default, and generate them.

- `mkd2hmtl.c`: **ADD:** Generate TOC if requested by `-T`.

- `pgm_options.c`: **ADD:** Detect flag `-f wiki`.


3  Changes to `discount` template files
---------------------------------------

The configuration script `configure.sh` generates four files:

1. `config.h` from user options and platform diagnostics.
2. `Makefile` from the template `Makefile.in`.
3. `version.c` from the template `version.c.in`.
4. `mkdio.h` from the template `mkdio.h.in`>

Required changes to these generated files are effected by changing the
corresponding template file:

- `version.c.in`: **ADD:** A suffix added to the version identification
  shown in the `markdown -V` output: `"-wiki"`. This differentiates it
  from the version information shown by an *original* `markdown` built
  from `discount/orc`.

- `mkdio.h.in`: **ADD:** Define new flag bit-values `MKD_`\*.
  **Important:** They must match the definition in `markdown.h`!


4  Files of `discount` intentionally *not* changed
--------------------------------------------------

- `VERSION`: This should only contain a numeric version identification.
  The version information output by `markdown -V` is modified in
  `version.c.in`.


5  Added files and directories
------------------------------

- `MSVC/`: All files in this directory are new, written by Martin
  Hofmann (<tin-pot@gmx.net>), except for these files:

     - `ansidecl.h`, `ansidecl_1996.h`, and `ansidecl_2001.h` are
       *Copyright (c) 1991--2001 Free Software Foundation*.
     - `getopt.c` and `getopt.h` are *Copyright (c) 2000--2003 Mark K.
       Kim*.
     - `inttypes.h`, `stdint.h` are *Copyright (c) 2006 Alexander
       Chemeris*.

     These files contain their own license conditions, see the contents
     of `MSVC/posc/`.


[gh-orc]:https://github.com/Orc/discount/
[gh-tp]:https://github.com/tin-pot/discount/
[dp]:http://www.pell.portland.or.us/~orc/Code/discount/
[co]:https://github.com/Orc/discount/commit/c1904052041dafc085791e6f06340dbd1472536b

