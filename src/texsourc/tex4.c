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

#pragma warning(disable:4996)
#pragma warning(disable:4131) // old style declarator
#pragma warning(disable:4135) // conversion between different integral types 
#pragma warning(disable:4127) // conditional expression is constant

#include <setjmp.h>

#define EXTERN extern

#include "texd.h"

#pragma warning(disable:4244)       /* 96/Jan/10 */

/* sec 0581 */
void char_warning_(internal_font_number f, eight_bits c)
{ 
  if (tracing_lost_chars > 0)
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (show_missing == 0)            /* show on screen 94/June/10 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      begin_diagnostic();
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (show_missing)
    {     /* add ! before 94/June/10 */
      print_nl("! ");
      print_string("Missing character: there is no ");
    }
    else
      print_nl("Missing character: there is no ");
    print(c);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (show_numeric)
    {             /* bkph 93/Dec/21 */
      print_char(' ');
      print_char('(');
      if (c / 100 > 0)
      {
        print_char(48 + c / 100);
/*      c = c % 100; */
        c = c - (c / 100) * 100;      /* ? */
        print_char(48 + c / 10);
      }
      else
      {
/*      c = c % 100; */
        c = c - (c / 100) * 100;      /* ? */
        if (c / 10 > 0) print_char(48 + c / 10);
      }
      print_char(48 + c % 10);
      print_char(')');
/*    print_char(32); */
    }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    print_string(" in font ");
    slow_print(font_name[f]);
    print_char('!');
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  if (show_missing) show_context(); */    /* in tex2.c 94/June/10 */
    if (show_missing)
    {
      if (f != 0) show_context();     /* not if its the nullfont */
    }
    if (show_missing == 0)            /* show on screen 94/June/10 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      end_diagnostic(false);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    missing_characters++;           /* bkph 93/Dec/16 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  }
}
/* sec 0582 */
halfword new_character_(internal_font_number f, eight_bits c)
{
  register halfword Result;
  halfword p;
  if (font_bc[f] <= c)
    if (font_ec[f] >= c)
      if ((font_info[char_base[f] + c].qqqq.b0 > 0))
      {
        p = get_avail();
        font(p) = f;
        character(p) = c;
        Result = p;
        return (Result);
      }
  char_warning(f, c); /* char_warning(f,c); l.11283 */
  Result = 0;       /* new_character:=null */
  return Result;
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0598 */
void dvi_swap (void)
{ 
  if (trace_flag)
  {
    show_char('\n');
    sprintf(log_line, "dvi_swap %d", dvi_gone);
    show_line(log_line, 0);
  }

  if (dvi_limit == dvi_buf_size)
  {
    writedvi(0, half_buf - 1);
    dvi_limit = half_buf;
    dvi_offset = dvi_offset + dvi_buf_size;
    dvi_ptr = 0;
  }
  else
  {
    writedvi(half_buf, dvi_buf_size - 1);
    dvi_limit = dvi_buf_size;
  }
  dvi_gone = dvi_gone + half_buf;
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* attempt at speeding up bkph - is compiler smart ? */
/* sec 0600 */
void dvi_four_(integer x)
{ 
  if (x >= 0)
    // dvi_out(x % 16777216L);
    dvi_out((x >> 24));
  else
  {
    x = x + 1073741824L;    /* 2^30 40000000 hex */
    x = x + 1073741824L;
    //dvi_out((x / 16777216L) + 128);
    dvi_out((x >> 24) + 128);
  }
/*  x = x % 16777216L;  */  /* % 2^24 */
  x = x & 16777215L;
  //dvi_out(x / 65536L);
  dvi_out((x >> 16));
/*  x = x % 65536L;  */ /* % 2^16 */
  x = x & 65535L;
  //dvi_out(x / 256);
  dvi_out((x >> 8));
  //dvi_out(x % 256);
  dvi_out(x & 255);
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0601 */
void zdvipop(integer l)
{
  if ((l == dvi_offset + dvi_ptr) && (dvi_ptr > 0))
    decr(dvi_ptr);
  else
    dvi_out(142);
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0602 */
void dvi_font_def_(internal_font_number f)
{
  pool_pointer k;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
  if (f <= 256)
  {
    dvi_out(243);
    dvi_out(f - 1);
  }
  else
  {
    dvi_out(244);
    dvi_out(((f - 1) >> 8));
    dvi_out(((f - 1) & 255));
  }
#else
  dvi_out(243);
  dvi_out(f - 1);
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* spit out the font checksum now */
  dvi_out(font_check[f].b0);
  dvi_out(font_check[f].b1);
  dvi_out(font_check[f].b2);
  dvi_out(font_check[f].b3);
  dvi_four(font_size[f]); 
  dvi_four(font_dsize[f]);
  dvi_out(length(font_area[f]));
  dvi_out(length(font_name[f]));
/* sec 0603 */
  for (k = str_start[font_area[f]]; k <= str_start[font_area[f]+ 1] - 1; k++)
    dvi_out(str_pool[k]);
  for (k = str_start[font_name[f]]; k <= str_start[font_name[f]+ 1] - 1; k++)
    dvi_out(str_pool[k]);
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0607 */
void zmovement(scaled w, eight_bits o)
{
  small_number mstate;
  halfword p, q;
  integer k;
  q = get_node(3);
  mem[q + 1].cint = w;
  mem[q + 2].cint = dvi_offset + dvi_ptr;
  if (o == 157) /* 157 == down1 */
  {
    mem[q].hh.v.RH = down_ptr;
    down_ptr = q;
  }
  else
  { /* 143 == right1 */
    mem[q].hh.v.RH = right_ptr;
    right_ptr = q;
  }
  p = mem[q].hh.v.RH;
  mstate = 0;
  while (p != 0) {  /* while p<>null do l.12153 */
    if (mem[p + 1].cint == w)
      switch(mstate + mem[p].hh.v.LH)
      {
        case 3:
        case 4:
        case 15:
        case 16:
          if (mem[p + 2].cint < dvi_gone)
            goto lab45;
          else
          {
            k = mem[p + 2].cint - dvi_offset;
            if (k < 0)
              k = k + dvi_buf_size;
            dvi_buf[k]= dvi_buf[k] + 5;
            mem[p].hh.v.LH = 1;
            goto lab40;
          }
          break;
        case 5:
        case 9:
        case 11:
          if (mem[p + 2].cint < dvi_gone)
            goto lab45;
          else
          {
            k = mem[p + 2].cint - dvi_offset;
            if (k < 0)
              k = k + dvi_buf_size;
            dvi_buf[k]= dvi_buf[k] + 10;
            mem[p].hh.v.LH = 2;
            goto lab40;
          }
          break;
        case 1:
        case 2:
        case 8:
        case 13:
          goto lab40;
          break;
        default:
          break;
      }
    else switch (mstate + mem[p].hh.v.LH)
    {
      case 1:
        mstate = 6;
        break;
      case 2:
        mstate = 12;
        break;
      case 8:
      case 13:
        goto lab45;
        break;
      default:
        break;
    }
    p = mem[p].hh.v.RH;
  }
lab45:
  ;
  mem[q].hh.v.LH = 3;
  if (abs(w) >= 8388608L) /* 2^23 */
  {
    dvi_out(o + 3);
    dvi_four(w);
    return;
  }
  if (abs(w) >= 32768L)
  {
    dvi_out(o + 2);
    if (w < 0)
      w = w + 16777216L;  /* 2^24 */
    //dvi_out(w / 65536L);
    dvi_out((w >> 16));
/*    w = w % 65536L; */
    w = w & 65535L;
    goto lab2;
  }
  if (abs(w)>= 128)
  {
    dvi_out(o + 1);
    if (w < 0)
      w = w + 65536L;
    goto lab2;
  }
  dvi_out(o);
  if (w < 0)
    w = w + 256;
  goto lab1;
lab2:
  //dvi_out(w / 256);
  dvi_out((w >> 8));
lab1:
  //dvi_out(w % 256);
  dvi_out(w & 255);
  return;
lab40:
  mem[q].hh.v.LH = mem[p].hh.v.LH;
  if (mem[q].hh.v.LH == 1)
  {
    dvi_out(o + 4);
    while (mem[q].hh.v.RH != p) {
      q = mem[q].hh.v.RH;
      switch (mem[q].hh.v.LH)
      {
        case 3:
          mem[q].hh.v.LH = 5;
          break;
        case 4:
          mem[q].hh.v.LH = 6;
          break;
        default:
          break;
      }
    }
  }
  else
  {
    dvi_out(o + 9);
    while (mem[q].hh.v.RH != p) {
      q = mem[q].hh.v.RH;
      switch (mem[q].hh.v.LH)
      {
        case 3:
          mem[q].hh.v.LH = 4;
          break;
        case 5:
          mem[q].hh.v.LH = 6;
          break;
        default:
          break;
      }
    }
  }
}
/* sec 0615 */
void prune_movements_(integer l)
{
  halfword p;
  while (down_ptr != 0)
  {
    if (mem[down_ptr + 2].cint < l)
      goto lab30;
    p = down_ptr;
    down_ptr = mem[p].hh.v.RH;
    free_node(p, 3);
  }
lab30:
  while (right_ptr != 0)
  {
    if (mem[right_ptr + 2].cint < l)
      return;
    p = right_ptr;
    right_ptr = mem[p].hh.v.RH;
    free_node(p, 3);
  }
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 1368 */
void special_out_(halfword p)
{
  char old_setting;
  pool_pointer k;

  if (cur_h != dvi_h)
  {
    movement(cur_h - dvi_h, 143); /* 143 == right1 */
    dvi_h = cur_h;
  }

  if (cur_v != dvi_v)
  {
    movement(cur_v - dvi_v, 157); /* 157 == down1 */
    dvi_v = cur_v;
  }
  old_setting = selector;
  selector = new_string;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* About output \special{...} make some space in string pool 97/Mar/9 */
#ifdef ALLOCATESTRING
  if (pool_ptr + 32000 > current_pool_size)
    str_pool = realloc_str_pool (increment_pool_size);
/* We don't bother to check whether this worked - will catch later */
#endif
/* Potential problem here if current_pool_size is not much more than pool_ptr! */
/* Fixed 97/Mar/9 in version 2.0.3 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
/*  show_token_list(mem[mem[p + 1].hh.v.RH].hh.v.RH, 0,
    current_pool_size - pool_ptr); */
  show_token_list(mem[mem[p + 1].hh.v.RH].hh.v.RH, 0, 10000000L);
/*  Above is safe, since print/print_char/print_esc will extend string space */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  show_token_list(mem[mem[p + 1].hh.v.RH].hh.v.RH, 0, pool_size - pool_ptr);
#endif
  selector = old_setting;
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* str_room(1) - is there space for one more character in string pool ? */
#ifdef ALLOCATESTRING
    if (pool_ptr + 1 > current_pool_size)
      str_pool = realloc_str_pool (increment_pool_size);
    if (pool_ptr + 1 > current_pool_size)
    { /* in case it failed 94/Jan/24 */
      overflow("pool size", current_pool_size - init_pool_ptr); /* 97/Mar/7 */
      return;     // abort_flag set
    }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (pool_ptr + 1 > pool_size)
    {
      overflow("pool size", pool_size - init_pool_ptr); /* pool size */
      return;     // abort_flag set
    }
#endif
  }
  if (cur_length < 256)  /* can use xxx1 ? */
  {
    dvi_out(239);
/* long to unsigned char ... */
    dvi_out(cur_length);
  } else { /* use xxx4 instead */
    dvi_out(242);
    dvi_four(cur_length); 
  } 

#ifdef IGNORED
/*  debugging code for \special{src: ... } 98/Nov/11 */
  {
    int k = str_start[str_ptr];
    int kend = pool_ptr;
    if (kend > k + 4) {
      if (str_pool [k] == 's' &&
        str_pool [k+1] == 'r' &&
        str_pool [k+2] == 'c' &&
        str_pool [k+3] == ':') {  /* \special{src: ... } */
        show_char('\n');
        s = log_line;
        while (k < kend) {
          *s++ = str_pool[k++];
        }
        *s++ = ' ';
        *s++ = '\0';
        show_line(log_line, 0)
#ifndef _WINDOWS
        fflush(stdout);
#endif
        if (cur_input.name_field > 17)  { /* redundant ? */
          print(cur_input.name_field);          
          print_char('(');
          print_int(line);      /* line number */
          print_char(')');
          print_char(' ');
          print_char(':');
        }
#ifndef _WINDOWS
        fflush(stdout);
#endif
      }
    }
  }
#endif
  for (k = str_start[str_ptr]; k <= pool_ptr - 1; k++)
    dvi_out(str_pool[k]);
  pool_ptr = str_start[str_ptr];
}
/* noew returns -1 if it fails */
/* sec 1370 */
void write_out_(halfword p)
{
  char old_setting;
/*  integer oldmode;  */
  int oldmode;          /* 1995/Jan/7 */
/*  small_number j;  */
  int j;              /* 1995/Jan/7 */
  halfword q, r;
/* q:=get_avail; info(q):=right_brace_token+"}";@/ */
  q = get_avail();
  info(q) = 637;
/* r:=get_avail; link(q):=r; info(r):=end_write_token; ins_list(q);@/ */
/* @d end_write_token==cs_token_flag+end_write */
  r = get_avail();
  link(q) = r;
/*  mem[r].hh.v.LH = (hash_size + 4617);  */
/*  mem[r].hh.v.LH = (hash_size + 4095 + 522); */
  info(r) = (hash_size + hash_extra + 4095 + 522);
  begin_token_list(q, 4);
  begin_token_list(mem[p + 1].hh.v.RH, 15);
  q = get_avail();
  mem[q].hh.v.LH = 379;
  begin_token_list(q, 4);
  oldmode = mode;
  mode = 0;
  cur_cs = write_loc;
  q = scan_toks(false, true);
  get_token();
/*  if (cur_tok != (hash_size + 4617)) */
/*  if (cur_tok != (hash_size + 4095 + 522)) */
  if (cur_tok != (hash_size + hash_extra + 4095 + 522))
  {
    print_err("Unbalanced write command");
    help2("On this page there's a \\write with fewer real {'s than }'s.",
        "I can't handle that very well; good luck.");
    error();
    do
    {
      get_token();
    }
    while(!(cur_tok == (hash_size + hash_extra + 4095 + 522))); /*1996/Jan/10*/
  }
  mode = oldmode;
  end_token_list();
  old_setting = selector;
  j = mem[p + 1].hh.v.LH;
  if (write_open[j])
    selector = j;
  else
  {
    if ((j == 17) && (selector == 19))
      selector = 18;
    print_nl("");
  }
  token_show(def_ref);
  print_ln();
  flush_list(def_ref);
  selector = old_setting;
}
/* sec 1373 */
void out_what_(halfword p)
{
/*  small_number j;  */
  int j;            /* 1995/Jan/7 */
  switch(mem[p].hh.b1)
  {case 0 : 
  case 1 : 
  case 2 : 
    if (!doing_leaders) {
      j = mem[p + 1].hh.v.LH; 
      if (mem[p].hh.b1 == 1){
      write_out(p);
    } else {
      if (write_open[j])
        (void) a_close(write_file[j]); 
      if (mem[p].hh.b1 == 2)
        write_open[j]= false; 
      else if (j < 16){
        cur_name = mem[p + 1].hh.v.RH; 
        cur_area = mem[p + 2].hh.v.LH; 
        cur_ext = mem[p + 2].hh.v.RH; 
        if (cur_ext == 335)  /* "" */
          cur_ext = 785;  /* => ".tex" */
        pack_file_name(cur_name, cur_area, cur_ext); 
        while(! a_open_out(write_file[j])) {
          prompt_file_name(1294, 785); /* output file name  .tex */
    }
    write_open[j]= true; 
  } 
      } 
    } 
    break; 
  case 3 : 
    special_out(p); 
    break; 
  case 4 : 
 ; 
    break; 
    default: 
    {
      confusion("ext4");
      return;       // abort_flag set
    }
    break; 
  } 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0619 */
void hlist_out (void)
{
  scaled baseline;
  scaled leftedge;
  scaled saveh, savev;
  halfword thisbox;
/*  glue_ord gorder;  */
  int gorder;           /* 95/Jan/7 */
/*  char gsign;  */
  int gsign;            /* 95/Jan/7 */
  halfword p;
  integer saveloc;
  halfword leaderbox;
  scaled leaderwd;
  scaled lx;
  bool outerdoingleaders;
  scaled edge;
  real gluetemp;
  real cur_glue;
  scaled cur_g;

  cur_g = 0;
  cur_glue = 0.0;
  thisbox = temp_ptr;
  gorder = glue_order(thisbox);
  gsign = glue_sign(thisbox);
  p = list_ptr(thisbox);
  incr(cur_s);
  if (cur_s > 0)
    dvi_out(141);
  if (cur_s > max_push)
    max_push = cur_s;
  saveloc = dvi_offset + dvi_ptr;
  baseline = cur_v;
  leftedge = cur_h;
/* while p<>null do l.12314 */
  while(p != 0)
lab21:
  if (is_char_node(p))
  {
    if (cur_h != dvi_h)
    {
      movement(cur_h - dvi_h, 143);   /* 143 == right1 */
      dvi_h = cur_h;
    }
    if (cur_v != dvi_v)
    {
      movement(cur_v - dvi_v, 157);   /* 157 == down1 */
      dvi_v = cur_v;
    }
    do
    {
      f = font(p);
      c = character(p);
      if (f != dvi_f)
      {
        if (!font_used[f])
        {
          dvi_font_def(f);
          font_used[f] = true;
        }
        if (f <= 64)
          dvi_out(f + 170); /* fnt_num_0 --- fnt_num_63 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
        /* if we allow greater than 256 fonts */
        else if (f <= 256)
        {
          dvi_out(235); /* fnt1 followed by f */
          dvi_out(f - 1);
        }
#else
        /* normal TeX 82 case */
        else
        {
          dvi_out(235); /* fnt1 followed by f */
          dvi_out(f - 1);
        }
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
        /* fnt2 followed by f / 256,  f % 256 */
        else
        {
          dvi_out(236);
          dvi_out(((f - 1) >> 8)); /* top byte */
          dvi_out(((f - 1) & 255)); /* bottom byte */
        }
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
        dvi_f = f;
      }
      if (c >= 128)
        dvi_out(128);
      dvi_out(c);
      cur_h = cur_h + font_info[width_base[f] + font_info[char_base[f] + c].qqqq.b0].cint;
      p = link(p);
    } while(!(!(p >= hi_mem_min)));
    dvi_h = cur_h;
  }
  else
  {
    switch (type(p))
    {
      case hlist_node:
      case vlist_node:
        if (list_ptr(p) == 0)
          cur_h = cur_h + width(p);
        else
        {
          saveh = dvi_h;
          savev = dvi_v;
          cur_v = baseline + shift_amount(p);
          temp_ptr = p;
          edge = cur_h;
          if (type(p) == vlist_node)
            vlist_out();
          else
            hlist_out();
          dvi_h = saveh;
          dvi_v = savev;
          cur_h = edge + width(p);
          cur_v = baseline;
        }
        break;
      case rule_node:
        {
          rule_ht = height(p);
          rule_dp = depth(p);
          rule_wd = width(p);
          goto lab14;
        }
        break;
      case whatsit_node:
        out_what(p);
        break;
      case glue_node:
        {
          g = glue_ptr(p);
          rule_wd = width(g) - cur_g;
          if (gsign != normal)
          {
            if (gsign == stretching)
            {
              if (stretch_order(g) == gorder)
              {
                //gluetemp = glue_set(thisbox) * stretch(g);
                cur_glue = cur_glue + stretch(g);
                gluetemp = glue_set(thisbox) * cur_glue;
                if (gluetemp > 1000000000.0)
                  gluetemp = 1000000000.0;
                else if (gluetemp < -1000000000.0)
                  gluetemp = -1000000000.0;
                cur_g = round(gluetemp);
                //rule_wd = rule_wd + round(gluetemp);
              }
            }
            else if (shrink_order(g) == gorder)
            {
              //gluetemp = glue_set(thisbox) * shrink(g);
              cur_glue = cur_glue - shrink(g);
              gluetemp = glue_set(thisbox) * cur_glue;
              if (gluetemp > 1000000000.0)
                gluetemp = 1000000000.0;
              else if (gluetemp < -1000000000.0)
                gluetemp = -1000000000.0;
              cur_g = round(gluetemp);
              //rule_wd = rule_wd - round(gluetemp);
            }
          }
          rule_wd = rule_wd + cur_g;
          if (subtype(p) >= a_leaders)
          {
            leaderbox = leader_ptr(p);
            if (type(leaderbox) == rule_node)
            {
              rule_ht = height(leaderbox);
              rule_dp = depth(leaderbox);
              goto lab14;
            }
            leaderwd = width(leaderbox);
            if ((leaderwd > 0) && (rule_wd > 0))
            {
              rule_wd = rule_wd + 10;
              edge = cur_h + rule_wd;
              lx = 0;
              if (subtype(p) == a_leaders)
              {
                saveh = cur_h;
                cur_h = leftedge + leaderwd * ((cur_h - leftedge) / leaderwd);
                if (cur_h < saveh)
                  cur_h = cur_h + leaderwd;
              }
              else
              {
                lq = rule_wd / leaderwd;
                lr = rule_wd % leaderwd;
                if (subtype(p) == c_leaders)
                  cur_h = cur_h + (lr / 2);
                else
                {
                  lx =(2 * lr + lq + 1) / (2 * lq + 2);
                  cur_h = cur_h + ((lr - (lq - 1)* lx) / 2);
                }
              }
              while (cur_h + leaderwd <= edge)
              {
                cur_v = baseline + shift_amount(leaderbox);
                if (cur_v != dvi_v)
                {
                  movement(cur_v - dvi_v, 157); /* 157 == down1 */
                  dvi_v = cur_v;
                }
                savev = dvi_v;
                if (cur_h != dvi_h)
                {
                  movement(cur_h - dvi_h, 143); /* 143 == right1 */
                  dvi_h = cur_h;
                }
                saveh = dvi_h;
                temp_ptr = leaderbox;
                outerdoingleaders = doing_leaders;
                doing_leaders = true;
                if (type(leaderbox) == vlist_node)
                  vlist_out();
                else
                  hlist_out();
                doing_leaders = outerdoingleaders;
                dvi_v = savev;
                dvi_h = saveh;
                cur_v = baseline;
                cur_h = saveh + leaderwd + lx;
              }
              cur_h = edge - 10;
              goto lab15;
            }
          }
          goto lab13;
        }
        break;
      case kern_node:
      case math_node:
        cur_h = cur_h + width(p);
        break;
      case ligature_node:
        {
          mem[lig_trick]= mem[p + 1];
          mem[lig_trick].hh.v.RH = mem[p].hh.v.RH;
          p = lig_trick;
          goto lab21;
        }
        break;
      default:
        break;
    }
    goto lab15;
lab14:
    if ((rule_ht == -1073741824L))  /* - 2^30 */
      rule_ht = height(thisbox);
    if ((rule_dp == -1073741824L))     /* - 2^30 */
      rule_dp = depth(thisbox);
    rule_ht = rule_ht + rule_dp;
    if ((rule_ht > 0)&&(rule_wd > 0))
    {
      if (cur_h != dvi_h)
      {
        movement(cur_h - dvi_h, 143);   /* 143 == right1 */
        dvi_h = cur_h;
      }
      cur_v = baseline + rule_dp;
      if (cur_v != dvi_v)
      {
        movement(cur_v - dvi_v, 157);   /* 157 == down1 */
        dvi_v = cur_v;
      }
      dvi_out(132);
      dvi_four(rule_ht);
      dvi_four(rule_wd);
      cur_v = baseline;
      dvi_h = dvi_h + rule_wd;
    }
lab13:
    cur_h = cur_h + rule_wd;
lab15:
    p = link(p);
  }
  prune_movements(saveloc);
  if (cur_s > 0)
    dvi_pop(saveloc);
  decr(cur_s);
}
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0629 */
void vlist_out (void)
{
  scaled leftedge;
  scaled topedge;
  scaled saveh, savev;
  halfword thisbox;
/*  glue_ord gorder;  */
  int gorder;         /* 95/Jan/7 */
/*  char gsign;  */
  int gsign;          /* 95/Jan/7 */
  halfword p;
  integer saveloc;
  halfword leaderbox;
  scaled leaderht;
  scaled lx;
  bool outerdoingleaders;
  scaled edge;
  real gluetemp;
  real cur_glue;
  scaled cur_g;

  cur_g = 0;
  cur_glue = 0.0;
  thisbox = temp_ptr;
  gorder = glue_order(thisbox);
  gsign = glue_sign(thisbox);
  p = list_ptr(thisbox);
  incr(cur_s);
  if (cur_s > 0)
    dvi_out(141);
  if (cur_s > max_push)
    max_push = cur_s;
  saveloc = dvi_offset + dvi_ptr;
  leftedge = cur_h;
  cur_v = cur_v - height(thisbox);
  topedge = cur_v;

  while (p != 0)
  {  /* while p<>null do l.12494 OK */
    if ((p >= hi_mem_min))
    {
      confusion("vlistout");
      return;       // abort_flag set
    }
    else
    {
      switch (mem[p].hh.b0)
      {
        case hlist_node:
        case vlist_node:
          if (mem[p + 5].hh.v.RH == 0)
            cur_v = cur_v + height(p) + depth(p);
          else
          {
            cur_v = cur_v + height(p);
            if (cur_v != dvi_v)
            {
              movement(cur_v - dvi_v, 157);   /* 157 == down1 */
              dvi_v = cur_v;
            }
            saveh = dvi_h;
            savev = dvi_v;
            cur_h = leftedge + shift_amount(p);
            temp_ptr = p;
            if (type(p) == vlist_node)
              vlist_out();
            else
              hlist_out();
            dvi_h = saveh;
            dvi_v = savev;
            cur_v = savev + depth(p);
            cur_h = leftedge;
          }
          break;
        case rule_node:
          {
            rule_ht = height(p);
            rule_dp = depth(p);
            rule_wd = width(p);
            goto lab14;
          }
          break;
        case whatsit_node:
          out_what(p);
          break;
        case glue_node:
          {
            g = glue_ptr(p);
            rule_ht = width(g) - cur_g;
            if (gsign != normal)
            {
              if (gsign == stretching)
              {
                if (stretch_order(g) == gorder)
                {
                  //gluetemp = glue_set(thisbox) * mem[g + 2].cint;
                  cur_glue = cur_glue + stretch(g);
                  gluetemp = glue_set(thisbox) * cur_glue;
                  if (gluetemp > 1000000000.0)
                    gluetemp = 1000000000.0;
                  else if (gluetemp < -1000000000.0)
                    gluetemp = -1000000000.0;
                  //rule_ht = rule_ht + round(gluetemp);
                  cur_g = round(gluetemp);
                }
              }
              else if (mem[g].hh.b1 == gorder)   /* BUG FIX !!! */
              {
                //gluetemp = glue_set(thisbox) * mem[g + 3].cint;
                cur_glue = cur_glue - shrink(g);
                gluetemp = glue_set(thisbox) * cur_glue;
                if (gluetemp > 1000000000.0)
                  gluetemp = 1000000000.0;
                else if (gluetemp < -1000000000.0)
                  gluetemp = -1000000000.0;
                //rule_ht = rule_ht - round(gluetemp);
                cur_g = round(gluetemp);
              }
            }
            rule_ht = rule_ht + cur_g;
            if (subtype(p) >= a_leaders)
            {
              leaderbox = leader_ptr(p);
              if (type(leaderbox) == rule_node)
              {
                rule_wd = width(leaderbox);
                rule_dp = 0;
                goto lab14;
              }
              leaderht = height(leaderbox) + depth(leaderbox);
              if ((leaderht > 0) && (rule_ht > 0))
              {
                rule_ht = rule_ht + 10;
                edge = cur_v + rule_ht;
                lx = 0;
                if (subtype(p) == a_leaders)
                {
                  savev = cur_v;
                  cur_v = topedge + leaderht * ((cur_v - topedge) / leaderht);
                  if (cur_v < savev)
                    cur_v = cur_v + leaderht;
                }
                else
                {
                  lq = rule_ht / leaderht;
                  lr = rule_ht % leaderht;
                  if (subtype(p) == c_leaders)
                    cur_v = cur_v + (lr / 2);
                  else
                  {
                    lx =(2 * lr + lq + 1) / (2 * lq + 2);
                    cur_v = cur_v + ((lr - (lq - 1) * lx) / 2);
                  }
                }
                while (cur_v + leaderht <= edge)
                {
                  cur_h = leftedge + shift_amount(leaderbox);
                  if (cur_h != dvi_h)
                  {
                    movement(cur_h - dvi_h, 143);   /* 143 == right1 */
                    dvi_h = cur_h;
                  }
                  saveh = dvi_h;
                  cur_v = cur_v + height(leaderbox);
                  if (cur_v != dvi_v)
                  {
                    movement(cur_v - dvi_v, 157);   /* 157 == down1 */
                    dvi_v = cur_v;
                  }
                  savev = dvi_v;
                  temp_ptr = leaderbox;
                  outerdoingleaders = doing_leaders;
                  doing_leaders = true;
                  if (type(leaderbox) == vlist_node)
                    vlist_out();
                  else
                    hlist_out();
                  doing_leaders = outerdoingleaders;
                  dvi_v = savev;
                  dvi_h = saveh;
                  cur_h = leftedge;
                  cur_v = savev - height(leaderbox) + leaderht + lx;
                }
                cur_v = edge - 10;
                goto lab15;
              }
            }
            goto lab13;
          }
          break;
        case kern_node:
          cur_v = cur_v + width(p);
          break;
        default:
          break;
      }
      goto lab15;
lab14:
      if ((rule_wd == -1073741824L))    /* -2^30 */
        rule_wd = width(thisbox);
      rule_ht = rule_ht + rule_dp;
      cur_v = cur_v + rule_ht;
      if ((rule_ht > 0) && (rule_wd > 0))
      {
        if (cur_h != dvi_h)
        {
          movement(cur_h - dvi_h, 143);   /* 143 == right1 */
          dvi_h = cur_h;
        }
        if (cur_v != dvi_v)
        {
          movement(cur_v - dvi_v, 157);   /* 157 == down1 */
          dvi_v = cur_v;
        }
        dvi_out(137);
        dvi_four(rule_ht);
        dvi_four(rule_wd);
      }
      goto lab15;
lab13:
      cur_v = cur_v + rule_ht;
    }
lab15:
    p = link(p);
  }
  prune_movements(saveloc);
  if (cur_s > 0)
    dvi_pop(saveloc);
  decr(cur_s);
}
/****************HPDF******************/
/*
void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void * user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
}
*/
/****************HPDF******************/
/* sec 0638 */
/* following needs access to dvi_buf=zdvibuf see coerce.h */
/* sec 0638 */
void ship_out_(halfword p)
{
  integer pageloc;
  char j, k;
  pool_pointer s;
  char old_setting;

  if (tracing_output > 0)
  {
    print_nl("");
    print_ln();
    print_string("Completed box being shipped out");
  }

  if (term_offset > max_print_line - 9)
    print_ln();
  else if ((term_offset > 0) || (file_offset > 0))
    print_char(' ');
  print_char('[');
  j = 9;
  while((count(j) == 0) && (j > 0))
    decr(j);
  for (k = 0; k <= j; k++)
  {
    print_int(count(k));
    if (k < j)
      print_char('.');
  }
#ifndef _WINDOWS
  fflush(stdout);
#endif
  if (tracing_output > 0)
  {
    print_char(']');
    begin_diagnostic();
    show_box(p);
    end_diagnostic(true);
  }
  if ((height(p) > 1073741823L) || /* 2^30 - 1 */
      (depth(p) > 1073741823L) ||
      (height(p) + depth(p) + v_offset > 1073741823L) ||
      (width(p) + h_offset > 1073741823L))
  {
    print_err("Huge page cannot be shipped out");
    help2("The page just created is more than 18 feet tall or",
        "more than 18 feet wide, so I suspect something went wrong.");
    error();
    if (tracing_output <= 0)
    {
      begin_diagnostic();
      print_nl("The following box has been deleted:");
      show_box(p);
      end_diagnostic(true);
    }
    goto lab30;
  }
  if (height(p) + depth(p) + v_offset > max_v)
    max_v = height(p) + depth(p) + v_offset;
  if (width(p) + h_offset > max_h)
    max_h = height(p) + h_offset;
  dvi_h = 0;
  dvi_v = 0;
  cur_h = h_offset;
  dvi_f = 0;
  if (output_file_name == 0)
  {
    if (job_name == 0)
      open_log_file();
    pack_job_name(788);   /* .dvi */
    while(!b_open_out(dvi_file))
    {
      prompt_file_name(789, 788); /* file name for output  .dvi */
    }
    output_file_name = b_make_name_string(dvi_file);
  }

  if (total_pages == 0)
  {
    dvi_out(247);
    dvi_out(2);
/********BINDING WITH LIBHARU*********/
/*
    yandy_pdf = HPDF_New(error_handler, NULL);
    yandy_page = HPDF_AddPage(yandy_pdf);
    HPDF_SetInfoAttr(yandy_pdf, HPDF_INFO_PRODUCER, "Y&Y TeX with ApTeX");
    HPDF_SetCompressionMode (yandy_pdf, HPDF_COMP_ALL);
    HPDF_Page_SetSize (yandy_page, HPDF_PAGE_SIZE_A5, HPDF_PAGE_PORTRAIT);
    yandy_font = HPDF_GetFont (yandy_pdf, "Times-Roman", NULL);
    HPDF_Page_BeginText (yandy_page);
    HPDF_Page_SetFontAndSize (yandy_page, yandy_font, 16);
    HPDF_Page_TextOut (yandy_page, 60, HPDF_Page_GetHeight (yandy_page)-90,  "This is the \xAErst page.");
    HPDF_Page_TextOut (yandy_page, 60, HPDF_Page_GetHeight (yandy_page)-180, "Wait for ...");
    HPDF_Page_EndText (yandy_page);
    HPDF_SaveToFile (yandy_pdf, "NOTICE.pdf");
    HPDF_Free(yandy_pdf);
 */
/********BINDING WITH LIBHARU*********/
    dvi_four(25400000L);  /* magic DVI scale factor */
    dvi_four(473628672L); /* 7227 * 65536 */
    prepare_mag();
    dvi_four(mag);
    old_setting = selector;
    selector = new_string;
    print_string(" TeX output ");
    print_int(year);
    print_char('.');
    print_two(month);
    print_char('.');
    print_two(day);
    print_char(':');
    print_two(tex_time / 60);
    print_two(tex_time % 60);
    selector = old_setting;
/* long to unsigned char ... */
    dvi_out(cur_length);
    for (s = str_start[str_ptr]; s <= pool_ptr - 1; s++)
      dvi_out(str_pool[s]);
    pool_ptr = str_start[str_ptr];
  } // end of if total_pages == 0

  pageloc = dvi_offset + dvi_ptr;
  dvi_out(139);
  for (k = 0; k <= 9; k++)
    dvi_four(count(k));
  dvi_four(last_bop);
  last_bop = pageloc;
  cur_v = height(p) + v_offset;
  temp_ptr = p;
  if (type(p) == vlist_node)
    vlist_out();
  else
    hlist_out();
  dvi_out(140);
  incr(total_pages);
  cur_s = -1;
lab30:;
  if (tracing_output <= 0)
    print_char(']');
  dead_cycles = 0;
#ifndef _WINDOWS
  fflush(stdout);
#endif

#ifdef STAT
  if (tracing_stats > 1)
  {
    print_nl("Memory usage before: ");
    print_int(var_used);
    print_char('&');
    print_int(dyn_used);
    print_char(';');
  }
#endif /* STAT */
  flush_node_list(p);
#ifdef STAT
  if (tracing_stats > 1)
  {
    print_string(" after: ");
    print_int(var_used);
    print_char('&');
    print_int(dyn_used);
    print_string("; still utouched: ");
    print_int(hi_mem_min - lo_mem_max - 1); /* somewhat inaccurate free mem */
    print_ln();
  }
#endif /* STAT */
}
/* sec 0645 */
void scan_spec_(group_code c, bool threecodes)
{
  integer s;
  char speccode;

  if (threecodes)
    s = save_stack[save_ptr + 0].cint;
  if (scan_keyword("to"))
    speccode = 0;
  else if (scan_keyword("spread"))
    speccode = 1;
  else
  {
    speccode = 1;
    cur_val = 0;
    goto lab40;
  }
  scan_dimen(false, false, false);
lab40:
  if (threecodes)
  {
    save_stack[save_ptr + 0].cint = s;  /* s may be used without ... */
    incr(save_ptr);
  }
  save_stack[save_ptr + 0].cint = speccode;
  save_stack[save_ptr + 1].cint = cur_val;
  save_ptr = save_ptr + 2;
  new_save_level(c);
  scan_left_brace();
}
/* sec 0649 */
halfword hpack_(halfword p, scaled w, small_number m)
{
  register halfword Result;
  halfword r;
  halfword q;
  scaled h, d, x;
  scaled s;
  halfword g;
/*  glue_ord o;  */
  int o;              /* 95/Jan/7 */
  internal_font_number f;
  ffourquarters i;
  eight_bits hd;

  last_badness = 0;
  r = get_node(box_node_size);
  type(r) = hlist_node;
  subtype(r) = 0;
  shift_amount(r) = 0;
  q = r + list_offset;
  link(q) = p;
  h = 0;
  d = 0;
  x = 0;
  total_stretch[normal] = 0;
  total_shrink[normal] = 0;
  total_stretch[fil] = 0;
  total_shrink[fil] = 0;
  total_stretch[fill] = 0;
  total_shrink[fill] = 0;
  total_stretch[filll] = 0;
  total_shrink[filll] = 0;
  while (p != 0)
  { /* while p<>null do l.12862 */
lab21:
    while ((p >= hi_mem_min))
    {
      f = font(p);
      i = font_info[char_base[f] + mem[p].hh.b1].qqqq;
      hd = i.b1;
      x = x + font_info[width_base[f] + i.b0].cint;
      s = font_info[height_base[f] + (hd) / 16].cint;
      if (s > h)
        h = s;
      s = font_info[depth_base[f] + (hd) % 16].cint;
      if (s > d)
        d = s;
      p = link(p);
    }
    if (p != 0)  /* if p<>null then l.12886 */
    {
      switch (mem[p].hh.b0)
      {
        case hlist_node:
        case vlist_node:
        case rule_node:
        case unset_node:
          {
            x = x + width(p);
            if (type(p) >= rule_node)
              s = 0;
            else
              s = shift_amount(p);
            if (height(p) - s > h)
              h = height(p) - s;
            if (depth(p) + s > d)
              d = depth(p) + s;
          }
          break;
        case ins_node:
        case mark_node:
        case adjust_node:
          if (adjust_tail != 0)
          {
            while (link(q) != p)
              q = link(q);
            if (type(p) == adjust_node)
            {
              link(adjust_tail) = adjust_ptr(p);
              while (link(adjust_tail)!= 0)
                adjust_tail = link(adjust_tail);
              p = link(p);
              free_node(link(q), small_node_size);
            }
            else
            {
              link(adjust_tail) = p;
              adjust_tail = p;
              p = link(p);
            }
            link(q) = p;
            p = q;
          }
          break;
        case whatsit_node:
          break;
        case glue_node:
          {
            g = glue_ptr(p);
            x = x + width(g);
            o = stretch_order(g);
            total_stretch[o]= total_stretch[o] + stretch(g);
            o = shrink_order(g);
            total_shrink[o]= total_shrink[o] + shrink(g);
            if (subtype(p) >= a_leaders)
            {
              g = leader_ptr(p);
              if (height(g) > h)
                h = height(g);
              if (depth(g) > d)
                d = depth(g);
            }
          }
          break;
        case kern_node:
        case math_node:
          x = x + width(p);
          break;
        case ligature_node:
          {
            mem[lig_trick] = mem[lig_char(p)];
            link(lig_trick) = link(p);
            p = lig_trick;
            goto lab21;
          }
          break;
        default:
          break;
      }
      p = link(p);
    }
  }
  if (adjust_tail != 0)
    link(adjust_tail) = 0;
  height(r) = h;
  depth(r) = d;
  if (m == 1)
    w = x + w;
  width(r) = w;
  x = w - x;
  if (x == 0)
  {
    glue_sign(r) = normal;
    glue_order(r) = normal;
    glue_set(r) = 0.0;
    goto lab10;
  }
  else if (x > 0)
  {
    if (total_stretch[filll] != 0)
      o = filll;
    else if (total_stretch[fill] != 0)
      o = fill;
    else if (total_stretch[fil] != 0)
      o = fil;
    else
      o = normal;
    glue_order(r) = o;
    glue_sign(r) = stretching;
    if (total_stretch[o]!= 0)
      glue_set(r) = x / ((double) total_stretch[o]);
    else
    {
      glue_sign(r) = normal;
      glue_set(r) = 0.0;
    }
    if (o == normal)
      if (list_ptr(r) != 0)
      {
        last_badness = badness(x, total_stretch[0]);
        if (last_badness > hbadness)
        {
          print_ln();
          if (last_badness > 100)
            print_nl("Underfull");
          else
            print_nl("Loose");
          print_string(" \\hbox (badness ");
          print_int(last_badness);
          if (last_badness > 100) /* Y&Y TeX */
            underfull_hbox++;   /* 1996/Feb/9 */
          goto lab50;
        }
      }
      goto lab10;
  }
  else
  {
    if (total_shrink[filll] != 0)
      o = filll;
    else if (total_shrink[fill] != 0)
      o = fill;
    else if (total_shrink[fil] != 0)
      o = fil;
    else
      o = normal;
    glue_order(r) = o;
    glue_sign(r) = shrinking;
    if (total_shrink[o] != 0)
      glue_set(r) =(- (integer) x) / ((double) total_shrink[o]);
    else
    {
      glue_sign(r) = normal;
      glue_set(r) = 0.0;
    }
    if ((total_shrink[o] < - (integer) x) && (o == 0) && (list_ptr(r) != 0))
    {
      last_badness = 1000000L;
      glue_set(r) = 1.0;
      if ((- (integer) x - total_shrink[0] > hfuzz) || (hbadness < 100))
      {
          if ((overfull_rule > 0) && (- (integer) x - total_shrink[0] > hfuzz))
          {
              while(link(q) != 0)
                q = link(q);
              link(q) = new_rule();
              width(link(q)) = overfull_rule;
          }
          print_ln();
          print_nl("Overfull \\hbox(");
          print_scaled(- (integer) x - total_shrink[0]);
          print_string("pt too wide");
          overfull_hbox++;      /* 1996/Feb/9 */
          goto lab50;
      }
    }
    else if (o == normal)
      if (list_ptr(r) != 0)
      {
        last_badness = badness(- (integer) x, total_shrink[normal]);
        if (last_badness > hbadness)
        {
          print_ln();
          print_nl("Tight \\hbox (badness ");
          print_int(last_badness);
          goto lab50;
        }
      }
      goto lab10;
  }
lab50:
  if (output_active)
    print_string(") has occurred while \\output is active");
  else
  {
    if (pack_begin_line != 0)
    {
      if (pack_begin_line > 0)
        print_string(") in paragraph at lines ");
      else
        print_string(") in alignment at lines ");
      print_int(abs(pack_begin_line));
      print_string("--");
    }
    else
      print_string(") detected at line ");
    print_int(line);
  }
  print_ln();
  font_in_short_display = 0;
  short_display(list_ptr(r));
  print_ln();
  begin_diagnostic();
  show_box(r);
  end_diagnostic(true);
lab10:
  Result = r;
  return Result;
}
/* sec 0668 */
halfword vpackage_(halfword p, scaled h, small_number m, scaled l)
{
  register halfword Result;
  halfword r;
  scaled w, d, x;
  scaled s;
  halfword g;
/*  glue_ord o;  */
  int o;              /* 95/Jan/7 */

  last_badness = 0;
  r = get_node(box_node_size);
  type(r) = vlist_node;
  subtype(r) = 0;
  shift_amount(r) = 0;
  list_ptr(r) = p;
  w = 0;
  d = 0;
  x = 0;
  total_stretch[normal] = 0;
  total_shrink[normal] = 0;
  total_stretch[fil] = 0;
  total_shrink[fil] = 0;
  total_stretch[fill] = 0;
  total_shrink[fill] = 0;
  total_stretch[filll] = 0;
  total_shrink[filll] = 0;

  while (p != 0)
  {
    if ((p >= hi_mem_min))
    {
      confusion("vpack");
      return 0;       // abort_flag set
    }
    else switch (mem[p].hh.b0)
    {
      case hlist_node:
      case vlist_node:
      case rule_node:
      case unset_node:
        {
          x = x + d + height(p);
          d = depth(p);
          if (type(p) >= rule_node)
            s = 0;
          else
            s = shift_amount(p);
          if (width(p) + s > w)
            w = width(p) + s;
        }
        break;
      case whatsit_node:
        break;
      case glue_node:
        {
          x = x + d;
          d = 0;
          g = glue_ptr(p);
          x = x + width(g);
          o = stretch_order(g);
          total_stretch[o] = total_stretch[o] + stretch(g);
          o = shrink_order(g);
          total_shrink[o] = total_shrink[o] + shrink(g);

          if (subtype(p) >= a_leaders)
          {
            g = leader_ptr(p);
            if (width(g) > w)
              w = width(g);
          }
        }
        break;
      case kern_node:
        {
          x = x + d + width(p);
          d = 0;
        }
        break;
      default:
        break;
    }
    p = link(p);
  }
  width(r) = w;

  if (d > l)
  {
    x = x + d - l;
    depth(r) = l;
  }
  else
    depth(r) = d;

  if (m == 1)
    h = x + h;
  height(r) = h;
  x = h - x;
  if (x == 0)
  {
    glue_sign(r) = normal;
    glue_order(r) = normal;
    glue_set(r) = 0.0;
    goto lab10;
  }
  else if (x > 0)
  {
    if (total_stretch[filll] != 0)
      o = filll;
    else if (total_stretch[fill] != 0)
      o = fill;
    else if (total_stretch[fil] != 0)
      o = fil;
    else
      o = normal;
    glue_order(r) = o;
    glue_sign(r) = stretching;

    if (total_stretch[o] != 0)
      glue_set(r) = x / ((double) total_stretch[o]);
    else
    {
      glue_sign(r) = normal;
      glue_set(r) = 0.0;
    }

    if (o == normal)
      if (list_ptr(r) != 0)
      {
        last_badness = badness(x, total_stretch[normal]);
        if (last_badness > vbadness)
        {
          print_ln();
          if (last_badness > 100)
            print_nl("Underfull");
          else
            print_nl("Loose");
          print_string(" \\vbox (badness ");
          print_int(last_badness);
          if (last_badness > 100)
            underfull_vbox++; /* 1996/Feb/9 */
          goto lab50;
        }
      }
      goto lab10;
  }
  else
  {
    if (total_shrink[filll] != 0)
      o = filll;
    else if (total_shrink[fill] != 0)
      o = fill;
    else if (total_shrink[fil] != 0)
      o = fil;
    else
      o = normal;
    glue_order(r) = o;
    glue_sign(r) = shrinking;
    if (total_shrink[o]!= 0)
      glue_set(r) =(- (integer) x)/ ((double) total_shrink[o]);
    else
    {
      glue_sign(r) = normal;
      glue_set(r) = 0.0;
    }

    if ((total_shrink[o] < - (integer) x) && (o == 0) && (list_ptr(r) != 0))
    {
      last_badness = 1000000L;
      glue_set(r) = 1.0;
      if ((- (integer) x - total_shrink[0] > vfuzz) || (vbadness < 100))
      {
        print_ln();
        print_nl("Overfull \\vbox(");
        print_scaled(- (integer) x - total_shrink[0]);
        print_string("pt too high");
        overfull_vbox++;    /* 1996/Feb/9 */
        goto lab50;
      }
    }
    else if (o == 0)
      if (list_ptr(r) != 0)
      {
        last_badness = badness(- (integer) x, total_shrink[normal]);
        if (last_badness > vbadness)
        {
          print_ln();
          print_nl("Tight \\vbox (badness ");
          print_int(last_badness);
          goto lab50;
        }
      }
    goto lab10;
  }
lab50:
  if (output_active)
    print_string(") has occurred while \\output is active");
  else {
    if (pack_begin_line != 0)
    {
      print_string(") in alignment at lines ");
      print_int(abs(pack_begin_line));
      print_string("--");
    }
    else
      print_string(") detected at line ");
    print_int(line);
    print_ln();
  }
  begin_diagnostic();
  show_box(r);
  end_diagnostic(true);
lab10:
  Result = r;
  return Result;
}
/* sec 0679 */
void append_to_vlist_(halfword b)
{
  scaled d;
  halfword p;

  if (cur_list.aux_field.cint > ignore_depth)
  {
    d = width(baseline_skip) - cur_list.aux_field.cint - height(b);

    if (d < line_skip_limit)
      p = new_param_glue(line_skip_code);
    else
    {
      p = new_skip_param(baseline_skip_code);
      width(temp_ptr) = d;
    }
    link(tail) = p;
    tail = p;
  }
  link(tail) = b;
  tail = b;
  cur_list.aux_field.cint = depth(b);
}
/* sec 0686 */
halfword new_noad (void)
{
  halfword p;
  p = get_node(noad_size);
  type(p) = ord_noad;
  subtype(p) = normal;
  mem[nucleus(p)].hh = empty_field;
  mem[subscr(p)].hh = empty_field;
  mem[supscr(p)].hh = empty_field;
  return p;
}
/* sec 0688 */
halfword new_style_(small_number s)
{
  halfword p;
  p = get_node(style_node_size);
  type(p) = style_node;
  subtype(p) = s;
  width(p) = 0;
  depth(p) = 0;
  return p;
}
/* sec 0689 */
halfword new_choice (void)
{
  halfword p;
  p = get_node(style_node_size);
  type(p) = choice_node;
  subtype(p) = 0;
  display_mlist(p) = 0;
  text_mlist(p) = 0;
  script_mlist(p) = 0;
  script_script_mlist(p) = 0;
  return p;
}
/* sec 0693 */
void show_info (void)
{
  show_node_list(info(temp_ptr));
}
/* sec 0704 */
halfword fraction_rule_(scaled t)
{
  halfword p;
  p = new_rule();
  height(p) = t;
  depth(p) = 0;
  return p;
}
halfword overbar_(halfword b, scaled k, scaled t)
{
  register halfword Result;
  halfword p, q;
  p = new_kern(k);
  mem[p].hh.v.RH = b;
  q = fraction_rule(t);
  mem[q].hh.v.RH = p;
  p = new_kern(t);
  mem[p].hh.v.RH = q;
  Result = vpackage(p, 0, 1, 1073741823L); /* 2^30 - 1 */
  return Result;
}
halfword char_box_(internal_font_number f, quarterword c)
{
  register halfword Result;
  ffourquarters q;
  eight_bits hd;
  halfword b, p;
  q = font_info[char_base[f] + c].qqqq;
  hd = q.b1;
  b = new_null_box();
  mem[b + 1].cint = font_info[width_base[f] + q.b0].cint + font_info[italic_base[f] + (q.b2) / 4].cint;
  mem[b + 3].cint = font_info[height_base[f] + (hd) / 16].cint;
  mem[b + 2].cint = font_info[depth_base[f] + (hd) % 16].cint;
/*  long to unsigned short ... */
  p = get_avail();
  mem[p].hh.b1 = c;
  mem[p].hh.b0 = f;
  mem[b + 5].hh.v.RH = p;
  Result = b;
  return Result;
}
void stack_into_box_(halfword b, internal_font_number f, quarterword c)
{
  halfword p;
  p = char_box(f, c);
  mem[p].hh.v.RH = mem[b + 5].hh.v.RH;
  mem[b + 5].hh.v.RH = p;
  mem[b + 3].cint = mem[p + 3].cint;
} 
scaled height_plus_depth_(internal_font_number f, fquarterword c)
{
  register scaled Result;
  ffourquarters q;
  eight_bits hd;
  q = font_info[char_base[f]+ c].qqqq;
  hd = q.b1;
  Result = font_info[height_base[f] + (hd) / 16].cint + font_info[depth_base[f] + (hd) % 16].cint;
  return Result;
}
halfword var_delimiter_(halfword d, small_number s, scaled v)
{
  register halfword Result;
  halfword b;
  internal_font_number f, g;
  quarterword c, x, y;
  integer m, n;
  scaled u;
  scaled w;
  ffourquarters q;
  ffourquarters r;
  eight_bits hd;
/*  small_number z;  */
  int z;                  /* 95/Jan/7 */
/*  bool largeattempt;  */
  int largeattempt;           /* 95/Jan/7 */
  f = 0;
  w = 0;
  largeattempt = false;
  z = mem[d].qqqq.b0;
  x = mem[d].qqqq.b1;
  while (true)
  {
    if ((z != 0) || (x != 0))
    {
      z = z + s + 16;
      do
      {
        z = z - 16;
        g = eqtb[(hash_size + 1835) + z].hh.v.RH;
        if (g != 0) {
          y = x;
          if ((y >= font_bc[g]) && (y <= font_ec[g]))
          {
lab22:
            q = font_info[char_base[g]+ y].qqqq;
            if ((q.b0 > 0))
            {
              if (((q.b2) % 4) == 3)
              {
                f = g;
                c = y;
                goto lab40;
              }
              hd = q.b1;
              u = font_info[height_base[g] + (hd) / 16].cint + font_info[depth_base[g] + (hd) % 16].cint;
              if (u > w) {
                f = g;
                c = y;
                w = u;
                if (u >= v)
                  goto lab40;
              }
              if (((q.b2)% 4)== 2) {
                y = q.b3;
                goto lab22;
              }
            }
          }
        }
      } while (!(z < 16));
    } 
    if (largeattempt)
      goto lab40;
    largeattempt = true;
    z = mem[d].qqqq.b2;
    x = mem[d].qqqq.b3;
  }
lab40:
  if (f != 0)
    if (((q.b2) % 4)== 3)   /* q may be used without ... */
    {
      b = new_null_box(); 
      mem[b].hh.b0 = 1; 
      r = font_info[exten_base[f]+ q.b3].qqqq;
      c = r.b3;
      u = height_plus_depth(f, c);
      w = 0;
      q = font_info[char_base[f]+ c].qqqq;
      mem[b + 1].cint = font_info[width_base[f]+ q.b0].cint + font_info[italic_base[f]+(q.b2) / 4].cint;
      c = r.b2;
      if (c != 0)
        w = w + height_plus_depth(f, c);
      c = r.b1;
      if (c != 0)
        w = w + height_plus_depth(f, c);
      c = r.b0; 
      if (c != 0)
        w = w + height_plus_depth(f, c);
      n = 0;
      if (u > 0)
        while(w < v) {
          w = w + u;
          incr(n);
          if (r.b1 != 0)
            w = w + u;
        }
        c = r.b2;
        if (c != 0)
          stack_into_box(b, f, c); 
        c = r.b3;
        for (m = 1; m <= n; m++)
        {
          stack_into_box(b, f, c);
        }
        c = r.b1;
        if (c != 0) {
          stack_into_box(b, f, c); 
          c = r.b3;
          for (m = 1; m <= n; m++)
          {
            stack_into_box(b, f, c);
          }
        }
        c = r.b0;
        if (c != 0)
          stack_into_box(b, f, c);
        mem[b + 2].cint = w - mem[b + 3].cint;
    } else b = char_box(f, c);
  else {    /* c may be used without ... */
    b = new_null_box();
    mem[b + 1].cint = null_delimiter_space;
  } 
  mem[b + 4].cint = half(mem[b + 3].cint - mem[b + 2].cint) - font_info[22 + param_base[eqtb[(hash_size + 1837) + s].hh.v.RH]].cint;
  Result = b;
  return Result;
}
/* rebox_ etc used to follow here in tex4.c */
