%% This is file 'gbkreport.cls'
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
\NeedsTeXFormat{LaTeX2e}[2003/12/01]
\ProvidesClass{gbkreport}[2004/09/13 v1.1 gbLaTeX standard class]
\def\pu@classname{gbkreport}
\InputIfFileExists{chinesereport.inc}{}{%
    \ClassError{\pu@classname}{%
      Can't load chinesereport.inc}{}}
\endinput
