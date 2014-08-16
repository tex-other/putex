%% This is file 'chinesesection.inc'
%%
%% Copyright 1998-2004
%% The PUTeX Project and any individual authors listed elsewhere
%% in this file.
%% This file is part of the PUTeX system. You are NOT allowed to
%% distribute a modified version of this file.
%% ----------------------------------------
%%   This system is distributed in the hope that it will be useful,
%%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
%%
\newcommand{\twsecsepchar}{.}
\newcommand{\twthesection}{\PUXcnumber\c@chapter\twsecsepchar\PUXcnumber\c@section}
\newcommand{\twthesubsection}{\twthesection\twsecsepchar\PUXcnumber\c@subsection}
\newcommand{\twthesubsubsection}{\twthesubsection\twsecsepchar\PUXcnumber\c@subsubsection}
\newcommand{\twsectionlabel}{\twthesection}
\newcommand{\twsubsectionlabel}{\twthesubsection}
\newcommand{\twsubsubsectionlabel}{\twthesubsubsection}
\newcommand{\twcsectionlabel}{\twthesection}
\newcommand{\twcsubsectionlabel}{\twthesubsection}
\newcommand{\twcsubsubsectionlabel}{\twthesubsubsection}
\def\@seccntformat#1{\csname tw#1label\endcsname\quad}
\def\@sect#1#2#3#4#5#6[#7]#8{%
  \ifnum #2>\c@secnumdepth
    \let\@svsec\@empty
  \else
    \refstepcounter{#1}%
    \protected@edef\@svsec{\@seccntformat{#1}\relax}%
  \fi
  \@tempskipa #5\relax
  \ifdim \@tempskipa>\z@
    \begingroup
      #6{%
        \@hangfrom{\hskip #3\relax\@svsec}%
          \interlinepenalty \@M #8\@@par}%
    \endgroup
    \csname #1mark\endcsname{#7}%
    \addcontentsline{toc}{#1}{%
      \ifnum #2>\c@secnumdepth \else
        \protect\numberline{\csname twc#1label\endcsname}%
      \fi
      #7}%
  \else
    \def\@svsechd{%
      #6{\hskip #3\relax
      \@svsec #8}%
      \csname #1mark\endcsname{#7}%
      \addcontentsline{toc}{#1}{%
        \ifnum #2>\c@secnumdepth \else
          \protect\numberline{\csname twc#1label\endcsname}%
        \fi
        #7}}%
  \fi
  \@xsect{#5}}
\def\@dottedtocline#1#2#3#4#5{%
  \ifnum #1>\c@tocdepth \else
    \vskip \z@ \@plus.2\p@
    {\leftskip #2\relax \rightskip \@tocrmarg \parfillskip -\rightskip
     \parindent #2\relax\@afterindenttrue
     \interlinepenalty\@M
     \leavevmode
     \def\numberline##1{%
        \settowidth{\@tempdima}{##1}%
        \addtolength{\@tempdima}{0.6em}%
        \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
        \tw@numberline{##1}}%
     #4\nobreak
     \leaders\hbox{$\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep
        mu$}\hfill
     \nobreak
     \hb@xt@\@pnumwidth{\hfil\normalfont \normalcolor #5}%
     \par}%
  \fi}
\endinput
