Static void closefilesandterminate(void)
{  /*1378:*/
  long k;
  for (k = 0; k <= 15; k++) {
    if (writeopen[k])   /*:1378*/
      aclose(&writefile[k]);
  }
  /*_STAT*/
  if (tracingstats > 0) {   /*_ENDSTAT*/
    /*1334:*/
    if (logopened) {   /*:1334*/
      fprintf(logfile, " \n");
      fprintf(logfile, "Here is how much of TeX's memory you used:\n");
	str_print_stats(logfile);
      fprintf(logfile, " %ld words of memory out of %ld\n",
	      lomemmax - memmin + memend - himemmin + 2L,
	      memend - memmin + 1L);
      fprintf(logfile, " %ld multiletter control sequences out of %ld\n",
	      cscount, (long)hashsize);
      fprintf(logfile, " %d words of font info for %d font",fmemptr,
		fontptr);
      if (fontptr !=  1) {
	fprintf(logfile, "s");
      }
      fprintf(logfile, ", out of %ld for %ld\n",
	      (long)fontmemsize, (long)(fontmax ));
      fprintf(logfile, " %d hyphenation exception",hyphcount);
      if (hyphcount != 1) {
	fprintf(logfile, "s");
      }
      fprintf(logfile, " out of %ld\n", (long)hyphsize);
      fprintf(logfile,
	" %di,%dn,%ldp,%db,%ds stack positions out of %ldi,%ldn,%ldp,%ldb,%lds\n",
	maxinstack, maxneststack, maxparamstack, maxbufstack + 1,
	maxsavestack + 6, (long)stacksize, (long)nestsize, (long)paramsize,
	(long)bufsize, (long)savesize);
    }
  }
  /*642:*/
  while (curs > -1) {
    if (curs > 0) {
	dvi_pop();
    } else {
	dvi_eop();
      totalpages++;
    }
    curs--;
  }
  if (totalpages == 0)
    printnl(S(1013));
  else {   /*:642*/
    long total_dvi_bytes;
    preparemag();
    dvipost(25400000L,473628672L,mag,maxv,maxh,maxpush,totalpages,fontptr);
    total_dvi_bytes=dviflush();
    printnl(S(1014));
    slowprint(outputfilename);
    print(S(303));
    printint(totalpages);
    print(S(1015));
    if (totalpages != 1)
      printchar('s');
    print(S(1016));
    printint(total_dvi_bytes);
    print(S(1017));
  }
  if (!logopened)
    return;
  putc('\n', logfile);
  aclose(&logfile);
  selector -= 2;
  if (selector != termonly)
    return;
  printnl(S(1018));
  slowprint(logname);
  printchar('.');
  println();
}
