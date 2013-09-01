/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/winansi.h,v 1.1 2003/12/27 18:03:16 csc Exp $
    
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

#ifndef _WINANSI_H_
#define _WINANSI_H_

static char *winansi_encoding[256] = {
  ".notdef", ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef", ".notdef", ".notdef", ".notdef",
  ".notdef", ".notdef",
  "space", "exclam", "quotedbl", "numbersign", "dollar",
  "percent", "ampersand", "quotesingle", "parenleft", "parenright",
  "asterisk", "plus", "comma", "hyphen", "period",
  "slash", "zero", "one", "two", "three", "four",
  "five", "six", "seven", "eight", "nine", "colon",
  "semicolon", "less", "equal", "greater", "question",
  "at", "A", "B", "C", "D",
  "E", "F", "G", "H", "I",
  "J", "K", "L", "M", "N",
  "O", "P", "Q", "R", "S",
  "T", "U", "V", "W", "X",
  "Y", "Z", "bracketleft", "backslash", "bracketright",
  "asciicircum", "underscore", "grave", "a", "b",
  "c", "d", "e", "f", "g",
  "h", "i", "j", "k", "l",
  "m", "n", "o", "p", "q",
  "r", "s", "t", "u", "v",
  "w", "x", "y", "z", "braceleft",
  "bar", "braceright", "asciitilde", "bullet", "bullet",
  "bullet", "quotesinglbase", "florin", "quotedblbase", "ellipsis",
  "dagger", "daggerdbl", "circumflex",  "perthousand", "Scaron",
  "guilsinglleft", "OE", "bullet", "bullet", "bullet",
  "bullet", "quoteleft", "quoteright", "quotedblleft",
  "quotedblright",
  "bullet", "endash", "emdash", "tilde", "trademark",
  "scaron", "guilsinglright", "oe", "bullet", "bullet",
  "Ydieresis", "space", "exclamdown", "cent", "sterling",
  "currency", "yen", "brokenbar", "section", "dieresis",
  "copyright", "ordfeminine", "guillemotleft", "logicalnot", "hyphen", "registered",
  "macron", "degree", "plusminus", "twosuperior", "threesuperior",
  "acute", "mu", "paragraph", "periodcentered", "cedilla",
  "onesuperior", "ordmasculine", "guillemotright", "onequarter", "onehalf",
  "threequarters", "questiondown", "Agrave", "Aacute", "Acircumflex",
  "Atilde", "Adieresis", "Aring", "AE", "Ccedilla", "Egrave",
  "Eacute", "Ecircumflex", "Edieresis", "Igrave", "Iacute",
  "Icircumflex", "Idieresis", "Eth", "Ntilde", "Ograve", "Oacute",
  "Ocircumflex", "Otilde", "Odieresis", "multiply", "Oslash",
  "Ugrave", "Uacute", "Ucircumflex", "Udieresis", "Yacute", "Thorn",
  "germandbls", "agrave", "aacute", "acircumflex", "atilde",
  "adieresis", "aring", "ae", "ccedilla", "egrave", "eacute",
  "ecircumflex", "edieresis", "igrave", "iacute", "icircumflex",
  "idieresis", "eth", "ntilde", "ograve", "oacute", "ocircumflex",
  "otilde", "odieresis", "divide", "oslash", "ugrave", "uacute",
  "ucircumflex", "udieresis", "yacute", "thorn", "ydieresis"
};

#endif /* _WINANSI_H_ */
