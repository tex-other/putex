Static void shiftcase(void)
{
  pointer b, p;
  halfword t;
  eightbits c;

  b = curchr;
  p = scantoks(false, false);
  p = link(defref);
  while (p != 0) {  /*1289:*/
    t = info(p);
    if (t < cstokenflag + singlebase) {   /*:1289*/
      c = t & (dwa_do_8-1);
      if (equiv(b + c) != 0)
	info(p) = t - c + equiv(b + c);
    }
    p = link(p);
  }
  backlist(link(defref));
  freeavail(defref);
}
