%% This is file 'twbook.cls'
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
\NeedsTeXFormat{LaTeX2e}[2003/12/01]
\ProvidesClass{twbook}[2004/09/13 v1.1 Big5LaTeX standard class]
\def\pu@classname{twbook}
\InputIfFileExists{chinesebook.inc}{}{%
    \ClassError{\pu@classname}{%
      Can't load chinesebook.inc}{}}
\endinput
