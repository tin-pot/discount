% ASCIIMath Sample
% mh@tin-pot.net
% 2014-04-24T12:25:30+0200


# Using _ASCIIMath_ with _discount_ Markdown

This _Markdown_ text was processed with ``mkd2hmtl`` (on Windows):

    SET SCRIPT=ASCIIMathML.js?config=default
    mkd2html -l -L -css default.css -header "<script src=\"%SCRIPT%\"></script>" -a\u0025 %* %~n1.html

Note the ``-a`` option, which is new. The (optional) character given
after the ``a`` option ist the delimiter for ASCIIMath formulas. The
default is the backtick character, but this is already used by the
standard markdown syntax. So we use a percent sign here (given as a
Unicode code point U+0025).

But in any case, one can still use double backticks for "code" spans:
here is some code: ``template<T> class foo {...}``.


## Original _discount_

### Using _ASCII_ syntax

1. Writing out the _ASCIIMath_ text in "raw" form:<br>

       Here is a formula: [](raw:a-math a^2 + b^2 = c^2 enda-math) And so on.

   Here is a formula: [](raw:amath a^2 + b^2 = c^2 endamath) And so on.

2. Writing out the _ASCIIMath_ text in "raw" form, delimited with U+0025:

       Here is a formula: [](raw:%a^2 + b^2 = c^2%) And so on.

   Here is a formula: [](raw:%a^2 + b^2 = c^2%) And so on.

3. Hiding the ``^`` characters from _discount_:<br>

       Here is a formula: % a\^2 + b\^2 = c\^2 %. And so on.

   Here is a formula: % a\^2 + b\^2 = c\^2 %. And so on.

4. Not hiding the ``^`` characters -- the result is *visually* correct, but in fact different.<br>

       Here is a formula: % a^2 + b^2 = c^2 %. And so on.

   Here is a formula: % a^2 + b^2 = c^2 %. And so on.

### Using _ASCIIMath_ with LaTeX syntax

Because ``$`` has no special meaning to _discount_, this is somewhat more
robust:

    Here is the same formula again: Sa^2 + b^2 = c^2S.

Here is the same formula again: $a^2 + b^2 = c^2$.


With the ``-a`` option, no escaping of the ``^`` markup is needed.

    And here one in "display" style: SS\sum a_i^2 = c^2 = n \cdot \bar cSS

And here one in "display" style: $$\sum a_i^2 = c^2 = n \cdot \bar c$$

## Modified _discount_ with the ``-a`` option

This let you delimit ASCIIMath syntax with any character you like.
Since only _Markdown_ (rsp `mkd2html`) sees this character, you can 
use it without quoting (with a backslash "`\`") in "code" sections:

    Here is a formula: %a^2 + b^2 = c^2%. And here is `code`. And so on.

Here is a formula: %a^2 + b^2 = c^2%. And here is `code`. And so on.

And because `mkd2html` will not create an empty formula (from a pair of
adjacent special characters), you can use a *double* delimiter character 
to denote itself in normal _Markdown_ text (**TODO**):

    Here is percent sign: %% -- no formula anywhere.

Here is percent sign: %% -- no formula anywhere.

Note that there is no need to hide the ``^`` characters by inserting extra backslashes.

