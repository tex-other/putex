%% This is file 'chinesereport.inc'
%%
%% Copyright 2004
%% The PUTeX Project and any individual authors listed elsewhere
%% in this file.
%% This file is part of the PUTeX system. You are NOT allowed to
%% distribute a modified version of this file.
%% ----------------------------------------
%%   This system is distributed in the hope that it will be useful,
%%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
%%
\InputIfFileExists{chineserbopts.inc}{}{%
    \ClassError{\pu@classname}{%
      Can't load chineserbopts.inc}{}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\ProcessOptions
\LoadClass{report}
\InputIfFileExists{chineserbcomm.inc}{}{%
    \ClassError{\pu@classname}{%
      Can't load chineserbcomm.inc}{}}
\if@twoside
  \def\ps@headings{%
      \let\@oddfoot\@empty\let\@evenfoot\@empty
      \def\@evenhead{\thepage\hfil\slshape\leftmark}%
      \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
      \let\@mkboth\markboth
    \def\chaptermark##1{%
      \markboth {\MakeUppercase{%
        \ifnum \c@secnumdepth >\m@ne
             \twchaptername\ \ \
        \fi
        ##1}}{}}%
    \def\sectionmark##1{%
      \markright {\MakeUppercase{%
        \ifnum \c@secnumdepth >\z@
          \thesection. \ %
        \fi
        ##1}}}}
\else
  \def\ps@headings{%
    \let\@oddfoot\@empty
    \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
    \let\@mkboth\markboth
    \def\chaptermark##1{%
      \markright {\MakeUppercase{%
        \ifnum \c@secnumdepth >\m@ne
            \twchaptername\ \ \
        \fi
        ##1}}}}
\fi
\def\@chapter[#1]#2{\ifnum \c@secnumdepth >\m@ne
                         \refstepcounter{chapter}%
                         \typeout{\twchaptername.}%
                         \addcontentsline{toc}{chapter}%
                                   {\protect\numberline{\twchaptername}#1}%
                    \else
                      \addcontentsline{toc}{chapter}{#1}%
                    \fi
                    \chaptermark{#1}%
                    \addtocontents{lof}{\protect\addvspace{10\p@}}%
                    \addtocontents{lot}{\protect\addvspace{10\p@}}%
                    \if@twocolumn
                      \@topnewpage[\@makechapterhead{#2}]%
                    \else
                      \@makechapterhead{#2}%
                      \@afterheading
                    \fi}
\def\@makechapterhead#1{%
  \vspace*{50\p@}%
  {\parindent \z@ \pu@chhdalign \TWchapHeadingFont
    \ifnum \c@secnumdepth >\m@ne
        \twchaptername
        \par\nobreak
        \vskip 20\p@
    \fi
    \interlinepenalty\@M
    \pu@chhdalign #1\par\nobreak
    \vskip 40\p@
  }}
\pagestyle{plain}
\endinput
