% README file for 'tin-pot/discount'
% Martin Hofmann <tin-pot@gmx.net>
% 2014-03-05


The Markdown tool "discount" on _Windows_:<br>"_tin-pot/discount_"
================================================================

This repository is a fork of [David Parsons' repository][dc-orc]
containing his [implementation][dp] in C of (a processor for) [John
Gruber's "Markdown" markup language][md], with

- added support to build the package with _Visual C++_ on _Windows_,

- and some minor new features.

See also the home page of this distribution at *github*: [tin-
pot/discount][gh].

In the following, the term "_UNIX_" refers to all the _UNIX_-like
platforms that `discount` does originally target.


1  Modifications
----------------

The modifications added in this distribution are:

- Added pre-configured files

  - `version.c`,
  - `mkdio.h`, and
  - `config.h`,

  in `MSVC/` so that no `configure.sh` run---which generates these files
  on _UNIX_---is needed on _Windows_.

- Added project files for all executables and libraries in `MSVC/`.

- Added supporting files for building on _Windows_ in `MSVC/posc/`,
  including the [`freegetopt`][go] implementation of _UNIX_'s `getopt()`
  function which is used by `main.c`.

See the [CHANGES](CHANGES_tin-pot.html) file for more details.


2  Building/Installing
----------------------

The package should build "out of the box" with _Visual Studio 2003.NET_ or
_Visual Studio 2008_ or newer (later versions of _Visual Studio_ will import
and upgrade the project files).

The generated executable are statically linked: they only depend on
`kernel32.dll`. Place them wherever you like on your `PATH`.


3  About the Port
-----------------

Porting to _Windows_ was done with a fairly general technique, which
presents to the compiler a "faked" environment of header files on
top of the "real" header files of _Visual C++_. All this is contained in the
`MSVC/posc` directory. See the [`MSVC/posc/README`][posc] file
there for explanations.

All `discount` source files in the project root directory are
unmodified. Their need for functions like `strncasecmp()` (_UNIX_), or
`vsnprintf()` (C99), or `getopt()` gets satisfied completely out of
files in `posc`.

The included `getopt()` implementation is [freegetopt][go], its license
is in the file `LICENSE-freegetopt`.


[gh]:http://github.com/tin-pot/discount/
[dc-orc]:http://github.com/orc/discount/
[dp]:http://www.pell.portland.or.us/~orc/Code/discount/
[md]:http://daringfireball.net/projects/markdown/
[go]:http://freegetopt.sourceforge.net/
[posc]:MSVC/posc/README.html


