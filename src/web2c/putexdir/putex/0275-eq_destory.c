Static void eqdestroy(memoryword w)
{
  pointer q;

  switch (eqtypefield(w)) {

  case call:
  case longcall:
  case outercall:
  case longoutercall:
    deletetokenref(equivfield(w));
    break;

  case glueref:
    deleteglueref(equivfield(w));
    break;

  case shaperef:
    q = equivfield(w);
    if (q != 0)
      freenode(q, info(q) + info(q) + 1);
    break;

  case boxref:
    flushnodelist(equivfield(w));
    break;
  }
}

