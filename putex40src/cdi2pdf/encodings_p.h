/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/encodings_p.h,v 1.1 2003/12/27 18:03:16 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>

    Copyright (C) 1998, 1999 by Mark A. Wicks <mwicks@kettering.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#ifndef _ENCODINGS_P_H_
#define _ENCODINGS_P_H_

/*
 * PDF predefined encodings: MacRomanEncoding, MacExpertEncoding, WinAnsiEncoding
 * StandardEncoding is not predefined, but used as default BaseEncoding for
 * nonsymbol fonts. StandardEncoding and ISOLatin1Encoding is required by Type1
 * font support.
 */

#ifdef  _ENCODINGS_C_
const char *
MacRomanEncoding[256] = {
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "space", "exclam",  "quotedbl", "numbersign",
  "dollar", "percent", "ampersand", "quotesingle",
  "parenleft", "parenright", "asterisk", "plus",
  "comma", "hyphen", "period", "slash",
  "zero", "one", "two", "three",
  "four", "five", "six", "seven",
  "eight", "nine", "colon", "semicolon",
  "less", "equal", "greater", "question",
  "at", "A", "B", "C",
  "D", "E", "F", "G", "H",
  "I", "J", "K", "L",
  "M", "N", "O", "P",
  "Q", "R", "S", "T",
  "U", "V", "W", "X",
  "Y", "Z", "bracketleft", "backslash",
  "bracketright", "asciicircum", "underscore",
  "grave", "a", "b", "c",
  "d", "e", "f", "g",
  "h", "i", "j", "k",
  "l", "m", "n", "o",
  "p", "q", "r", "s",
  "t", "u", "v", "w",
  "x", "y", "z", "braceleft",
  "bar", "braceright", "asciitilde", ".notdef",
  "Adieresis", "Aring", "Ccedilla", "Eacute",
  "Ntilde", "Odieresis", "Udieresis", "aacute",
  "agrave", "acircumflex", "adieresis", "atilde",
  "aring", "ccedilla", "eacute", "egrave",
  "ecircumflex", "edieresis", "iacute", "igrave",
  "icircumflex", "idieresis", "ntilde", "oacute",
  "ograve", "ocircumflex", "odieresis", "otilde",
  "uacute", "ugrave", "ucircumflex", "udieresis",
  "dagger", "degree", "cent", "sterling",
  "section", "bullet", "paragraph", "germandbls",
  "registered", "copyright", "trademark", "acute",
  "dieresis", "notequal", "AE", "Oslash",
  "infinity", "plusminus", "lessequal", "greaterequal",
  "yen", "mu", "partialdiff", "summation",
  "product", "pi", "integral", "ordfeminine",
  "ordmasculine", "Omega", "ae", "oslash",
  "questiondown", "exclamdown", "logicalnot", "radical",
  "florin", "approxequal", "Delta", "guillemotleft",
  "guillemotright", "ellipsis", "space", "Agrave",
  "Atilde", "Otilde", "OE", "oe",
  "endash", "emdash", "quotedblleft", "quotedblright",
  "quoteleft", "quoteright", "divide", "lozenge",
  "ydieresis", "Ydieresis", "fraction", "currency",
  "guilsinglleft", "guilsinglright", "fi", "fl",
  "daggerdbl", "periodcentered", "quotesinglbase", "quotedblbase",
  "perthousand", "Acircumflex", "Ecircumflex", "Aacute",
  "Edieresis", "Egrave", "Iacute", "Icircumflex",
  "Idieresis", "Igrave", "Oacute", "Ocircumflex",
  "apple", "Ograve", "Uacute", "Ucircumflex",
  "Ugrave", "dotlessi", "circumflex", "tilde",
  "macron", "breve", "dotaccent", "ring",
  "cedilla", "hungarumlaut", "ogonek", "caron"
};

const char *
MacExpertEncoding[256] = {
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "space", "exclamsmall", "Hungarumlautsmall", "centoldstyle",
  "dollaroldstyle", "dollarsuperior", "ampersandsmall", "Acutesmall",
  "parenleftsuperior", "parenrightsuperior", "twodotenleader", "onedotenleader",
  "comma", "hyphen", "period", "fraction",
  "zerooldstyle", "oneoldstyle", "twooldstyle", "threeoldstyle",
  "fouroldstyle", "fiveoldstyle", "sixoldstyle", "sevenoldstyle",
  "eightoldstyle", "nineoldstyle", "colon", "semicolon",
  ".notdef", "threequartersemdash", ".notdef", "questionsmall",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "Ethsmall", ".notdef", ".notdef", "onequarter",
  "onehalf", "threequarters", "oneeighth", "threeeighths",
  "fiveeighths", "seveneighths", "onethird", "twothirds",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", "ff", "fi",
  "fl", "ffi", "ffl", "parenleftinferior",
  ".notdef", "parenrightinferior", "Circumflexsmall", "hypheninferior",
  "Gravesmall", "Asmall", "Bsmall", "Csmall",
  "Dsmall", "Esmall", "Fsmall", "Gsmall",
  "Hsmall", "Ismall", "Jsmall", "Ksmall",
  "Lsmall", "Msmall", "Nsmall", "Osmall",
  "Psmall", "Qsmall", "Rsmall", "Ssmall",
  "Tsmall", "Usmall", "Vsmall", "Wsmall",
  "Xsmall", "Ysmall", "Zsmall", "colonmonetary",
  "onefitted", "rupiah", "Tildesmall", ".notdef",
  ".notdef", "asuperior", "centsuperior", ".notdef",
  ".notdef", ".notdef", ".notdef", "Aacutesmall",
  "Agravesmall", "Acircumflexsmall", "Adieresissmall", "Atildesmall",
  "Aringsmall", "Ccedillasmall", "Eacutesmall", "Egravesmall",
  "Ecircumflexsmall", "Edieresissmall", "Iacutesmall", "Igravesmall",
  "Icircumflexsmall", "Idieresissmall", "Ntildesmall", "Oacutesmall",
  "Ogravesmall", "Ocircumflexsmall", "Odieresissmall", "Otildesmall",
  "Uacutesmall", "Ugravesmall", "Ucircumflexsmall", "Udieresissmall",
  ".notdef", "eightsuperior", "fourinferior", "threeinferior",
  "sixinferior", "eightinferior", "seveninferior", "Scaronsmall",
  ".notdef", "centinferior", "twoinferior", ".notdef",
  "Dieresissmall", ".notdef", "Caronsmall", "osuperior",
  "fiveinferior", ".notdef", "commainferior", "periodinferior",
  "Yacutesmall", ".notdef", "dollarinferior", ".notdef",
  ".notdef", "Thornsmall", ".notdef", "nineinferior",
  "zeroinferior", "Zcaronsmall", "AEsmall", "Oslashsmall",
  "questiondownsmall", "oneinferior", "Lslashsmall", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", "Cedillasmall", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", "OEsmall",
  "figuredash", "hyphensuperior", ".notdef", ".notdef",
  ".notdef", ".notdef", "exclamdownsmall", ".notdef",
  "Ydieresissmall", ".notdef", "onesuperior", "twosuperior",
  "threesuperior", "foursuperior", "fivesuperior", "sixsuperior",
  "sevensuperior", "ninesuperior", "zerosuperior", ".notdef",
  "esuperior", "rsuperior", "tsuperior", ".notdef",
  ".notdef", "isuperior", "ssuperior", "dsuperior",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", "lsuperior", "Ogoneksmall", "Brevesmall",
  "Macronsmall", "bsuperior", "nsuperior", "msuperior",
  "commasuperior", "periodsuperior", "Dotaccentsmall", "Ringsmall",
  ".notdef", ".notdef", ".notdef", ".notdef"
};

const char *
WinAnsiEncoding[256] = {
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "space", "exclam", "quotedbl", "numbersign",
  "dollar", "percent", "ampersand", "quotesingle",
  "parenleft", "parenright", "asterisk", "plus",
  "comma", "hyphen", "period", "slash",
  "zero", "one", "two", "three",
  "four", "five", "six", "seven",
  "eight", "nine", "colon", "semicolon",
  "less", "equal", "greater", "question",
  "at", "A", "B", "C",
  "D", "E", "F", "G",
  "H", "I", "J", "K",
  "L", "M", "N", "O",
  "P", "Q", "R", "S",
  "T", "U", "V", "W",
  "X", "Y", "Z", "bracketleft",
  "backslash", "bracketright", "asciicircum", "underscore",
  "grave", "a", "b", "c",
  "d", "e", "f", "g",
  "h", "i", "j", "k",
  "l", "m", "n", "o",
  "p", "q", "r", "s",
  "t", "u", "v", "w",
  "x", "y", "z", "braceleft",
  "bar", "braceright", "asciitilde", "bullet",
  "Euro", "bullet", "quotesinglbase", "florin",
  "quotedblbase", "ellipsis", "dagger", "daggerdbl",
  "circumflex", "perthousand", "Scaron", "guilsinglleft",
  "OE", "bullet", "Zcaron", "bullet",
  "bullet", "quoteleft", "quoteright", "quotedblleft",
  "quotedblright", "bullet", "endash", "emdash",
  "tilde", "trademark", "scaron", "guilsinglright",
  "oe", "bullet", "zcaron", "Ydieresis",
  "space", "exclamdown", "cent", "sterling",
  "currency", "yen", "brokenbar", "section",
  "dieresis", "copyright", "ordfeminine", "guillemotleft",
  "logicalnot", "hyphen", "registered", "macron",
  "degree", "plusminus", "twosuperior", "threesuperior",
  "acute", "mu", "paragraph", "periodcentered",
  "cedilla", "onesuperior", "ordmasculine", "guillemotright",
  "onequarter", "onehalf", "threequarters", "questiondown",
  "Agrave", "Aacute", "Acircumflex", "Atilde",
  "Adieresis", "Aring", "AE", "Ccedilla",
  "Egrave", "Eacute", "Ecircumflex", "Edieresis",
  "Igrave", "Iacute", "Icircumflex", "Idieresis",
  "Eth", "Ntilde", "Ograve", "Oacute",
  "Ocircumflex", "Otilde", "Odieresis", "multiply",
  "Oslash", "Ugrave", "Uacute", "Ucircumflex",
  "Udieresis", "Yacute", "Thorn", "germandbls",
  "agrave", "aacute", "acircumflex", "atilde",
  "adieresis", "aring", "ae", "ccedilla",
  "egrave", "eacute", "ecircumflex", "edieresis",
  "igrave", "iacute", "icircumflex", "idieresis",
  "eth", "ntilde", "ograve", "oacute",
  "ocircumflex", "otilde", "odieresis", "divide",
  "oslash", "ugrave", "uacute", "ucircumflex",
  "udieresis", "yacute", "thorn", "ydieresis"
};

const char *
StandardEncoding[256] = {
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "space", "exclam", "quotedbl", "numbersign",
  "dollar", "percent", "ampersand", "quoteright",
  "parenleft", "parenright", "asterisk", "plus",
  "comma", "hyphen", "period", "slash",
  "zero", "one", "two", "three",
  "four", "five", "six", "seven",
  "eight", "nine", "colon", "semicolon",
  "less", "equal", "greater", "question",
  "at", "A", "B", "C",
  "D", "E", "F", "G",
  "H", "I", "J", "K",
  "L", "M", "N", "O",
  "P", "Q", "R", "S",
  "T", "U", "V", "W",
  "X", "Y", "Z", "bracketleft",
  "backslash", "bracketright", "asciicircum", "underscore",
  "quoteleft", "a", "b", "c",
  "d", "e", "f", "g",
  "h", "i", "j", "k",
  "l", "m", "n", "o",
  "p", "q", "r", "s",
  "t", "u", "v", "w",
  "x", "y", "z", "braceleft",
  "bar", "braceright", "asciitilde", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", "exclamdown", "cent", "sterling",
  "fraction", "yen", "florin", "section",
  "currency", "quotesingle", "quotedblleft", "guillemotleft",
  "guilsinglleft", "guilsinglright", "fi", "fl",
  ".notdef", "endash", "dagger", "daggerdbl",
  "periodcentered", ".notdef", "paragraph", "bullet",
  "quotesinglbase", "quotedblbase", "quotedblright", "guillemotright",
  "ellipsis", "perthousand", ".notdef", "questiondown",
  ".notdef", "grave", "acute", "circumflex",
  "tilde", "macron", "breve", "dotaccent",
  "dieresis", ".notdef", "ring", "cedilla",
  ".notdef", "hungarumlaut", "ogonek", "caron",
  "emdash", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", "AE", ".notdef", "ordfeminine",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "Lslash", "Oslash", "OE", "ordmasculine",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", "ae", ".notdef", ".notdef",
  ".notdef", "dotlessi", ".notdef", ".notdef",
  "lslash", "oslash", "oe", "germandbls",
  ".notdef", ".notdef", ".notdef", ".notdef"
};

const char *
ISOLatin1Encoding[256] = {
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "space", "exclam", "quotedbl", "numbersign",
  "dollar", "percent", "ampersand", "quotesingle",
  "parenleft", "parenright", "asterisk", "plus",
  "comma", "hyphen", "period", "slash",
  "zero", "one", "two", "three",
  "four", "five", "six", "seven",
  "eight", "nine", "colon", "semicolon",
  "less", "equal", "greater", "question",
  "at", "A", "B", "C",
  "D", "E", "F", "G",
  "H", "I", "J", "K",
  "L", "M", "N", "O",
  "P", "Q", "R", "S",
  "T", "U", "V", "W",
  "X", "Y", "Z", "bracketleft",
  "backslash", "bracketright", "asciicircum", "underscore",
  "grave", "a", "b", "c",
  "d", "e", "f", "g",
  "h", "i", "j", "k",
  "l", "m", "n", "o",
  "p", "q", "r", "s",
  "t", "u", "v", "w",
  "x", "y", "z", "braceleft",
  "bar", "braceright", "asciitilde", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef",
  "dotlessi", "quoteleft", "quoteright", "circumflex",
  "tilde", "macron", "breve", "dotaccent",
  "dieresis", ".notdef", "ring", "cedilla",
  ".notdef", "hungarumlaut", "ogonek", "caron",
  "space", "exclamdown", "cent", "sterling",
  "currency", "yen", "brokenbar", "section",
  "dieresis", "copyright", "ordfeminine", "guillemotleft",
  "logicalnot", "hyphen", "registered", "macron",
  "degree", "plusminus", "twosuperior", "threesuperior",
  "acute", "mu", "paragraph", "periodcentered",
  "cedilla", "onesuperior", "ordmasculine", "guillemotright",
  "onequarter", "onehalf", "threequarters", "questiondown",
  "Agrave", "Aacute", "Acircumflex", "Atilde",
  "Adieresis", "Aring", "AE", "Ccedilla",
  "Egrave", "Eacute", "Ecircumflex", "Edieresis",
  "Igrave", "Iacute", "Icircumflex", "Idieresis",
  "Eth", "Ntilde", "Ograve", "Oacute",
  "Ocircumflex", "Otilde", "Odieresis", "multiply",
  "Oslash", "Ugrave", "Uacute", "Ucircumflex",
  "Udieresis", "Yacute", "Thorn", "germandbls",
  "agrave", "aacute", "acircumflex", "atilde",
  "adieresis", "aring", "ae", "ccedilla",
  "egrave", "eacute", "ecircumflex", "edieresis",
  "igrave", "iacute", "icircumflex", "idieresis",
  "eth", "ntilde", "ograve", "oacute",
  "ocircumflex", "otilde", "odieresis", "divide",
  "oslash", "ugrave", "uacute", "ucircumflex",
  "udieresis", "yacute", "thorn", "ydieresis"
};
#endif /* _ENCODINGS_C_ */

#endif /* _ENCODINGS_P_H_ */
