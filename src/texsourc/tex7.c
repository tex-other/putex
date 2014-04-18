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

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* sec 0944 */
void build_page (void)
{
  halfword p;
  halfword q, r;
  integer b, c;
  integer pi;
/*  unsigned char n;  */
  unsigned int n;             /* 95/Jan/7 */
  scaled delta, h, w;

/* begin if (link(contrib_head)=null)or output_active then return; l.19351 */
  if ((mem[contrib_head].hh.v.RH == 0) || output_active)
    return;
  do
  {
lab22:
    p = mem[contrib_head].hh.v.RH;
/*    if (last_glue != 262143L) */
    if (last_glue != empty_flag)
      delete_glue_ref(last_glue);
    last_penalty = 0;
    last_kern = 0;
    if (mem[p].hh.b0 == 10)
    {
      last_glue = mem[p + 1].hh.v.LH;
      incr(mem[last_glue].hh.v.RH);
    }
    else
    {
/*      last_glue = 262143L;  */
      last_glue = empty_flag;
      if (mem[p].hh.b0 == 12)
        last_penalty = mem[p + 1].cint;
      else if (mem[p].hh.b0 == 11)
        last_kern = mem[p + 1].cint;
    }
    switch (mem[p].hh.b0)
    {
      case 0:
      case 1:
      case 2:
        if (page_contents < 2)
        {
          if (page_contents == 0)
            freeze_page_specs(2);
          else
            page_contents = 2;
          q = new_skip_param(9);
          if (mem[temp_ptr + 1].cint > mem[p + 3].cint)
            mem[temp_ptr + 1].cint = mem[temp_ptr + 1].cint - mem[p + 3].cint;
          else
            mem[temp_ptr + 1].cint = 0;
          mem[q].hh.v.RH = p;
          mem[contrib_head].hh.v.RH = q;
          goto lab22;
        }
        else
        {
          page_so_far[1] = page_so_far[1] + page_so_far[7] + mem[p + 3].cint;
          page_so_far[7] = mem[p + 2].cint;
          goto lab80;
        }
        break;
      case 8:
        goto lab80;
        break;
      case 10:
        if (page_contents < 2)
          goto lab31;
        else if ((mem[page_tail].hh.b0 < 9))
          pi = 0;
        else
          goto lab90;
        break;
      case 11:
        if (page_contents < 2)
          goto lab31;
        else if (mem[p].hh.v.RH == 0)
          return;
        else if (mem[mem[p].hh.v.RH].hh.b0 == 10)
          pi = 0;
        else
          goto lab90;
        break;
      case 12:
        if (page_contents < 2)
          goto lab31;
        else
          pi = mem[p + 1].cint;
        break;
      case 4:
        goto lab80;
        break;
      case 3:
        {
          if (page_contents == 0)
            freeze_page_specs(1);
          n = mem[p].hh.b1;
          r = mem_top;
          while (n >= mem[mem[r].hh.v.RH].hh.b1)
            r = mem[r].hh.v.RH;
          n = n;
          if (mem[r].hh.b1 != n)
          {
            q = get_node(4);
            mem[q].hh.v.RH = mem[r].hh.v.RH;
            mem[r].hh.v.RH = q;
            r = q;
            mem[r].hh.b1 = n;
            mem[r].hh.b0 = 0;
            ensure_vbox(n);
            if (eqtb[(hash_size + 1578) + n].hh.v.RH == 0)
              mem[r + 3].cint = 0;
            else
              mem[r + 3].cint = mem[eqtb[(hash_size + 1578) + n].hh.v.RH + 3].cint + mem[eqtb[(hash_size + 1578) + n].hh.v.RH + 2].cint;
            mem[r + 2].hh.v.LH = 0;
            q = eqtb[(hash_size + 800) + n].hh.v.RH;
            if (eqtb[(hash_size + 3218) + n].cint == 1000)
              h = mem[r + 3].cint;
            else
              h = x_over_n(mem[r + 3].cint, 1000)* eqtb[(hash_size + 3218) + n].cint;
            page_so_far[0]= page_so_far[0]- h - mem[q + 1].cint;
            page_so_far[2 + mem[q].hh.b0]= page_so_far[2 + mem[q].hh.b0]+ mem[q + 2].cint;
            page_so_far[6]= page_so_far[6]+ mem[q + 3].cint;
            if ((mem[q].hh.b1 != 0)&&(mem[q + 3].cint != 0))
            {
              print_err("Infinite glue shrinkage inserted from");
              print_esc("skip");
              print_int(n);
              help3("The correction glue for page breaking with insertions",
                  "must have finite shrinkability. But you may proceed,",
                  "since the offensive shrinkability has been made finite.");
              error();
            }
          }
          if (mem[r].hh.b0 == 1)
            insert_penalties = insert_penalties + mem[p + 1].cint;
          else
          {
            mem[r + 2].hh.v.RH = p;
            delta = page_so_far[0] - page_so_far[1] - page_so_far[7] + page_so_far[6];
            if (eqtb[(hash_size + 3218) + n].cint == 1000)
              h = mem[p + 3].cint;
            else
              h = x_over_n(mem[p + 3].cint, 1000)* eqtb[(hash_size + 3218) + n].cint;
            if (((h <= 0) || (h <= delta)) && (mem[p + 3].cint + mem[r + 3].cint <= eqtb[(hash_size + 3751) + n].cint))
            {
              page_so_far[0] = page_so_far[0]- h;
              mem[r + 3].cint = mem[r + 3].cint + mem[p + 3].cint;
            }
            else
            {
              if (eqtb[(hash_size + 3218) + n].cint <= 0)
                w = 1073741823L;  /* 2^30 - 1 */
              else
              {
                w = page_so_far[0] - page_so_far[1] - page_so_far[7];
                if (eqtb[(hash_size + 3218) + n].cint != 1000)
                  w = x_over_n(w, eqtb[(hash_size + 3218) + n].cint)* 1000;
              }
              if (w > eqtb[(hash_size + 3751) + n].cint - mem[r + 3].cint)
                w = eqtb[(hash_size + 3751) + n].cint - mem[r + 3].cint;
              q = vert_break(mem[p + 4].hh.v.LH, w, mem[p + 2].cint);
              mem[r + 3].cint = mem[r + 3].cint + best_height_plus_depth;
              ;
#ifdef STAT
              if (tracing_pages > 0)
              {
                begin_diagnostic();
                print_nl("%% split");
                print_int(n);
                print_string(" to");
                print_scaled(w);
                print_char(',');
                print_scaled(best_height_plus_depth);
                print_string(" p=");
                if (q == 0)    /* if q=null l.19614 */
                  print_int(-10000);
                else if (mem[q].hh.b0 == 12)
                  print_int(mem[q + 1].cint);
                else
                  print_char('0');
                end_diagnostic(false);
              }
#endif /* STAT */
              if (eqtb[(hash_size + 3218) + n].cint != 1000)
                best_height_plus_depth = x_over_n(best_height_plus_depth, 1000) * eqtb[(hash_size + 3218) + n].cint;
              page_so_far[0]= page_so_far[0]- best_height_plus_depth;
              mem[r].hh.b0 = 1;
              mem[r + 1].hh.v.RH = q;
              mem[r + 1].hh.v.LH = p;
              if (q == 0)
                insert_penalties = insert_penalties - 10000;
              else if (mem[q].hh.b0 == 12)
                insert_penalties = insert_penalties + mem[q + 1].cint;
            }
          }
          goto lab80;
        }
        break;
      default:
        {
          confusion("page");
          return;       // abort_flag set
        }
        break;
    }
    if (pi < 10000)/* pi may be used ... */
    {
      if (page_so_far[1] < page_so_far[0])
        if ((page_so_far[3] != 0) || (page_so_far[4] != 0) || (page_so_far[5] != 0))
          b = 0;
        else
          b = badness(page_so_far[0] - page_so_far[1], page_so_far[2]);
      else if (page_so_far[1]- page_so_far[0]> page_so_far[6])
        b = 1073741823L;  /* 2^30 - 1 */
      else
        b = badness(page_so_far[1]- page_so_far[0], page_so_far[6]);
      if (b < 1073741823L) /* 2^30 - 1 */
        if (pi <= -10000)
          c = pi; 
        else if (b < 10000)
          c = b + pi + insert_penalties;
        else
          c = 100000L;
      else
        c = b;
      if (insert_penalties >= 10000)
        c = 1073741823L;  /* 2^30 - 1 */
      ;
#ifdef STAT
      if (tracing_pages > 0)
      {
        begin_diagnostic();
        print_nl("%");
        print_string(" t=");
        print_totals();
        print_string(" g=");
        print_scaled(page_so_far[0]);
        print_string(" b=");
        if (b == 1073741823L) /* 2^30 - 1 */
          print_char('*');
        else
          print_int(b);
        print_string(" p=");
        print_int(pi);
        print_string(" c=");
        if (c == 1073741823L) /* 2^30 - 1 */
          print_char('*');
        else
          print_int(c);
        if (c <= least_page_cost)
          print_char('#');
        end_diagnostic(false);
      }
#endif /* STAT */
      if (c <= least_page_cost)
      {
        best_page_break = p;
        best_size = page_so_far[0];
        least_page_cost = c;
        r = mem[mem_top].hh.v.RH;
        while (r != mem_top)
        {
          mem[r + 2].hh.v.LH = mem[r + 2].hh.v.RH;
          r = mem[r].hh.v.RH;
        }
      }
      if ((c == 1073741823L) || (pi <= -10000))  /* 2^30 - 1 */
      {
        fire_up(p);
        if (output_active)
          return;
        goto lab30;
      }
    }
    if ((mem[p].hh.b0 < 10)||(mem[p].hh.b0 > 11))
      goto lab80; 
lab90:
    if (mem[p].hh.b0 == 11)
      q = p; 
    else
    {
      q = mem[p + 1].hh.v.LH;
      page_so_far[2 + mem[q].hh.b0] = page_so_far[2 + mem[q].hh.b0] + mem[q + 2].cint;
      page_so_far[6]= page_so_far[6]+ mem[q + 3].cint;
      if ((mem[q].hh.b1 != 0)&&(mem[q + 3].cint != 0))
      {
        print_err("Infinite glue shrinkage found on current page");
        help4("The page about to be output contains some infinitely",
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
    page_so_far[1] = page_so_far[1] + page_so_far[7] + mem[q + 1].cint;
    page_so_far[7] = 0;
lab80:
    if (page_so_far[7]> page_max_depth)
    {
      page_so_far[1] = page_so_far[1] + page_so_far[7] - page_max_depth;
      page_so_far[7] = page_max_depth;
    }
    mem[page_tail].hh.v.RH = p;
    page_tail = p;
    mem[contrib_head].hh.v.RH = mem[p].hh.v.RH;
    mem[p].hh.v.RH = 0;
    goto lab30;
lab31:
    mem[contrib_head].hh.v.RH = mem[p].hh.v.RH;
    mem[p].hh.v.RH = 0;
    flush_node_list(p);
lab30:; 
  } while (!(mem[contrib_head].hh.v.RH == 0));
  if (nest_ptr == 0)
    tail = contrib_head;
  else
    nest[0].tail_field = contrib_head;
} 
/* sec 1043 */
void app_space (void)
{
  halfword q;

  if ((space_factor >= 2000) && (xspace_skip != zero_glue))
    q = new_param_glue(xspace_skip_code);
  else
  {
    if (space_skip != zero_glue)
      main_p = space_skip;
    else
    {
      main_p = font_glue[cur_font];
      if (main_p == 0)
      {
        main_p = new_spec(zero_glue);
        main_k = param_base[cur_font] + 2;
        width(main_p) = font_info[main_k].cint;
        stretch(main_p) = font_info[main_k + 1].cint;
        shrink(main_p) = font_info[main_k + 2].cint;
        font_glue[cur_font] = main_p;
      }
    }
    main_p = new_spec(main_p);

    if (space_factor >= 2000)
      width(main_p) = width(main_p) + font_info[7 + param_base[eqtb[(hash_size + 1834)].hh.v.RH]].cint;
    stretch(main_p) = xn_over_d(stretch(main_p), space_factor, 1000);
    shrink(main_p) = xn_over_d(shrink(main_p), 1000, space_factor);
    q = new_glue(main_p);
    glue_ref_count(main_p) = 0;
  }
  link(tail) = q;
  tail = q;
}
/* called from tex8.c only */
/* sec 1047 */
void insert_dollar_sign (void)
{
  back_input();
  cur_tok = 804;
  print_err("Missing $ inserted");
  help2("I've inserted a begin-math/end-math symbol since I think",
      "you left one out. Proceed, with fingers crossed.");
  ins_error();
}
/* sec 1049 */
void you_cant (void)
{
  print_err("You can't use `");
  print_cmd_chr(cur_cmd, cur_chr);
  print_string("' in ");
  print_mode(mode);
}
/* sec 1050 */
void report_illegal_case (void)
{
  you_cant();
  help4("Sorry, but I'm not programmed to handle this case;",
      "I'll just pretend that you didn't ask for it.",
      "If you're in the wrong mode, you might be able to",
      "return to the right one by typing `I}' or `I$' or `I\\par'.");
  error();
}
/* sec 1051 */
bool privileged (void)
{
  if (mode > 0)
  {
    return true;
  }
  else
  {
    report_illegal_case();
    return false;
  }
}
/* sec 1054 */
bool its_all_over (void)
{
  register bool Result;

  if (privileged ())
  {
    if ((page_head == page_tail) && (head == cur_list.tail_field) && (dead_cycles == 0))
    {
      Result = true;
      return(Result);
    }
    back_input();
    tail_append(new_null_box());
    width(tail) = hsize;
    tail_append(new_glue(8));
    tail_append(new_penalty(-1073741824L));
    build_page();
  }
  Result = false;
  return Result;
}
/* sec 1060 */
void append_glue (void)
{
  small_number s;

  s = cur_chr;

  switch(s)
  {
    case 0:
      cur_val = 4;
      break;
    case 1:
      cur_val = 8;
      break;
    case 2:
      cur_val = 12;
      break;
    case 3:
      cur_val = 16;
      break;
    case 4:
      scan_glue(2);
      break;
    case 5:
      scan_glue(3);
      break;
  }

  tail_append(new_glue(cur_val));

  if (s >= 4)
  {
    decr(mem[cur_val].hh.v.RH);
    if (s > 4)
      mem[tail].hh.b1 = 99;
  }
}
/* sec 1061 */
void append_kern (void)
{ 
  quarterword s;

  s = cur_chr;

  scan_dimen(s == mu_glue, false, false);
  tail_append(new_kern(cur_val));
  subtype(tail) = s;
}
/* sec 1054 */
void off_save (void)
{
  halfword p;

  if (cur_group == 0)
  {
    print_err("Extra ");
    print_cmd_chr(cur_cmd, cur_chr);
    help1("Things are pretty mixed up, but I think the worst is over.");
    error();
  }
  else
  {
    back_input();
    p = get_avail();
    mem[temp_head].hh.v.RH = p;
    print_err("Missing ");

    switch (cur_group)
    {
      case 14:
        {
/*  mem[p].hh.v.LH = (hash_size + 4611);  */
/*  mem[p].hh.v.LH = (hash_size + 4095 + 516);  */
          mem[p].hh.v.LH = (hash_size + hash_extra + 4095 + 516); /* 96/Jan/10 */
          print_esc("endgroup");
        }
        break;
      case 15:
        {
          mem[p].hh.v.LH = 804;
          print_char('$');
        }
        break;
      case 16:
        {
/*  mem[p].hh.v.LH = (hash_size + 4612);  */
/*  mem[p].hh.v.LH = (hash_size + 4095 + 517);  */
          mem[p].hh.v.LH = (hash_size + hash_extra + 4095 + 517); /* 96/Jan/10 */
          mem[p].hh.v.RH = get_avail();
          p = mem[p].hh.v.RH;
          mem[p].hh.v.LH = 3118;
          print_esc("right.");
        }
        break;
      default:
        {
          mem[p].hh.v.LH = 637;
          print_char('}');
        }
        break;
    }
    print_string(" inserted");
    begin_token_list(mem[temp_head].hh.v.RH, 4);
    help5("I've inserted something that you may have forgotten.",
        "(See the <inserted text> above.)",
        "With luck, this will get me unwedged. But if you",
        "really didn't forget anything, try typing `2' now; then",
        "my insertion and my current dilemma will both disappear.");
    error();
  }
}
/* only called from tex8.c */
/* sec 1069 */
void extra_right_brace (void)
{
  print_err("Extra }, or forgotten");

  switch(cur_group)
  {
    case 14:
      print_esc("endgroup");
      break;
    case 15:
      print_char('$');
      break;
    case 16:
      print_esc("right");
      break;
  }
  help5("I've deleted a group-closing symbol because it seems to be",
      "spurious, as in `$x}$'. But perhaps the } is legitimate and",
      "you forgot something else, as in `\\hbox{$x}'. In such cases",
      "the way to recover is to insert both the forgotten and the",
      "deleted material, e.g., by typing `I$}'.");
  error();
  incr(align_state);
}
/* sec 1070 */
void normal_paragraph (void)
{
  if (looseness != 0)
    eq_word_define(int_base + looseness_code, 0);

  if (hang_indent != 0)
    eq_word_define(dimen_base + hang_indent_code, 0);

  if (hang_after != 1)
    eq_word_define(int_base + hang_after_code, 1);

  if (par_shape_ptr != 0)
    eq_define(par_shape_loc, shape_ref, 0);
}
/* sec 1075 */
void box_end_(integer boxcontext)
{
  halfword p;

  if (boxcontext < box_flag)
  {
    if (cur_box != 0)
    {
      shift_amount(cur_box) = boxcontext;
      if (abs(mode) == vmode)
      {
        append_to_vlist(cur_box);
        if (adjust_tail != 0)
        {
          if (adjust_head != adjust_tail)
          {
            link(tail) = link(adjust_head);
            tail = adjust_tail;
          }
          adjust_tail = 0;
        }

        if (mode > 0)
          build_page();
      }
      else
      {
        if (abs(mode) == hmode)
          space_factor = 1000;
        else
        {
          p = new_noad();
          math_type(nucleus(p)) = sub_box;
          info(nucleus(p)) = cur_box;
          cur_box = p;
        }
        link(tail) = cur_box;
        tail = cur_box;
      }
    }
  }
  else if (boxcontext < ship_out_flag)
    if (boxcontext < (box_flag + 256))
      eq_define((box_base - box_flag) + boxcontext, box_ref, cur_box);
    else
      geq_define((box_base - box_flag - 256) + boxcontext, box_ref, cur_box);
  else if (cur_box != 0)
    if (boxcontext > (ship_out_flag))
    {
      do
        {
          get_x_token();
        }
      while(!((cur_cmd != spacer) && (cur_cmd != relax)));

      if (((cur_cmd == hskip) && (abs(mode)!= vmode)) || ((cur_cmd == vskip) && (abs(mode) == vmode)))
      {
        append_glue();
        subtype(tail) = boxcontext - (leader_flag - a_leaders);
        leader_ptr(tail) = cur_box;
      }
      else
      {
        print_err("Leaders not followed by proper glue");
        help3("You should say `\\leaders <box or rule><hskip or vskip>'.",
            "I found the <box or rule>, but there's no suitable",
            "<hskip or vskip>, so I'm ignoring these leaders.");
        back_error();
        flush_node_list(cur_box);
      }
    }
    else
      ship_out(cur_box);
}
/* called only from tex8.c */
/* sec 1079 */
void begin_box_(integer boxcontext)
{
  halfword p, q;
  quarterword m;
  halfword k;
  eight_bits n;

  switch(cur_chr)
  {
    case box_code:
      {
        scan_eight_bit_int();
        cur_box = box(cur_val);
        box(cur_val) = 0;
      }
      break;
    case copy_code:
      {
        scan_eight_bit_int();
        cur_box = copy_node_list(box(cur_val));
      }
      break;
    case last_box_code:
      {
        cur_box = 0;

        if (abs(mode) == mmode)
        {
          you_cant();
          help1("Sorry; this \\lastbox will be void.");
          error();
        }
        else if ((mode == vmode) && (head == cur_list.tail_field))
        {
          you_cant();
          help2("Sorry...I usually can't take things from the current page.",
              "This \\lastbox will therefore be void.");
          error();
        }
        else
        {
          if (!(tail >= hi_mem_min))
            if ((type(tail) == hlist_node) || (type(tail) == vlist_node))
            {
              q = head;
              do
                {
                  p = q;
                  if (!(q >= hi_mem_min))
                    if (type(q) == disc_node)
                    {
                      for (m = 1; m <= replace_count(q); m++)
                        p = link(p);

                      if (p == tail)
                        goto lab30;
                    }
                  q = link(p);
                }
              while (!(q == tail));
              cur_box = tail;
              shift_amount(cur_box) = 0;
              tail = p;
              link(p) = 0;
lab30:
              ;
            }
        }
      }
      break;
    case vsplit_code:
      {
        scan_eight_bit_int();
        n = cur_val;
        if (!scan_keyword("to"))
        {
          print_err("Missing `to' inserted");
          help2("I'm working on `\\vsplit<box number> to <dimen>';",
              "will look for the <dimen> next.");
          error();
        }
        scan_dimen(false, false, false);
        cur_box = vsplit(n, cur_val);
      }
      break;
    default:
      {
        k = cur_chr - vtop_code;
        save_stack[save_ptr + 0].cint = boxcontext;

        if (k == hmode)
          if ((boxcontext < box_flag) && (abs(mode) == vmode))
            scan_spec(adjust_hbox_group, true);
          else
            scan_spec(hbox_group, true);
        else
        {
          if (k == vmode)
            scan_spec(vbox_group, true);
          else
          {
            scan_spec(vtop_group, true);
            k = vmode;
          }
          normal_paragraph();
        }
        push_nest();
        mode = - (integer) k;

        if (k == vmode)
        {
          prev_depth = ignore_depth;

          if (every_vbox != 0)
            begin_token_list(every_vbox, every_vbox_text);
        }
        else
        {
          space_factor = 1000;

          if (every_hbox != 0)
            begin_token_list(every_hbox, every_vbox_text);
        }
        return;
      }
      break;
  }
  box_end(boxcontext);
}
/* sec 1084 */
void scan_box_(integer boxcontext)
{
  do
    {
      get_x_token(); 
    }
  while (!((cur_cmd != spacer) && (cur_cmd != relax)));

  if (cur_cmd == make_box)
  {
    begin_box(boxcontext);
  }
  else if ((boxcontext >= leader_flag) && ((cur_cmd == hrule) || (cur_cmd == vrule)))
  {
    cur_box = scan_rule_spec();
    box_end(boxcontext);
  }
  else
  {
    print_err("A <box> was supposed to be here");
    help3("I was expecting to see \\hbox or \\vbox or \\copy or \\box or",
        "something like that. So you might find something missing in",
        "your output. But keep trying; you can fix this later.");
    back_error();
  }
}
/****************************************************************************/
void package_ (small_number);
/****************************************************************************/
/* sec 1091 */
small_number norm_min_ (integer h)
{
  if (h <= 0)
    return 1;
  else if (h >= 63)
    return 63;
  else
    return h;
}
/* sec 1091 */
void new_graf_(bool indented)
{
  prev_graf = 0;

  if ((mode == vmode) || (head != tail))
    tail_append(new_param_glue(par_skip_code));

  push_nest();
  mode = hmode;
  space_factor = 1000;
  set_cur_lang();
  clang = cur_lang;
  prev_graf =(norm_min(left_hyphen_min) * 64 + norm_min(right_hyphen_min)) * 65536L + cur_lang;

  if (indented)
  {
    tail = new_null_box();
    link(head) = tail;
    width(tail) = par_indent;
  }

  if (every_par != 0)
    begin_token_list(every_par, every_par_text);

  if (nest_ptr == 1)
    build_page();
}
/* procedure indent_in_hmode; l.21058 */
/* sec 1093 */
void indent_in_hmode (void)
{
  halfword p, q;

  if (cur_chr > 0)
  {
    p = new_null_box();
    width(p) = par_indent;

    if (abs(mode) == hmode)
      space_factor = 1000;
    else
    {
      q = new_noad();
      math_type(nucleus(q)) = sub_box;
      info(nucleus(q)) = p;
      p = q;
    }
    tail_append(p);
  }
}
/* only called from tex8.c */
/* sec 1095 */
void head_for_vmode (void)
{
  if (mode < 0)
    if (cur_cmd != hrule)
      off_save();
    else
    {
      print_err("You can't use `");
      print_esc("hrule");
      print_string("' here except with leaders");
      help2("To put a horizontal rule in an hbox or an alignment,",
          "you should use \\leaders or \\hrulefill (see The TeXbook).");
      error();
    }
  else
  {
    back_input();
    cur_tok = par_token;
    back_input();
    cur_input.index_field = inserted;
  }
}
/* sec 1096 */
void end_graf (void)
{
  if (mode == hmode)
  {
    if (head == tail)
      pop_nest();
    else
      line_break(widow_penalty);

    normal_paragraph();
    error_count = 0;
  }
}
/* only called form tex8.c */
/* sec 1099 */
void begin_insert_or_adjust (void)
{
  if (cur_cmd == vadjust)
    cur_val = 255;
  else
  {
    scan_eight_bit_int();

    if (cur_val == 255)
    {
      print_err("You can't ");
      print_esc("insert");
      print_int(255);
      help1("I'm changing to \\insert0; box 255 is special.");
      error();
      cur_val = 0;
    }
  }
  save_stack[save_ptr + 0].cint = cur_val;
  incr(save_ptr);
  new_save_level(insert_group);
  scan_left_brace();
  normal_paragraph();
  push_nest();
  mode = -vmode;
  prev_depth = ignore_depth;
}
/* sec 1101 */
void make_mark (void)
{
  halfword p;
  p = scan_toks(false, true);
  p = get_node(small_node_size);
  type(p) = mark_node;
  subtype(p) = 0;
  mark_ptr(p) = def_ref;
  link(tail) = p;
  tail = p;
}
/* sec 1103 */
void append_penalty (void)
{
  scan_int();
  tail_append(new_penalty(cur_val));

  if (mode == vmode)
    build_page();
}
/* only called from tex8.c */
/* sec 1105 */
void delete_last (void)
{
  halfword p, q;
  quarterword m;

  if ((mode == vmode) && (tail == head))
  {
    if ((cur_chr != glue_node) || (last_glue != empty_flag))
    {
      you_cant();
      help2("Sorry...I usually can't take things from the current page.",
          "Try `I\\vskip-\\lastskip' instead.");

      if (cur_chr == kern_node)
        help_line[0] = "Try `I\\kern-\\last_kern' instead.";
      else if (cur_chr != glue_node)
        help_line[0] = "Perhaps you can make the output routine do it.";
      error();
    }
  }
  else
  {
    if (!(tail >= hi_mem_min))
      if (type(tail) == cur_chr)
      {
        q = head;

        do
          {
            p = q;

            if (!(q >= hi_mem_min))
              if (type(q) == disc_node)
              {
                for (m = 1; m <= replace_count(q); m++)
                  p = link(p);

                if (p == tail)
                  return;
              }
            q = link(p);
          }
        while (!(q == tail));
        link(p) = 0;
        flush_node_list(tail);
        tail = p;
      }
  }
}
/* only called from tex8.c */
/* procedure unpackage; l.21256 */
/* sec 1110 */
void unpackage (void)
{
  halfword p;
  char c;

  c = cur_chr;
  scan_eight_bit_int();
  p = box(cur_val);

  if (p == 0)
    return; 
  if ((abs(mode) == mmode) || ((abs(mode) == vmode) && (type(p) != vlist_node)) ||
    ((abs(mode) == hmode) && (type(p) != hlist_node)))
  {
    print_err("Incompatible list can't be unboxed");
    help3("Sorry, Pandora. (You sneaky devil.)",
        "I refuse to unbox an \\hbox in vertical mode or vice versa.",
        "And I can't open any boxes in math mode.");
    error();
    return;
  }

  if (c == copy_code)
    link(tail) = copy_node_list(list_ptr(p));
  else
  {
    link(tail) = list_ptr(p);
    box(cur_val) = 0;
    free_node(p, box_node_size);
  }

  while (link(tail) != 0)
    tail = link(tail);
}
/* sec 1113 */
void append_italic_correction (void)
{
  halfword p;
  internal_font_number f;

  if (tail != head)
  {
    if ((tail >= hi_mem_min))
      p = tail;
    else if (type(tail) == ligature_node)
      p = tail + 1;
    else
      return;
    f = font(p);
    tail_append(new_kern(font_info[italic_base[f] + (font_info[char_base[f] + mem[p].hh.b1].qqqq.b2) / 4].cint));
    subtype(tail) = explicit;
  }
}
/* sec 1117 */
void append_discretionary (void)
{
  integer c;

  tail_append(new_disc());

  if (cur_chr == 1)
  {
    c = hyphen_char[cur_font];
    if (c >= 0)
      if (c < 256)
        pre_break(tail) = new_character(cur_font, c);
  }
  else
  {
    incr(save_ptr);
    save_stack[save_ptr - 1].cint = 0;
    new_save_level(disc_group);
    scan_left_brace();
    push_nest();
    mode = -102;
    space_factor = 1000;
  }
}
/* only called form tex8.c */
/* sec 1119 */
void build_discretionary (void)
{
  halfword p, q;
  integer n;

  unsave();
  q = head;
  p = mem[q].hh.v.RH;
  n = 0;

  while (p != 0)
  {
    if (!(p >= hi_mem_min))
      if (mem[p].hh.b0 > 2)
        if (mem[p].hh.b0 != 11)
          if (mem[p].hh.b0 != 6)
          {
            print_err("Improper discretionary list");
            help1("Discretionary lists must contain only boxes and kerns.");
            error();
            begin_diagnostic();
            print_nl("The following discretionary sublist has been deleted:");
            show_box(p);
            end_diagnostic(true);
            flush_node_list(p);
            mem[q].hh.v.RH = 0;
            goto lab30;
          }
    q = p;
    p = mem[q].hh.v.RH;
    incr(n);
  }
lab30:
  ;
  p = mem[head].hh.v.RH;
  pop_nest();

  switch (save_stack[save_ptr - 1].cint)
  {
    case 0:
      mem[tail + 1].hh.v.LH = p;
      break;
    case 1:
      mem[tail + 1].hh.v.RH = p;
      break;
    case 2:
      {
        if ((n > 0) && (abs(mode)== 203))
        {
          print_err("Illegal math ");
          print_esc("discretionary");
          help2("Sorry: The third part of a discretionary break must be",
              "empty, in math formulas. I had to delete your third part.");
          flush_node_list(p);
          n = 0;
          error();
        }
        else
          mem[tail].hh.v.RH = p;
/* if n <= max_quarterword then replace_count(tail) <- n; p.1120 */
/*      if (n <= 255) */       /* 94/Apr/4 ? */
        if (n <= max_quarterword)     /* 96/Oct/12 ??? */
          mem[tail].hh.b1 = n;
        else
        {
          print_err("Discretionary list is too long");
          help2("Wow---I never thought anybody would tweak me here.",
              "You can't seriously need such a huge discretionary list?");
          error();
        }
        if (n > 0)
          tail = q;
        decr(save_ptr);
        return;
      }
      break;
  }
  incr(save_stack[save_ptr - 1].cint);
  new_save_level(10);
  scan_left_brace();
  push_nest();
  mode = -102;
  space_factor = 1000;
}
/* called only from tex8.c */
/* sec 1123 */
void make_accent (void)
{
  real s, t;
  halfword p, q, r;
  internal_font_number f;
  scaled a, h, x, w, delta;
  ffourquarters i;

  scan_char_num();
  f = cur_font;
  p = new_character(f, cur_val);

  if (p != 0)
  {
    x = x_height(f);
    s = slant(f) / ((double) 65536.0);
    a = char_width(f, char_info(f, character(p)));
    do_assignments();
    q = 0;
    f = cur_font;

    if ((cur_cmd == letter) || (cur_cmd == other_char) || (cur_cmd == char_given))
      q = new_character(f, cur_chr);
    else if (cur_cmd == char_num)
    {
      scan_char_num();
      q = new_character(f, cur_val);
    }
    else
      back_input();

    if (q != 0)
    {
      t = slant(f) / ((double) 65536.0);
      i = char_info(f, character(q));
      w = char_width(f, i);
      h = char_height(f, height_depth(i));

      if (h != x)
      {
        p = hpack(p, 0, 1);
        shift_amount(p) = x - h;
      }
      delta = round((w - a) / ((double) 2.0)+ h * t - x * s);
      r = new_kern(delta);
      subtype(r) = acc_kern;
      link(tail) = r;
      link(r) = p;
      tail = new_kern(- (integer) a - delta);
      subtype(tail) = acc_kern;
      link(p) = tail;
      p = q;
    }
    link(tail) = p;
    tail = p;
    space_factor = 1000;
  }
}
/* sec 1127 */
void align_error (void)
{
  if (abs(align_state) > 2)
  {
    print_err("Misplaced ");
    print_cmd_chr(cur_cmd, cur_chr);
    if (cur_tok == 1062)
    {
      help6("I can't figure out why you would want to use a tab mark",
          "here. If you just want an ampersand, the remedy is",
          "simple: Just type `I\\&' now. But if some right brace",
          "up above has ended a previous alignment prematurely,",
          "you're probably due for more error messages, and you",
          "might try typing `S' now just to see what is salvageable.");
    }
    else
    {
      help5("I can't figure out why you would want to use a tab mark",
          "or \\cr or \\span just now. If something like a right brace",
          "up above has ended a previous alignment prematurely,",
          "you're probably due for more error messages, and you",
          "might try typing `S' now just to see what is salvageable.");
    }
    error();
  }
  else
  {
    back_input();

    if (align_state < 0)
    {
      print_err("Missing { inserted");
      incr(align_state);
      cur_tok = 379;    /* belowdisplayshortskip ? */
    }
    else
    {
      print_err("Missing } inserted");
      decr(align_state);
      cur_tok = 637;
    }
    help3("I've put in what seems to be necessary to fix",
        "the current column of the current alignment.",
        "Try to go on, since this might almost work.");
    ins_error();
  }
}
/* sec 1129 */
void noalign_error (void)
{
  print_err("Misplaced ");
  print_esc("noalign");
  help2("I expect to see \\noalign only after the \\cr of",
      "an alignment. Proceed, and I'll ignore this case.");
  error();
}
/* only called from tex8.c */
/* sec 1129 */
void omit_error (void)
{
  print_err("Misplaced ");
  print_esc("omit");
  help2("I expect to see \\omit only after tab marks or the \\cr of",
      "an alignment. Proceed, and I'll ignore this case.");
  error();
}
/* sec 1131 */
void do_endv (void)
{
  if (cur_group == 6)
  {
    end_graf();

    if (fin_col ())
      fin_row();
  }
  else
    off_save();
}
/* only called form tex8.c */
/* sec 1135 */
void cs_error (void)
{
  print_err("Extra ");
  print_esc("endcsname");
  help1("I'm ignoring this, since I wasn't doing a \\csname."); 
  error();
}
/* sec 1136 */
void push_math_(group_code c)
{
  push_nest();
  mode = -203;
  cur_list.aux_field.cint = 0;
  new_save_level(c);
}
/* sec 1138 */
void init_math (void)
{
  scaled w;
  scaled l;
  scaled s;
  halfword p;
  halfword q;
  internal_font_number f;
  integer n;
  scaled v;
  scaled d;

  get_token();

  if ((cur_cmd == 3) && (mode > 0))
  {
    if (head == tail)
    {
      pop_nest();
      w = -1073741823L; /* - (2^30 - 1) */
    }
    else
    {
      line_break(display_widow_penalty);
      v = mem[just_box + 4].cint + 2 * font_info[6 + param_base[eqtb[(hash_size + 1834)].hh.v.RH]].cint;
      w = -1073741823L;  /* - (2^30 - 1) */
      p = mem[just_box + 5].hh.v.RH;
      while (p != 0)
      {
lab21:
        if ((p >= hi_mem_min))
        {
          f = mem[p].hh.b0;
          d = font_info[width_base[f] + font_info[char_base[f] + mem[p].hh.b1].qqqq.b0].cint;
          goto lab40;
        }
        switch (mem[p].hh.b0)
        {
          case 0:
          case 1:
          case 2:
            {
              d = mem[p + 1].cint;
              goto lab40;
            }
            break;
          case 6:
            {
              mem[lig_trick]= mem[p + 1];
              mem[lig_trick].hh.v.RH = mem[p].hh.v.RH;
              p = lig_trick;
              goto lab21;
            }
            break;
          case 11:
          case 9:
            d = mem[p + 1].cint;
            break;
          case 10:
            {
              q = mem[p + 1].hh.v.LH;
              d = mem[q + 1].cint;
              if (mem[just_box + 5].hh.b0 == 1)
              {
                if ((mem[just_box + 5].hh.b1 == mem[q].hh.b0) && (mem[q + 2].cint != 0))
                  v = 1073741823L;  /* - (2^30 - 1) */
              }
              else if (mem[just_box + 5].hh.b0 == 2)
              {
                if ((mem[just_box + 5].hh.b1 == mem[q].hh.b1) && (mem[q + 3].cint != 0))
                  v = 1073741823L;  /* - (2^30 - 1) */
              }
              if (mem[p].hh.b1 >= 100)
                goto lab40;
            }
            break;
          case 8:
            d = 0;
            break;
          default:
            d = 0;
            break;
        }
        if (v < 1073741823L) /* - (2^30 - 1) */
          v = v + d;
        goto lab45;
lab40:
        if (v < 1073741823L) /* - (2^30 - 1) */
        {
          v = v + d;
          w = v;
        }
        else
        {
          w = 1073741823L;  /* - (2^30 - 1) */
          goto lab30;
        }
lab45:
        p = mem[p].hh.v.RH;
      }
lab30:
      ;
    }
    if (par_shape_ptr == 0)
      if ((hang_indent != 0) && (((hang_after >= 0) && (prev_graf + 2 > hang_after)) || (prev_graf + 1 < - (integer) hang_after)))
      {
        l = hsize - abs(hang_indent);
        if (hang_indent > 0)
          s = hang_indent;
        else
          s = 0;
      }
      else
      {
        l = hsize;
        s = 0;
      }
    else
    {
      n = mem[par_shape_ptr].hh.v.LH;
      if (prev_graf + 2 >= n)
        p = par_shape_ptr + 2 * n;
      else
        p = par_shape_ptr + 2 *(prev_graf + 2);
      s = mem[p - 1].cint;
      l = mem[p].cint;
    }
    push_math(15);
    mode = 203;
    eq_word_define((hash_size + 3207), -1);
    eq_word_define((hash_size + 3743), w);
    eq_word_define((hash_size + 3744), l);
    eq_word_define((hash_size + 3745), s);

    if (every_display != 0)/* everydisplay */
      begin_token_list(every_display, 9);

    if (nest_ptr == 1)
    {
      build_page();
    }
  }
  else
  {
    back_input();
    {
      push_math(15);
      eq_word_define((hash_size + 3207), -1);
      if (every_math != 0)/* everymath */
        begin_token_list(every_math, 8);
    }
  }
}
/* sec 1142 */
void start_eq_no (void)
{
  save_stack[save_ptr + 0].cint = cur_chr;
  incr(save_ptr);
  {
    push_math(math_shift_group);
    eq_word_define(int_base + cur_fam_code, -1);

    if (every_math != 0)
      begin_token_list(every_math, every_math_text);
  }
}
/* sec 1151 */
void scan_math_(halfword p)
{
  integer c;

lab20:
  do
    {
      get_x_token();
    }
  while(!((cur_cmd != spacer) && (cur_cmd != relax)));

lab21:
  switch (cur_cmd)
  {
    case letter:
    case other_char:
    case char_given:
      {
        c = math_code(cur_chr);

        if (c == 32768L)
        {
          {
            cur_cs = cur_chr + active_base;
            cur_cmd = eq_type(cur_cs);
            cur_chr = equiv(cur_cs);
            x_token();
            back_input();
          }
          goto lab20;
        }
      }
      break;
    case char_num:
      {
        scan_char_num();
        cur_chr = cur_val;
        cur_cmd = char_given;
        goto lab21;
      }
      break;
    case math_char_num:
      {
        scan_fifteen_bit_int();
        c = cur_val;
      }
      break;
    case math_given:
      c = cur_chr;
      break;
    case delim_num:
      {
        scan_twenty_seven_bit_int();
        c = cur_val / 4096;
      }
      break;
    default:
      {
        back_input();
        scan_left_brace();
        save_stack[save_ptr + 0].cint = p;
        incr(save_ptr);
        push_math(math_group);
        return;
      }
      break;
  }
  math_type(p) = math_char;
  character(p) = c % 256;

  if ((c >= var_code) && ((cur_fam >= 0) && (cur_fam < 16)))
    fam(p) = cur_fam;
  else
    fam(p) = (c / 256) % 16;
}
/* sec 1155 */
void set_math_char_(integer c)
{
  halfword p;

  if (c >= 32768L)
  {
    cur_cs = cur_chr + active_base;
    cur_cmd = eq_type(cur_cs);
    cur_chr = equiv(cur_cs);
    x_token();
    back_input();
  }
  else
  {
    p = new_noad();
    math_type(nucleus(p)) = math_char;
    character(nucleus(p)) = c % 256;
    fam(nucleus(p)) = (c / 256) % 16;

    if (c >= var_code)
    {
      if (((cur_fam >= 0) && (cur_fam < 16)))
        fam(nucleus(p)) = cur_fam;

      type(p) = ord_noad;
    }
    else
      type(p) = ord_noad + (c / 4096);

    link(tail) = p;
    tail = p;
  }
}
/* sec 1159 */
void math_limit_switch (void)
{
  if (head != tail)
    if (mem[tail].hh.b0 == op_noad)
    {
      subtype(tail) = cur_chr;
      return;
    }

  print_err("Limit controls must follow a math operator");
  help1("I'm ignoring this misplaced \\limits or \\nolimits command.");
  error();
}
/* sec 1160 */
void scan_delimiter_(halfword p, bool r)
{
   if (r)
   {
     scan_twenty_seven_bit_int();
   }
   else
   {
     do
      {
        get_x_token();
      }
     while (!((cur_cmd != spacer) && (cur_cmd != relax)));

     switch (cur_cmd)
     {
       case letter:
       case other_char:
         cur_val = del_code(cur_chr);
         break;
       case delim_num:
         scan_twenty_seven_bit_int();
         break;
       default:
         cur_val = -1;
         break;
     }
   }

   if (cur_val < 0)
   {
     print_err("Missing delimiter (. inserted)");
     help6("I was expecting to see something like `(' or `\\{' or",
         "`\\}' here. If you typed, e.g., `{' instead of `\\{', you",
         "should probably delete the `{' by typing `1' now, so that",
         "braces don't get unbalanced. Otherwise just proceed.",
         "Acceptable delimiters are characters whose \\delcode is",
         "nonnegative, or you can use `\\delimiter <delimiter code>'.");
     back_error();
     cur_val = 0;
   }

   small_fam(p) = (cur_val / 1048576L) % 16;
   small_char(p) = (cur_val / 4096) % 256;
   large_fam(p) = (cur_val / 256) % 16;
   large_char(p) = cur_val % 256;
}
/* sec 1163 */
void math_radical (void)
{
  tail_append(get_node(radical_noad_size));
  type(tail) = radical_noad;
  subtype(tail) = normal;
  mem[nucleus(tail)].hh = empty_field;
  mem[subscr(tail)].hh = empty_field;
  mem[supscr(tail)].hh = empty_field;
  scan_delimiter(left_delimiter(tail), true);
  scan_math(nucleus(tail));
}
/* sec 1165 */
void math_ac (void)
{
  if (cur_cmd == accent)
  {
    print_err("Please use ");
    print_esc("mathaccent");
    print_string(" for accents in math mode");
    help2("I'm changing \\accent to \\mathaccent here; wish me luck.",
      "(Accents are not the same in formulas as they are in text.)");
    error();
  }

  tail_append(get_node(5));
  type(tail) = accent_noad;
  subtype(tail) = normal;
  mem[nucleus(tail)].hh = empty_field;
  mem[subscr(tail)].hh = empty_field;
  mem[supscr(tail)].hh = empty_field;
  math_type(accent_chr(tail)) = math_char;
  scan_fifteen_bit_int();
  character(accent_chr(tail)) = cur_val % 256;

  if ((cur_val >= var_code) && ((cur_fam >= 0) && (cur_fam < 16)))
    fam(accent_chr(tail)) = cur_fam;
  else
    fam(accent_chr(tail)) = (cur_val / 256) % 16;

  scan_math(nucleus(tail));
}
/* sec 1172 */
void append_choices (void)
{
  tail_append(new_choice());
  incr(save_ptr);
  save_stack[save_ptr - 1].cint = 0;
  push_math(math_choice_group);
  scan_left_brace();
}
/* sec 1184 */
halfword fin_mlist_(halfword p)
{
  register halfword Result;
  halfword q;

  if (incompleat_noad != 0)
  {
    math_type(denominator(incompleat_noad)) = sub_mlist;
    info(denominator(incompleat_noad)) = link(head);

    if (p == 0)
      q = incompleat_noad;
    else
    {
      q = info(numerator(incompleat_noad));

      if (type(q) != left_noad)
      {
        confusion("right");
        return 0;       // abort_flag set
      }

      info(numerator(incompleat_noad)) = link(q);
      link(q) = incompleat_noad;
      link(incompleat_noad) = p;
    }
  }
  else
  {
    link(tail) = p;
    q = link(head);
  }
  pop_nest();
  Result = q;
  return Result;
}
/* sec 1174 */
void build_choices (void)
{
  halfword p;

  unsave();
  p = fin_mlist(0);

  switch (save_stack[save_ptr - 1].cint)
  {
    case 0:
      display_mlist(tail) = p;
      break;
    case 1:
      text_mlist(tail) = p;
      break;
    case 2:
      script_mlist(tail) = p;
      break;
    case 3:
      {
        script_script_mlist(tail) = p;
        decr(save_ptr);
        return;
      }
      break;
  }
  incr(save_stack[save_ptr - 1].cint);
  push_math(math_choice_group);
  scan_left_brace();
}
/* sec 1176 */
void sub_sup (void)
{
/*  small_number t; */
  int t;              /* 95/Jan/7 */
  halfword p;

  t = 0;
  p = 0;

  if (tail != head)
    if ((mem[tail].hh.b0 >= 16) && (mem[tail].hh.b0 < 30))
    {
      p = supscr(tail) + cur_cmd - sup_mark;
      t = math_type(p);
    }

  if ((p == 0) || (t != 0))
  {
    tail_append(new_noad());
    p = supscr(tail) + cur_cmd - sup_mark;

    if (t != 0)
    {
      if (cur_cmd == sup_mark)
      {
        print_err("Double superscript");
        help1("I treat `x^1^2' essentially like `x^1{}^2'.");
      }
      else
      {
        print_err("Double subscript");
        help1("I treat `x_1_2' essentially like `x_1{}_2'.");
      }
      error();
    }
  }
  scan_math(p);
}
/* used to continue here with math_fraction etc in tex7.c */
/*****************************************************************************/
/* moved down here to avoid pragma optimize questions 96/Sep/12 */
/* sec 1086 */
void package_(small_number c)
{
  scaled h;
  halfword p;
  scaled d;

  d = box_max_depth;
  unsave();
  save_ptr = save_ptr - 3;

  if (mode == -102)
    cur_box = hpack(link(head), save_stack[save_ptr + 2].cint, save_stack[save_ptr + 1].cint);
  else
  {
    cur_box = vpackage(link(head), save_stack[save_ptr + 2].cint, save_stack[save_ptr + 1].cint, d);

    if (c == vtop_code)
    {
      h = 0;
      p = list_ptr(cur_box);

      if (p != 0)
        if (type(p) <= rule_node)
          h = height(p);

      depth(cur_box) = depth(cur_box) - h + height(cur_box);
      height(cur_box) = h;
    }
  }
  pop_nest();
  box_end(save_stack[save_ptr + 0].cint);
}
#pragma optimize ("", on)           /* 96/Sep/12 */