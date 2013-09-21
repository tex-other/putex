Static void getnext(void)
{
  short k;
  halfword t;
  char cat;
  ASCIIcode c, cc=0 /* XXXX */;
  char d;
  int cur_cs, cur_chr, cur_cmd;
_Lrestart:
  cur_cs = 0;
  if (state != tokenlist) {   /*343:*/
_Lswitch__:
    if (loc > limit) {
      state = newline;   /*360:*/
      if (name > 17) {   /*362:*/
	line++;
	first = start;
	if (!forceeof) {
	  if (inputln(&curfile, true))
	    firmuptheline();
	  else
	    forceeof = true;
	}
	if (forceeof) {
	  printchar(')');
	  openparens--;
	  fflush(termout);
	  forceeof = false;
	  endfilereading();
	  curchr=cur_chr;
	  curcmd=cur_cmd;
	  cur_cs=checkoutervalidity(cur_cs);
	  goto _Lrestart;
	}
	if (endlinecharinactive) {
	  limit--;
	} else
	  buffer[limit] = endlinechar;
	first = limit + 1;
	loc = start;
      }  /*:362*/
      else {   /*:360*/
	if (!terminalinput) {
	  cur_cmd = 0;
	  cur_chr = 0;
	  goto _Lexit;
	}
	if (inputptr > 0) {
	  endfilereading();
	  goto _Lrestart;
	}
	if (selector < logonly)
	  openlogfile();
	if (interaction > nonstopmode) {
	  if (endlinecharinactive) {
	    limit++;
	  }
	  if (limit == start)
	    printnl(S(527));
	  println();
	  first = start;
	  print('*');
	  terminput();
	  limit = last;
	  if (endlinecharinactive) {
	    limit--;
	  } else
	    buffer[limit] = endlinechar;
	  first = limit + 1;
	  loc = start;
	} else
	  fatalerror(S(528));
      }
      checkinterrupt();
      goto _Lswitch__;
    }
    cur_chr = buffer[loc];
    loc++;
_Lreswitch:
    cur_cmd = catcode(cur_chr);   /*344:*/
    switch (state + cur_cmd) {   /*345:*/

    case midline + ignore:
    case skipblanks + ignore:
    case newline + ignore:
    case skipblanks + spacer:
    case newline + spacer:   /*:345*/
      goto _Lswitch__;
      break;

    case midline:
    case skipblanks:
    case newline:   /*354:*/
      if (loc > limit)
	cur_cs = nullcs;
      else {
_Lstartcs_:
	k = loc;
	cur_chr = buffer[k];
	cat = catcode(cur_chr);
	k++;
	if (cat == letter)
	  state = skipblanks;
	else if (cat == spacer)
	  state = skipblanks;
	else
	  state = midline;
	if (cat == letter && k <= limit) {   /*356:*/
	  do {
	    cur_chr = buffer[k];
	    cat = catcode(cur_chr);
	    k++;
	  } while (cat == letter && k <= limit);   /*355:*/
	  if (buffer[k] == cur_chr) {   /*:355*/
	    if (cat == supmark) {
	      if (k < limit) {
		c = buffer[k + 1];
		if (c < 128) {
		  d = 2;
		  if (ishex(c)) {
		    if (k + 2 <= limit) {
		      cc = buffer[k + 2];
		      if (ishex(cc)) {
			d++;
		      }
		    }
		  }
		  if (d > 2) {
		    buffer[k - 1] = cur_chr = hex_to_i(c,cc);
		  } else if (c < 64)
		    buffer[k - 1] = c + 64;
		  else
		    buffer[k - 1] = c - 64;
		  limit -= d;
		  first -= d;
		  while (k <= limit) {
		    buffer[k] = buffer[k + d];
		    k++;
		  }
		  goto _Lstartcs_;
		}
	      }
	    }
	  }
	  if (cat != letter)
	    k--;
	  if (k > loc + 1) {
	    cur_cs = idlookup_p(buffer+loc, k - loc, nonewcontrolsequence);
	    loc = k;
	    goto _Lfound;
	  }
	} else  /*355:*/
	{   /*:355*/
	  if (buffer[k] == cur_chr) {
	    if (cat == supmark) {
	      if (k < limit) {
		c = buffer[k + 1];
		if (c < 128) {
		  d = 2;
		  if (ishex(c)) {
		    if (k + 2 <= limit) {
		      cc = buffer[k + 2];
		      if (ishex(cc)) {
			d++;
		      }
		    }
		  }
		  if (d > 2) {
		    buffer[k - 1] = cur_chr = hex_to_i(c,cc);
		  } else if (c < 64)
		    buffer[k - 1] = c + 64;
		  else
		    buffer[k - 1] = c - 64;
		  limit -= d;
		  first -= d;
		  while (k <= limit) {
		    buffer[k] = buffer[k + d];
		    k++;
		  }
		  goto _Lstartcs_;
		}
	      }
	    }
	  }
	}
	/*:356*/
	cur_cs = singlebase + buffer[loc];
	loc++;
      }
#define check_outer {\
        curchr = cur_chr;\
        curcmd=cur_cmd;\
        cur_cs=checkoutervalidity(cur_cs);\
        cur_cmd=curcmd;\
        cur_chr = curchr;\
      }

#define process_cmd \
      if (cur_cmd >= outercall) check_outer \
      break;

#define Process_cs \
      cur_cmd = eqtype(cur_cs);\
      cur_chr = equiv(cur_cs);\
      process_cmd

_Lfound:
      Process_cs
      /*:354*/

    case midline + activechar:
    case skipblanks + activechar:
    case newline + activechar:   /*353:*/
      state = midline;
      cur_cs = cur_chr + activebase;
      Process_cs
      /*:353*/

    case midline + supmark:
    case skipblanks + supmark:
    case newline + supmark:   /*352:*/
      if (cur_chr == buffer[loc]) {
	if (loc < limit) {
	  c = buffer[loc + 1];
	  if (c < 128) {
	    loc += 2;
	    if (ishex(c)) {
	      if (loc <= limit) {
		cc = buffer[loc];
		if (ishex(cc)) {
		  loc++;
		  cur_chr = hex_to_i(c,cc);
		  goto _Lreswitch;
		}
	      }
	    }
	    if (c < 64)
	      cur_chr = c + 64;
	    else
	      cur_chr = c - 64;
	    goto _Lreswitch;
	  }
	}
      }
      state = midline;
      break;
      /*:352*/

    case midline + invalidchar:
    case skipblanks + invalidchar:
    case newline + invalidchar:   /*346:*/
      printnl(S(292));
      print(S(529));
      help2(S(530),S(531));
      deletionsallowed = false;
      error();
      deletionsallowed = true;
      goto _Lrestart;
      break;
      /*:346*/
      /*347:*/

    case midline + spacer:   /*349:*/
      state = skipblanks;
      cur_chr = ' ';
      break;
      /*:349*/

    case midline + carret:   /*348:*/
      loc = limit + 1;
      cur_cmd = spacer;
      cur_chr = ' ';
      break;
      /*:348*/

    case skipblanks + carret:
    case midline + comment:
    case skipblanks + comment:
    case newline + comment:   /*:350*/
      /*
350:*/
      loc = limit + 1;
      goto _Lswitch__;
      break;

    case newline + carret:   /*351:*/
      loc = limit + 1;
      cur_cs = parloc;
      Process_cs
      /*:351*/

    case midline + leftbrace:
      alignstate++;
      break;

    case skipblanks + leftbrace:
    case newline + leftbrace:
      state = midline;
      alignstate++;
      break;

    case midline + rightbrace:
      alignstate--;
      break;

    case skipblanks + rightbrace:
    case newline + rightbrace:
      state = midline;
      alignstate--;
      break;

    case skipblanks + mathshift:
    case skipblanks + tabmark:
    case skipblanks + macparam:
    case skipblanks + submark:
    case skipblanks + letter:
    case skipblanks + otherchar:
    case newline + mathshift:
    case newline + tabmark:
    case newline + macparam:
    case newline + submark:
    case newline + letter:
    case newline + otherchar:   /*:347*/
      state = midline;
      break;
    }
    /*:344*/
  } else {  /*357:*/
    if (loc == 0) {
      endtokenlist();
      goto _Lrestart;
    }
    t = info(loc);
    loc = link(loc);
    if (t >= cstokenflag) {
      cur_cs = t - cstokenflag;
      cur_cmd = eqtype(cur_cs);
      cur_chr = equiv(cur_cs);
      if (cur_cmd >= outercall) {
	if (cur_cmd == dontexpand) {   /*358:*/
	  cur_cs = info(loc) - cstokenflag;
	  loc = 0;
	  cur_cmd = eqtype(cur_cs);
	  cur_chr = equiv(cur_cs);
	  if (cur_cmd > maxcommand) {
	    cur_cmd = relax;
	    cur_chr = noexpandflag;
	  }
	} else {
	    check_outer
	}
	/*:358*/
      }
    } else {
      cur_cmd = t / dwa_do_8;
      cur_chr = t & (dwa_do_8-1);
      switch (cur_cmd) {

      case leftbrace:
	alignstate++;
	break;

      case rightbrace:
	alignstate--;
	break;

      case outparam:   /*359:*/
	begintokenlist(paramstack[paramstart + cur_chr - 1], parameter);
	goto _Lrestart;
	break;
	/*:359*/
      }
    }
  }
  /*:343*/
  /*342:*/
  if (cur_cmd <= carret) {
    if (cur_cmd >= tabmark) { /*789:*/
      if (alignstate == 0) {   
	if (scannerstatus == aligning)
	  fatalerror(S(509));
	cur_cmd = extrainfo(curalign);
	extrainfo(curalign) = cur_chr;
	if (cur_cmd == omit)
	  begintokenlist(omittemplate, vtemplate);
	else
	  begintokenlist(vpart(curalign), vtemplate);
	alignstate = 1000000L;
	goto _Lrestart;
      }
      /*:789*/
      /*:342*/
    }
  }
_Lexit: ;
  curcmd = cur_cmd;
  curchr = cur_chr;
  curcs = cur_cs;

  /*:357*/
}
