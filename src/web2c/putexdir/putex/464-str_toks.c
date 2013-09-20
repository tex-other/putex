pointer tex_global_p;

Static void strtoks_helper(strASCIIcode t)
{
    long tt=t;
    pointer p=tex_global_p;
    if (tt == ' ')
      tt = spacetoken;
    else
      tt += othertoken;
    faststorenewtoken(p,tt);
    tex_global_p=p;
}

Static halfword strtoks(str_poolpointer b)
{
  pointer p;
  str_room(1);
  p = temphead;
  link(p) = 0;
  tex_global_p=p;
  str_map_from_mark(b, strtoks_helper); 
  p=tex_global_p;
  return p;
}
