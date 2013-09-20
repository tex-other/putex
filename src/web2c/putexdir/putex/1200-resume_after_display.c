Static void resumeafterdisplay(void)
{
  if (curgroup != mathshiftgroup)
    confusion(S(934));
  unsave();
  prevgraf += 3;
  pushnest();
  mode = hmode;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
      /*443:*/
  getxtoken();
  if (curcmd != spacer)   /*:443*/
    backinput();
  if (nestptr == 1)
    buildpage();
}
