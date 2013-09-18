Static void newfont(smallnumber a)
{
  pointer u;
  scaled s;
  internalfontnumber f;
  strnumber t;
  char oldsetting;
/* XXXX  strnumber flushablestring; */

  if (jobname == 0)
    openlogfile();
  getrtoken();
  u = curcs;
  if (u >= hashbase)
    t = text(u);
  else if (u >= singlebase) {
    if (u == nullcs)
      t = S(950);
    else
      t = u - singlebase;
  } else {
    oldsetting = selector;
    selector = newstring;
    print(S(950));
    print(u - activebase);
    selector = oldsetting;
    str_room(1);
    t = makestring();
  }
  define(u, setfont, nullfont);
  scanoptionalequals();
  scanfilename();   /*1258:*/
  nameinprogress = true;
  if (scankeyword(S(951))) {   /*1259:*/
    scannormaldimen();
    s = curval;
    if (s <= 0 || s >= 134217728L) {
      printnl(S(292));
      print(S(952));
      printscaled(s);
      print(S(953));
      help2(S(954),S(955));
      error();
      s = unity * 10;
    }
  }  /*:1259*/
  else if (scankeyword(S(956))) {
    scanint();
    s = -curval;
    if (curval <= 0 || curval > 32768L) {
      printnl(S(292));
      print(S(486));
      help1(S(487));
      interror(curval);
      s = -1000;
    }
  } else
    s = -1000;
  nameinprogress = false;   /*:1258*/
  /*1260:*/
#if 0
  flushablestring = strptr - 1;
#endif
  for (f = 1; f <= fontptr; f++) {
    if (streqstr(get_fontname(f), curname) /* &
	streqstr(fontarea[f ], curarea) */ )
    {   /*:1260*/
#if 0
      if (curname == flushablestring) {
	flushstring();
	curname = fontname[f ];
      }
#endif
      if (s > 0) {
	if (s == get_fontsize(f ))
	  goto _Lcommonending;
      } else if (get_fontsize(f) ==
		 xnoverd(get_fontdsize(f), -s, 1000))
	goto _Lcommonending;
    }
  }
  f = readfontinfo(u, curname, curarea, s);
_Lcommonending:
  equiv(u) = f;
  eqtb[fontidbase + f - activebase] = eqtb[u - activebase];
  set_fontidtext(f,t);
}
