Static void checkmem(boolean printlocs)
{
  pointer p, q;
  boolean clobbered;
  halfword FORLIM;

  for (p = memmin; p <= lomemmax; p++)
    P_clrbits_B(free_, p - memmin, 0, 3);
  for (p = himemmin; p <= memend; p++)   /*168:*/
    P_clrbits_B(free_, p - memmin, 0, 3);
  p = avail;
  q = 0;
  clobbered = false;
  while (p != 0) {
    if (p > memend || p < himemmin)
      clobbered = true;
    else {
      if (P_getbits_UB(free_, p - memmin, 0, 3))
	clobbered = true;
    }
    if (clobbered) {
      printnl(S(318));
      printint(q);
      goto _Ldone1;
    }
    P_putbits_UB(free_, p - memmin, 1, 0, 3);
    q = p;
    p = link(q);
  }
_Ldone1:   /*:168*/
  /*169:*/
  p = rover;
  q = 0;
  clobbered = false;
  do {
    if (p >= lomemmax || p < memmin)
      clobbered = true;
    else if ((rlink(p) >= lomemmax) | (rlink(p) < memmin))
      clobbered = true;
    else if ((!isempty(p)) | (nodesize(p) < 2) | (p + nodesize(p) >
	       lomemmax) | (llink(rlink(p)) != p)) {
      clobbered = true;
    }
    if (clobbered) {
      printnl(S(319));
      printint(q);
      goto _Ldone2;
    }
    FORLIM = p + nodesize(p);
    for (q = p; q < FORLIM; q++) {
      if (P_getbits_UB(free_, q - memmin, 0, 3)) {
	printnl(S(320));
	printint(q);
	goto _Ldone2;
      }
      P_putbits_UB(free_, q - memmin, 1, 0, 3);
    }
    q = p;
    p = rlink(p);
  } while (p != rover);
_Ldone2:   /*:169*/
  /*170:*/
  p = memmin;
  while (p <= lomemmax) {   /*:170*/
    if (isempty(p)) {
      printnl(S(321));
      printint(p);
    }
    while ((p <= lomemmax) & (!P_getbits_UB(free_, p - memmin, 0, 3)))
      p++;
    while ((p <= lomemmax) & P_getbits_UB(free_, p - memmin, 0, 3))
      p++;
  }
  if (printlocs) {   /*171:*/
    printnl(S(322));
    FORLIM = lomemmax;
    for (p = memmin; p <= lomemmax; p++) {
      if ((!P_getbits_UB(free_, p - memmin, 0, 3)) &
	  ((p > waslomax) | P_getbits_UB(wasfree, p - memmin, 0, 3))) {
	printchar(' ');
	printint(p);
      }
    }
    for (p = himemmin; p <= memend ; p++) {
      if ((!P_getbits_UB(free_, p - memmin, 0, 3)) & ((p < washimin ||
	      p > wasmemend) | P_getbits_UB(wasfree, p - memmin, 0, 3))) {
	printchar(' ');
	printint(p);
      }
    }
  }
  /*:171*/
  for (p = memmin; p <= lomemmax; p++) {
    P_clrbits_B(wasfree, p - memmin, 0, 3);
    P_putbits_UB(wasfree, p - memmin, P_getbits_UB(free_, p - memmin, 0, 3),
		 0, 3);
  }
  for (p = himemmin; p <= memend; p++) {
    P_clrbits_B(wasfree, p - memmin, 0, 3);
    P_putbits_UB(wasfree, p - memmin, P_getbits_UB(free_, p - memmin, 0, 3),
		 0, 3);
  }
  wasmemend = memend;
  waslomax = lomemmax;
  washimin = himemmin;
}
