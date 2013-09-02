%% This is file 'chineserbcomm.inc'
%%
%%   Common parts for chinese report and book
%%
%% Copyright 1999-2004
%% The PUTeX Project and any individual authors listed elsewhere
%% in this file.
%% This file is part of the PUTeX system. You are NOT allowed to
%% distribute a modified version of this file.
%% ----------------------------------------
%%   This system is distributed in the hope that it will be useful,
%%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
%%
\usepackage{chinesenames}
\def\@pu@setref#1#2#3{%
  \ifx#1\relax
   \protect\G@refundefinedtrue
   \nfss@text{\reset@font\bfseries ??}%
   \@latex@warning{Reference `#3' on page \thepage \space
             undefined}%
  \else
   \pu@chapcnum\expandafter#2#1\null
  \fi}
\newcommand*{\twref}[1]{\expandafter\@pu@setref\csname r@#1\endcsname\@firstoftwo{#1}}
\newcommand*{\TWchapHeadingFont}{\normalfont\Huge\bfseries}
\newcommand*{\TWchapContentFont}{\large\bfseries}
\ifnum\the\puxCharSet=1 % Big5
  \renewcommand*{\chaptername}{³¹}
  \newcommand*{\twchaptername}{²Ä\pu@chapcnum\c@chapter \chaptername}
\else % GBK
  \renewcommand*{\chaptername}{ÕÂ}
  \newcommand*{\twchaptername}{µÚ\pu@chapcnum\c@chapter \chaptername}
\fi
\def\@makeschapterhead#1{%
  \vspace*{50\p@}%
  {\parindent \z@ \pu@chhdalign
    \TWchapHeadingFont
    \interlinepenalty\@M
    #1\par\nobreak
    \vskip 40\p@
  }}
\renewcommand\appendix{\par
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  \renewcommand\@chapapp{\appendixname}%
  \renewcommand\thechapter{\@Alph\c@chapter}%
  \renewcommand\twchaptername{\appendixname \thechapter}}
\let\tw@numberline\numberline
\renewcommand*\l@chapter[2]{%
  \ifnum \c@tocdepth >\m@ne
    \addpenalty{-\@highpenalty}%
    \vskip 1.0em \@plus\p@
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode \TWchapContentFont
      \def\numberline##1{%
        \settowidth{\@tempdima}{##1}%
        \addtolength{\@tempdima}{0.6em}%
        \advance\leftskip \@tempdima
        \hskip -\leftskip
        \tw@numberline{##1}}%
      #1\nobreak\hfil \nobreak\hb@xt@\@pnumwidth{\hss\normalsize #2}\par
      \vskip 0.4em \@plus\p@
      \penalty\@highpenalty
    \endgroup
  \fi}
\if@twsec
  \InputIfFileExists{chinesesection.inc}{}{%
    \ClassError{\pu@classname}{%
      Can't load chinesesection.inc}{}}
\fi
\endinput
