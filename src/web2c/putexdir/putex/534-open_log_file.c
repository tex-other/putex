Static void openlogfile(void)
{
  char oldsetting;
  short k;
  short l;
  Char months[36];
  short FORLIM;

  oldsetting = selector;
  if (jobname == 0)
    jobname = S(672);
  packjobname(S(673));
  while (!aopenout(&logfile)) {   /*535:*/
    selector = termonly;
    promptfilename(S(674), S(673));
  }
  /*:535*/
  logname = amakenamestring(&logfile);
  selector = logonly;
  logopened = true;
  /*536:*/
  fprintf(logfile,"%s",banner);
  slowprint(formatident);
  print(S(675));
  printint(day);
  printchar(' ');
  memcpy(months, "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC", 36);
  FORLIM = month * 3;
  for (k = month * 3 - 3; k < FORLIM; k++)
    fwrite(&months[k], 1, 1, logfile);
  printchar(' ');
  printint(year);
  printchar(' ');
  printtwo(tex_time / 60);
  printchar(':');
  printtwo(tex_time % 60);   /*:536*/
  inputstack[inputptr] = curinput;
  printnl(S(676));
  l = inputstack[0].limitfield;
  if (buffer[l] == endlinechar)
    l--;
  for (k = 1; k <= l; k++)
    print(buffer[k]);
  println();
  selector = oldsetting + 2;
}
