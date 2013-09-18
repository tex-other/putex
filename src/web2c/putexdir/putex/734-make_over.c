Static void makeover(halfword q)
{
  info(nucleus(q)) = overbar(
      cleanbox(nucleus(q), crampedstyle(curstyle)),
      defaultrulethickness * 3, defaultrulethickness);
  mathtype(nucleus(q)) = subbox;
}
