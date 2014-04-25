SET SCRIPT=MdAsciiMathML.js
mkd2html -l -U -S -css default.css -header "<script type=\"text/javascript\" src=\"%SCRIPT%\"></script>" -a\u0025 %* %~n1.html

