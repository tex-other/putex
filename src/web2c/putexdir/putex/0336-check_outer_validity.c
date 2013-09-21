Static int checkoutervalidity(int local_curcs)
{
  pointer p, q;
  if (scannerstatus == normal)
    return local_curcs;
  deletionsallowed = false;   /*337:*/
  if (local_curcs != 0) {   /*:337*/
    if (state == tokenlist || name < 1 || name > 17) {
      p = getavail();
      info(p) = cstokenflag + local_curcs;
      backlist(p);
    }
    curcmd = spacer;
    curchr = ' ';
  }
  curcs = local_curcs;
  if (scannerstatus > skipping) {   /*338:*/
    runaway();
    if (curcs == 0) {
      printnl(S(292));
      print(S(512));
    } else {
      curcs = 0;
      printnl(S(292));
      print(S(513));
    }
    print(S(514));   /*339:*/
    p = getavail();
    switch (scannerstatus) {

    case defining:
      print(S(313));
      info(p) = rightbracetoken + '}';
      break;

    case matching:
      print(S(515));
      info(p) = partoken;
      longstate = outercall;
      break;

    case aligning:
      print(S(315));
      info(p) = rightbracetoken + '}';
      q = p;
      p = getavail();
      link(p) = q;
      info(p) = cstokenflag + frozencr;
      alignstate = -1000000L;
      break;

    case absorbing:
      print(S(316));
      info(p) = rightbracetoken + '}';
      break;
    }
    inslist(p);   /*:339*/
    print(S(516));
    sprintcs(warningindex);
    help4(S(517),S(518),S(519),S(520));
    error();
  } else {
    printnl(S(292));
    print(S(521));
    printcmdchr(iftest, curif);
    print(S(522));
    printint(skipline);
    help3(S(523),S(524),S(525));
    if (curcs != 0)
      curcs = 0;
    else
      helpline[2] = S(526);
    curtok = cstokenflag + frozenfi;
    inserror();
  }
  /*:338*/
  deletionsallowed = true;
  return curcs;
}
