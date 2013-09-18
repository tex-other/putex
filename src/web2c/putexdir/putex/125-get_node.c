Static halfword getnode(long s)
{
  halfword Result;
  pointer p, q;
  long r, t;

_Lrestart:
  p = rover;
  do {   /*127:*/
    q = p + nodesize(p);
    while (isempty(q)) {
      t = rlink(q);
      if (q == rover)
	rover = t;
      llink(t) = llink(q);
      rlink(llink(q)) = t;
      q += nodesize(q);
    }
    r = q - s;
    if (r > p + 1) {   /*128:*/
      nodesize(p) = r - p;
      rover = p;
      goto _Lfound;
    }
    /*:128*/
    if (r == p) {
      if (rlink(p) != p) {   /*129:*/
	rover = rlink(p);
	t = llink(p);
	llink(rover) = t;
	rlink(t) = rover;
	goto _Lfound;
      }
      /*:129*/
    }
    nodesize(p) = q - p;   /*:127*/
    p = rlink(p);
  } while (p != rover);
  if (s == 1073741824L) {
    Result = maxhalfword;
    goto _Lexit;
  }
  if (lomemmax + 2 < himemmin) {
    if (lomemmax + 2 <= membot + maxhalfword) {   /*126:*/
      if (himemmin - lomemmax >= 1998)
	t = lomemmax + 1000;
      else
	t = lomemmax + (himemmin - lomemmax) / 2 + 1;
      p = llink(rover);
      q = lomemmax;
      rlink(p) = q;
      llink(rover) = q;
      if (t > membot + maxhalfword)
	t = membot + maxhalfword;
      rlink(q) = rover;
      llink(q) = p;
      link(q) = emptyflag;
      nodesize(q) = t - lomemmax;
      lomemmax = t;
      link(lomemmax) = 0;
      info(lomemmax) = 0;
      rover = q;
      goto _Lrestart;
    }
    /*:126*/
  }
  overflow(S(317), memmax - memmin + 1);
_Lfound:
  unset_is_char_node(r);
  link(r) = 0;   /*_STAT*/
  varused += s;   /*_ENDSTAT*/
  Result = r;
_Lexit:
  return Result;
}
