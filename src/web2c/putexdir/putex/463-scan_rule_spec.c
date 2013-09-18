Static halfword scanrulespec(void)
{
  pointer q;

  q = newrule();
  if (curcmd == vrule)
    width(q) = defaultrule;
  else {
    height(q) = defaultrule;
    depth(q) = 0;
  }
_Lreswitch:
  if (scankeyword(S(639))) {
    scannormaldimen();
    width(q) = curval;
    goto _Lreswitch;
  }
  if (scankeyword(S(640))) {
    scannormaldimen();
    height(q) = curval;
    goto _Lreswitch;
  }
  if (!scankeyword(S(641)))
    return q;
  scannormaldimen();
  depth(q) = curval;
  goto _Lreswitch;
/* p2c: tex1.p: Note: Deleting unreachable code [255] */
}
