%% This is file 'chineserbopts.inc'
%%
%%   Options for Chinese report and book
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
\RequirePackage{chinesebasic}[2004/09/13]
\let\pu@chapcnum\PUXcnumber
\DeclareOption{chapscnum}{\let\pu@chapcnum\PUXscnumber}
\DeclareOption{chapucnum}{\let\pu@chapcnum\PUXucnumber}
\DeclareOption{chapfcnum}{\let\pu@chapcnum\PUXfcnumber}
\DeclareOption{chapacnum}{\let\pu@chapcnum\PUXacnumber}
\let\pu@chhdalign\raggedright
\DeclareOption{rightchhd}{\let\pu@chhdalign\raggedleft}
\DeclareOption{centerchhd}{\let\pu@chhdalign\centering}
\newif\if@twsec \@twsecfalse
\DeclareOption{twsec}{\@twsectrue}
\endinput
