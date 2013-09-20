Static void newgraf(boolean indented)
{
  prevgraf = 0;
  if (mode == vmode || head != tail) {
    tailappend(newparamglue(parskipcode));
  }
  pushnest();
  mode = hmode;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
  if (indented) {
    tail = newnullbox();
    link(head) = tail;
    width(tail) = parindent;
  }
  if (everypar != 0)
    begintokenlist(everypar, everypartext);
  if (nestptr == 1)
    buildpage();
}
