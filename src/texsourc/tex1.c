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

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void show_box_(halfword p)
{
  depth_threshold = eqtb[(hash_size + 3188)].cint;
  breadth_max = eqtb[(hash_size + 3187)].cint;
  if (breadth_max <= 0)
    breadth_max = 5; 
#ifdef ALLOCATESTRING
  if (pool_ptr + depth_threshold >= current_pool_size)
    str_pool = realloc_str_pool (increment_pool_size);
  if (pool_ptr + depth_threshold >= current_pool_size)
    depth_threshold = current_pool_size - pool_ptr - 1;
#else
  if (pool_ptr + depth_threshold >= pool_size)
    depth_threshold = pool_size - pool_ptr - 1;
#endif
  show_node_list(p);
  print_ln();
} 
void delete_token_ref_(halfword p)
{
  if (mem[p].hh.v.LH == 0)
    flush_list(p);
  else
    decr(mem[p].hh.v.LH);
}
void delete_glue_ref_(halfword p)
{
  if (mem[p].hh.v.RH == 0)
    free_node(p, 4);
  else
    decr(mem[p].hh.v.RH);
}
void flush_node_list_(halfword p)
{
  halfword q; 
  while (p != 0) {      /* while p<>null */
    q = mem[p].hh.v.RH;
    if ((p >= hi_mem_min))
    {
      mem[p].hh.v.RH = avail;
      avail = p;
      ;
#ifdef STAT
      decr(dyn_used);
#endif /* STAT */
    } else {
      switch (mem[p].hh.b0)
      {
        case 0:
        case 1:
        case 13:
          {
            flush_node_list(mem[p + 5].hh.v.RH);
            free_node(p, 7);
            goto lab30;
          }
          break;
        case 2:
          {
            free_node(p, 4);
            goto lab30;
          }
          break;
        case 3:
          {
            flush_node_list(mem[p + 4].hh.v.LH);
            delete_glue_ref(mem[p + 4].hh.v.RH);
            free_node(p, 5);
            goto lab30;
          }
          break;
        case 8:
          {
            switch (mem[p].hh.b1)
            {
              case 0:
                free_node(p, 3);
                break;
              case 1:
              case 3:
                {
                  delete_token_ref(mem[p + 1].hh.v.RH);
                  free_node(p, 2);
                  goto lab30;
                }
                break;
              case 2:
              case 4:
                free_node(p, 2);
                break;
              default:
                {
                  confusion("ext3");
                  return;         // abort_flag set
                }
                break;
            }
            goto lab30;
          }
          break;
        case 10:
          {
            {
              if (mem[mem[p + 1].hh.v.LH].hh.v.RH == 0)
                free_node(mem[p + 1].hh.v.LH, 4);
              else
                decr(mem[mem[p + 1].hh.v.LH].hh.v.RH);
            }
/*     if leader_ptr(p)<>null then flush_node_list(leader_ptr(p)); */
            if (mem[p + 1].hh.v.RH != 0)
              flush_node_list(mem[p + 1].hh.v.RH);
          }
          break;
        case 11:
        case 9:
        case 12:
          ;
          break;
        case 6:
          flush_node_list(mem[p + 1].hh.v.RH);
          break;
        case 4:
          delete_token_ref(mem[p + 1].cint);
          break;
        case 7:
          {
            flush_node_list(mem[p + 1].hh.v.LH);
            flush_node_list(mem[p + 1].hh.v.RH);
          }
          break;
        case 5:
          flush_node_list(mem[p + 1].cint);
          break;
        case 14:
          {
            free_node(p, 3);
            goto lab30;
          }
          break;
        case 15 :
          {
            flush_node_list(mem[p + 1].hh.v.LH);
            flush_node_list(mem[p + 1].hh.v.RH);
            flush_node_list(mem[p + 2].hh.v.LH);
            flush_node_list(mem[p + 2].hh.v.RH);
            free_node(p, 3);
            goto lab30;
          }
          break;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 27:
        case 26:
        case 29:
        case 28:
          {
            if (mem[p + 1].hh.v.RH >= 2)
              flush_node_list(mem[p + 1].hh.v.LH);
            if (mem[p + 2].hh.v.RH >= 2)
              flush_node_list(mem[p + 2].hh.v.LH);
            if (mem[p + 3].hh.v.RH >= 2)
              flush_node_list(mem[p + 3].hh.v.LH);
            if (mem[p].hh.b0 == 24)
              free_node(p, 5);
            else if (mem[p].hh.b0 == 28)
              free_node(p, 5);
            else
              free_node(p, 4);
            goto lab30;
          }
          break;
        case 30:
        case 31:
          {
            free_node(p, 4);
            goto lab30;
          }
          break;
        case 25:
          {
            flush_node_list(mem[p + 2].hh.v.LH);
            flush_node_list(mem[p + 3].hh.v.LH);
            free_node(p, 6);
            goto lab30;
          }
          break;
        default:
          {
            confusion("flushing");
            return;         // abort_flag set
          }
          break;
      }
      free_node(p, 2);
lab30:;
    }
    p = q;
  }
}
halfword copy_node_list_(halfword p)
{
  register halfword Result;
  halfword h;
  halfword q;
  halfword r;
  char words;
  h = get_avail ();
  q = h;
  while (p != 0) {      /* while p<>null do l.3969 */
    words = 1;
    if ((p >= hi_mem_min)) 
      r = get_avail ();
    else switch (mem[p].hh.b0)
    {
      case 0:
      case 1:
      case 13:
        {
          r = get_node(7);
          mem[r + 6]= mem[p + 6];
          mem[r + 5]= mem[p + 5];
          mem[r + 5].hh.v.RH = copy_node_list(mem[p + 5].hh.v.RH);
          words = 5;
        }
        break;
      case 2:
        {
          r = get_node(4);
          words = 4;
        }
        break;
      case 3:
        {
          r = get_node(5);
          mem[r + 4]= mem[p + 4];
          incr(mem[mem[p + 4].hh.v.RH].hh.v.RH);
          mem[r + 4].hh.v.LH = copy_node_list(mem[p + 4].hh.v.LH);
          words = 4;
        }
        break;
      case 8:
        switch (mem[p].hh.b1)
        {
          case 0:
            {
              r = get_node(3);
              words = 3;
            }
            break;
          case 1:
          case 3:
            {
              r = get_node(2);
              incr(mem[mem[p + 1].hh.v.RH].hh.v.LH);
              words = 2;
            }
            break;
          case 2:
          case 4:
            {
              r = get_node(2);
              words = 2;
            }
            break;
          default:
            {
              confusion("ext2");
              return 0;         // abort_flag set
            }
            break;
        }
        break;
      case 10:
        {
          r = get_node(2);
          incr(mem[mem[p + 1].hh.v.LH].hh.v.RH);
          mem[r + 1].hh.v.LH = mem[p + 1].hh.v.LH;
          mem[r + 1].hh.v.RH = copy_node_list(mem[p + 1].hh.v.RH);
        }
        break;
      case 11:
      case 9:
      case 12:
        {
          r = get_node(2);
          words = 2;
        }
        break;
      case 6:
        {
          r = get_node(2);
          mem[r + 1]= mem[p + 1];
          mem[r + 1].hh.v.RH = copy_node_list(mem[p + 1].hh.v.RH);
        }
        break;
      case 7:
        {
          r = get_node(2);
          mem[r + 1].hh.v.LH = copy_node_list(mem[p + 1].hh.v.LH);
          mem[r + 1].hh.v.RH = copy_node_list(mem[p + 1].hh.v.RH);
        }
        break;
      case 4:
        {
          r = get_node(2);
          incr(mem[mem[p + 1].cint].hh.v.LH);
          words = 2;
        }
        break;
      case 5:
        {
          r = get_node(2);
          mem[r + 1].cint = copy_node_list(mem[p + 1].cint);
        }
        break;
      default:
        {
          confusion("copying");
          return 0;       // abort_flag set
        }
        break;
    }
    while (words > 0) {
      decr(words);
      mem[r + words]= mem[p + words]; /* r may be used without having ... */
    }
    mem[q].hh.v.RH = r;
    q = r;
    p = mem[p].hh.v.RH;
  }
  mem[q].hh.v.RH = 0;
  q = mem[h].hh.v.RH;
  {
    mem[h].hh.v.RH = avail;
    avail = h;
    ;
#ifdef STAT
    decr(dyn_used); 
#endif /* STAT */
  }
  Result = q;
  return Result;
} 
void print_mode_(integer m)
{ 
  if (m > 0)
    switch (m /(101))
    {
      case 0:
        print_string("vertical");
        break;
      case 1:
        print_string("horizontal");
        break;
      case 2:
        print_string("display math");
        break;
    } else if (m == 0)
      print_string("no");
  else switch ((- (integer) m)/(101))
  {
    case 0:
      print_string("internal vertical");
      break;
    case 1:
      print_string("restricted horizontal");
      break;
    case 2:
      print_string("math");
      break;
  }
  print_string(" mode");
}
void push_nest (void) 
{
  if (nest_ptr > max_nest_stack)
  {
    max_nest_stack = nest_ptr;
#ifdef ALLOCATEINPUTSTACK
    if (nest_ptr == current_nest_size)
      nest = realloc_nest_stack (increment_nest_size);
    if (nest_ptr == current_nest_size) {  /* check again after allocation */
      overflow("semantic nest size", current_nest_size);
      return;     // abort_flag set
    }
#else
    if (nest_ptr == nest_size) {
      overflow("semantic nest size", nest_size); /* semantic next size - not dynamic */
      return;     // abort_flag set
    }
#endif
  }
  nest[nest_ptr]= cur_list;
  incr(nest_ptr);
  head = get_avail ();
  tail = head;
  prev_graf = 0;
  mode_line = line;
}
void pop_nest (void) 
{
  {
    mem[head].hh.v.RH = avail;
    avail = head;
    ;
#ifdef STAT
    decr(dyn_used);
#endif /* STAT */
  }
  decr(nest_ptr);
  cur_list = nest[nest_ptr];
}
void show_activities (void) 
{
  integer p;
  short m;
  memory_word a;
  halfword q, r;
  integer t;
  nest[nest_ptr]= cur_list;
  print_nl("  ");
  print_ln (); 
  {
    register integer for_end; 
    p = nest_ptr; 
    for_end = 0; 
    if (p >= for_end) do
    {
      m = nest[p].mode_field;
      a = nest[p].aux_field;
      print_nl("### ");
      print_mode(m);
      print_string(" entered at line ");
      print_int(abs(nest[p].ml_field));
      if (m == 102)
/* ************************************************************************ */
/* major change from 3.141 -> 3.14159 in following */
/*.pg_field instead of .lhmfield and .rhmfield */
/* WAS if ((nest[p].lhmfield != 2)||(nest[p].rhmfield != 3)) */
        if (nest[p].pg_field != 8585216L)  /* 830000 hex ??? */
        {
          print_string(" (language");
          print_int(nest[p].pg_field % 65536L);   /* last 16 bits */
/*  print_int(nest[p].pg_field & 65535L);  */
          print_string(":hyphenmin");
          print_int(nest[p].pg_field / 4194304L);   /* 400000 hex ??? */
/*  print_int(nest[p].pg_field >> 22); */ /* top 10 bits */
          print_char(',');
          print_int((nest[p].pg_field / 65536L)% 64); 
/*  print_int((nest[p].pg_field >> 16)& 63); */ /* next 8 bits */
/*  this used to refer to .lhmfield and .rhmfield ... */
/* ********************************************************************* */
          print_char(')');
        } 
      if (nest[p].ml_field < 0)
        print_string(" (\\output routine)");
      if (p == 0)
      {
        if (page_head != page_tail)
        {
          print_nl("### current page:");
          if (output_active)
            print_string(" (held over for next output)");
          show_box(mem[page_head].hh.v.RH);
          if (page_contents > 0)
          {
            print_nl("total height ");
            print_totals ();
            print_nl("goal height ");
            print_scaled(page_so_far[0]);
            r = mem[mem_top].hh.v.RH; 
            while (r != mem_top) {
              print_ln ();
              print_esc("insert");
              t = mem[r].hh.b1;
              print_int(t);
              print_string(" adds ");
/* 427. Tell more precisely the effective size of 1:1 insertions (DEK, 27 Feb 08) */
              if (eqtb[(hash_size + 3218) + t].cint == 1000)
                t = mem[r + 3].cint;
              else
                t = x_over_n(mem[r + 3].cint, 1000) * eqtb[(hash_size + 3218) + t].cint;
              print_scaled(t);
              if (mem[r].hh.b0 == 1)
              {
                q = page_head;
                t = 0;
                do {
                  q = mem[q].hh.v.RH;
                  if ((mem[q].hh.b0 == 3) && (mem[q].hh.b1 == mem[r].hh.b1))
                    incr(t);
                } while (!(q == mem[r + 1].hh.v.LH)); 
                print_string(", #");
                print_int(t);
                print_string(" might split");
              }
              r = mem[r].hh.v.RH;
            }
          }
        }
/*  if link(contrib_head)<>null then l.4393 */
        if (mem[contrib_head].hh.v.RH != 0)
          print_nl(" (\\output routine)");
      }
      show_box(mem[nest[p].head_field].hh.v.RH);
      switch (abs(m)/(101))
      {
        case 0:
          {
            print_nl("### recent contributions:");
            if  (a.cint <= ignore_depth)
              print_string("ignored");
            else
              print_scaled(a.cint);
            if (nest[p].pg_field != 0)
            {
              print_string(", prevgraf ");
              print_int(nest[p].pg_field);
              print_string(" line");
              if (nest[p].pg_field != 1)
                print_char('s');
            }
          }
          break;
        case 1:
          {
            print_nl("spacefactor ");
            print_int(a.hh.v.LH); 
            if (m > 0)
              if (a.hh.v.RH > 0)
              {
                print_string(", current language ");
                print_int(a.hh.v.RH);
              }
          }
          break;
        case 2:
          if (a.cint != 0)
          {
            print_string("this will be denominator of:");
            show_box(a.cint);
          }
          break;
      }
    } while(p-- > for_end); } 
}
/* sec 0237 */
void print_param_(integer n)
{
  switch (n)
  {
    case pretolerance_code:
      print_esc("pretolerance");
      break;
    case tolerance_code:
      print_esc("tolerance");
      break;
    case line_penalty_code:
      print_esc("linepenalty");
      break;
    case hyphen_penalty_code:
      print_esc("hyphenpenalty");
      break;
    case ex_hyphen_penalty_code:
      print_esc("exhyphenpenalty");
      break;
    case club_penalty_code:
      print_esc("clubpenalty");
      break;
    case widow_penalty_code:
      print_esc("widowpenalty");
      break;
    case display_widow_penalty_code:
      print_esc("displaywidowpenalty");
      break;
    case broken_penalty_code:
      print_esc("brokenpenalty");
      break;
    case bin_op_penalty_code:
      print_esc("binoppenalty");
      break;
    case rel_penalty_code:
      print_esc("relpenalty");
      break;
    case pre_display_penalty_code:
      print_esc("predisplaypenalty");
      break;
    case post_display_penalty_code:
      print_esc("postdisplaypenalty");
      break;
    case inter_line_penalty_code:
      print_esc("interlinepenalty");
      break;
    case double_hyphen_demerits_code:
      print_esc("doublehyphendemerits");
      break;
    case final_hyphen_demerits_code:
      print_esc("finalhyphendemerits");
      break;
    case adj_demerits_code:
      print_esc("adjdemerits");
      break;
    case mag_code:
      print_esc("mag");
      break;
    case delimiter_factor_code:
      print_esc("delimiterfactor");
      break;
    case looseness_code:
      print_esc("looseness");
      break;
    case time_code:
      print_esc("time");
      break;
    case day_code:
      print_esc("day");
      break;
    case month_code:
      print_esc("month");
      break;
    case year_code:
      print_esc("year");
      break;
    case show_box_breadth_code:
      print_esc("showboxbreadth");
      break;
    case show_box_depth_code:
      print_esc("showboxdepth");
      break;
    case hbadness_code:
      print_esc("hbadness");
      break;
    case vbadness_code:
      print_esc("vbadness");
      break;
    case pausing_code:
      print_esc("pausing");
      break;
    case tracing_online_code:
      print_esc("tracingonline");
      break;
    case tracing_macros_code:
      print_esc("tracingmacros");
      break;
    case tracing_stats_code:
      print_esc("tracingstats");
      break;
    case tracing_paragraphs_code:
      print_esc("tracingparagraphs");
      break;
    case tracing_pages_code:
      print_esc("tracingpages");
      break;
    case tracing_output_code:
      print_esc("tracingoutput");
      break;
    case tracing_lost_chars_code:
      print_esc("tracinglostchars");
      break;
    case tracing_commands_code:
      print_esc("tracingcommands");
      break;
    case tracing_restores_code:
      print_esc("tracingrestores");
      break;
    case uc_hyph_code:
      print_esc("uchyph");
      break;
    case output_penalty_code:
      print_esc("outputpenalty");
      break;
    case max_dead_cycles_code:
      print_esc("maxdeadcycles");
      break;
    case hang_after_code:
      print_esc("hangafter");
      break;
    case floating_penalty_code:
      print_esc("floatingpenalty");
      break;
    case global_defs_code:
      print_esc("globaldefs");
      break;
    case cur_fam_code:
      print_esc("fam");
      break;
    case escape_char_code:
      print_esc("escapechar");
      break;
    case default_hyphen_char_code:
      print_esc("defaulthyphenchar");
      break;
    case default_skew_char_code:
      print_esc("defaultskewchar");
      break;
    case end_line_char_code:
      print_esc("endlinechar");
      break;
    case new_line_char_code:
      print_esc("newlinechar");
      break;
    case language_code:
      print_esc("language");
      break;
    case left_hyphen_min_code:
      print_esc("lefthyphenmin");
      break;
    case right_hyphen_min_code:
      print_esc("righthyphenmin");
      break;
    case holding_inserts_code:
      print_esc("holdinginserts");
      break;
    case error_context_lines_code:
      print_esc("errorcontextlines");
      break;
    default:
      print_string("[unknown integer parameter!]");
      break;
  }
}
void begin_diagnostic (void)
{
  old_setting = selector;
  if ((eqtb[(hash_size + 3192)].cint <= 0) && (selector == 19)) {
    decr(selector);
    if (history == 0) history = 1;
  }
}
void end_diagnostic_(bool blankline)
{
  print_nl("  ");
  if (blankline) print_ln ();
  selector = old_setting;
}
void print_length_param_ (integer n)
{
  switch (n)
  {
    case par_indent_code:
      print_esc("parindent");
      break;
    case math_surround_code:
      print_esc("mathsurround");
      break;
    case line_skip_limit_code:
      print_esc("lineskiplimit");
      break;
    case hsize_code:
      print_esc("hsize");
      break;
    case vsize_code:
      print_esc("vsize");
      break;
    case max_depth_code:
      print_esc("maxdepth");
      break;
    case split_max_depth_code:
      print_esc("splitmaxdepth");
      break;
    case box_max_depth_code:
      print_esc("boxmaxdepth");
      break;
    case hfuzz_code:
      print_esc("hfuzz");
      break;
    case vfuzz_code:
      print_esc("vfuzz");
      break;
    case delimiter_shortfall_code:
      print_esc("delimitershortfall");
      break;
    case null_delimiter_space_code:
      print_esc("nulldelimiterspace");
      break;
    case script_space_code:
      print_esc("scriptspace");
      break;
    case pre_display_size_code:
      print_esc("predisplaysize");
      break;
    case display_width_code:
      print_esc("displaywidth");
      break;
    case display_indent_code:
      print_esc("displayindent");
      break;
    case overfull_rule_code:
      print_esc("overfullrule");
      break;
    case hang_indent_code:
      print_esc("hangindent");
      break;
    case h_offset_code:
      print_esc("hoffset");
      break;
    case v_offset_code:
      print_esc("voffset");
      break;
    case emergency_stretch_code:
      print_esc("emergencystretch");
      break;
    default:
      print_string("[unknown dimen parameter!]");
      break;
  }
}
void print_cmd_chr_ (quarterword cmd, halfword chrcode)
{
  switch (cmd)
  {
    case 1:
      {
        print_string("begin-group character ");
        print(chrcode);
      }
      break;
    case 2:
      {
        print_string("end-group character ");
        print(chrcode);
      }
      break;
    case 3:
      {
        print_string("math shift character ");
        print(chrcode);
      }
      break;
    case 6:
      {
        print_string("macro parameter character ");
        print(chrcode);
      }
      break;
    case 7:
      {
        print_string("superscript character ");
        print(chrcode);
      }
      break;
    case 8:
      {
        print_string("subscript character ");
        print(chrcode);
      }
      break;
    case 9:
      print_string("end of alignment template");
      break;
    case 10:
      {
        print_string("blank space ");
        print(chrcode);
      }
      break;
    case 11:
      {
        print_string("the letter ");
        print(chrcode);
      }
      break;
    case 12:
      {
        print_string("the character ");
        print(chrcode);
      }
      break;
    case 75:
    case 76:
/* if chr_code<skip_base then print_skip_param(chr_code-glue_base) */
      if (chrcode < (hash_size + 800))
        print_skip_param(chrcode - (hash_size + 782));  /* lineskip */
/* else if chr_code<mu_skip_base then
    begin print_esc("skip"); print_int(chr_code-skip_base); */
      else if (chrcode < (hash_size + 1056))
      {
        print_esc("skip");
        print_int(chrcode - (hash_size + 800));
      } else {
/*   else  begin print_esc("muskip"); print_int(chr_code-mu_skip_base); */
        print_esc("muskip");
        print_int(chrcode - (hash_size + 1056));
      }
      break;
    case 72:
      if (chrcode >= (hash_size + 1322))
      {
        print_esc("toks");
        print_int(chrcode - (hash_size + 1322));
      } else switch (chrcode)
      {
        case (hash_size + 1313):
          print_esc("output");
          break;
        case (hash_size + 1314):
          print_esc("everypar");
          break;
        case (hash_size + 1315):
          print_esc("everymath");
          break;
        case (hash_size + 1316):
          print_esc("everydisplay");
          break;
        case (hash_size + 1317):
          print_esc("everyhbox");
          break;
        case (hash_size + 1318):
          print_esc("everyvbox");
          break;
        case (hash_size + 1319):
          print_esc("everyjob");
          break;
        case (hash_size + 1320):
          print_esc("everycr");
          break;
        default:
          print_esc("errhelp");
          break;
      }
      break;
    case 73:
      if (chrcode < (hash_size + 3218))
        print_param(chrcode - (hash_size + 3163));
      else {
        print_esc("count");
        print_int(chrcode - (hash_size + 3218));
      }
      break;
    case 74:
      if (chrcode < (hash_size + 3751))
        print_length_param(chrcode - (hash_size + 3730)); 
      else {
        print_esc("dimen");
        print_int(chrcode - (hash_size + 3751));
      }
      break;
    case 45:
      print_esc("accent");
      break;
    case 90:
      print_esc("advance");
      break;
    case 40:
      print_esc("afterassignment");
      break;
    case 41:
      print_esc("aftergroup");
      break;
    case 77:
      print_esc("fontdimen");
      break;
    case 61:
      print_esc("begingroup");
      break;
    case 42:
      print_esc("penalty");
      break;
    case 16:
      print_esc("char");
      break;
    case 107:
      print_esc("csname");
      break;
    case 88:
      print_esc("font");
      break;
    case 15:
      print_esc("delimiter");
      break;
    case 92:
      print_esc("divide");
      break;
    case 67:
      print_esc("endcsname");
      break;
    case 62:
      print_esc("endgroup");
      break;
    case 64:
      print_esc(" ");
      break;
    case 102:
      print_esc("expandafter");
      break;
    case 32:
      print_esc("halign");
      break;
    case 36:
      print_esc("hrule");
      break;
    case 39:
      print_esc("ignorespaces");
      break;
    case 37:
      print_esc("insert");
      break;
    case 44:
      print_esc("/");
      break;
    case 18:
      print_esc("mark");
      break;
    case 46:
      print_esc("mathaccent");
      break;
    case 17:
      print_esc("mathchar");
      break;
    case 54:
      print_esc("mathchoice");
      break;
    case 91:
      print_esc("multiply");
      break;
    case 34:
      print_esc("noalign");
      break;
    case 65:
      print_esc("noboundary");
      break;
    case 103:
      print_esc("noexpand");
      break;
    case 55:
      print_esc("nonscript");
      break;
    case 63:
      print_esc("omit");
      break;
    case 66:
      print_esc("radical");
      break;
    case 96:
      print_esc("read");
      break;
    case 0:
      print_esc("relax");
      break;
    case 98:
      print_esc("setbox");
      break;
    case 80:
      print_esc("prevgraf");
      break;
    case 84:
      print_esc("parshape");
      break;
    case 109:
      print_esc("the");
      break;
    case 71:
      print_esc("toks");
      break;
    case 38:
      print_esc("vadjust");
      break;
    case 33:
      print_esc("valign");
      break;
    case 56:
      print_esc("vcenter");
      break;
    case 35:
      print_esc("vrule");
      break;
    case 13:
      print_esc("par");
      break;
    case 104:
      if (chrcode == 0)
        print_esc("input");
      else
        print_esc("endinput");
      break;
    case 110:
      switch (chrcode)
      {
        case 1:
          print_esc("firstmark");
          break;
        case 2:
          print_esc("botmark");
          break;
        case 3:
          print_esc("splitfirstmark");
          break;
        case 4:
          print_esc("splitbotmark");
          break;
        default:
          print_esc("topmark");
          break;
      }
      break;
    case 89:
      if (chrcode == 0)
        print_esc("count");
      else if (chrcode == 1)
        print_esc("dimen");
      else if (chrcode == 2)
        print_esc("skip");
      else
        print_esc("muskip");
      break;
    case 79:
      if (chrcode == 1)
        print_esc("prevdepth");
      else
        print_esc("spacefactor");
      break;
    case 82:
      if (chrcode == 0)
        print_esc("deadcycles");
      else
        print_esc("insertpenalties");
      break;
    case 83:
      if (chrcode == 1)
        print_esc("wd");
      else if (chrcode == 3)
        print_esc("ht");
      else
        print_esc("dp");
      break;
    case 70:
      switch (chrcode)
      {
        case 0:
          print_esc("lastpenalty");
          break;
        case 1:
          print_esc("lastkern");
          break;
        case 2:
          print_esc("lastskip");
          break;
        case 3:
          print_esc("inputlineno");
          break;
        default:
          print_esc("badness");
          break;
      }
      break;
    case 108:
      switch (chrcode)
      {
        case 0:
          print_esc("number");
          break;
        case 1:
          print_esc("romannumeral");
          break;
        case 2:
          print_esc("string");
          break;
        case 3:
          print_esc("meaning");
          break;
        case 4:
          print_esc("fontname");
          break;
        default:
          print_esc("jobname");
          break;
      }
      break;
    case 105:
      switch (chrcode)
      {
        case 1:
          print_esc("ifcat");
          break;
        case 2:
          print_esc("ifnum");
          break;
        case 3:
          print_esc("ifdim");
          break;
        case 4:
          print_esc("ifodd");
          break;
        case 5:
          print_esc("ifvmode");
          break;
        case 6:
          print_esc("ifhmode");
          break;
        case 7:
          print_esc("ifmmode");
          break;
        case 8:
          print_esc("ifinner");
          break;
        case 9:
          print_esc("ifvoid");
          break;
        case 10:
          print_esc("ifhbox");
          break;
        case 11:
          print_esc("ifvbox");
          break;
        case 12:
          print_esc("ifx");
          break;
        case 13:
          print_esc("ifeof");
          break;
        case 14:
          print_esc("iftrue");
          break;
        case 15:
          print_esc("iffalse");
          break;
        case 16:
          print_esc("ifcase");
          break;
        default:
          print_esc("if");
          break;
      }
      break;
    case 106:
      if (chrcode == 2)
        print_esc("fi");
      else if (chrcode == 4)
        print_esc("or");
      else print_esc("else");
      break;
    case 4:
      if (chrcode == 256)  /* pool size */ /* max_quarterword + 1 ? */
        print_esc("span");
      else {
        print_string("alignment tab character ");
        print(chrcode);
      }
      break;
    case 5:
      if (chrcode == 257)    /* cr_code */
        print_esc("cr");
      else print_esc("crcr");
      break;
    case 81:
      switch (chrcode)
      {
        case 0:
          print_esc("pagegoal");
          break;
        case 1:
          print_esc("pagetotal");
          break;
        case 2:
          print_esc("pagestretch");
          break;
        case 3:
          print_esc("pagefilstretch");
          break;
        case 4:
          print_esc("pagefillstretch");
          break;
        case 5:
          print_esc("pagefilllstretch");
          break;
        case 6:
          print_esc("pageshrink");
          break;
        default:
          print_esc("pagedepth");
          break;
      }
      break;
    case 14:
      if (chrcode == 1)
        print_esc("dump");
      else print_esc("end");
      break;
    case 26:
      switch (chrcode)
      {
        case 4:
          print_esc("hskip");
          break;
        case 0:
          print_esc("hfil");
          break;
        case 1:
          print_esc("hfill");
          break;
        case 2:
          print_esc("hss");
          break;
        default:
          print_esc("hfilneg");
          break;
      }
      break;
    case 27:
      switch (chrcode)
      {
        case 4:
          print_esc("vskip");
          break;
        case 0:
          print_esc("vfil");
          break;
        case 1:
          print_esc("vfill");
          break;
        case 2:
          print_esc("vss");
          break;
        default:
          print_esc("vfilneg");
          break;
      }
      break;
    case 28:
      print_esc("mskip");
      break;
    case 29:
      print_esc("kern");
      break;
    case 30:
      print_esc("mkern");
      break;
    case 21:
      if (chrcode == 1)
        print_esc("moveleft");
      else print_esc("moveright");
      break;
    case 22:
      if (chrcode == 1)
        print_esc("raise");
      else print_esc("lower");
      break;
    case 20:
      switch (chrcode)
      {
        case 0:
          print_esc("box");
          break;
        case 1:
          print_esc("copy");
          break;
        case 2:
          print_esc("lastbox");
          break;
        case 3:
          print_esc("vsplit");
          break;
        case 4:
          print_esc("vtop");
          break;
        case 5:
          print_esc("vbox");
          break;
        default:
          print_esc("hbox");
          break;
      }
      break;
    case 31:
      if (chrcode == 100)
        print_esc("leaders");
      else if (chrcode == 101)
        print_esc("cleaders");
      else if (chrcode == 102)
        print_esc("xleaders");
      else print_esc("shipout");
      break;
    case 43:
      if (chrcode == 0)
        print_esc("noindent");
      else print_esc("indent");
      break;
    case 25:
      if (chrcode == 10)
        print_esc("unskip");
      else if (chrcode == 11)
        print_esc("unkern");
      else print_esc("unpenalty");
      break;
    case 23:
      if (chrcode == 1)
        print_esc("unhcopy");
      else print_esc("unhbox");
      break;
    case 24:
      if (chrcode == 1)
        print_esc("unvcopy");
      else print_esc("unvbox");
      break;
    case 47:
      if (chrcode == 1)
        print_esc("-");
      else print_esc("discretionary");
      break;
    case 48:
      if (chrcode == 1)
        print_esc("leqno");
      else print_esc("eqno");
      break;
    case 50:
      switch (chrcode)
      {
        case 16:
          print_esc("mathord");
          break;
        case 17:
          print_esc("mathop");
          break;
        case 18:
          print_esc("mathbin");
          break;
        case 19:
          print_esc("mathrel");
          break;
        case 20:
          print_esc("mathopen");
          break;
        case 21:
          print_esc("mathclose");
          break;
        case 22:
          print_esc("mathpunct");
          break;
        case 23:
          print_esc("mathinner");
          break;
        case 26:
          print_esc("underline");
          break;
        default:
          print_esc("overline");
          break;
      }
      break;
    case 51:
      if (chrcode == 1)
        print_esc("limits");
      else if (chrcode == 2)
        print_esc("nolimits");
      else print_esc("displaylimits");
      break;
    case 53:
      print_style(chrcode);
      break;
    case 52:
      switch (chrcode)
      {
        case 1:
          print_esc("over");
          break;
        case 2:
          print_esc("atop");
          break;
        case 3:
          print_esc("abovewithdelims");
          break;
        case 4:
          print_esc("overwithdelims");
          break;
        case 5:
          print_esc("atopwithdelims");
          break;
        default:
          print_esc("above");
          break;
      }
      break;
    case 49:
      if (chrcode == 30)
        print_esc("left");
      else print_esc("right");
      break;
    case 93:
      if (chrcode == 1)
        print_esc("long");
      else if (chrcode == 2)
        print_esc("outer");
      else print_esc("global");
      break;
    case 97:
      if (chrcode == 0)
        print_esc("def");
      else if (chrcode == 1)
        print_esc("gdef");
      else if (chrcode == 2)
        print_esc("edef");
      else print_esc("xdef");
      break;
    case 94:
      if (chrcode != 0)
        print_esc("futurelet");
      else print_esc("let");
      break;
    case 95:
      switch (chrcode)
      {
        case 0:
          print_esc("chardef");
          break;
        case 1:
          print_esc("mathchardef");
          break;
        case 2:
          print_esc("countdef");
          break;
        case 3:
          print_esc("dimendef");
          break;
        case 4:
          print_esc("skipdef");
          break;
        case 5:
          print_esc("muskipdef");
          break;
        default:
          print_esc("toksdef");
          break;
      }
      break;
    case 68:
      {
        print_esc("char");
        print_hex(chrcode);
      }
      break;
    case 69:
      {
        print_esc("mathchar");
        print_hex(chrcode);
      }
      break;
    case 85:
      if (chrcode == (hash_size + 1883))
        print_esc("catcode");
      else if (chrcode == (hash_size + 2907))
        print_esc("mathcode");
      else if (chrcode == (hash_size + 2139))
        print_esc("lccode");
      else if (chrcode == (hash_size + 2395))
        print_esc("uccode");
      else if (chrcode == (hash_size + 2651))
        print_esc("sfcode");
      else print_esc("delcode");
      break;
    case 86:
      print_size(chrcode - (hash_size + 1835)); /* chr - math_font_base */
      break; 
    case 99:
      if (chrcode == 1)
        print_esc("patterns");
      else print_esc("hyphenation");
      break;
    case 78:
      if (chrcode == 0)
        print_esc("hyphenchar");
      else print_esc("skewchar");
      break;
    case 87:
      {
        print_string("select font ");
        slow_print(font_name[chrcode]);
        if (font_size[chrcode]!= font_dsize[chrcode])
        {
          print_string(" at ");
          print_scaled(font_size[chrcode]);
          print_string("pt");
        }
      }
      break;
    case 100:
      switch (chrcode)
      {
        case 0:
          print_esc("batchmode");
          break;
        case 1:
          print_esc("nonstopmode");
          break;
        case 2:
          print_esc("scrollmode");
          break;
        default:
          print_esc("errorstopmode");
          break;
      }
      break;
    case 60:
      if (chrcode == 0)
        print_esc("closein");
      else print_esc("openin");
      break;
    case 58:
      if (chrcode == 0)
        print_esc("message");
      else print_esc("errmessage");
      break;
    case 57:
      if (chrcode == (hash_size + 2139))
        print_esc("lowercase");
      else print_esc("uppercase");
      break;
    case 19:
      switch (chrcode)
      {
        case 1:
          print_esc("showbox");
          break;
        case 2:
          print_esc("showthe");
          break;
        case 3:
          print_esc("showlists");
          break;
        default:
          print_esc("show");
          break;
      }
      break;
    case 101:
      print_string("undefined");
      break;
    case 111:
      print_string("macro");
      break;
    case 112:
      print_esc("long macro");
      break;
    case 113:
      print_esc("outer macro");
      break;
    case 114:
      {
        print_esc("long");
        print_esc("outer macro");
      }
      break;
    case 115:
      print_esc("outer endtemplate");
      break;
    case 59:
      switch (chrcode)
      {
        case 0:
          print_esc("openout");
          break;
        case 1:
          print_esc("write");
          break;
        case 2:
          print_esc("closeout");
          break;
        case 3:
          print_esc("special");
          break;
        case 4:
          print_esc("immediate");
          break;
        case 5:
          print_esc("setlanguage");
          break;
        default:
          print_string("[unknown extension!]");
          break;
      }
      break;
    default:
      print_string("[unknown command code!]");
      break;
  }
}

#ifdef STAT
void show_eqtb_(halfword n)
{ 
  if (n < 1)
    print_char('?');
  else if (n < (hash_size + 782))    /* lineskip */
  {
    sprint_cs(n); 
    print_char('=');
    print_cmd_chr(eqtb[n].hh.b0, eqtb[n].hh.v.RH);
    if (eqtb[n].hh.b0 >= 111)
    {
      print_char(':');
      show_token_list(mem[eqtb[n].hh.v.RH].hh.v.RH, 0, 32);
    }
  } else if (n < (hash_size + 1312))
    if (n < (hash_size + 800))
    {
      print_skip_param(n - (hash_size + 782));  /* lineskip */
      print_char('=');
      if (n < (hash_size + 797))
        print_spec(eqtb[n].hh.v.RH, 394); /* pt */
      else
        print_spec(eqtb[n].hh.v.RH, 334);  /* mu */
    } else if (n < (hash_size + 1056))
    {
      print_esc("skip");
      print_int(n - (hash_size + 800));
      print_char('=');
      print_spec(eqtb[n].hh.v.RH, 394);   /* pt */
    } else {
      print_esc("muskip");
      print_int(n - (hash_size + 1056));
      print_char('=');
      print_spec(eqtb[n].hh.v.RH, 334);   /* mu */
    } else if (n < (hash_size + 3163))
      if (n == (hash_size + 1312))
      {
        print_esc("parshape");
        print_char('=');
        if (eqtb[(hash_size + 1312)].hh.v.RH == 0)
          print_char('0');
        else
          print_int(mem[eqtb[(hash_size + 1312)].hh.v.RH].hh.v.LH);
      } else if (n < (hash_size + 1322))
      {
        print_cmd_chr(72, n); /* H */
        print_char('=');
        if (eqtb[n].hh.v.RH != 0)
          show_token_list(mem[eqtb[n].hh.v.RH].hh.v.RH, 0, 32);
      } else if (n < (hash_size + 1578))
      {
        print_esc("toks");
        print_int(n - (hash_size + 1322));
        print_char('=');
        if (eqtb[n].hh.v.RH != 0)
          show_token_list(mem[eqtb[n].hh.v.RH].hh.v.RH, 0, 32);
      } else if (n < (hash_size + 1834))
      {
        print_esc("box");
        print_int(n - (hash_size + 1578));
        print_char('=');
        if (eqtb[n].hh.v.RH == 0)
          print_string("void");
        else {
          depth_threshold = 0;
          breadth_max = 1;
          show_node_list(eqtb[n].hh.v.RH);
        }
      } else if (n < (hash_size + 1883))   /* cat_code_base ... */
      {
        if (n == (hash_size + 1834))
          print_string("current font");
        else if (n < (hash_size + 1851))
        {
          print_esc("textfont");
          print_int(n - (hash_size + 1835));
        } else if (n < (hash_size + 1867))
        {
          print_esc("scriptfont");
          print_int(n - (hash_size + 1851));
        } else {
          print_esc("scriptscriptfont");
          print_int(n - (hash_size + 1867));
        } 
        print_char('=');
/*    print_esc(hash[(hash_size + 524) + eqtb[n].hh.v.RH].v.RH); */
    /*print_esc(hash[(hash_size + hash_extra + 524) + eqtb[n].hh.v.RH].v.RH);*/
        print_esc("");print(hash[(hash_size + hash_extra + 524) + eqtb[n].hh.v.RH].v.RH);
      } else if (n < (hash_size + 2907))
      {
        if (n < (hash_size + 2139))
        {
          print_esc("catcode");
          print_int(n - (hash_size + 1883));
        } else if (n < (hash_size + 2395))
        {
          print_esc("lccode");
          print_int(n - (hash_size + 2139));
        } else if (n < (hash_size + 2651))
        {
          print_esc("uccode");
          print_int(n - (hash_size + 2395));
        } else {
          print_esc("sfcode");
          print_int(n - (hash_size + 2651));
        } 
        print_char('=');
        print_int(eqtb[n].hh.v.RH);
      } else {
        print_esc("mathcode");
        print_int(n - (hash_size + 2907));
        print_char('=');
        print_int(eqtb[n].hh.v.RH);
      } else if (n < (hash_size + 3730))
      {
        if (n < (hash_size + 3218))
          print_param(n - (hash_size + 3163));
        else if (n < (hash_size + 3474))
        {
          print_esc("count");
          print_int(n - (hash_size + 3218));
        } else {
          print_esc("delcode");
          print_int(n - (hash_size + 3474));
        }
        print_char('=');
        print_int(eqtb[n].cint);
      } else if (n <= (hash_size + 4006))
      {
        if (n < (hash_size + 3751))
          print_length_param(n - (hash_size + 3730)); 
        else {
          print_esc("dimen");
          print_int(n - (hash_size + 3751));
        }
        print_char('=');
        print_scaled(eqtb[n].cint);
        print_string("pt");
      } else print_char('?');
} 
#endif /* STAT */

halfword id_lookup_(integer j, integer l)
{
  register halfword Result;
  integer h;
  integer d;
  halfword p;
  halfword k;
  h = buffer[j];
  {
    register integer for_end;
    k = j + 1;
    for_end = j + l - 1;
    if (k <= for_end) do
    {
      h = h + h + buffer[k];
      while (h >= hash_prime) h = h - hash_prime; /* buffer size hash prime */
    } while (k++ < for_end);
  }
  p = h + 514;            /* h + hash_base */
  while (true) {
    if (hash[p].v.RH > 0)
      if ((str_start[hash[p].v.RH + 1]- str_start[hash[p].v.RH]) == l)
        if (str_eq_buf(hash[p].v.RH, j))
          goto lab40;
    if (hash[p].v.LH == 0) {
      if (no_new_control_sequence)
        p = (hash_size + 781);    /* undefine_control_sequence */
      else {
        if (hash[p].v.RH > 0) {
          do {
            if ((hash_used == 514)) {  /* if hash_used = hashbase ... */ 
/*        we can't expand the hash table ... */
/*        overflow("hash size", hash_size); */ /* hash size - NOT DYNAMIC */
              overflow("hash size", hash_size + hash_extra); /* 96/Jan/10 */
              return 0;     // abort_flag set
            }
            decr(hash_used); 
          } while (!(hash[hash_used].v.RH == 0));
#ifdef SHORTHASH
          if (hash_used > 65535L) {     /* debugging only 1996/Jan/20 */
            sprintf(log_line, "ERROR: %s too large %d\n",
                "hash entry", hash_used);
            show_line(log_line, 1);
          }
#endif
          hash[p].v.LH = hash_used;
          p = hash_used;
        } 
#ifdef CHECKPOOL
        if (checkpool(NULL)) show_line("after hash_used\n", 0); 
#endif
        {
#ifdef ALLOCATESTRING
          if (pool_ptr + l > current_pool_size)
            str_pool = realloc_str_pool (increment_pool_size + 1);
          if (pool_ptr + l > current_pool_size){ /* in case it failed 97/Mar/7 */
            overflow("pool size", current_pool_size - init_pool_ptr); /* pool size */
            return 0;     // abort_flag set
          }
#else
          if (pool_ptr + l > pool_size) {
            overflow("pool size", pool_size - init_pool_ptr); /* pool size - not dynamic */
            return;     // abort_flag set
          }
#endif
        } 
        d =(pool_ptr - str_start[str_ptr]);
        while (pool_ptr > str_start[str_ptr]) {
          decr(pool_ptr);
          str_pool[pool_ptr + l]= str_pool[pool_ptr];
        }
#ifdef CHECKPOOL
        if (checkpool(NULL)) show_line("after pool_ptr\n", 0);
#endif
        {
          register integer for_end; 
          k = j; 
          for_end = j + l - 1; 
          if (k <= for_end) do {
            str_pool[pool_ptr]= buffer[k]; 
            incr(pool_ptr); 
          } 
          while(k++ < for_end);
        } 
#ifdef SHORTHASH
        {
          pool_pointer tempstring = make_string ();
          if (tempstring > 65535L) {      /* cannot happen */
            sprintf(log_line, "ERROR: %s too large %d\n",
                "string ptr", tempstring);
            show_line(log_line, 1);
          }
          hash[p].v.RH = tempstring;
        }
#else
        hash[p].v.RH = make_string (); 
#endif
#ifdef CHECKPOOL
        if (checkpool(NULL)) show_line("after make_string\n", 0); 
#endif
        pool_ptr = pool_ptr + d; 
        ;
#ifdef STAT
        incr(cs_count); 
        if (trace_flag) {
          str_pool[pool_ptr] = '\0';
          sprintf(log_line, " tex1 cs_count++ '%s' ", &str_pool[pool_ptr-l-d]);
          show_line(log_line, 0);      /* debugging */
        }
#endif /* STAT */
      } 
#ifdef CHECKPOOL
      if (checkpool(NULL)) show_line("after cs_count++\n", 0); 
#endif
      goto lab40; 
    } 
    p = hash[p].v.LH; 
  } 
#ifdef CHECKPOOL
  if (checkpool(NULL)) show_line("before return\n", 0); 
#endif
lab40:
  Result = p; 
  return Result; 
} 
void new_save_level_(group_code c)
{ 
   if (save_ptr > max_save_stack)        /* check_full_save_stack; p.274 */
   {
     max_save_stack = save_ptr;
#ifdef ALLOCATESAVESTACK
     if (max_save_stack > current_save_size - 6)
       save_stack = realloc_save_stack (increment_save_size);
     if (max_save_stack > current_save_size - 6){ /* check again after allocation */
       overflow("save size", current_save_size);
       return;     // abort_flag set
     }
#else
     if (max_save_stack > save_size - 6) { /* save size - not dynamic */
       overflow("save size", save_size);
       return;     // abort_flag set
     }
#endif
  }
/* save_type(save_ptr) <- level_boundary; */
  save_stack[save_ptr].hh.b0 = 3; 
/*  save_stack[save_ptr].hh.b1 = cur_group;  *//* keep compiler happy */
  save_stack[save_ptr].hh.b1 = (quarterword) cur_group; 
/* save_index(save_ptr):=cur_boundary; */
  save_stack[save_ptr].hh.v.RH = cur_boundary; 
/* if cur_level = max_quarterword then ... p.274 */
/*  if (cur_level == 255)*/      /* 94/Apr/4 */
  if (cur_level == max_quarterword)  {
/* { quit if (cur_level + 1) is too large to store in eqtb } */
/*  overflow("grouping levels", max_quarterword - min_quarterword); */
/*  overflow("grouping levels", 255); */      /* grouping levels - not dynamic */
    overflow("grouping levels", max_quarterword - min_quarterword); /* 96/Oct/12 ??? */
    return;     // abort_flag set
  }
/* cur_boundary <- save_ptr */
  cur_boundary = save_ptr; 
  incr(cur_level); 
  incr(save_ptr); 
  cur_group = c; 
} 
void eq_destroy_(memory_word w)
{ 
  halfword q;
  switch (w .hh.b0)
  {
    case 111:
    case 112:
    case 113:
    case 114:
      delete_token_ref(w.hh.v.RH);
      break;
    case 117:
      delete_glue_ref(w.hh.v.RH);
      break;
    case 118:
      {
        q = w.hh.v.RH; 
        if (q != 0)    /* if q<>null then free_node(... l.5937 */
          free_node(q, mem[q].hh.v.LH + mem[q].hh.v.LH + 1);
      }
      break;
    case 119:
      flush_node_list(w.hh.v.RH);
      break;
    default:
      ;
      break;
  }
}

void eq_save_(halfword p, quarterword l)
{
  if (save_ptr > max_save_stack)
  {
    max_save_stack = save_ptr;
#ifdef ALLOCATESAVESTACK
    if (max_save_stack > current_save_size - 6)
      save_stack = realloc_save_stack (increment_save_size);
    if (max_save_stack > current_save_size - 6){ /* check again after allocation */
      overflow("save size", current_save_size);
      return;     // abort_flag set
    }
#else
    if (max_save_stack > save_size - 6) { /* save size not dynamic */
      overflow("save size", save_size);
      return;     // abort_flag set
    }
#endif
  }
  if (l == 0)
    save_stack[save_ptr].hh.b0 = 1;
  else {
    save_stack[save_ptr]= eqtb[p];
    incr(save_ptr);
    save_stack[save_ptr].hh.b0 = 0;
  }
  save_stack[save_ptr].hh.b1 = l;
  save_stack[save_ptr].hh.v.RH = p;
  incr(save_ptr);
}
void eq_define_(halfword p, quarterword t, halfword e)
{
  if (eqtb[p].hh.b1 == cur_level)
    eq_destroy(eqtb[p]);
  else if (cur_level > 1)
    eq_save(p, eqtb[p].hh.b1);
  eqtb[p].hh.b1 = (quarterword) cur_level; /* because cur_level padded out */
  eqtb[p].hh.b0 = t;
  eqtb[p].hh.v.RH = e;
}

void eq_word_define_(halfword p, integer w)
{
  if (xeq_level[p]!= cur_level)
  {
    eq_save(p, xeq_level[p]);
    xeq_level[p]= (quarterword) cur_level; /* because cur_level padded out */
  }
  eqtb[p].cint = w;
}
void geq_define_(halfword p, quarterword t, halfword e)
{
  eq_destroy(eqtb[p]);
  eqtb[p].hh.b1 = 1;
  eqtb[p].hh.b0 = t;
  eqtb[p].hh.v.RH = e;
}

void geq_word_define_(halfword p, integer w)
{
  eqtb[p].cint = w;
  xeq_level[p]= 1;
}

void save_for_after_(halfword t)
{ 
  if (cur_level > 1)
  {
    if (save_ptr > max_save_stack)
    {
      max_save_stack = save_ptr;
#ifdef ALLOCATESAVESTACK
      if (max_save_stack > current_save_size - 6)
        save_stack = realloc_save_stack (increment_save_size);
      if (max_save_stack > current_save_size - 6) { /* check again after allocation */
        overflow("save size", current_save_size);
        return;     // abort_flag set
      }
#else
      if (max_save_stack > save_size - 6) {   /* save satck - not dynamic */
        overflow("save size", save_size);
        return;     // abort_flag set
      }
#endif
    }
    save_stack[save_ptr].hh.b0 = 2;
    save_stack[save_ptr].hh.b1 = 0;
    save_stack[save_ptr].hh.v.RH = t;
    incr(save_ptr);
  }
}
/* restore_trace_, unsave followed in the old tex1.c */
