#ifdef _WINDOWS
  #define NOCOMM
  #define NOSOUND
  #define NODRIVERS
  #define STRICT
  #pragma warning(disable:4115) // kill rpcasync.h complaint
  #include <windows.h>
  #define MYLIBAPI __declspec(dllexport)
#endif

#include "texwin.h"

#pragma warning(disable:4131) // old style declarator
#pragma warning(disable:4135) // conversion between different integral types 
#pragma warning(disable:4127) // conditional expression is constant

#include <setjmp.h>

#define EXTERN extern

#include "texd.h"

#pragma warning(disable:4244)       /* 96/Jan/10 */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void align_peek (void) 
{
lab20:
  align_state = 1000000L;
  do {
    get_x_token();
  } while (!(cur_cmd != 10));
  if (cur_cmd == 34) {
    scan_left_brace();
    new_save_level(7);
    if (mode == -1)
      normal_paragraph();
  } else if (cur_cmd == 2) {
    fin_align();
  } else if ((cur_cmd == 5) && (cur_chr == 258))
    goto lab20;
  else {
    init_row();
    init_col();
  }
}
/* used in itex.c only */
halfword finite_shrink_(halfword p)
{
  register halfword Result;
  halfword q;
  if (noshrinkerroryet) {
    noshrinkerroryet = false;
    print_err("Infinite glue shrinkage found in a paragraph");
    help5("The paragraph just ended includes some glue that has",
      "infinite shrinkability, e.g., `\\hskip 0pt minus 1fil'.",
      "Such glue doesn't belong there---it allows a paragraph",
      "of any length to fit on one line. But it's safe to proceed,",
      "since the offensive shrinkability has been made finite.");
    error();
  }
  q = new_spec(p);
  mem[q].hh.b1 = 0;
  delete_glue_ref(p);
  Result = q;
  return Result;
}
void try_break_ (integer pi, small_number breaktype)
{
  halfword r;
  halfword prevr;
  halfword oldl;
  bool nobreakyet;
  halfword prevprevr;
  halfword s;
  halfword q;
  halfword v;
  integer t;
  internal_font_number f;
  halfword l;
  bool noderstaysactive;
  scaled linewidth;
  char fitclass;
  halfword b;       /* current badness */
  integer d;
  bool artificialdemerits;
  halfword savelink;
  scaled shortfall;
  if (abs(pi)>= 10000)
    if (pi > 0)
      goto lab10;
    else
      pi = -10000;
  nobreakyet = true;
  prevr = active;
  oldl = 0;
  cur_active_width[1] = active_width[1];
  cur_active_width[2] = active_width[2];
  cur_active_width[3] = active_width[3];
  cur_active_width[4] = active_width[4];
  cur_active_width[5] = active_width[5];
  cur_active_width[6] = active_width[6];
  while (true) {
lab22:
    r = mem[prevr].hh.v.RH;
    if (mem[r].hh.b0 == 2) {
      cur_active_width[1] = cur_active_width[1] + mem[r + 1].cint;
      cur_active_width[2] = cur_active_width[2] + mem[r + 2].cint;
      cur_active_width[3] = cur_active_width[3] + mem[r + 3].cint;
      cur_active_width[4] = cur_active_width[4] + mem[r + 4].cint;
      cur_active_width[5] = cur_active_width[5] + mem[r + 5].cint;
      cur_active_width[6] = cur_active_width[6] + mem[r + 6].cint;
      prevprevr = prevr;
      prevr = r;
      goto lab22;
    }
    {
      l = mem[r + 1].hh.v.LH; 
      if (l > oldl) {
        if ((minimum_demerits < 1073741823L) &&  /* 2^30 - 1 */
          ((oldl != easyline)||(r == active))) {
            if (nobreakyet) {
              nobreakyet = false; 
              break_width[1] = background[1];
              break_width[2] = background[2];
              break_width[3] = background[3];
              break_width[4] = background[4];
              break_width[5] = background[5];
              break_width[6] = background[6];
              s = cur_p;
/* if break_type>unhyphenated then if cur_p<>null then l.16451 */
              if (breaktype > 0)
                if (cur_p != 0)
                {
                  t = mem[cur_p].hh.b1;
                  v = cur_p;
                  s = mem[cur_p + 1].hh.v.RH;
                  while (t > 0) {
                    decr(t);
                    v = mem[v].hh.v.RH; 
                    if ((v >= hi_mem_min)) {
                      f = mem[v].hh.b0; 
                      break_width[1]= break_width[1]- font_info[width_base[f]+ font_info[char_base[f]+ mem[v].hh.b1].qqqq.b0].cint;
                    } else switch (mem[v].hh.b0)
                    {
                    case 6:
                      {
                        f = mem[v + 1].hh.b0;
                        break_width[1]= break_width[1]- font_info[width_base[f]+ font_info[char_base[f]+ mem[v + 1].hh.b1].qqqq.b0].cint;
                      }
                      break;
                    case 0:
                    case 1:
                    case 2:
                    case 11:
                      break_width[1]= break_width[1]- mem[v + 1].cint;
                      break;
                    default:
                      {
                        confusion("disc1");
                        return;       // abort_flag set
                      }
                      break;
                    }
                  }
                  while (s != 0) {  /* while s<>null do l.16453 */
                    if ((s >= hi_mem_min)) {
                      f = mem[s].hh.b0;
                      break_width[1]= break_width[1]+ font_info[width_base[f]+ font_info[char_base[f]+ mem[s].hh.b1].qqqq.b0].cint;
                    } else switch(mem[s].hh.b0)
                    {
                    case 6:
                      {
                        f = mem[s + 1].hh.b0;
                        break_width[1]= break_width[1]+ font_info[width_base[f]+ font_info[char_base[f]+ mem[s + 1].hh.b1].qqqq.b0].cint;
                      }
                      break;
                    case 0:
                    case 1:
                    case 2:
/* didn't used to drop through to case 11 from case 2 in  3.141 */
                    case 11:
/* used to be some extra code here in case 11 in 3.141 */
                      break_width[1]= break_width[1]+ mem[s + 1].cint;
                      break;
                    default:
                      {
                        confusion("disc1");
                        return;       // abort_flag set
                      }
                      break;
                    }
/* used to be an increment t here in 3.141 */
                    s = mem[s].hh.v.RH;
                  }
                  break_width[1] = break_width[1] + disc_width;
/* used to be a test on t here instead in 3.141 */
                  if (mem[cur_p + 1].hh.v.RH == 0)
                    s = mem[v].hh.v.RH;
                } 
                while (s != 0) {
                  if ((s >= hi_mem_min))
                    goto lab30;
                  switch(mem[s].hh.b0)
                  {
                  case 10:
                    {
                      v = mem[s + 1].hh.v.LH;
                      break_width[1]= break_width[1]- mem[v + 1].cint;
                      break_width[2 + mem[v].hh.b0]= break_width[2 + mem[v].hh.b0]- mem[v + 2].cint;
                      break_width[6]= break_width[6]- mem[v + 3].cint;
                    }
                    break;
                  case 12:
                    ;
                    break;
                  case 9:
/* case 9 used to drop through to case 11 in 3.141 */
                    break_width[1]= break_width[1]- mem[s + 1].cint;
                    break;
                  case 11:
/* changes here in nature of test since 3.141 */
                    if (mem[s].hh.b1 != 1)
                      goto lab30;
                    else
                      break_width[1]= break_width[1]- mem[s + 1].cint;
                    break;
                  default:
                    goto lab30;
                    break;
                  }
                  s = mem[s].hh.v.RH;
                }
lab30:
                ;
            }
            if (mem[prevr].hh.b0 == 2) {
              mem[prevr + 1].cint = mem[prevr + 1].cint - cur_active_width[1] + break_width[1];
              mem[prevr + 2].cint = mem[prevr + 2].cint - cur_active_width[2] + break_width[2];
              mem[prevr + 3].cint = mem[prevr + 3].cint - cur_active_width[3] + break_width[3];
              mem[prevr + 4].cint = mem[prevr + 4].cint - cur_active_width[4] + break_width[4];
              mem[prevr + 5].cint = mem[prevr + 5].cint - cur_active_width[5] + break_width[5];
              mem[prevr + 6].cint = mem[prevr + 6].cint - cur_active_width[6] + break_width[6];
            } else if (prevr == active) {
              active_width[1] = break_width[1];
              active_width[2] = break_width[2];
              active_width[3] = break_width[3];
              active_width[4] = break_width[4];
              active_width[5] = break_width[5];
              active_width[6] = break_width[6];
            } else {
              q = get_node(7);
              mem[q].hh.v.RH = r;
              mem[q].hh.b0 = 2;
              mem[q].hh.b1 = 0;
              mem[q + 1].cint = break_width[1]- cur_active_width[1];
              mem[q + 2].cint = break_width[2]- cur_active_width[2];
              mem[q + 3].cint = break_width[3]- cur_active_width[3];
              mem[q + 4].cint = break_width[4]- cur_active_width[4];
              mem[q + 5].cint = break_width[5]- cur_active_width[5];
              mem[q + 6].cint = break_width[6]- cur_active_width[6];
              mem[prevr].hh.v.RH = q;
              prevprevr = prevr;
              prevr = q;
            }
            if (abs(adj_demerits) >= 1073741823L - minimum_demerits)
              minimum_demerits = 1073741822L; /* 2^30 - 2 */
            else
              minimum_demerits = minimum_demerits + abs(adj_demerits); 
            {
              register integer for_end;
              fitclass = 0;
              for_end = 3;
              if (fitclass <= for_end) do {
                if (minimal_demerits[fitclass] <= minimum_demerits) {
                  q = get_node(2);
                  mem[q].hh.v.RH = passive;
                  passive = q;
                  mem[q + 1].hh.v.RH = cur_p;
                  ;
#ifdef STAT
                  incr(pass_number);
                  mem[q].hh.v.LH = pass_number;
#endif /* STAT */
                  mem[q + 1].hh.v.LH = best_place[fitclass];
                  q = get_node(3);
                  mem[q + 1].hh.v.RH = passive;
                  mem[q + 1].hh.v.LH = best_pl_line[fitclass] + 1;
                  mem[q].hh.b1 = fitclass;
                  mem[q].hh.b0 = breaktype;
                  mem[q + 2].cint = minimal_demerits[fitclass];
                  mem[q].hh.v.RH = r;
                  mem[prevr].hh.v.RH = q;
                  prevr = q;
                  ;
#ifdef STAT
                  if (tracing_paragraphs > 0) {
                    print_nl("@@");
                    print_int(mem[passive].hh.v.LH);
                    print_string(": line ");
                    print_int(mem[q + 1].hh.v.LH - 1);
                    print_char('.');
                    print_int(fitclass);
                    if (breaktype == 1)
                      print_char('-');
                    print_string(" t=");
                    print_int(mem[q + 2].cint);
                    print_string(" -> @@");
                    if (mem[passive + 1].hh.v.LH == 0)
                      print_char('0');
                    else
                      print_int(mem[mem[passive + 1].hh.v.LH].hh.v.LH);
                  } 
#endif /* STAT */
                }
                minimal_demerits[fitclass]= 1073741823L; /* 2^30 - 1 */
              } while(fitclass++ < for_end);
            }
            minimum_demerits = 1073741823L; /* 2^30 - 1 */
            if (r != active) {
              q = get_node(7);
              mem[q].hh.v.RH = r;
              mem[q].hh.b0 = 2;
              mem[q].hh.b1 = 0;
              mem[q + 1].cint = cur_active_width[1] - break_width[1];
              mem[q + 2].cint = cur_active_width[2] - break_width[2];
              mem[q + 3].cint = cur_active_width[3] - break_width[3];
              mem[q + 4].cint = cur_active_width[4] - break_width[4];
              mem[q + 5].cint = cur_active_width[5] - break_width[5];
              mem[q + 6].cint = cur_active_width[6] - break_width[6];
              mem[prevr].hh.v.RH = q;
              prevprevr = prevr;
              prevr = q;
            }
        }
        if (r == active)
          goto lab10;
        if (l > easyline) {
          linewidth = second_width; 
          oldl = 262142L;       /* 2^18 - 2 ? */
        } else {
          oldl = l;
          if (l > last_special_line)
            linewidth = second_width;
          else if (par_shape_ptr == 0)
            linewidth = first_width;
          else
            linewidth = mem[par_shape_ptr + 2 * l].cint;
        }
      }
    }
    {
      artificialdemerits = false; 
      shortfall = linewidth - cur_active_width[1];  /* linewidth may be ... */
      if (shortfall > 0)
        if ((cur_active_width[3]!= 0) || (cur_active_width[4]!= 0) || (cur_active_width[5]!= 0)) {
          b = 0;
          fitclass = 2;
        } else {
          if (shortfall > 7230584L)
            if (cur_active_width[2]< 1663497L) {
              b = 10000;
              fitclass = 0;
              goto lab31;
            }
          b = badness(shortfall, cur_active_width[2]);
          if (b > 12)
            if (b > 99)
              fitclass = 0;
            else
              fitclass = 1;
          else fitclass = 2;
lab31:
          ;
        } else {
          if (- (integer) shortfall > cur_active_width[6])
            b = 10001;
          else
            b = badness(- (integer) shortfall, cur_active_width[6]);
          if (b > 12)
            fitclass = 3;
          else
            fitclass = 2;
        }
      if ((b > 10000)||(pi == -10000)) {
        if (final_pass && (minimum_demerits == 1073741823L) &&  /* 2^30 - 1 */
            (mem[r].hh.v.RH == active) && (prevr == active))
          artificialdemerits = true;
        else if (b > threshold)
          goto lab60;
        noderstaysactive = false;
      } else {
        prevr = r;
        if (b > threshold)
          goto lab22;
        noderstaysactive = true;
      }
      if (artificialdemerits)
        d = 0;
      else {
        d = line_penalty + b;
        if (abs(d)>= 10000)
          d = 100000000L;
        else
          d = d * d;
        if (pi != 0)
          if (pi > 0)
            d = d + pi * pi;
          else if (pi > -10000)
            d = d - pi * pi;
        if ((breaktype == 1) && (mem[r].hh.b0 == 1))
          if (cur_p != 0)
            d = d + double_hyphen_demerits;
          else
            d = d + final_hyphen_demerits;
        if (abs(toint(fitclass)- toint(mem[r].hh.b1)) > 1)
          d = d + adj_demerits;
      }
      ;
#ifdef STAT
      if (tracing_paragraphs > 0) {
        if (printed_node != cur_p) {
          print_nl("");
          if (cur_p == 0)
            short_display(mem[printed_node].hh.v.RH);
          else {
            savelink = mem[cur_p].hh.v.RH;
            mem[cur_p].hh.v.RH = 0;
            print_nl("");
            short_display(mem[printed_node].hh.v.RH);
            mem[cur_p].hh.v.RH = savelink;
          }
          printed_node = cur_p;
        }
        print_nl("@");
        if (cur_p == 0)
          print_esc("par");
        else if (mem[cur_p].hh.b0 != 10) {
          if (mem[cur_p].hh.b0 == 12)
            print_esc("penalty");
          else if (mem[cur_p].hh.b0 == 7)
            print_esc("discretionary");
          else if (mem[cur_p].hh.b0 == 11)
            print_esc("kern");
          else
            print_esc("math");
        }
        print_string(" via @@");
        if (mem[r + 1].hh.v.RH == 0)
          print_char('0');
        else
          print_int(mem[mem[r + 1].hh.v.RH].hh.v.LH);
        print_string(" b=");
        if (b > 10000)
          print_char('*');
        else print_int(b);
        print_string(" p=");
        print_int(pi);
        print_string(" d=");
        if (artificialdemerits)
          print_char('*');
        else print_int(d);
      }
#endif /* STAT */
      d = d + mem[r + 2].cint; 
      if (d <= minimal_demerits[fitclass]) {
        minimal_demerits[fitclass]= d;
        best_place[fitclass]= mem[r + 1].hh.v.RH;
        best_pl_line[fitclass]= l;
        if (d < minimum_demerits)
          minimum_demerits = d;
      }
      if (noderstaysactive)
        goto lab22;
lab60:
      mem[prevr].hh.v.RH = mem[r].hh.v.RH;
      free_node(r, 3);
      if (prevr == active) {
        r = mem[active].hh.v.RH;
        if (mem[r].hh.b0 == 2) {
          active_width[1] = active_width[1] + mem[r + 1].cint;
          active_width[2] = active_width[2] + mem[r + 2].cint;
          active_width[3] = active_width[3] + mem[r + 3].cint;
          active_width[4] = active_width[4] + mem[r + 4].cint;
          active_width[5] = active_width[5] + mem[r + 5].cint;
          active_width[6] = active_width[6] + mem[r + 6].cint;
          cur_active_width[1] = active_width[1];
          cur_active_width[2] = active_width[2];
          cur_active_width[3] = active_width[3];
          cur_active_width[4] = active_width[4];
          cur_active_width[5] = active_width[5];
          cur_active_width[6] = active_width[6];
          mem[active].hh.v.RH = mem[r].hh.v.RH;
          free_node(r, 7);
        }
      } else if (mem[prevr].hh.b0 == 2) {
        r = mem[prevr].hh.v.RH;
        if (r == active) {
          cur_active_width[1] = cur_active_width[1] - mem[prevr + 1].cint;
          cur_active_width[2] = cur_active_width[2] - mem[prevr + 2].cint;
          cur_active_width[3] = cur_active_width[3] - mem[prevr + 3].cint;
          cur_active_width[4] = cur_active_width[4] - mem[prevr + 4].cint;
          cur_active_width[5] = cur_active_width[5] - mem[prevr + 5].cint;
          cur_active_width[6] = cur_active_width[6] - mem[prevr + 6].cint;
          mem[prevprevr].hh.v.RH = active; /* prevprevr may be used ... */
          free_node(prevr, 7);
          prevr = prevprevr;
        } else if (mem[r].hh.b0 == 2) {
          cur_active_width[1] = cur_active_width[1] + mem[r + 1].cint;
          cur_active_width[2] = cur_active_width[2] + mem[r + 2].cint;
          cur_active_width[3] = cur_active_width[3] + mem[r + 3].cint;
          cur_active_width[4] = cur_active_width[4] + mem[r + 4].cint;
          cur_active_width[5] = cur_active_width[5] + mem[r + 5].cint;
          cur_active_width[6] = cur_active_width[6] + mem[r + 6].cint;
          mem[prevr + 1].cint = mem[prevr + 1].cint + mem[r + 1].cint;
          mem[prevr + 2].cint = mem[prevr + 2].cint + mem[r + 2].cint;
          mem[prevr + 3].cint = mem[prevr + 3].cint + mem[r + 3].cint;
          mem[prevr + 4].cint = mem[prevr + 4].cint + mem[r + 4].cint;
          mem[prevr + 5].cint = mem[prevr + 5].cint + mem[r + 5].cint;
          mem[prevr + 6].cint = mem[prevr + 6].cint + mem[r + 6].cint;
          mem[prevr].hh.v.RH = mem[r].hh.v.RH;
          free_node(r, 7);
        }
      }
    }
  }
lab10:
  ;
#ifdef STAT
  if (cur_p == printed_node)
    if (cur_p != 0)
      if (mem[cur_p].hh.b0 == 7) {
        t = mem[cur_p].hh.b1;
        while (t > 0) {
          decr(t);
          printed_node = mem[printed_node].hh.v.RH;
        }
      } 
#endif /* STAT */
/*  must exit here, there are no internal return - except for confusion */
/*  savedbadness = b; */      /* 96/Feb/9 - for test in itex.c */
}
/* end of the old tex5.c here */
void post_line_break_(integer finalwidowpenalty)
{
  halfword q, r, s;
  bool discbreak;
  bool postdiscbreak;
  scaled curwidth;
  scaled curindent;
  quarterword t;
  integer pen;
  halfword curline;
  q = mem[best_bet + 1].hh.v.RH;
  cur_p = 0;
  do {
    r = q;
    q = mem[q + 1].hh.v.LH;
    mem[r + 1].hh.v.LH = cur_p;
    cur_p = r;
  } while (!(q == 0));
  curline = prev_graf + 1;
  do {
    q = mem[cur_p + 1].hh.v.RH;
    discbreak = false;
    postdiscbreak = false;
    if (q != 0)
      if (mem[q].hh.b0 == 10) {
        delete_glue_ref(mem[q + 1].hh.v.LH);
        mem[q + 1].hh.v.LH = eqtb[(hash_size + 790)].hh.v.RH;
        mem[q].hh.b1 = 9;
        incr(mem[eqtb[(hash_size + 790)].hh.v.RH].hh.v.RH);
        goto lab30;
      } else {
        if (mem[q].hh.b0 == 7) {
          t = mem[q].hh.b1;
          if (t == 0)
            r = mem[q].hh.v.RH;
          else {
            r = q;
            while (t > 1) {
              r = mem[r].hh.v.RH;
              decr(t);
            }
            s = mem[r].hh.v.RH;
            r = mem[s].hh.v.RH;
            mem[s].hh.v.RH = 0;
            flush_node_list(mem[q].hh.v.RH);
            mem[q].hh.b1 = 0;
          }
          if (mem[q + 1].hh.v.RH != 0) {
            s = mem[q + 1].hh.v.RH;
            while (mem[s].hh.v.RH != 0)
              s = mem[s].hh.v.RH;
            mem[s].hh.v.RH = r;
            r = mem[q + 1].hh.v.RH;
            mem[q + 1].hh.v.RH = 0;
            postdiscbreak = true;
          }
          if (mem[q + 1].hh.v.LH != 0) {
            s = mem[q + 1].hh.v.LH;
            mem[q].hh.v.RH = s;
            while (mem[s].hh.v.RH != 0)
              s = mem[s].hh.v.RH;
            mem[q + 1].hh.v.LH = 0;
            q = s;
          }
          mem[q].hh.v.RH = r;
          discbreak = true;
        } else if ((mem[q].hh.b0 == 9)||(mem[q].hh.b0 == 11))
          mem[q + 1].cint = 0;
      } else {
        q = temp_head;
        while (mem[q].hh.v.RH != 0)
          q = mem[q].hh.v.RH;
      }
    r = new_param_glue(8);
    mem[r].hh.v.RH = mem[q].hh.v.RH;
    mem[q].hh.v.RH = r;
    q = r;
lab30:
    ;
    r = mem[q].hh.v.RH;
    mem[q].hh.v.RH = 0;
    q = mem[temp_head].hh.v.RH;
    mem[temp_head].hh.v.RH = r;
    if (eqtb[(hash_size + 789)].hh.v.RH != 0) {
      r = new_param_glue(7);
      mem[r].hh.v.RH = q;
      q = r;
    }
    if (curline > last_special_line) {
      curwidth = second_width;
      curindent = second_indent;
    } else if (par_shape_ptr == 0) {
      curwidth = first_width;
      curindent = first_indent;
    } else {
      curwidth = mem[par_shape_ptr + 2 * curline].cint;
      curindent = mem[par_shape_ptr + 2 * curline - 1].cint;
    }
    adjust_tail = adjust_head;
    just_box = hpack(q, curwidth, 0);
    mem[just_box + 4].cint = curindent;
    append_to_vlist(just_box);
/* if adjust_head<>adjust_tail then l.17346 */
    if (adjust_head != adjust_tail) {
      mem[tail].hh.v.RH = mem[adjust_head].hh.v.RH;
      tail = adjust_tail;
    }
    adjust_tail = 0; /* adjust_tail:=null */
    if (curline + 1 != best_line) {
      pen = inter_line_penalty;
      if (curline == prev_graf + 1)
        pen = pen + club_penalty;
      if (curline + 2 == best_line)
        pen = pen + finalwidowpenalty;
      if (discbreak)
        pen = pen + broken_penalty;
      if (pen != 0) {
        r = new_penalty(pen);
        mem[tail].hh.v.RH = r;
        tail = r;
      }
    }
    incr(curline);
    cur_p = mem[cur_p + 1].hh.v.LH;
    if (cur_p != 0)
      if (!postdiscbreak) {
        r = temp_head;
        while (true) {
          q = mem[r].hh.v.RH;
          if (q == mem[cur_p + 1].hh.v.RH)
            goto lab31;
          if ((q >= hi_mem_min))
            goto lab31;
          if ((mem[q].hh.b0 < 9))
            goto lab31;
/* change in tests here from 3.141 != 1 instead of == 2 */
          if (mem[q].hh.b0 == 11)
            if (mem[q].hh.b1 != 1)
              goto lab31;
          r = q;
        }
lab31:
        if (r != temp_head) {
          mem[r].hh.v.RH = 0;
  flush_node_list(mem[temp_head].hh.v.RH); 
  mem[temp_head].hh.v.RH = q; 
      } 
    } 
  } while(!(cur_p == 0)); 
  if ((curline != best_line)||(mem[temp_head].hh.v.RH != 0)) {
    confusion("disc2");
    return;       // abort_flag set
  }
  prev_graf = best_line - 1; 
} 
/* Reconstitute ligatures during hyphenation pass */
small_number reconstitute_(small_number j, small_number n, halfword bchar, halfword hchar) 
{/* 22 30 */ register small_number Result; 
  halfword p; 
  halfword t; 
  ffourquarters q; 
  halfword currh; 
  halfword testchar; 
  scaled w; 
  font_index k; 
  hyphen_passed = 0;            /* paragraph 907 ? */
  t = hold_head; 
  w = 0; 
  mem[hold_head].hh.v.RH = 0; 
  cur_l = hu[j]; 
  cur_q = t; 
  if (j == 0)
  {
    ligature_present = init_lig; 
    p = init_list; 
    if (ligature_present)
    lft_hit = init_lft; 
/*   while p>null do l.17772 */
/*    while(p > 0){ */  /* NO! */
    while(p != 0){          /* 94/Mar/22 BUG FIX */
      {
/*    append_charnode_to_t(character(p)); */ 
  mem[t].hh.v.RH = get_avail(); 
  t = mem[t].hh.v.RH; 
  mem[t].hh.b0 = hf; 
  mem[t].hh.b1 = mem[p].hh.b1; 
      } 
      p = mem[p].hh.v.RH; /* p:=link(p); */
    } 
  } 
  else if (cur_l < 256)
  {
    mem[t].hh.v.RH = get_avail(); 
    t = mem[t].hh.v.RH; 
    mem[t].hh.b0 = hf; 
    mem[t].hh.b1 = cur_l; 
  } 
  lig_stack = 0;    /* lig_stack:=null; */
  {
    if (j < n)
    cur_r = hu[j + 1]; 
    else cur_r = bchar; 
    if (odd(hyf[j])) 
    currh = hchar; 
    else currh = 256; 
  } 
  lab22: if (cur_l == 256)   /* if cur_l = non_char then */
  {
    k = bchar_label[hf];    /* begin k:=bchar_label[hf]; */
/*  if k=non_address then goto done@+else q:=font_info[k].qqqq; l.17812 */
/*    if (k == font_mem_size) */
    if (k == non_address)    /* i.e. 0 ---  96/Jan/15 */
    goto lab30; 
    else q = font_info[k].qqqq; 
  } 
  else {
    q = font_info[char_base[hf]+ cur_l].qqqq; 
    if (((q.b2)% 4)!= 1)
    goto lab30; 
    k = lig_kern_base[hf]+ q.b3; 
    q = font_info[k].qqqq; 
    if (q.b0 > 128)
    {
      k = lig_kern_base[hf]+ 256 * q.b2 + q.b3 + 32768L - 256 *(128); 
      q = font_info[k].qqqq; 
    } 
  } 
  if (currh < 256) /* if cur_rh < non_char then test_char:=cur_rh */
    testchar = currh; 
  else testchar = cur_r;  /* else test_char:=cur_r; l.17817 */
/* loop@+begin if next_char(q)=test_char then if skip_byte(q)<=stop_flag then */
  while(true){
    if (q.b1 == testchar)
    if (q.b0 <= 128)
    if (currh < 256)   /*  if cur_rh<non_char then */
    {
      hyphen_passed = j; 
      hchar = 256; 
      currh = 256; 
      goto lab22;     /* goto continue; */
    } 
    else {    /* else begin if hchar<non_char then if odd(hyf[j]) then */
    if (hchar < 256)
      if (odd(hyf[j])) 
      {
  hyphen_passed = j; 
  hchar = 256; 
      } 
      if (q.b2 < 128)  /* if op_byte(q)<kern_flag then */
      {
// @<Carry out a ligature replacement, updating the cursor structure
//  and possibly advancing~|j|; |goto continue| if the cursor doesn't
//  advance, otherwise |goto done|@>;  => l.17869
  if (cur_l == 256)    /* begin if cur_l=non_char then lft_hit:=true; */
    lft_hit = true; 
  if (j == n)
    if (lig_stack == 0)/* if lig_stack=null ? */
      rt_hit = true; 
  {
    if (interrupt != 0){
      pause_for_instructions();
    }
  } 
  switch(q.b2)  /* case op_byte(q) of */
  {case 1 : 
  case 5 : 
    {
      cur_l = q.b3; 
      ligature_present = true; 
    } 
    break; 
  case 2 : 
  case 6 : 
    {
      cur_r = q.b3; 
/*   if lig_stack>null then character(lig_stack):=cur_r */
/*      if (lig_stack > 0) */      /* 94/Mar/22 */
      if (lig_stack != 0)      /* line breaking?? */
      mem[lig_stack].hh.b1 = cur_r; /* l.17877 ? */
      else {
        lig_stack = new_lig_item(cur_r); 
        if (j == n)
        bchar = 256; 
        else {
    p = get_avail(); 
    mem[lig_stack + 1].hh.v.RH = p; 
    mem[p].hh.b1 = hu[j + 1]; 
    mem[p].hh.b0 = hf; 
        } 
      } 
    } 
    break; 
  case 3 : 
    {
      cur_r = q.b3; 
      p = lig_stack; 
      lig_stack = new_lig_item(cur_r); 
      mem[lig_stack].hh.v.RH = p; 
    } 
    break; 
  case 7 : 
  case 11 : 
    {
      if (ligature_present)
      {
        p = new_ligature(hf, cur_l, mem[cur_q].hh.v.RH); 
        if (lft_hit)
        {
    mem[p].hh.b1 = 2; 
    lft_hit = false; 
        } 
/*        if (false)
        if (lig_stack == 0){
          incr(mem[p].hh.b1); 
          rt_hit = false; 
        } */              /* removed 99/Jan/6 */
      mem[cur_q].hh.v.RH = p; 
      t = p; 
        ligature_present = false; 
      } 
      cur_q = t; 
      cur_l = q.b3; 
      ligature_present = true; 
    } 
    break; 
    default: 
    {
/* othercases begin cur_l:=rem_byte(q);
    ligature_present:=true; {\.{=:}} l.17869 */
      cur_l = q.b3; 
      ligature_present = true; 
/*   if lig_stack>null then pop_lig_stack l.17870 => l.17828 */
/*      if (lig_stack > 0)*/       /* 94/Mar/22 ??? */
      if (lig_stack != 0)        /* BUG FIX  */
      {
/*        if (mem[lig_stack + 1].hh.v.RH > 0) */ /* 94/Mar/22  */
        if (mem[lig_stack + 1].hh.v.RH != 0) /* l.17828 ? */
        {
    mem[t].hh.v.RH = mem[lig_stack + 1].hh.v.RH; 
    t = mem[t].hh.v.RH; 
    incr(j); 
        } 
        p = lig_stack; 
        lig_stack = mem[p].hh.v.RH; 
        free_node(p, 2); 
        if (lig_stack == 0)  /* if lig_stack=null ? */
        {
    if (j < n)
    cur_r = hu[j + 1]; 
    else cur_r = bchar; 
    if (odd(hyf[j])) 
    currh = hchar; 
    else currh = 256; 
        } 
        else cur_r = mem[lig_stack].hh.b1; 
      } 
      else if (j == n)
      goto lab30; 
      else {
    
        {
    mem[t].hh.v.RH = get_avail(); 
    t = mem[t].hh.v.RH; 
    mem[t].hh.b0 = hf; 
    mem[t].hh.b1 = cur_r; 
        } 
        incr(j); 
        {
    if (j < n)
    cur_r = hu[j + 1]; 
    else cur_r = bchar; 
    if (odd(hyf[j])) 
    currh = hchar; 
    else currh = 256; 
        } 
      } 
    } 
    break; 
  } 
  if (q.b2 > 4)
  if (q.b2 != 7)
  goto lab30; 
  goto lab22; 
      } 
      w = font_info[kern_base[hf]+ 256 * q.b2 + q.b3].cint; 
      goto lab30; 
    } 
    if (q.b0 >= 128)
    if (currh == 256)
    goto lab30; 
    else {
  
      currh = 256; 
      goto lab22; 
    } 
    k = k + q.b0 + 1; 
    q = font_info[k].qqqq; 
  } 
  lab30:; 
  if (ligature_present)
  {
    p = new_ligature(hf, cur_l, mem[cur_q].hh.v.RH); 
    if (lft_hit)
    {
      mem[p].hh.b1 = 2; 
      lft_hit = false; 
    } 
    if (rt_hit)
    if (lig_stack == 0)  /* if lig_stack=null ? */
    {
      incr(mem[p].hh.b1); 
      rt_hit = false; 
    } 
    mem[cur_q].hh.v.RH = p; 
    t = p; 
    ligature_present = false; 
  } 
  if (w != 0)
  {
    mem[t].hh.v.RH = new_kern(w); 
    t = mem[t].hh.v.RH; 
    w = 0; 
  } 
/*  if (lig_stack > 0)*/     /* 94/Mar/22 ??? l.17870 */
  if (lig_stack != 0)        /* l.17841 */
  {
/* begin cur_q:=t; cur_l:=character(lig_stack);
  ligature_present:=true; l.17842 */
    cur_q = t; 
    cur_l = mem[lig_stack].hh.b1; 
    ligature_present = true; 
    {
/*   pop_lig_stack; goto continue; l.17843 => l.17828 */
/*      if (mem[lig_stack + 1].hh.v.RH > 0) *//* 94/Mar/22 */
      if (mem[lig_stack + 1].hh.v.RH != 0) /* BUG FIX */
      {
  mem[t].hh.v.RH = mem[lig_stack + 1].hh.v.RH; 
  t = mem[t].hh.v.RH; 
  incr(j); 
      } 
      p = lig_stack; 
      lig_stack = mem[p].hh.v.RH; 
      free_node(p, 2); 
      if (lig_stack == 0)
      {
  if (j < n)
  cur_r = hu[j + 1]; 
  else cur_r = bchar; 
  if (odd(hyf[j])) 
  currh = hchar; 
  else currh = 256; 
      } 
      else cur_r = mem[lig_stack].hh.b1; 
    } 
    goto lab22; 
  } 
  Result = j; 
  return Result; 
}
/* #pragma optimize ("g", off) */ /* not needed for MSVC it seems ... */
void hyphenate (void) 
{/* 50 30 40 41 42 45 10 */
/*  char i, j, l;  */
  char i, j; 
  int l;              /* 95/Jan/7 */
  halfword q, r, s; 
  halfword bchar; 
  halfword majortail, minortail; 
/*  ASCII_code c;  */
  int c;              /* 95/Jan/7 */
  char cloc; 
/*  integer rcount; */
  int rcount;           /* 95/Jan/7 */
  halfword hyfnode; 
  trie_pointer z; 
  integer v; 
  hyph_pointer h; 
  str_number k; 
  pool_pointer u; 
  {
    register integer for_end; 
    j = 0; 
    for_end = hn; 
    if (j <= for_end) do 
      hyf[j]= 0; 
    while(j++ < for_end);
  } 
  h = hc[1]; 
  incr(hn); 
  hc[hn]= cur_lang; 
  {
    register integer for_end; 
    j = 2; 
    for_end = hn; 
    if (j <= for_end) do 
/*    h =(h + h + hc[j])% 607;  */
      h =(h + h + hc[j])% hyphen_prime; 
    while(j++ < for_end);
  } 
  while(true){
    k = hyph_word[h]; 
    if (k == 0)
    goto lab45; 
    if (length(k) < hn)
    goto lab45; 
    if (length(k) == hn)
    {
      j = 1; 
      u = str_start[k]; 
      do {
    if (str_pool[u]< hc[j])
  goto lab45; 
  if (str_pool[u]> hc[j])
  goto lab30; 
  incr(j); 
  incr(u); 
      } while(!(j > hn)); 
      s = hyph_list[h]; 
      while(s != 0){  /* while s<>null do l.18173 */
    
  hyf[mem[s].hh.v.LH]= 1; 
  s = mem[s].hh.v.RH; 
      } 
      decr(hn); 
      goto lab40; 
    } 
    lab30:; 
    if (h > 0)
    decr(h); 
/*    else h = 607;  */
    else h = hyphen_prime; 
  } 
  lab45: decr(hn); 
  if (trie_trc[cur_lang + 1]!= cur_lang)
  return; 
  hc[0]= 0; 
  hc[hn + 1]= 0; 
  hc[hn + 2]= 256; 
  {
    register integer for_end; 
    j = 0; 
    for_end = hn - rhyf + 1; 
    if (j <= for_end) do 
    {
      z = trie_trl[cur_lang + 1]+ hc[j]; 
      l = j; 
      while(hc[l]== trie_trc[z]){
      if (trie_tro[z]!= min_trie_op)
  {
    v = trie_tro[z]; 
    do {
        v = v + op_start[cur_lang]; 
      i = l - hyf_distance[v]; 
      if (hyf_num[v]> hyf[i])
      hyf[i]= hyf_num[v]; 
      v = hyf_next[v]; 
    } while(!(v == min_trie_op)); 
  } 
  incr(l); 
  z = trie_trl[z]+ hc[l]; 
      } 
    } 
  while(j++ < for_end); } 
  lab40: {
      register integer for_end; 
    j = 0; 
    for_end = lhyf - 1; 
    if (j <= for_end) do 
      hyf[j]= 0; 
    while(j++ < for_end);
     } 
  {
    register integer for_end; 
    j = 0; 
    for_end = rhyf - 1; 
    if (j <= for_end) do 
      hyf[hn - j]= 0; 
    while(j++ < for_end);
  } 
/* was j = 0; for_end = rhyf - 1; in 3.141 */
  {
    register integer for_end; 
    j = lhyf; 
    for_end = hn - rhyf; 
    if (j <= for_end) do 
/* lost if (j <= for_end) do since 3.141 */
      if (odd(hyf[j])) 
        goto lab41; 
    while(j++ < for_end);
  } 
  return; 
  lab41:; 
  q = mem[hb].hh.v.RH; 
  mem[hb].hh.v.RH = 0; 
  r = mem[ha].hh.v.RH; 
  mem[ha].hh.v.RH = 0; 
  bchar = hyfbchar; 
  if ((ha >= hi_mem_min)) 
  if (mem[ha].hh.b0 != hf)
  goto lab42; 
  else {
      
    init_list = ha; 
    init_lig = false; 
    hu[0]= mem[ha].hh.b1; 
  } 
  else if (mem[ha].hh.b0 == 6)
  if (mem[ha + 1].hh.b0 != hf)
  goto lab42; 
  else {
      
    init_list = mem[ha + 1].hh.v.RH; 
    init_lig = true; 
    init_lft =(mem[ha].hh.b1 > 1); 
    hu[0]= mem[ha + 1].hh.b1; 
    if (init_list == 0)
    if (init_lft)
    {
      hu[0]= 256; 
      init_lig = false; 
    } 
    free_node(ha, 2); 
  } 
  else {
      
    if (!(r >= hi_mem_min)) 
    if (mem[r].hh.b0 == 6)
    if (mem[r].hh.b1 > 1)
    goto lab42; 
    j = 1; 
    s = ha; 
    init_list = 0; 
    goto lab50; 
  } 
  s = cur_p; 
  while(mem[s].hh.v.RH != ha)s = mem[s].hh.v.RH; 
  j = 0; 
  goto lab50; 
  lab42: s = ha; 
  j = 0; 
  hu[0]= 256; 
  init_lig = false; 
  init_list = 0; 
  lab50: flush_node_list(r); 
  do {
      l = j; 
    j = reconstitute(j, hn, bchar, hyf_char)+ 1; 
    if (hyphen_passed == 0)
    {
      mem[s].hh.v.RH = mem[hold_head].hh.v.RH; 
/*      while(mem[s].hh.v.RH > 0)*/ /* 94/Mar/22 */
      while(mem[s].hh.v.RH != 0)    /* l.17903 */
      s = mem[s].hh.v.RH; 
      if (odd(hyf[j - 1])) 
      {
  l = j; 
  hyphen_passed = j - 1; 
  mem[hold_head].hh.v.RH = 0; /* link(hold_head):=null; */
      } 
    } 
    if (hyphen_passed > 0)
    do {
  r = get_node(2); 
      mem[r].hh.v.RH = mem[hold_head].hh.v.RH; 
      mem[r].hh.b0 = 7; 
      majortail = r; 
      rcount = 0; 
/* while link(major_tail)>null do advance_major_tail; l.17929 */
/*      while(mem[majortail].hh.v.RH > 0){ */ /* 94/Mar/22 */
      while(mem[majortail].hh.v.RH != 0){   /* ??? */
    
  majortail = mem[majortail].hh.v.RH; 
  incr(rcount); 
      } 
      i = hyphen_passed; 
      hyf[i]= 0; 
/* minor_tail:=null; pre_break(r):=null; l.17943 */
      minortail = 0; 
      mem[r + 1].hh.v.LH = 0; 
/* hyf_node:=new_character(hf,hyf_char); */
      hyfnode = new_character(hf, hyf_char); 
      if (hyfnode != 0)  /* if hyf_node<>null then */
      {
  incr(i); 
  c = hu[i]; 
  hu[i]= hyf_char; 
  {
    mem[hyfnode].hh.v.RH = avail; 
    avail = hyfnode; 
  ;
#ifdef STAT
    decr(dyn_used); 
#endif /* STAT */
  } 
      } 
      while(l <= i){
/*  begin l:=reconstitute(l,i,font_bchar[hf],non_char)+1; l.17948 */
  l = reconstitute(l, i, font_bchar[hf], 256)+ 1; 
/*  if link(hold_head)>null then l.17949 */
/*  if (mem[hold_head].hh.v.RH > 0)*/  /* 94/Mar/22 */
  if (mem[hold_head].hh.v.RH != 0) /* BUG FIX ??? */
  {
    if (minortail == 0)/*      if minor_tail=null then */
    mem[r + 1].hh.v.LH = mem[hold_head].hh.v.RH; 
    else mem[minortail].hh.v.RH = mem[hold_head].hh.v.RH; 
    minortail = mem[hold_head].hh.v.RH; 
/*    while link(minor_tail)>null do minor_tail:=link(minor_tail); l.17953 */
/*    while(mem[minortail].hh.v.RH > 0)*/ /* 94/Mar/22 */
    while(mem[minortail].hh.v.RH != 0)  /* BUG FIX */
      minortail = mem[minortail].hh.v.RH; 
  } 
      } 
      if (hyfnode != 0) /* if hyf_node<>null then l.17956 */
      {
  hu[i]= c; 
  l = i; 
  decr(i); 
      } 
/* minor_tail:=null; post_break(r):=null; c_loc:=0; l.17964 */
      minortail = 0; 
      mem[r + 1].hh.v.RH = 0; 
      cloc = 0; 
/* if bchar_label[hf]<non_address then l.17991 3.1415 */
/*      if (bchar_label[hf]< font_mem_size) */
/* if bchar_label[hf]<>non_address then l.17991 3.14159 */
      if (bchar_label[hf]!= non_address) /* i.e. 0 --- 96/Jan/15 */
      {
  decr(l); 
  c = hu[l]; 
  cloc = l; 
  hu[l]= 256; 
      } 
      while(l < j){
    
  do {
      l = reconstitute(l, hn, bchar, 256)+ 1; 
    if (cloc > 0)
    {
      hu[cloc]= c;    /* c may be used ... */
      cloc = 0; 
    } 
/*   if link(hold_head)>null then l. l.17973 */
/*    if (mem[hold_head].hh.v.RH > 0)  */    /* 94/Mar/22 ??? */
    if (mem[hold_head].hh.v.RH != 0)     /* BUG FIX */
    {
      if (minortail == 0)/*     begin if minor_tail=null then */
      mem[r + 1].hh.v.RH = mem[hold_head].hh.v.RH; 
      else mem[minortail].hh.v.RH = mem[hold_head].hh.v.RH; 
      minortail = mem[hold_head].hh.v.RH; 
/*     while link(minor_tail)>null do minor_tail:=link(minor_tail); l.17977 */
/*      while(mem[minortail].hh.v.RH > 0)*/ /* 94/Mar/22 */
      while(mem[minortail].hh.v.RH != 0)    /* ??? */
      minortail = mem[minortail].hh.v.RH; 
    } 
  } while(!(l >= j)); 
  while(l > j){
      
    j = reconstitute(j, hn, bchar, 256)+ 1; 
    mem[majortail].hh.v.RH = mem[hold_head].hh.v.RH; 
/* while link(major_tail)>null do advance_major_tail; l.17987 */
/*    while(mem[majortail].hh.v.RH > 0){ */ /* 94/Mar/22 */
    while(mem[majortail].hh.v.RH != 0){   /* ??? */
      majortail = mem[majortail].hh.v.RH; 
      incr(rcount); 
    } 
  } 
      } 
      if (rcount > 127)
      {
  mem[s].hh.v.RH = mem[r].hh.v.RH; 
  mem[r].hh.v.RH = 0; /* link(r):=null */
  flush_node_list(r); 
      } 
      else {
    
  mem[s].hh.v.RH = r; 
  mem[r].hh.b1 = rcount; 
      } 
      s = majortail; 
      hyphen_passed = j - 1; 
      mem[hold_head].hh.v.RH = 0;  /* link(hold_head):=null; */
    } while(!(! odd(hyf[j - 1]))); 
  } while(!(j > hn)); 
  mem[s].hh.v.RH = q; 
  flush_list(init_list); 
} 
/* #pragma optimize ("g", off) */ /* not needed for MSVC it seems ... */
/* used only in itex.c */
void new_hyph_exceptions (void) 
{/* 21 10 40 45 */ 
/*  small_number n;  */ /* in 3.141 */
  char n; 
/*  small_number j;  */ /* in 3.141 */
  char j; 
  hyph_pointer h; 
  str_number k; 
  halfword p; 
  halfword q; 
  str_number s, t; 
  pool_pointer u, v; 
  scan_left_brace(); 
  if (language <= 0)
    cur_lang = 0; 
  else if (language > 255)
    cur_lang = 0; 
  else cur_lang = language; 
  n = 0; 
  p = 0; 

  while(true){
    get_x_token(); 

lab21: switch(cur_cmd){
    case 11 : 
    case 12 : 
    case 68 : 
      if (cur_chr == 45)
      {
  if (n < 63)
  {
    q = get_avail(); 
    mem[q].hh.v.RH = p; 
    mem[q].hh.v.LH = n; 
    p = q; 
  } 
      } 
      else {
    
  if (eqtb[(hash_size + 2139) + cur_chr].hh.v.RH == 0)
  {
	  print_err("Not a letter");
	  help2("Letters in \\hyphenation words must have \\lccode>0.",
		  "Proceed; I'll ignore the character I just read.");
    error(); 
  } 
  else if (n < 63)
  {
    incr(n); 
    hc[n]= eqtb[(hash_size + 2139) + cur_chr].hh.v.RH; 
  } 
      } 
      break; 
    case 16 : 
      {
  scan_char_num(); 
  cur_chr = cur_val; 
  cur_cmd = 68; 
  goto lab21; 
      } 
      break; 
    case 10 : 
    case 2 : 
      {
  if (n > 1)
  {
    incr(n); 
    hc[n]= cur_lang; 
    {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
      if (pool_ptr + n > current_pool_size)
/*      str_pool = realloc_str_pool (increment_pool_size); */
      str_pool = realloc_str_pool (increment_pool_size + n);
      if (pool_ptr + n > current_pool_size){ /* in case it failed 94/Jan/24 */
      overflow("pool size", current_pool_size - init_pool_ptr); /* 97/Mar/7 */
      return;     // abort_flag set
    }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      if (pool_ptr + n > pool_size){
      overflow("pool size", pool_size - init_pool_ptr); /* pool size */
      return;     // abort_flag set
    }
#endif
    } 
    h = 0; 
    {
      register integer for_end; 
      j = 1; 
      for_end = n; 
      if (j <= for_end) do 
      {
/*      h =(h + h + hc[j])% 607;  */
        h =(h + h + hc[j])% hyphen_prime; 
        {
          str_pool[pool_ptr]= hc[j]; 
          incr(pool_ptr); 
        } 
      } 
      while(j++ < for_end);
    } 
    s = make_string(); 
/*    if (hyph_count == 607)*/
    if (hyph_count == hyphen_prime){
/*      overflow(942, 607);  */ 
      overflow("exception dictionary", hyphen_prime); /* exception dictionary - NOT DYNAMIC */
/*    not dynamic ---- but can be set -e=... from command line in ini-TeX */
      return;     // abort_flag set
    }
    incr(hyph_count); 
    while (hyph_word[h]!= 0) {
      k = hyph_word[h];
      if (length(k) < length(s))
        goto lab40;
      if (length(k) > length(s))
        goto lab45; 
      u = str_start[k]; 
      v = str_start[s]; 
      do {
    if (str_pool[u]< str_pool[v])
        goto lab40; 
        if (str_pool[u]> str_pool[v])
        goto lab45; 
        incr(u); 
        incr(v); 
      } while(!(u == str_start[k + 1])); 
      lab40: q = hyph_list[h]; 
      hyph_list[h]= p; 
      p = q; 
      t = hyph_word[h]; 
      hyph_word[h]= s; 
      s = t; 
      lab45:; 
      if (h > 0)
      decr(h); 
/*      else h = 607;  */
      else h = hyphen_prime; 
    } 
    hyph_word[h]= s; 
    hyph_list[h]= p; 
  } 
  if (cur_cmd == 2)
  return; 
  n = 0; 
  p = 0; 
      } 
      break; 
      default: 
      {
		  print_err("Improper ");
  print_esc("hyphenation");
  print_string("will be flushed");
  help2("Hyphenation exceptions must contain only letters",
	  "and hyphens. But continue; I'll forgive and forget.");
  error(); 
    }
      break; 
  } /* end of switch */
  }
} 
halfword prune_page_top_(halfword p)
{register halfword Result;
  halfword prevp; 
  halfword q; 
  prevp = temp_head; 
  mem[temp_head].hh.v.RH = p; 
/* while p<>null do l.18803 */
  while(p != 0)switch(mem[p].hh.b0)
  {case 0 : 
  case 1 : 
  case 2 : 
    {
      q = new_skip_param(10); 
      mem[prevp].hh.v.RH = q; 
      mem[q].hh.v.RH = p; 
      if (mem[temp_ptr + 1].cint > mem[p + 3].cint)
      mem[temp_ptr + 1].cint = mem[temp_ptr + 1].cint - mem[p + 3]
     .cint; 
      else mem[temp_ptr + 1].cint = 0; 
      p = 0;  /* p:=null */
    } 
    break; 
  case 8 : 
  case 4 : 
  case 3 : 
    {
      prevp = p; 
      p = mem[prevp].hh.v.RH; 
    } 
    break; 
  case 10 : 
  case 11 : 
  case 12 : 
    {
      q = p; 
      p = mem[q].hh.v.RH; 
      mem[q].hh.v.RH = 0; 
      mem[prevp].hh.v.RH = p; 
      flush_node_list(q); 
    } 
    break; 
    default: 
    {
      confusion("pruning");
      return 0;       // abort_flag set
    }
    break; 
  } 
  Result = mem[temp_head].hh.v.RH; 
  return Result; 
} 
halfword vert_break_(halfword p, scaled h, scaled d)
{/* 30 45 90 */ register halfword Result;
  halfword prevp; 
  halfword q, r; 
  integer pi; 
  integer b; 
  integer leastcost; 
  halfword best_place; 
  scaled prevdp; 
/*  small_number t;  */
  int t;              /* 95/Jan/7 */
  prevp = p; 
  leastcost = 1073741823L;  /* 2^30 - 1 */
  active_width[1]= 0; 
  active_width[2]= 0; 
  active_width[3]= 0; 
  active_width[4]= 0; 
  active_width[5]= 0; 
  active_width[6]= 0; 
  prevdp = 0; 
  while(true){
    if (p == 0)  /* if p=null l.18879 */
    pi = -10000; 
    else switch(mem[p].hh.b0)
    {case 0 : 
    case 1 : 
    case 2 : 
      {
  active_width[1]= active_width[1]+ prevdp + mem[p + 3].cint; 
  prevdp = mem[p + 2].cint; 
  goto lab45; 
      } 
      break; 
    case 8 : 
      goto lab45; 
      break; 
    case 10 : 
      if ((mem[prevp].hh.b0 < 9)) 
      pi = 0; 
      else goto lab90; 
      break; 
    case 11 : 
      {
  if (mem[p].hh.v.RH == 0)/* if link(p)=null l.18903 */
  t = 12; 
  else t = mem[mem[p].hh.v.RH].hh.b0; 
  if (t == 10)
  pi = 0; 
  else goto lab90; 
      } 
      break; 
    case 12 : 
      pi = mem[p + 1].cint; 
      break; 
    case 4 : 
    case 3 : 
      goto lab45; 
      break; 
      default: 
      {
        confusion("vertbreak");
        return 0;       // abort_flag set
      }
      break; 
    } 
    if (pi < 10000)      /* pi may be used ... */
    {
      if (active_width[1]< h)
      if ((active_width[3]!= 0)||(active_width[4]!= 0)||(
      active_width[5]!= 0)) 
      b = 0; 
      else b = badness(h - active_width[1], active_width[2]); 
      else if (active_width[1]- h > active_width[6])
      b = 1073741823L;  /* 2^30 - 1 */
      else b = badness(active_width[1]- h, active_width[6]); 
      if (b < 1073741823L) /* 2^30 - 1 */
      if (pi <= -10000)
      b = pi; 
      else if (b < 10000)
      b = b + pi; 
      else b = 100000L; 
      if (b <= leastcost)
      {
  best_place = p; 
  leastcost = b; 
  best_height_plus_depth = active_width[1]+ prevdp; 
      } 
      if ((b == 1073741823L)||(pi <= -10000))  /* 2^30 - 1 */
      goto lab30; 
    } 
    if ((mem[p].hh.b0 < 10)||(mem[p].hh.b0 > 11)) 
    goto lab45; 
    lab90: if (mem[p].hh.b0 == 11)
    q = p; 
    else {
  
      q = mem[p + 1].hh.v.LH; 
      active_width[2 + mem[q].hh.b0]= active_width[2 + mem[q]
      .hh.b0]+ mem[q + 2].cint; 
      active_width[6]= active_width[6]+ mem[q + 3].cint; 
      if ((mem[q].hh.b1 != 0)&&(mem[q + 3].cint != 0)) 
      {
		  print_err("Infinite glue shrinkage found in box being split");
		  help4("The box you are \\vsplitting contains some infinitely",
			  "shrinkable glue, e.g., `\\vss' or `\\vskip 0pt minus 1fil'.",
			  "Such glue doesn't belong there; but you can safely proceed,",
			  "since the offensive shrinkability has been made finite.");
  error(); 
  r = new_spec(q); 
  mem[r].hh.b1 = 0; 
  delete_glue_ref(q); 
  mem[p + 1].hh.v.LH = r; 
  q = r; 
      } 
    } 
    active_width[1]= active_width[1]+ prevdp + mem[q + 1].cint; 
    prevdp = 0; 
    lab45: if (prevdp > d)
    {
      active_width[1]= active_width[1]+ prevdp - d; 
      prevdp = d; 
    } 
    prevp = p; 
    p = mem[prevp].hh.v.RH; 
  } 
  lab30: Result = best_place; /* best_place may be used ... */
  return Result; 
} 
/* called only from tex7.c */
halfword vsplit_(eight_bits n, scaled h)
{/* 10 30 */ register halfword Result;
  halfword v; 
  halfword p; 
  halfword q; 
  v = eqtb[(hash_size + 1578) + n].hh.v.RH; 
  if (cur_mark[3]!= 0)
  {
    delete_token_ref(cur_mark[3]); 
    cur_mark[3]= 0; 
    delete_token_ref(cur_mark[4]); 
    cur_mark[4]= 0; 
  } 
  if (v == 0)    /* if v=null then l.18999 */
  {
    Result = 0; /*   begin vsplit:=null; return; */
    return(Result); 
  } 
  if (mem[v].hh.b0 != 1)
  {
	  print_err("");
    print_esc("vsplit");
    print_string("needs a ");
    print_esc("vbox");
	help2("The box you are trying to split is an \\hbox.",
		"I can't split such a box, so I'll leave it alone.");
    error(); 
    Result = 0; 
    return(Result); 
  } 
  q = vert_break(mem[v + 5].hh.v.RH, h, split_max_depth); 
  p = mem[v + 5].hh.v.RH; 
  if (p == q)
  mem[v + 5].hh.v.RH = 0; 
  else while(true){
    if (mem[p].hh.b0 == 4)
    if (cur_mark[3]== 0)
    {
      cur_mark[3]= mem[p + 1].cint; 
      cur_mark[4]= cur_mark[3]; 
      mem[cur_mark[3]].hh.v.LH = mem[cur_mark[3]].hh.v.LH + 2; 
    } 
    else {
      delete_token_ref(cur_mark[4]); 
      cur_mark[4]= mem[p + 1].cint; 
      incr(mem[cur_mark[4]].hh.v.LH); 
    } 
    if (mem[p].hh.v.RH == q)
    {
      mem[p].hh.v.RH = 0; 
      goto lab30; 
    } 
    p = mem[p].hh.v.RH; 
  } 
  lab30:; 
  q = prune_page_top(q); 
  p = mem[v + 5].hh.v.RH; 
  free_node(v, 7); 
  if (q == 0)    /* if q=null l.18993 */
  eqtb[(hash_size + 1578) + n].hh.v.RH = 0;  /* then box(n):=null */
  else eqtb[(hash_size + 1578) + n].hh.v.RH =
    vpackage(q, 0, 1, 1073741823L);  /* 2^30 - 1 */
  Result = vpackage(p, h, 0, split_max_depth); 
  return Result; 
} 
void print_totals (void) 
{ 
  print_scaled(page_so_far[1]); 
  if (page_so_far[2]!= 0)
  {
    print_string(" plus ");
    print_scaled(page_so_far[2]); 
    print_string("");
  } 
  if (page_so_far[3]!= 0)
  {
    print_string(" plus ");
    print_scaled(page_so_far[3]); 
    print_string("fil");
  } 
  if (page_so_far[4]!= 0)
  {
    print_string(" plus ");
    print_scaled(page_so_far[4]); 
    print_string("fill");
  } 
  if (page_so_far[5]!= 0)
  {
    print_string(" plus ");
    print_scaled(page_so_far[5]); 
    print_string("filll");
  } 
  if (page_so_far[6]!= 0)
  {
    print_string(" minus ");
    print_scaled(page_so_far[6]); 
  } 
} 
void freeze_page_specs_(small_number s)
{ 
  page_contents = s; 
  page_so_far[0]= vsize; 
  page_max_depth = max_depth; 
  page_so_far[7]= 0; 
  page_so_far[1]= 0; 
  page_so_far[2]= 0; 
  page_so_far[3]= 0; 
  page_so_far[4]= 0; 
  page_so_far[5]= 0; 
  page_so_far[6]= 0; 
  least_page_cost = 1073741823L;  /* 2^30 - 1 */
  ;
#ifdef STAT
  if (tracing_pages > 0)
  {
    begin_diagnostic(); 
    print_nl("might split");  /*  */
    print_scaled(page_so_far[0]); 
    print_string(", max depth=");
    print_scaled(page_max_depth); 
    end_diagnostic(false); 
  } 
#endif /* STAT */
} 
void box_error_(eight_bits n)
{
    error(); 
  begin_diagnostic(); 
  print_nl("The following box has been deleted:");  /*  */
  show_box(eqtb[(hash_size + 1578) + n].hh.v.RH); 
  end_diagnostic(true); 
  flush_node_list(eqtb[(hash_size + 1578) + n].hh.v.RH); 
  eqtb[(hash_size + 1578) + n].hh.v.RH = 0; 
} 
void ensure_vbox_(eight_bits n)
{
  halfword p; 
  p = eqtb[(hash_size + 1578) + n].hh.v.RH; 
  if (p != 0) /* if p<>null then if type(p)=hlist_node then l.19324 */
  if (mem[p].hh.b0 == 0)
  {
	  print_err("Insertions can only be added to a vbox");
	  help3("Tut tut: You're trying to \\insert into a",
		  "\\box register that now contains an \\hbox.",
		  "Proceed, and I'll discard its present contents.");
    box_error(n); 
  } 
} 
/* called only from tex7.c */
void fire_up_(halfword c)
{/* 10 */ 
  halfword p, q, r, s; 
  halfword prevp; 
/*  unsigned char n;  */
  unsigned int n;         /* 95/Jan/7 */
  bool wait; 
  integer savevbadness; 
  scaled savevfuzz; 
  halfword savesplittopskip; 
  if (mem[best_page_break].hh.b0 == 12)
  {
    geq_word_define((hash_size + 3202), mem[best_page_break + 1].cint); 
    mem[best_page_break + 1].cint = 10000; 
  } 
  else geq_word_define((hash_size + 3202), 10000); 
  if (cur_mark[2]!= 0)
  {
    if (cur_mark[0]!= 0)
    delete_token_ref(cur_mark[0]); 
    cur_mark[0]= cur_mark[2]; 
    incr(mem[cur_mark[0]].hh.v.LH); 
    delete_token_ref(cur_mark[1]); 
    cur_mark[1]= 0; 
  } 
  if (c == best_page_break)
  best_page_break = 0; 
  if (eqtb[(hash_size + 1833)].hh.v.RH != 0)
  {
	  print_err("");
    print_esc("box");
    print_string("255 is not void");
	help2("You shouldn't use \\box255 except in \\output routines.",
		"Proceed, and I'll discard its present contents.");
    box_error(255); 
  } 
  insert_penalties = 0; 
  savesplittopskip = eqtb[(hash_size + 792)].hh.v.RH; 
  if (holding_inserts <= 0)
  {
    r = mem[mem_top].hh.v.RH; 
    while(r != mem_top){
  
      if (mem[r + 2].hh.v.LH != 0)
      {
  n = mem[r].hh.b1; 
  ensure_vbox(n); 
/*    if box(n)=null then box(n):=new_null_box; l.19759 */
  if (eqtb[(hash_size + 1578) + n].hh.v.RH == 0)
  eqtb[(hash_size + 1578) + n].hh.v.RH = new_null_box(); 
  p = eqtb[(hash_size + 1578) + n].hh.v.RH + 5; 
  while(mem[p].hh.v.RH != 0)p = mem[p].hh.v.RH; 
  mem[r + 2].hh.v.RH = p; 
      } 
      r = mem[r].hh.v.RH; 
    } 
  } 
  q = hold_head; 
  mem[q].hh.v.RH = 0; 
  prevp = page_head; 
  p = mem[prevp].hh.v.RH; 
  while(p != best_page_break){
      
    if (mem[p].hh.b0 == 3)
    {
      if (holding_inserts <= 0)
      {
  r = mem[mem_top].hh.v.RH; 
  while(mem[r].hh.b1 != mem[p].hh.b1)r = mem[r].hh.v.RH 
  ; 
/*  if best_ins_ptr(r)=null then wait:=true l.19783 */
  if (mem[r + 2].hh.v.LH == 0)
  wait = true; 
  else {
      
    wait = false; 
    s = mem[r + 2].hh.v.RH; 
    mem[s].hh.v.RH = mem[p + 4].hh.v.LH; 
    if (mem[r + 2].hh.v.LH == p)
    {
      if (mem[r].hh.b0 == 1)
      if ((mem[r + 1].hh.v.LH == p)&&(mem[r + 1].hh.v.RH 
      != 0)) 
      {
        while(mem[s].hh.v.RH != mem[r + 1].hh.v.RH)s = mem 
       [s].hh.v.RH; 
        mem[s].hh.v.RH = 0; 
        eqtb[(hash_size + 792)].hh.v.RH = mem[p + 4].hh.v.RH; 
        mem[p + 4].hh.v.LH = prune_page_top(mem[r + 1].hh.v.RH 
      ); 
        if (mem[p + 4].hh.v.LH != 0)
        {
    temp_ptr = vpackage(mem[p + 4].hh.v.LH, 0, 1, 
    1073741823L);  /* 2^30 - 1 */
    mem[p + 3].cint = mem[temp_ptr + 3].cint + mem[
    temp_ptr + 2].cint; 
    free_node(temp_ptr, 7); 
    wait = true; 
        } 
      } 
      mem[r + 2].hh.v.LH = 0; 
      n = mem[r].hh.b1; 
      temp_ptr = mem[eqtb[(hash_size + 1578) + n].hh.v.RH + 5].hh.v.RH; 
      free_node(eqtb[(hash_size + 1578) + n].hh.v.RH, 7); 
      eqtb[(hash_size + 1578) + n].hh.v.RH = vpackage(temp_ptr, 0, 1, 
      1073741823L);  /* 2^30 - 1 */
    } 
    else {
        
      while(mem[s].hh.v.RH != 0)s = mem[s].hh.v.RH; 
      mem[r + 2].hh.v.RH = s; 
    } 
  } 
  mem[prevp].hh.v.RH = mem[p].hh.v.RH; 
  mem[p].hh.v.RH = 0; 
  if (wait)
  {
    mem[q].hh.v.RH = p; 
    q = p; 
    incr(insert_penalties); 
  } 
  else {
      
    delete_glue_ref(mem[p + 4].hh.v.RH); 
    free_node(p, 5); 
  } 
  p = prevp; 
      } 
    } 
    else if (mem[p].hh.b0 == 4)
    {
      if (cur_mark[1]== 0)
      {
  cur_mark[1]= mem[p + 1].cint; 
  incr(mem[cur_mark[1]].hh.v.LH); 
      } 
      if (cur_mark[2]!= 0)
      delete_token_ref(cur_mark[2]); 
      cur_mark[2]= mem[p + 1].cint; 
      incr(mem[cur_mark[2]].hh.v.LH); 
    } 
    prevp = p; 
    p = mem[prevp].hh.v.RH; 
  } 
  eqtb[(hash_size + 792)].hh.v.RH = savesplittopskip; 
  if (p != 0)    /* if p<>null then l.19730 */
  {
    if (mem[contrib_head].hh.v.RH == 0)/* if link(contrib_head)=null then */
    if (nest_ptr == 0)
    tail = page_tail; 
    else nest[0].tail_field = page_tail; 
    mem[page_tail].hh.v.RH = mem[contrib_head].hh.v.RH; 
    mem[contrib_head].hh.v.RH = p; 
    mem[prevp].hh.v.RH = 0; /*   link(prev_p):=null; */
  } 
  savevbadness = vbadness; 
  vbadness = 10000; 
  savevfuzz = eqtb[(hash_size + 3739)].cint; 
  eqtb[(hash_size + 3739)].cint = 1073741823L;  /* 2^30 - 1 */
  eqtb[(hash_size + 1833)].hh.v.RH = vpackage(mem[page_head].hh.v.RH, 
  best_size, 0, page_max_depth); 
  vbadness = savevbadness; 
  eqtb[(hash_size + 3739)].cint = savevfuzz; 
/*  if (last_glue != 262143L) */
  if (last_glue != empty_flag)
  delete_glue_ref(last_glue); 
  page_contents = 0; 
  page_tail = page_head; 
  mem[page_head].hh.v.RH = 0; 
/*  last_glue = 262143L;  */
  last_glue = empty_flag; 
  last_penalty = 0; 
  last_kern = 0; 
  page_so_far[7]= 0; 
  page_max_depth = 0; 
  if (q != hold_head)
  {
    mem[page_head].hh.v.RH = mem[hold_head].hh.v.RH; 
    page_tail = q; 
  } 
  r = mem[mem_top].hh.v.RH; 
  while(r != mem_top){
      
    q = mem[r].hh.v.RH; 
    free_node(r, 4); 
    r = q; 
  } 
  mem[mem_top].hh.v.RH = mem_top; 
/* if (top_mark<>null)and(first_mark=null) then l.19654 */
  if ((cur_mark[0]!= 0)&&(cur_mark[1]== 0)) 
  {
    cur_mark[1]= cur_mark[0]; 
    incr(mem[cur_mark[0]].hh.v.LH); 
  } 
/* if output_routine<>null then */
  if (output_routine != 0)
  if (dead_cycles >= max_dead_cycles)
  {
	  print_err("Output loop---");
    print_int(dead_cycles); 
    print_string(" consecutive dead cycles");
	help3("I've concluded that your \\output is awry; it never does",
		"\\ship_out, so I'm shipping \box255 out myself. Next ",
		"increase \\maxdeadcycles if you want me to be more patient!");
    error(); 
  } 
  else {
    output_active = true; 
    incr(dead_cycles); 
    push_nest(); 
    mode = -1; 
    cur_list.aux_field.cint = ignore_depth; 
    mode_line = - (integer) line; 
    begin_token_list(output_routine, 6); /* output */
    new_save_level(8); 
    normal_paragraph(); 
    scan_left_brace(); 
    return; 
  } 
  {
    if (mem[page_head].hh.v.RH != 0)
    {
      if (mem[contrib_head].hh.v.RH == 0)
      if (nest_ptr == 0)
      tail = page_tail; 
      else nest[0].tail_field = page_tail; 
      else mem[page_tail].hh.v.RH = mem[contrib_head].hh.v.RH; 
      mem[contrib_head].hh.v.RH = mem[page_head].hh.v.RH; 
      mem[page_head].hh.v.RH = 0; 
      page_tail = page_head; 
    } 
    ship_out(eqtb[(hash_size + 1833)].hh.v.RH);
    eqtb[(hash_size + 1833)].hh.v.RH = 0; 
  } 
} 
/* used to continue here with build_page etc in tex6.c */

