Static void runaway(void)
{
  pointer p=0 /* XXXX */;

  if (scannerstatus <= skipping)
    return;
  printnl(S(312));
  switch (scannerstatus) {

  case defining:
    print(S(313));
    p = defref;
    break;

  case matching:
    print(S(314));
    p = temphead;
    break;

  case aligning:
    print(S(315));
    p = holdhead;
    break;

  case absorbing:
    print(S(316));
    p = defref;
    break;
  }
  printchar('?');
  println();
  showtokenlist(link(p), 0, errorline - 10);
}

