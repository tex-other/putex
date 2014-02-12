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

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/* math_fraction etc used to be in tex7.c */
/* sec 1181 */
void math_fraction (void) 
{
  small_number c; 
  c = cur_chr; 
  if (cur_list.aux_field.cint != 0)
  {
    if (c >= 3)
    {
      scan_delimiter(lig_trick, false); 
      scan_delimiter(lig_trick, false); 
    } 
    if (c % 3 == 0)
      scan_dimen(false, false, false);
    print_err("Ambiguous; you need another { and }");
    help3("I'm ignoring this fraction specification, since I don't",
      "know whether a construction like `x \\over y \\over z'",
      "means `{x \\over y} \\over z' or `x \\over {y \\over z}'.");
    error();
  } else {
    cur_list.aux_field.cint = get_node(6);
    mem[cur_list.aux_field.cint].hh.b0 = 25;
    mem[cur_list.aux_field.cint].hh.b1 = 0;
    mem[cur_list.aux_field.cint + 2].hh.v.RH = 3;
    mem[cur_list.aux_field.cint + 2].hh.v.LH = mem[head].hh.v.RH;
    mem[cur_list.aux_field.cint + 3].hh = empty_field;
    mem[cur_list.aux_field.cint + 4].qqqq = null_delimiter;
    mem[cur_list.aux_field.cint + 5].qqqq = null_delimiter;
    mem[head].hh.v.RH = 0;
    tail = head;
    if (c >= 3) {
      scan_delimiter(cur_list.aux_field.cint + 4, false);
      scan_delimiter(cur_list.aux_field.cint + 5, false);
    }
    switch(c % 3)
    {
    case 0:
      {
        scan_dimen(false, false, false);
        mem[cur_list.aux_field.cint + 1].cint = cur_val;
      }
      break;
    case 1:
      mem[cur_list.aux_field.cint + 1].cint = 1073741824L;  /* 2^30 */
      break;
    case 2:
      mem[cur_list.aux_field.cint + 1].cint = 0;
      break;
    }
  }
}
/* sec 1191 */
void math_left_right (void) 
{ 
  small_number t; 
  halfword p; 
  t = cur_chr; 
  if ((t == 31) && (cur_group != 16)) {
    if (cur_group == 15) {
      scan_delimiter(lig_trick, false);
      print_err("Extra ");
      print_esc("right");
      help1("I'm ignoring a \\right that had no matching \\left.");
      error();
    } else {
      off_save();
    }
  } else {
    p = new_noad();
    mem[p].hh.b0 = t;
    scan_delimiter(p + 1, false);
    if (t == 30) {
      push_math(16);
      mem[head].hh.v.RH = p;
      tail = p;
    } else {
      p = fin_mlist(p);
      unsave();
      {
        mem[tail].hh.v.RH = new_noad();
        tail = mem[tail].hh.v.RH;
      }
      mem[tail].hh.b0 = 23;
      mem[tail + 1].hh.v.RH = 3;
      mem[tail + 1].hh.v.LH = p;
    }
  }
}
/* sec 1194 */
void after_math (void) 
{ 
  bool l; 
  bool danger; 
  integer m; 
  halfword p; 
  halfword a; 
  halfword b; 
  scaled w; 
  scaled z; 
  scaled e; 
  scaled q; 
  scaled d; 
  scaled s; 
  small_number g1, g2; 
  halfword r; 
  halfword t; 
  danger = false; 
  if ((font_params[eqtb[(hash_size + 1837)].hh.v.RH]< 22)||
    (font_params[eqtb[(hash_size + 1853)].hh.v.RH]< 22)||
    (font_params[eqtb[(hash_size + 1869)].hh.v.RH]< 22)) 
  {
	  print_err("Math formula deleted: Insufficient symbol fonts");
	  help3("Sorry, but I can't typeset math unless \\textfont Sorry, but I can't typeset math unless \\textfont 2",
		  "and \\scriptfont 2 and \\scriptscriptfont 2 have and \\scriptfont 2 and \\scriptscriptfont 2 have all",
		  "the \\fontdimen values needed in math symbol the \\fontdimen values needed in math symbol fonts.."); 
    error(); 
    flush_math(); 
    danger = true; 
  } 
  else if ((font_params[eqtb[(hash_size + 1838)].hh.v.RH]< 13)||
      (font_params[eqtb[(hash_size + 1854)].hh.v.RH]< 13)||
      (font_params[eqtb[(hash_size + 1870)].hh.v.RH]< 13)) 
  {
	  print_err("Math formula deleted: Insufficient extension fonts");
	  help3("Sorry, but I can't typeset math unless \\textfont 3",
		  "and \\scriptfont 3 and \\scriptscriptfont 3 have all",
		  "the \\fontdimen values needed in math extension fonts.");
    error(); 
    flush_math(); 
    danger = true; 
  } 
  m = mode; 
  l = false; 
  p = fin_mlist(0); 
  if (mode == - (integer) m)
  {
    {
      get_x_token(); 
      if (cur_cmd != 3)
      {
		  print_err("Display math should end with $$");
		  help2("The `$' that I just saw supposedly matches a previous `$$'.",
			  "So I shall assume that you typed `$$' both times.");
  back_error(); 
      } 
    } 
    cur_mlist = p; 
    cur_style = 2; 
    mlist_penalties = false; 
    mlist_to_hlist(); 
    a = hpack(mem[temp_head].hh.v.RH, 0, 1); 
    unsave(); 
    decr(save_ptr); 
    if (save_stack[save_ptr + 0].cint == 1)
    l = true; 
    danger = false; 
    if ((font_params[eqtb[(hash_size + 1837)].hh.v.RH]< 22)||
    (font_params[eqtb[(hash_size + 1853)].hh.v.RH]< 22)||
    (font_params[eqtb[(hash_size + 1869)].hh.v.RH]< 22)) 
    {
		print_err("Math formula deleted: Insufficient symbol fonts");
		help3("Sorry, but I can't typeset math unless \\textfont 2",
			"and \\scriptfont 2 and \\scriptscriptfont 2 have all",
			"the \\fontdimen values needed in math symbol fonts.");
      error(); 
      flush_math(); 
      danger = true; 
    } 
    else if ((font_params[eqtb[(hash_size + 1838)].hh.v.RH]< 13)||
       (font_params[eqtb[(hash_size + 1854)].hh.v.RH]< 13)|| 
       (font_params[eqtb[(hash_size + 1870)].hh.v.RH]< 13)) 
    {
		print_err("Math formula deleted: Insufficient extension fonts");
		help3("Sorry, but I can't typeset math unless \\textfont 3",
			"and \\scriptfont 3 and \\scriptscriptfont 3 have all",
			"the \\fontdimen values needed in math extension fonts.");
      error(); 
      flush_math(); 
      danger = true; 
    } 
    m = mode; 
    p = fin_mlist(0); 
  } 
  else a = 0; 
  if (m < 0)
  {
    {
      mem[tail].hh.v.RH = new_math(math_surround, 0); 
      tail = mem[tail].hh.v.RH; 
    } 
    cur_mlist = p; 
    cur_style = 2; 
    mlist_penalties =(mode > 0); 
    mlist_to_hlist(); 
    mem[tail].hh.v.RH = mem[temp_head].hh.v.RH; 
    while(mem[tail].hh.v.RH != 0)tail = 
    mem[tail].hh.v.RH; 
    {
      mem[tail].hh.v.RH = new_math(math_surround, 1); 
      tail = mem[tail].hh.v.RH; 
    } 
    space_factor = 1000; 
    unsave(); 
  } 
  else {
      
    if (a == 0)
    {
      get_x_token(); 
      if (cur_cmd != 3) {
		  print_err("Display math should end with $$");
		  help2("The `$' that I just saw supposedly matches a previous `$$'.",
			  "So I shall assume that you typed `$$' both times.");
  back_error(); 
      } 
    } 
    cur_mlist = p; 
    cur_style = 0; 
    mlist_penalties = false; 
    mlist_to_hlist(); 
    p = mem[temp_head].hh.v.RH; 
    adjust_tail = adjust_head; 
    b = hpack(p, 0, 1); 
    p = mem[b + 5].hh.v.RH; 
    t = adjust_tail; 
    adjust_tail = 0; 
    w = mem[b + 1].cint; 
    z = display_width; 
    s = display_indent; 
    if ((a == 0)|| danger)
    {
      e = 0; 
      q = 0; 
    } 
    else {
  
      e = mem[a + 1].cint; 
      q = e + font_info[6 + param_base[eqtb[(hash_size + 1837)].hh.v.RH]].cint; 
    } 
    if (w + q > z)
    {
      if ((e != 0)&&((w - totalshrink[0]+ q <= z)||(totalshrink 
     [1]!= 0)||(totalshrink[2]!= 0)||(totalshrink[3]!= 0)
    )) 
      {
  free_node(b, 7); 
  b = hpack(p, z - q, 0); 
      } 
      else {
    
  e = 0; 
  if (w > z)
  {
    free_node(b, 7); 
    b = hpack(p, z, 0); 
  } 
      } 
      w = mem[b + 1].cint; 
    } 
    d = half(z - w); 
    if ((e > 0)&&(d < 2 * e)) 
    {
      d = half(z - w - e); 
      if (p != 0)
      if (!(p >= hi_mem_min)) 
      if (mem[p].hh.b0 == 10)
      d = 0; 
    } 
    {
      mem[tail].hh.v.RH = new_penalty(pre_display_penalty); 
      tail = mem[tail].hh.v.RH; 
    } 
    if ((d + s <= pre_display_size)|| l)
    {
      g1 = 3; 
      g2 = 4; 
    } 
    else {
  
      g1 = 5; 
      g2 = 6; 
    } 
    if (l &&(e == 0)) 
    {
      mem[a + 4].cint = s; 
      append_to_vlist(a); 
      {
  mem[tail].hh.v.RH = new_penalty(10000); 
  tail = mem[tail].hh.v.RH; 
      } 
    } 
    else {
  
      mem[tail].hh.v.RH = new_param_glue(g1); 
      tail = mem[tail].hh.v.RH; 
    } 
    if (e != 0)
    {
      r = new_kern(z - w - e - d); 
      if (l)
      {
  mem[a].hh.v.RH = r; 
  mem[r].hh.v.RH = b; 
  b = a; 
  d = 0; 
      } 
      else {
    
  mem[b].hh.v.RH = r; 
  mem[r].hh.v.RH = a; 
      } 
      b = hpack(b, 0, 1); 
    } 
    mem[b + 4].cint = s + d; 
    append_to_vlist(b); 
    if ((a != 0)&&(e == 0)&& ! l)
    {
      {
  mem[tail].hh.v.RH = new_penalty(10000); 
  tail = mem[tail].hh.v.RH; 
      } 
      mem[a + 4].cint = s + z - mem[a + 1].cint; 
      append_to_vlist(a); 
      g2 = 0; 
    } 
    if (t != adjust_head)
    {
      mem[tail].hh.v.RH = mem[adjust_head].hh.v.RH; 
      tail = t; 
    } 
    {
      mem[tail].hh.v.RH = new_penalty(post_display_penalty); 
      tail = mem[tail].hh.v.RH; 
    } 
    if (g2 > 0)
    {
      mem[tail].hh.v.RH = new_param_glue(g2); 
      tail = mem[tail].hh.v.RH; 
    } 
    resume_after_display(); 
  } 
}
/* sec 1200 */
void resume_after_display (void) 
{
  if (cur_group != 15) {
    confusion("display");
    return;       // abort_flag set
  }
  unsave();
  prev_graf = prev_graf + 3;
  push_nest();
  mode = 102;
  space_factor = 1000;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* was   clang = 0; etc in 3.141 new stuff follows */
  if (language <= 0)
    cur_lang = 0; 
  else if (language > 255)
    cur_lang = 0;
  else cur_lang = language;
  clang = cur_lang;
  prev_graf =(norm_min(left_hyphen_min) * 64 +
     norm_min(right_hyphen_min)) * 65536L + cur_lang; 
/* eqtb ??? hash_size ? hash_size + hash_extra ? norm_min etc */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  {
    get_x_token(); 
    if (cur_cmd != 10) back_input(); 
  } 
  if (nest_ptr == 1) {
    build_page();
  }
}
/* sec 1215 */
void get_r_token (void) 
{
lab20:
  do {
    get_token(); 
  } while (!(cur_tok != 2592));
/*  if ((cur_cs == 0)||(cur_cs > (hash_size + 514))) */  /* 95/Jan/10 */
  if ((cur_cs == 0) || (cur_cs > (hash_size + hash_extra + 514))) {
    print_err("Missing control sequence inserted");
    help5("Please don't say `\\def cs{...}', say `\\def\\cs{...}'.",
      "I've inserted an inaccessible control sequence so that your",
      "definition will be completed without mixing me up too badly.",
      "You can recover graciously from this error, if you're",
      "careful; see exercise 27.2 in The TeXbook.");
    if (cur_cs == 0)
      back_input(); 
/*    cur_tok = (hash_size + 4609);  */
/*    cur_tok = (hash_size + 4095 + 514);  */
    cur_tok = (hash_size + hash_extra + 4095 + 514); /* 96/Jan/10 */
    ins_error();
    goto lab20;
  }
}
/* sec 1229 */
void trap_zero_glue (void) 
{
  if ((mem[cur_val + 1].cint == 0) && (mem[cur_val + 2].cint == 0) &&
    (mem[cur_val + 3].cint == 0)) {
    incr(mem[0].hh.v.RH); /* mem[mem_bot]? */ /* mem[null]? */
    delete_glue_ref(cur_val);
    cur_val = 0;
  }
}
/* sec 1236 */
void do_register_command_ (small_number a) 
{/* 40 10 */ 
  halfword l, q, r, s; 
  char p; 
  q = cur_cmd; 
  {
    if (q != 89)
    {
      get_x_token(); 
      if ((cur_cmd >= 73)&&(cur_cmd <= 76)) 
      {
  l = cur_chr; 
  p = cur_cmd - 73; 
  goto lab40; 
      } 
      if (cur_cmd != 89)
      {
		  print_err("You can't use `");
  print_cmd_chr(cur_cmd, cur_chr); 
  print_string("' after ");
  print_cmd_chr(q, 0); 
  help1("I'm forgetting what you said and not changing anything.");
  error(); 
  return; 
      } 
    } 
    p = cur_chr; 
    scan_eight_bit_int(); 
    switch(p)
    {case 0 : 
      l = cur_val + (hash_size + 3218); 
      break; 
    case 1 : 
      l = cur_val + (hash_size + 3751); 
      break; 
    case 2 : 
      l = cur_val + (hash_size + 800); 
      break; 
    case 3 : 
      l = cur_val + (hash_size + 1056); 
      break; 
    } 
  } 
  lab40:; 
  if (q == 89)
  scan_optional_equals(); 
  else if (scan_keyword("by")); 
  arith_error = false; 
  if (q < 91)
  if (p < 2)
  {
    if (p == 0){
    scan_int();
  }
    else {
    scan_dimen(false, false, false);
  }
    if (q == 90)
    cur_val = cur_val + eqtb[l].cint; 
  } 
  else {
      
    scan_glue(p); 
    if (q == 90){
      q = new_spec(cur_val); 
      r = eqtb[l].hh.v.RH; 
      delete_glue_ref(cur_val); 
      mem[q + 1].cint = mem[q + 1].cint + mem[r + 1].cint; 
      if (mem[q + 2].cint == 0)
      mem[q].hh.b0 = 0; 
      if (mem[q].hh.b0 == mem[r].hh.b0)
      mem[q + 2].cint = mem[q + 2].cint + mem[r + 2].cint; 
      else if ((mem[q].hh.b0 < mem[r].hh.b0)&&(mem[r + 2]
     .cint != 0)) 
      {
  mem[q + 2].cint = mem[r + 2].cint; 
  mem[q].hh.b0 = mem[r].hh.b0; 
      } 
      if (mem[q + 3].cint == 0)
      mem[q].hh.b1 = 0; 
      if (mem[q].hh.b1 == mem[r].hh.b1)
      mem[q + 3].cint = mem[q + 3].cint + mem[r + 3].cint; 
      else if ((mem[q].hh.b1 < mem[r].hh.b1)&&(mem[r + 3]
     .cint != 0)) 
      {
  mem[q + 3].cint = mem[r + 3].cint; 
  mem[q].hh.b1 = mem[r].hh.b1; 
      } 
      cur_val = q; 
    } 
  } 
  else {
    scan_int(); 
    if (p < 2)
    if (q == 91)
    if (p == 0)
    cur_val = mult_and_add(eqtb[l].cint, cur_val, 0, 2147483647L); 
/*  2^31 - 1 */
    else cur_val = mult_and_add(eqtb[l].cint, cur_val, 0, 1073741823L); 
/*  2^30 - 1 */
    else cur_val = x_over_n(eqtb[l].cint, cur_val); 
    else {
      s = eqtb[l].hh.v.RH;  /* l may be used ... */
      r = new_spec(s); 
      if (q == 91)
      {
  mem[r + 1].cint = mult_and_add(mem[s + 1].cint, cur_val, 0, 
  1073741823L);  /* 2^30 - 1 */
  mem[r + 2].cint = mult_and_add(mem[s + 2].cint, cur_val, 0, 
  1073741823L);  /* 2^30 - 1 */
  mem[r + 3].cint = mult_and_add(mem[s + 3].cint, cur_val, 0, 
  1073741823L);  /* 2^30 - 1 */
      } 
      else {
  mem[r + 1].cint = x_over_n(mem[s + 1].cint, cur_val); 
  mem[r + 2].cint = x_over_n(mem[s + 2].cint, cur_val); 
  mem[r + 3].cint = x_over_n(mem[s + 3].cint, cur_val); 
      } 
      cur_val = r; 
    } 
  } 
  if (arith_error){
	  print_err("Arithmetic overflow");
	  help2("I can't carry out that multiplication or division,",
		  "since the result is out of range.");
/* 423 in tex82.bug */
	  if (p >= 2) delete_glue_ref(cur_val);
    error(); 
    return; 
  } 
  if (p < 2)
  if ((a >= 4)) 
  geq_word_define(l, cur_val); 
  else eq_word_define(l, cur_val); 
  else {
      
    trap_zero_glue(); 
    if ((a >= 4)) 
    geq_define(l, 117, cur_val); 
    else eq_define(l, 117, cur_val); 
  } 
} 
/* called only from itex.c */
/* sec 1243 */
void alter_aux (void) 
{
  halfword c; 
  if (cur_chr != abs(mode)) {
    report_illegal_case();
  }
  else {
    c = cur_chr; 
    scan_optional_equals(); 
    if (c == 1)
    {
      scan_dimen(false, false, false); 
      cur_list.aux_field.cint = cur_val; 
    } 
    else {
      scan_int(); 
      if ((cur_val <= 0)||(cur_val > 32767)) 
      {
		  print_err("Bad space factor");
		  help1("I allow only values in the range 1..32767 here.");
  int_error(cur_val); 
      } 
      else space_factor = cur_val; 
    } 
  } 
}
/* sec 1244 */
void alter_prev_graf (void) 
{
  integer p; 
  nest[nest_ptr]= cur_list; 
  p = nest_ptr; 
  while(abs(nest[p].mode_field)!= 1)decr(p); 
  scan_optional_equals(); 
  scan_int(); 
  if (cur_val < 0)
  {
	  print_err("Bad ");
    print_esc("prevgraf");
	help1("I allow only nonnegative values here.");
    int_error(cur_val); 
  } 
  else {
      
    nest[p].pg_field = cur_val; 
    cur_list = nest[nest_ptr]; 
  } 
}
/* sec 1245 */
void alter_page_so_far (void) 
{
  char c; 
  c = cur_chr; 
  scan_optional_equals(); 
  scan_dimen(false, false, false); 
  page_so_far[c]= cur_val; 
}
/* sec 1246 */
void alter_integer (void) 
{
  char c; 
  c = cur_chr; 
  scan_optional_equals(); 
  scan_int(); 
  if (c == 0)dead_cycles = cur_val; 
  else insert_penalties = cur_val; 
}
/* sec 1247 */
void alter_box_dimen (void) 
{ 
  small_number c; 
  eight_bits b; 
  c = cur_chr; 
  scan_eight_bit_int(); 
  b = cur_val; 
  scan_optional_equals(); 
  scan_dimen(false, false, false); 
  if (eqtb[(hash_size + 1578) + b].hh.v.RH != 0)
  mem[eqtb[(hash_size + 1578) + b].hh.v.RH + c].cint = cur_val; 
}
/* sec 1257 */
void new_font_(small_number a)
{/* 50 */ 
  halfword u; 
  scaled s; 
  internal_font_number f; 
  str_number t; 
  char old_setting; 
  str_number flushablestring; 
  if (job_name == 0) open_log_file(); 
  get_r_token(); 
  u = cur_cs; 
  if (u >= hash_base)      /* if u >= hash_base then t <- text(u); p.1257 */
    t = hash[u].v.RH; 
  else if (u >= single_base)   /* if u >= single_base then ... */
/*    if u=null_cs then t:="FONT"@+else t:=u-single_base */
  if (u == null_cs)
    t = 1213;     /* FONT */
  else t = u - single_base;   /* else t <- u - single_base */
  else {
    old_setting = selector; 
    selector = 21; 
    print_string("FONT");
    print(u - active_base); 
    selector = old_setting; 
    {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
    if (pool_ptr + 1 > current_pool_size)
      str_pool = realloc_str_pool (increment_pool_size);
    if (pool_ptr + 1 > current_pool_size) {     /* 94/Jan/24 */
      overflow("pool size", current_pool_size - init_pool_ptr); /* 97/Mar/9 */
      return;     // abort_flag set
    }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      if (pool_ptr + 1 > pool_size) {
      overflow("pool size", pool_size - init_pool_ptr); /* pool size */
      return;     // abort_flag set
    }
#endif
    } 
    t = make_string(); 
  } 
  if ((a >= 4)) geq_define(u, 87, 0); 
  else eq_define(u, 87, 0); 
  scan_optional_equals(); 
  scan_file_name(); 

/* paragraph 1258 */
  name_in_progress = true; 
  if (scan_keyword("at"))  /* at */
  {
    scan_dimen(false, false, false); 
    s = cur_val; 
    if ((s <= 0)||(s >= 134217728L)) /* 2^27 */
    {
      print_err("Improper `at' size(");
      print_scaled(s); 
      print_string("pt), replaced by 10pt");
	  help2("I can only handle fonts at positive sizes that are",
		  "less than 2048pt, so I've changed what you said to 10pt.");
      error(); 
      s = 10 * 65536L;    /* 10pt */
    } 
  } 
  else if (scan_keyword("scaled")) /* scaled */
  {
    scan_int(); 
    s = - (integer) cur_val; 
    if ((cur_val <= 0)||(cur_val > 32768L))  {
		print_err("Illegal magnification has been changed to 1000");
		help1("The magnification ratio must be between 1 and 32768.");
      int_error(cur_val); 
      s = -1000; 
    } 
  } 
  else s = -1000; 
  name_in_progress = false; 

  flushablestring = str_ptr - 1; 
  if (trace_flag) {         /* debugging stuff only 98/Oct/5 */
    int i, k1, k2, l1, l2;
    char *sch=log_line;
    k1 = str_start[cur_area];
    k2 = str_start[cur_name];
    l1 = length(cur_area);
    l2 = length(cur_name);
    show_char('\n');
    show_line("FONT ", 0);
    for (i = 0; i < l1; i++) {
      *sch++ = str_pool[i+k1];
    }
    for (i = 0; i < l2; i++) {
      *sch++ = str_pool[i+k2];
    }
    *sch++ = ' ';
    *sch++ = '\0';
    show_line(log_line, 0);
  }
/*  if (ignore_frozen) goto lab69; */     /* 98/Oct/5 */

/* paragraph 1260 for f <- fontbase+1 to font_ptr do */
  {
    register integer for_end; 
    f = 1; 
    for_end = font_ptr; 
    if (f <= for_end) 
      do 
/* if str_eq_str(font_name[f],cur_name) ^ str_eq_str(font_area[f],cur_area) */
        if (str_eq_str(font_name[f], cur_name)&&
           str_eq_str(font_area[f], cur_area))  {
          if (cur_name == flushablestring) {
            flush_string();
            cur_name = font_name[f]; 
          } 
/*    if (ignore_frozen) continue; */       /* 98/Oct/5 */
          if (s > 0) {   /* if pt size was specified */
            if (s == font_size[f]){
/*              if (ignore_frozen == 0)  */
              if (ignore_frozen == 0 || f > frozenfontptr) { /* 99/Mar/26 */
                if (trace_flag) {
                  sprintf(log_line, "SKIPPING %ld ", s);
                  show_line(log_line, 0);
                }
                goto lab50;
              }
            }
          } 
/* else if font_size[f] = xn_over_d(font_dsize[f],-s,1000) then goto common_ending */
          else if (font_size[f]== xn_over_d(font_dsize[f],
            - (integer) s, 1000)) { /* if using design size */
/*            if (ignore_frozen == 0) */
            if (ignore_frozen == 0 || f > frozenfontptr) { /* 99/Mar/26 */
              if (trace_flag) {
                sprintf(log_line, "SKIPPING %ld ", s);
                show_line(log_line, 0);
              }
              goto lab50;
            }
          }
        } 
    while(f++ < for_end);
  } 

/* lab69: */  /* 98/Oct/5 */
/* end of paragraph 1257 */
  if (trace_flag) show_line("READING ", 0);  /* debugging only */
  f = read_font_info(u, cur_name, cur_area, s); 

/* common_ending: equiv[u] <- f;  */  /* use existing font info */
  lab50:
  if (trace_flag) {
    sprintf(log_line, "NEW FONT %d ", f); /* debugging only */
    show_line(log_line, 0);
  }
  eqtb[u].hh.v.RH = f; 
/*  eqtb[(hash_size + 524) + f]= eqtb[u];*/ /* eqtb[frozen_null+font+f] */
  eqtb[(hash_size + hash_extra + 524) + f]= eqtb[u]; /* 96/Jan/10 */
#ifdef SHORTHASH            /*  debugging only  1996/Jan/20 */
  if (t > 65535L) {
    sprintf(log_line, "ERROR: %s too large %d\n",  "hash_used", t);
    show_line(log_line, 1);
  }
#endif
/*  hash[(hash_size + 524) + f].v.RH = t;  */
  hash[(hash_size + hash_extra + 524) + f].v.RH = t; /* 96/Jan/10 */
}
/* sec 1265 */
void new_interaction (void) 
{ 
  print_ln(); 
  interaction = cur_chr; 
  if (interaction == batch_mode) selector = 16; 
  else selector = 17; 
  if (log_opened) selector = selector + 2; 
}
/* sec 1270 */
void do_assignments (void) 
{/* 10 */ 
  while(true){
    do {
    get_x_token(); 
    } while(!((cur_cmd != 10)&&(cur_cmd != 0))); 
    if (cur_cmd <= 70)
    return; 
    set_box_allowed = false; 
    prefixed_command(); 
    set_box_allowed = true; 
  } 
}
/* sec 1275 */
void open_or_close_in (void) 
{
  char c; 
  char n; 
  c = cur_chr; 
  scan_four_bit_int(); 
  n = cur_val; 
  if (read_open[n]!= 2)
  {
    (void) a_close(read_file[n]); 
    read_open[n]= 2; 
  } 
  if (c != 0)
  {
    scan_optional_equals(); 
    scan_file_name(); 
    pack_file_name(cur_name, cur_area, cur_ext); 
/* *** some changes in following in 3.14159 *** */
/*  if current extension is *not* empty, try to open using name as is */
/*  string 335 is "" the empty string */
    if ((cur_ext != 335) && a_open_in(read_file[n], TEXINPUTPATH)) 
    read_open[n]= 1; 
/*  we get here if extension is "", or file with extension failed to open */
/*  if current extension is not `tex,' and `tex' is not irrelevant, try it */
/*  string 785 is  .tex */
    else if ((cur_ext != 785)&&(name_length + 5 < PATHMAX)&& 
/* *** some changes in above file name handling *** */
/*       (! extensionirrelevantp(name_of_file, "tex"))) */
       (! extensionirrelevantp(name_of_file, name_length, "tex")))
  {
      name_of_file[name_length + 1]= 46;    /* .tex  */
      name_of_file[name_length + 2]= 116; 
      name_of_file[name_length + 3]= 101; 
      name_of_file[name_length + 4]= 120; 
      name_of_file[name_length + 5]= 32; 
      name_length = name_length + 4; 
      if (a_open_in(read_file[n], TEXINPUTPATH)) 
      read_open[n]= 1; 
      else {
/*    more changes here in 3.14159 *** */
      name_length = name_length - 4;      /* remove ".tex" again */
      name_of_file[name_length + 1]= 32;  /* '  '  */
/*    string 335 is "" the empty string */
      if ((cur_ext == 335)&& a_open_in(read_file[n], TEXINPUTPATH)) 
        read_open[n]= 1; 
      else if (maketextex () && a_open_in(read_file[n], TEXINPUTPATH)) 
        read_open[n]= 1; 
    } 
    } 
  } 
}
/* sec 1279 */
void issue_message (void) 
{
  char old_setting; 
  char c; 
  str_number s; 
  c = cur_chr; 
  mem[lig_trick].hh.v.RH = scan_toks(false, true); 
  old_setting = selector; 
  selector = 21; 
  token_show(def_ref); 
  selector = old_setting; 
  flush_list(def_ref); 
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
  if (pool_ptr + 1 > current_pool_size)
      str_pool = realloc_str_pool (increment_pool_size);
  if (pool_ptr + 1 > current_pool_size){ /* in case it failed 94/Jan/24 */
    overflow("pool size", current_pool_size - init_pool_ptr); /* 97/Mar/7 */
    return;     // abort_flag set
  }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (pool_ptr + 1 > pool_size){
    overflow("pool size", pool_size - init_pool_ptr); /* pool size */
    return;     // abort_flag set
  }
#endif
  } 
  s = make_string(); 
  if (c == 0)
  {
    if (term_offset + length(s) > max_print_line - 2)
      print_ln(); 
    else if ((term_offset > 0)||(file_offset > 0))
      print_char(' ');
    slow_print(s);
#ifndef f_WINDOWS
    fflush(stdout);
#endif
  } else {
    print_err("");
    slow_print(s);
    if (err_help != 0)
      use_err_help = true;
    else if (long_help_seen)
      help1("(That was another \\errmessage.)");
    else {
      if (interaction < 3)
        long_help_seen = true;
      help4("This error message was generated by an \\errmessage",
        "command, so I can't give any explicit help.",
        "Pretend that you're Hercule Poirot: Examine all clues,",
        "and deduce the truth by order and method.");
    }
    error(); 
    use_err_help = false; 
  }
  flush_string();
}
/* sec 1288 */
void shift_case (void) 
{
  halfword b; 
  halfword p; 
  halfword t; 
  eight_bits c; 
  b = cur_chr; 
  p = scan_toks(false, false); 
  p = mem[def_ref].hh.v.RH;
  while (p != 0) {      /* while p <> null ... p.1288 */
    t = info(p); /* t <- info(p) p.1289 */ 
/*    if (t < 4352) */
    if (t < 4095 + single_base)    /* 4095 + 257 = cs_tokenflag + single_base */
    {
      c = t % 256;  
      if (eqtb[b + c].hh.v.RH != 0)
      mem[p].hh.v.LH = t - c + eqtb[b + c].hh.v.RH; 
    } 
    p = link(p); 
  } 
  begin_token_list(mem[def_ref].hh.v.RH, 3); 
  {
    mem[def_ref].hh.v.RH = avail; 
    avail = def_ref; 
  ;
#ifdef STAT
    decr(dyn_used); 
#endif /* STAT */
  } 
}
/* sec 1293 */
void show_whatever (void) 
{
  halfword p; 
  switch(cur_chr)
  {
  case 3:
    {
      begin_diagnostic();
      show_activities();
    }
    break;
  case 1:
    {
      scan_eight_bit_int();
      begin_diagnostic();
      print_nl("> \box");
      print_int(cur_val);
      print_char('=');
      if (eqtb[(hash_size + 1578) + cur_val].hh.v.RH == 0)
        print_string("void");
      else
        show_box(eqtb[(hash_size + 1578) + cur_val].hh.v.RH);
    }
    break;
  case 0:
    {
      get_token();
      if (interaction == error_stop_mode)
        ;
      print_nl("> ");
      if (cur_cs != 0)
      {
        sprint_cs(cur_cs);
        print_char('=');
      }
      print_meaning();
      goto lab50;
    }
    break;
  default:
    {
      p = the_toks();
      if (interaction == error_stop_mode)
        ; 
      print_nl(" > ");
      token_show(temp_head);
      flush_list(mem[temp_head].hh.v.RH);
      goto lab50;
    }
    break;
  }
  end_diagnostic(true);
  print_err("OK");
  if (selector == 19)
  if (tracing_online <= 0)
  {
    selector = 17;
    print_string(" (see the transcript file)");
    selector = 19;
  }
lab50:
  if (interaction < 3) {
    help_ptr = 0;
    decr(error_count);
  } else if (tracing_online > 0) {
	  help3("This isn't an error message; I'm just \\showing something.",
		  "Type `I\\show...' to show more (e.g., \\show\\cs,",
		  "\\showthe\\count10, \\showbox255, \\showlists).");
  } else {
    help5("This isn't an error message; I'm just \\showing something.",
      "Type `I\\show...' to show more (e.g., \\show\\cs,",
      "\\showthe\\count10, \\showbox255, \\showlists).",
		  "And type `I\\tracingonline=1\\show...' to show boxes and",
		  "lists on your terminal as well as in the transcript file."); 
  }
  error();
} 
/* sec 1339 */
void new_whatsit_(small_number s, small_number w)
{
  halfword p; 
  p = get_node(w); 
  mem[p].hh.b0 = 8; 
  mem[p].hh.b1 = s; 
  mem[tail].hh.v.RH = p; 
  tail = p; 
} 
/* sec 1350 */
void new_write_whatsit_(small_number w)
{
  new_whatsit(cur_chr, w); 
  if (w != 2) {
    scan_four_bit_int();
  } else {
    scan_int(); 
    if (cur_val < 0)
      cur_val = 17; 
    else if (cur_val > 15)
      cur_val = 16; 
  }
  mem[tail + 1].hh.v.LH = cur_val;
}
/* sec 1348 */
void do_extension (void) 
{
/*  integer i, j, k;  */
  integer k; 
/*  halfword p, q, r;  */
  halfword p; 
  switch(cur_chr)
  {case 0 : 
    {
      new_write_whatsit(3); 
      scan_optional_equals(); 
      scan_file_name(); 
      mem[tail + 1].hh.v.RH = cur_name; 
      mem[tail + 2].hh.v.LH = cur_area; 
      mem[tail + 2].hh.v.RH = cur_ext; 
    } 
    break; 
  case 1 : 
    {
      k = cur_cs; 
      new_write_whatsit(2); 
      cur_cs = k; 
      p = scan_toks(false, false); 
      mem[tail + 1].hh.v.RH = def_ref; 
    } 
    break; 
  case 2 : 
    {
      new_write_whatsit(2); 
      mem[tail + 1].hh.v.RH = 0; 
    } 
    break; 
  case 3 : 
    {
      new_whatsit(3, 2); 
      mem[tail + 1].hh.v.LH = 0; 
      p = scan_toks(false, true); 
      mem[tail + 1].hh.v.RH = def_ref; 
    } 
    break; 
  case 4 : 
    {
      get_x_token(); 
      if ((cur_cmd == 59)&&(cur_chr <= 2)) 
      {
  p = tail; 
  do_extension(); 
  out_what(tail);
  flush_node_list(tail); 
  tail = p; 
  mem[p].hh.v.RH = 0; 
      } 
      else back_input(); 
    } 
    break; 
  case 5 : 
    if (abs(mode)!= 102){
    report_illegal_case();
  }
    else {
      new_whatsit(4, 2); 
      scan_int(); 
      if (cur_val <= 0) clang = 0; 
      else if (cur_val > 255)
      clang = 0; 
      else clang = cur_val; 
      mem[tail + 1].hh.v.RH = clang; 
      mem[tail + 1].hh.b0 = norm_min(left_hyphen_min); 
      mem[tail + 1].hh.b1 = norm_min(right_hyphen_min); 
    } 
    break; 
    default: 
    {
      confusion("display");
      return;       // abort_flag set
    }
    break; 
  } 
} 
/* sec 1376 */
void fix_language (void) 
{ 
/*  ASCII_code l;  */
  int l;                  /* 95/Jan/7 */
  if (language <= 0)
    l = 0; 
  else if (language > 255)
    l = 0;
  else l = language;
  if (l != clang) {
    new_whatsit(4, 2);
    mem[tail + 1].hh.v.RH = l;
    clang = l;
    mem[tail + 1].hh.b0 = norm_min(left_hyphen_min);
    mem[tail + 1].hh.b1 = norm_min(right_hyphen_min);
  }
}
/* sec 1068 */
void handle_right_brace (void) 
{ 
  halfword p, q; 
  scaled d; 
  integer f; 

  switch(cur_group)
  {case 1 : 
    unsave(); 
    break; 
  case 0 : 
    {
		  print_err("Too many }'s");
  		help2("You've closed more groups than you opened.",
  			"Such booboos are generally harmless, so keep going.");
        error();
    } 
    break; 
  case 14 : 
  case 15 : 
  case 16 : 
    extra_right_brace();
    break; 
  case 2 : 
    package(0); 
    break; 
  case 3 : 
    {
      adjust_tail = adjust_head; 
      package(0); 
    } 
    break; 
  case 4 : 
    {
      end_graf(); 
      package(0); 
    } 
    break; 
  case 5 : 
    {
      end_graf();
      package(4);
    } 
    break; 
  case 11 : 
    {
      end_graf(); 
      q = eqtb[(hash_size + 792)].hh.v.RH; 
      incr(mem[q].hh.v.RH); 
      d = split_max_depth; 
      f = floating_penalty; 
      unsave(); 
      decr(save_ptr); 
      p = vpackage(mem[head].hh.v.RH, 0, 1, 1073741823L);  /* 2^30 - 1 */
      pop_nest(); 
      if (save_stack[save_ptr + 0].cint < 255) {
        {
          mem[tail].hh.v.RH = get_node(5);
          tail = mem[tail].hh.v.RH;
        }
        mem[tail].hh.b0 = 3;
        mem[tail].hh.b1 = save_stack[save_ptr + 0].cint;
        mem[tail + 3].cint = mem[p + 3].cint + mem[p + 2].cint;
        mem[tail + 4].hh.v.LH = mem[p + 5].hh.v.RH;
        mem[tail + 4].hh.v.RH = q;
        mem[tail + 2].cint = d;
        mem[tail + 1].cint = f; 
      } else {
        {
          mem[tail].hh.v.RH = get_node(2);
          tail = mem[tail].hh.v.RH;
        }
        mem[tail].hh.b0 = 5;
        mem[tail].hh.b1 = 0;
        mem[tail + 1].cint = mem[p + 5].hh.v.RH;
        delete_glue_ref(q);
      } 
      free_node(p, 7); 
      if (nest_ptr == 0) {
        build_page();
      }
    } 
    break; 
  case 8 : 
    {
      if ((cur_input.loc_field != 0)||((cur_input.index_field != 6)&&(
      cur_input.index_field != 3)))
      {
		  print_err("Unbalanced output routine");
		  help2("Your sneaky output routine has problematic {'s and/or }'s.",
			  "I can't handle that very well; good luck.");
  error(); 
  do {
      get_token(); 
  } while(!(cur_input.loc_field == 0)); 
      } 
      end_token_list(); 
      end_graf(); 
      unsave(); 
      output_active = false; 
      insert_penalties = 0; 
      if (eqtb[(hash_size + 1833)].hh.v.RH != 0)
      {
		  print_err("Output routine didn't use all of ");
      print_esc("box");
      print_int(255); 
	  help3("Your \\output commands should empty \\box255,",
		  "e.g., by saying `\\ship_out\\box255'.",
		  "Proceed; I'll discard its present contents.");
      box_error(255); 
    }
      if (tail != head)
      {
  mem[page_tail].hh.v.RH = mem[head].hh.v.RH; 
  page_tail = tail; 
      } 
      if (mem[page_head].hh.v.RH != 0)
      {
  if (mem[contrib_head].hh.v.RH == 0)
  nest[0].tail_field = page_tail; 
  mem[page_tail].hh.v.RH = mem[contrib_head].hh.v.RH; 
  mem[contrib_head].hh.v.RH = mem[page_head].hh.v.RH; 
  mem[page_head].hh.v.RH = 0; 
  page_tail = page_head; 
      } 
      pop_nest(); 
      build_page(); 
    } 
    break; 
  case 10 : 
    build_discretionary(); 
    break; 
  case 6 : 
/* align_group: begin back_input; cur_tok:=cs_token_flag+frozen_cr; */
    {
      back_input(); 
/*      cur_tok = (hash_size + 4610);  */
/*      cur_tok = (hash_size + 4095 + 515);  */
      cur_tok = (hash_size + hash_extra + 4095 + 515);
	  print_err("Missing ");
      print_esc("cr");
      print_string("inserted");
	  help1("I'm guessing that you meant to end an alignment here.");
      ins_error(); 
    } 
    break; 
  case 7 : 
    {
      end_graf(); 
      unsave(); 
      align_peek(); 
    } 
    break; 
  case 12 : 
    {
      end_graf(); 
      unsave(); 
      save_ptr = save_ptr - 2; 
      p = vpackage(mem[head].hh.v.RH, save_stack[
      save_ptr + 1].cint, save_stack[save_ptr + 0].cint,
      1073741823L);   /* 2^30 - 1 */
      pop_nest(); 
      {
  mem[tail].hh.v.RH = new_noad(); 
  tail = mem[tail].hh.v.RH; 
      } 
      mem[tail].hh.b0 = 29; 
      mem[tail + 1].hh.v.RH = 2; 
      mem[tail + 1].hh.v.LH = p; 
    } 
    break; 
  case 13 : 
    build_choices(); 
    break; 
  case 9 : 
    {
      unsave(); 
      decr(save_ptr); 
      mem[save_stack[save_ptr + 0].cint].hh.v.RH = 3; 
      p = fin_mlist(0); 
      mem[save_stack[save_ptr + 0].cint].hh.v.LH = p; 
      if (p != 0)
      if (mem[p].hh.v.RH == 0)
      if (mem[p].hh.b0 == 16)
      {
  if (mem[p + 3].hh.v.RH == 0)
  if (mem[p + 2].hh.v.RH == 0)
  {
    mem[save_stack[save_ptr + 0].cint].hh = mem[p + 1].hh; 
    free_node(p, 4); 
  } 
      } 
      else if (mem[p].hh.b0 == 28)
      if (save_stack[save_ptr + 0].cint == tail + 1)
      if (mem[tail].hh.b0 == 16)
      {
  q = head; 
  while(mem[q].hh.v.RH != tail)q = mem[q].hh 
  .v.RH; 
  mem[q].hh.v.RH = p; 
  free_node(tail, 4); 
  tail = p; 
      } 
    } 
    break; 
    default: 
    {
      confusion("rightbrace");
      return;       // abort_flag set
    }
    break; 
  } 
} 
/* sec 1030 */
/* main control loop */
void main_control (void) 
{/* 60 21 70 80 90 91 92 95 100 101 110 111 112 120 10 */
    integer t; 
  integer bSuppress;    /* 199/Jan/5 */

  if (every_job != 0)/* everyjob */
    begin_token_list(every_job, 12); 

lab60:
  get_x_token();       /* big_switch */
lab21:
  if (interrupt != 0)
    if (OK_to_interrupt){
      back_input(); 
      {
        if (interrupt != 0){
          pause_for_instructions();
        }
      }  
      goto lab60; 
    } 
  ;
#ifdef DEBUG
  if (panicking)check_mem(false); 
#endif /* DEBUG */
  if (tracing_commands > 0)
    show_cur_cmd_chr(); 

/*  the big switch --- don't bother to test abort_flag ??? */
  switch(abs(mode)+ cur_cmd){
       case 113 : 
       case 114 : 
       case 170 : 
         goto lab70; 
         break; 
       case 118 : 
       {
         scan_char_num(); 
         cur_chr = cur_val; 
         goto lab70; 
       } 
       break; 
       case 167 : 
       {
         get_x_token(); 
         if ((cur_cmd == 11)||(cur_cmd == 12)||(cur_cmd == 68)||(
           cur_cmd == 16)) 
           cancel_boundary = true; 
         goto lab21; 
       } 
       break; 
  case 112 : 
    if (space_factor == 1000)goto lab120; 
    else {
    app_space();
  }
    break; 
  case 166 : 
  case 267 : 
    goto lab120; 
    break; 
  case 1 : 
  case 102 : 
  case 203 : 
  case 11 : 
  case 213 : 
  case 268 : 
 ; 
    break; 
  case 40 : 
  case 141 : 
  case 242 : 
    {
      do {
      get_x_token(); 
      } while(!(cur_cmd != 10)); 
      goto lab21; 
    } 
    break; 
  case 15 : 
    if (its_all_over ())return; 
    break; 
  case 23 : 
  case 123 : 
  case 224 : 
  case 71 : 
  case 172 : 
  case 273 : 
  case 39 : 
  case 45 : 
  case 49 : 
  case 150 : 
  case 7 : 
  case 108 : 
  case 209 : 
    report_illegal_case(); 
    break; 
  case 8 : 
  case 109 : 
  case 9 : 
  case 110 : 
  case 18 : 
  case 119 : 
  case 70 : 
  case 171 : 
  case 51 : 
  case 152 : 
  case 16 : 
  case 117 : 
  case 50 : 
  case 151 : 
  case 53 : 
  case 154 : 
  case 67 : 
  case 168 : 
  case 54 : 
  case 155 : 
  case 55 : 
  case 156 : 
  case 57 : 
  case 158 : 
  case 56 : 
  case 157 : 
  case 31 : 
  case 132 : 
  case 52 : 
  case 153 : 
  case 29 : 
  case 130 : 
  case 47 : 
  case 148 : 
  case 212 : 
  case 216 : 
  case 217 : 
  case 230 : 
  case 227 : 
  case 236 : 
  case 239 : 
    insert_dollar_sign(); 
    break; 
  case 37 : 
  case 137 : 
  case 238 : 
    {
      {
      mem[tail].hh.v.RH = scan_rule_spec(); 
      tail = mem[tail].hh.v.RH; 
      } 
      if (abs(mode)== 1)
      cur_list.aux_field.cint = ignore_depth; 
      else if (abs(mode)== 102)
      space_factor = 1000; 
    } 
    break; 
  case 28 : 
  case 128 : 
  case 229 : 
  case 231 : 
    append_glue(); 
    break; 
  case 30 : 
  case 131 : 
  case 232 : 
  case 233 : 
    append_kern(); 
    break; 
  case 2 : 
  case 103 : 
    new_save_level(1); 
    break; 
  case 62 : 
  case 163 : 
  case 264 : 
    new_save_level(14); 
    break; 
  case 63 : 
  case 164 : 
  case 265 : 
    if (cur_group == 14) unsave(); 
    else off_save();
    break; 
  case 3 : 
  case 104 : 
  case 205 : 
    handle_right_brace(); 
    break; 
  case 22 : 
  case 124 : 
  case 225 : 
    {
      t = cur_chr; 
      scan_dimen(false, false, false); 
      if (t == 0)scan_box(cur_val);
      else scan_box(- (integer) cur_val);
    } 
    break; 
  case 32 : 
  case 133 : 
  case 234 : 
/* scan_box(leader_flag-a_leaders+cur_chr); */
    scan_box(1073742237L + cur_chr); /* 2^30 + 513 - 100 ? */
    break; 
  case 21 : 
  case 122 : 
  case 223 : 
    begin_box(0); 
    break; 
  case 44 : 
    new_graf(cur_chr > 0); 
    break; 
  case 12 : 
  case 13 : 
  case 17 : 
  case 69 : 
  case 4 : 
  case 24 : 
  case 36 : 
  case 46 : 
  case 48 : 
  case 27 : 
  case 34 : 
  case 65 : 
  case 66 : 
    {
      back_input(); 
      new_graf(true); 
    } 
    break; 
  case 145 : 
  case 246 : 
    indent_in_hmode(); 
    break; 
  case 14 : 
    {
      normal_paragraph(); 
      if (mode > 0){
      build_page();
    }
    } 
    break; 
  case 115 : 
    {
      if (align_state < 0){
      off_save();
    }
      end_graf(); 
      if (mode == 1){
      build_page();
    }
    } 
    break; 
  case 116 : 
  case 129 : 
  case 138 : 
  case 126 : 
  case 134 : 
    head_for_vmode(); 
    break; 
  case 38 : 
  case 139 : 
  case 240 : 
  case 140 : 
  case 241 : 
    begin_insert_or_adjust(); 
    break; 
  case 19 : 
  case 120 : 
  case 221 : 
    make_mark(); 
    break; 
  case 43 : 
  case 144 : 
  case 245 : 
    append_penalty(); 
    break; 
  case 26 : 
  case 127 : 
  case 228 : 
    delete_last(); 
    break; 
  case 25 : 
  case 125 : 
  case 226 : 
    unpackage(); 
    break; 
  case 146 : 
    append_italic_correction(); 
    break; 
  case 247 : 
    {
      mem[tail].hh.v.RH = new_kern(0); 
      tail = mem[tail].hh.v.RH; 
    } 
    break; 
  case 149 : 
  case 250 : 
    append_discretionary(); 
    break; 
  case 147 : 
    make_accent(); 
    break; 
  case 6 : 
  case 107 : 
  case 208 : 
  case 5 : 
  case 106 : 
  case 207 : 
    align_error(); 
    break; 
  case 35 : 
  case 136 : 
  case 237 : 
    noalign_error(); 
    break; 
  case 64 : 
  case 165 : 
  case 266 : 
    omit_error(); 
    break; 
  case 33 : 
  case 135 : 
    init_align(); 
    break; 
  case 235 : 
    if (privileged ())
    if (cur_group == 15)init_align();
    else off_save();

    break; 
  case 10 : 
  case 111 : 
    do_endv(); 
    break; 
  case 68 : 
  case 169 : 
  case 270 : 
    cs_error(); 
    break; 
  case 105 : 
    init_math(); 
    break; 
  case 251 : 
    if (privileged ())
    if (cur_group == 15) start_eq_no(); 
    else off_save();
    break; 
  case 204 : 
    {
      {
  mem[tail].hh.v.RH = new_noad(); 
  tail = mem[tail].hh.v.RH; 
      } 
      back_input(); 
      scan_math(tail + 1); 
    } 
    break; 
  case 214 : 
  case 215 : 
  case 271 : 
    set_math_char(eqtb[(hash_size + 2907) + cur_chr].hh.v.RH); 
    break; 
  case 219 : 
    {
      scan_char_num(); 
      cur_chr = cur_val; 
      set_math_char(eqtb[(hash_size + 2907) + cur_chr].hh.v.RH); 
    } 
    break; 
  case 220 : 
    {
      scan_fifteen_bit_int(); 
      set_math_char(cur_val); 
    } 
    break; 
  case 272 : 
    set_math_char(cur_chr); 
    break; 
  case 218 : 
    {
      scan_twenty_seven_bit_int(); 
      set_math_char(cur_val / 4096);  
/*      set_math_char(cur_val >> 12); */
    } 
    break; 
  case 253 : 
    {
      {
      mem[tail].hh.v.RH = new_noad(); 
      tail = mem[tail].hh.v.RH; 
      } 
      mem[tail].hh.b0 = cur_chr; 
      scan_math(tail + 1); 
    } 
    break; 
  case 254 : 
    math_limit_switch(); 
    break; 
  case 269 : 
    math_radical(); 
    break; 
  case 248 : 
  case 249 : 
    math_ac(); 
    break; 
  case 259 : 
    {
      scan_spec(12, false); 
      normal_paragraph(); 
      push_nest(); 
      mode = -1; 
      cur_list.aux_field.cint = ignore_depth; 
      if (every_vbox != 0)/* everyvbox */
      begin_token_list(every_vbox, 11); 
    } 
    break; 
  case 256 : 
    {
      mem[tail].hh.v.RH = new_style(cur_chr); 
      tail = mem[tail].hh.v.RH; 
    } 
    break; 
  case 258 : 
    {
      {
      mem[tail].hh.v.RH = new_glue(0); 
      tail = mem[tail].hh.v.RH; 
      } 
      mem[tail].hh.b1 = 98; 
    } 
    break; 
  case 257 : 
    append_choices(); 
    break; 
  case 211 : 
  case 210 : 
    sub_sup(); 
    break; 
  case 255 : 
    math_fraction(); 
    break; 
  case 252 : 
    math_left_right(); 
    break; 
  case 206 : 
    if (cur_group == 15)after_math(); 
    else off_save();
    break; 
  case 72 : 
  case 173 : 
  case 274 : 
  case 73 : 
  case 174 : 
  case 275 : 
  case 74 : 
  case 175 : 
  case 276 : 
  case 75 : 
  case 176 : 
  case 277 : 
  case 76 : 
  case 177 : 
  case 278 : 
  case 77 : 
  case 178 : 
  case 279 : 
  case 78 : 
  case 179 : 
  case 280 : 
  case 79 : 
  case 180 : 
  case 281 : 
  case 80 : 
  case 181 : 
  case 282 : 
  case 81 : 
  case 182 : 
  case 283 : 
  case 82 : 
  case 183 : 
  case 284 : 
  case 83 : 
  case 184 : 
  case 285 : 
  case 84 : 
  case 185 : 
  case 286 : 
  case 85 : 
  case 186 : 
  case 287 : 
  case 86 : 
  case 187 : 
  case 288 : 
  case 87 : 
  case 188 : 
  case 289 : 
  case 88 : 
  case 189 : 
  case 290 : 
  case 89 : 
  case 190 : 
  case 291 : 
  case 90 : 
  case 191 : 
  case 292 : 
  case 91 : 
  case 192 : 
  case 293 : 
  case 92 : 
  case 193 : 
  case 294 : 
  case 93 : 
  case 194 : 
  case 295 : 
  case 94 : 
  case 195 : 
  case 296 : 
  case 95 : 
  case 196 : 
  case 297 : 
  case 96 : 
  case 197 : 
  case 298 : 
  case 97 : 
  case 198 : 
  case 299 : 
  case 98 : 
  case 199 : 
  case 300 : 
  case 99 : 
  case 200 : 
  case 301 : 
  case 100 : 
  case 201 : 
  case 302 : 
  case 101 : 
  case 202 : 
  case 303 : 
    prefixed_command(); 
    break; 
  case 41 : 
  case 142 : 
  case 243 : 
    {
      get_token(); 
      after_token = cur_tok; 
    } 
    break; 
  case 42 : 
  case 143 : 
  case 244 : 
    {
      get_token(); 
      save_for_after(cur_tok); 
    } 
    break; 
  case 61 : 
  case 162 : 
  case 263 : 
    open_or_close_in(); 
    break; 
  case 59 : 
  case 160 : 
  case 261 : 
    issue_message(); 
    break; 
  case 58 : 
  case 159 : 
  case 260 : 
    shift_case(); 
    break; 
  case 20 : 
  case 121 : 
  case 222 : 
    show_whatever(); 
    break; 
  case 60 : 
  case 161 : 
  case 262 : 
    do_extension(); 
    break; 
  } /* end of big switch */
  goto lab60; /*  main_loop */

lab70:
  main_s = eqtb[(hash_size + 2651) + cur_chr].hh.v.RH; 
  if (main_s == 1000)
    space_factor = 1000; 
  else if (main_s < 1000)
  {
    if (main_s > 0)
    space_factor = main_s; 
  } 
  else if (space_factor < 1000)
    space_factor = 1000; 
  else space_factor = main_s; 
  main_f = eqtb[(hash_size + 1834)].hh.v.RH; 
  bchar = font_bchar[main_f]; 
  false_bchar = font_false_bchar[main_f]; 
  if (mode > 0)
  if (language != clang)
  fix_language(); 
  {
    lig_stack = avail; 
    if (lig_stack == 0)
    lig_stack = get_avail(); 
    else {
      avail = mem[lig_stack].hh.v.RH; 
      mem[lig_stack].hh.v.RH = 0; 
  ;
#ifdef STAT
      incr(dyn_used); 
#endif /* STAT */
    } 
  } 
  mem[lig_stack].hh.b0 = main_f; 
  cur_l = cur_chr; 
  mem[lig_stack].hh.b1 = cur_l; 
  cur_q = tail; 
  if (cancel_boundary)
  {
/*  begin cancel_boundary:=false; main_k:=non_address; l.20093 */
    cancel_boundary = false; 
/* main_k:=non_address 3.14159 */
/*    main_k = font_mem_size; */    /* OK ? 1993/Nov/29 */
    main_k = non_address;     /* i.e. --- 1995/Jan/15 3.14159 */
  } 
  else main_k = bchar_label[main_f]; 
/* if main_k=non_address then goto main_loop_move+2; l.20096 */
/*  if (main_k == font_mem_size) */
  if (main_k == non_address)   /* i.e. 0 --- 1995/Jan/15 */
/* cur_r:=cur_l; cur_l:=non_char; */
  goto lab92; 
  cur_r = cur_l; 
  cur_l = 256;            /* cur_l:=non_char; */
/* goto main_lig_loop+1; l.20071 */
  goto lab111; 

/* main_loop_wrapup:@<Make a ligature node, if |ligature_present|;
  insert a null discretionary, if appropriate@>; */
/* @d wrapup(#)==if cur_l<non_char then */
/*  main_loop_wrapup */
lab80: if (cur_l < 256)
  {
/*  begin if character(tail)=qi(hyphen_char[main_f]) then
  if link(cur_q)>null then ... l.20107 */
    if (mem[tail].hh.b1 == hyphen_char[main_f])
/*    if (mem[cur_q].hh.v.RH > 0) */ /* NO! */
    if (mem[cur_q].hh.v.RH != 0) /* BUG FIX l.20107 */
    ins_disc = true; 
    if (ligature_present)
    {
      main_p = new_ligature(main_f, cur_l, mem[cur_q].hh.v.RH); 
      if (lft_hit)
      {
  mem[main_p].hh.b1 = 2; 
  lft_hit = false; 
      } 
      if (rt_hit)
      if (lig_stack == 0)
      {
  incr(mem[main_p].hh.b1); 
  rt_hit = false; 
      } 
      mem[cur_q].hh.v.RH = main_p; 
      tail = main_p; 
      ligature_present = false; 
    } 
/*   if ins_disc then l.20110 */
    if (ins_disc)
    {
      ins_disc = false; 
/*   if mode>0 then tail_append(new_disc); l.20112 */
      if (mode > 0)
      {
  mem[tail].hh.v.RH = new_disc(); 
  tail = mem[tail].hh.v.RH; 
      } 
    } 
  } 

/*  main_loop_move */
lab90:
  if (lig_stack == 0) goto lab21; 
  cur_q = tail; 
  cur_l = mem[lig_stack].hh.b1; 

lab91:
  if (!(lig_stack >= hi_mem_min)) goto lab95; 

lab92: if ((cur_chr < font_bc[main_f])||(cur_chr > font_ec[main_f])) 
  {
    char_warning(main_f, cur_chr); 
    {
      mem[lig_stack].hh.v.RH = avail; 
      avail = lig_stack; 
  ;
#ifdef STAT
      decr(dyn_used); 
#endif /* STAT */
    } 
    goto lab60; 
  } 
  main_i = font_info[char_base[main_f]+ cur_l].qqqq; 
  if (!(main_i.b0 > 0)) 
  {
    char_warning(main_f, cur_chr); 
    {
      mem[lig_stack].hh.v.RH = avail; 
      avail = lig_stack; 
  ;
#ifdef STAT
      decr(dyn_used); 
#endif /* STAT */
    } 
    goto lab60; 
  } 
  {
    link(tail) = lig_stack; 
    tail = lig_stack; 
  } 

/*  main_loop_lookahead */
lab100:
  get_next(); 
  if (cur_cmd == 11) goto lab101; 
  if (cur_cmd == 12) goto lab101; 
  if (cur_cmd == 68) goto lab101; 
  x_token(); 
  if (cur_cmd == 11) goto lab101; 
  if (cur_cmd == 12) goto lab101; 
  if (cur_cmd == 68) goto lab101; 
  if (cur_cmd == 16) {
    scan_char_num(); 
    cur_chr = cur_val; 
    goto lab101; 
  } 
  if (cur_cmd == 65) bchar = 256; 
  cur_r = bchar; 
  lig_stack = 0; 
  goto lab110; 

lab101: main_s = eqtb[(hash_size + 2651) + cur_chr].hh.v.RH; 
  if (main_s == 1000)
  space_factor = 1000; 
  else if (main_s < 1000)
  {
    if (main_s > 0)
    space_factor = main_s; 
  } 
  else if (space_factor < 1000)
  space_factor = 1000; 
  else space_factor = main_s; 
  {
    lig_stack = avail; 
    if (lig_stack == 0)
    lig_stack = get_avail(); 
    else {
      avail = mem[lig_stack].hh.v.RH; 
      mem[lig_stack].hh.v.RH = 0; 
  ;
#ifdef STAT
      incr(dyn_used); 
#endif /* STAT */
    } 
  } 
  mem[lig_stack].hh.b0 = main_f; 
  cur_r = cur_chr; 
  mem[lig_stack].hh.b1 = cur_r; 
  if (cur_r == false_bchar)
    cur_r = 256; 

// main_lig_loop:@<If there's a ligature/kern command relevant to |cur_l| and
//  |cur_r|, adjust the text appropriately; exit to |main_loop_wrapup|@>;
lab110:
/*  if char_tag(main_i)<>lig_tag then goto main_loop_wrapup; */
  if (((main_i.b2) % 4)!= 1)
    goto lab80;
/* 425 in tex82.bug */
  if (cur_r == 256)
	  goto lab80;
/*  main_k:=lig_kern_start(main_f)(main_i); */
  main_k = lig_kern_base[main_f]+ main_i.b3; 
/*  main_j:=font_info[main_k].qqqq; */
  main_j = font_info[main_k].qqqq; 
/* if skip_byte(main_j)<=stop_flag then goto main_lig_loop+2; */
  if (main_j.b0 <= 128)goto lab112; 
/* main_k:=lig_kern_restart(main_f)(main_j); */
  main_k = lig_kern_base[main_f]+ 256 * main_j.b2 + main_j.b3 + 32768L - 256 * 
 (128); 

/* main_lig_loop+1:main_j:=font_info[main_k].qqqq; */
lab111: main_j = font_info[main_k].qqqq; 

/* main_lig_loop+2:if next_char(main_j)=cur_r then l.20184 */
lab112:
/*  provide for suppression of f-ligatures 99/Jan/5 */
  bSuppress = 0;
  if (suppress_f_ligs && main_j.b1 == cur_r && main_j.b2 == 0) {
    if (cur_l == 'f') 
        bSuppress = 1;
  }

/*  if (main_j.b1 == cur_r)*/
  if (main_j.b1 == cur_r && bSuppress == 0)  /* 99/Jan/5 */
/*  if skip_byte(main_j)<=stop_flag then l.20185 */
//   @<Do ligature or kern command, returning to |main_lig_loop|
//    or |main_loop_wrapup| or |main_loop_move|@>;
  if (main_j.b0 <= 128)
  {
/* begin if op_byte(main_j)>=kern_flag then l.20225 */
    if (main_j.b2 >= 128)
    {
/* @d wrapup(#)==if cur_l<non_char then */
      if (cur_l < 256)
      {
/* if character(tail)=qi(hyphen_char[main_f]) then if link(cur_q)>null */
  if (mem[tail].hh.b1 == hyphen_char[main_f])
/*  if (mem[cur_q].hh.v.RH > 0) */ /* 94/Mar/22 ?????????????? */
  if (mem[cur_q].hh.v.RH != 0) /* BUG FIX l.20107l.20186  */
  ins_disc = true; 
/*   if ligature_present then pack_lig(#); */
  if (ligature_present)
  {
    main_p = new_ligature(main_f, cur_l, mem[cur_q].hh.v.RH); 
    if (lft_hit)
    {
      mem[main_p].hh.b1 = 2; 
      lft_hit = false; 
    } 
    if (rt_hit)
    if (lig_stack == 0)
    {
      incr(mem[main_p].hh.b1); 
      rt_hit = false; 
    } 
    mem[cur_q].hh.v.RH = main_p; 
    tail = main_p; 
    ligature_present = false; 
  } 
  if (ins_disc)
  {
    ins_disc = false; 
    if (mode > 0)
    {
      mem[tail].hh.v.RH = new_disc(); 
      tail = mem[tail].hh.v.RH; 
    } 
  } 
      } 
      {
  mem[tail].hh.v.RH = new_kern(font_info[kern_base[
  main_f]+ 256 * main_j.b2 + main_j.b3].cint); 
  tail = mem[tail].hh.v.RH; 
      } 
      goto lab90; 
    } 
/* begin if cur_l=non_char then lft_hit:=true; */
    if (cur_l == 256)
    lft_hit = true; 
    else if (lig_stack == 0)
    rt_hit = true; 
    {
      if (interrupt != 0){
      pause_for_instructions();
    }
    } 
    switch(main_j.b2)
    {case 1 : 
    case 5 : 
      {
  cur_l = main_j.b3; 
  main_i = font_info[char_base[main_f]+ cur_l].qqqq; 
  ligature_present = true; 
      } 
      break; 
    case 2 : 
    case 6 : 
      {
  cur_r = main_j.b3; 
  if (lig_stack == 0)
  {
    lig_stack = new_lig_item(cur_r); 
    bchar = 256; 
  } 
  else if ((lig_stack >= hi_mem_min)) 
  {
    main_p = lig_stack; 
    lig_stack = new_lig_item(cur_r); 
    mem[lig_stack + 1].hh.v.RH = main_p; 
  } 
  else mem[lig_stack].hh.b1 = cur_r; 
      } 
      break; 
    case 3 : 
      {
  cur_r = main_j.b3; 
  main_p = lig_stack; 
  lig_stack = new_lig_item(cur_r); 
  mem[lig_stack].hh.v.RH = main_p; 
      } 
      break; 
    case 7 : 
    case 11 : 
      {
  if (cur_l < 256) /* if cur_l<non_char then  */
/*  begin if character(tail)=qi(hyphen_char[main_f]) then if link(cur_q)>null
then */
  {
    if (mem[tail].hh.b1 == hyphen_char[main_f])
/*    if (mem[cur_q].hh.v.RH > 0) */  /* 94/Mar/22 */
    if (mem[cur_q].hh.v.RH != 0) /* BUG FIX ???????????? */
    ins_disc = true; 
    if (ligature_present)
    {
      main_p = new_ligature(main_f, cur_l, mem[cur_q].hh.v.RH); 
      if (lft_hit)
      {
        mem[main_p].hh.b1 = 2; 
        lft_hit = false; 
      } 
/*      if (false)
      if (lig_stack == 0){
        incr(mem[main_p].hh.b1); 
        rt_hit = false; 
      } */              /* removed 99/Jan/6 */
    mem[cur_q].hh.v.RH = main_p; 
      tail = main_p; 
      ligature_present = false; 
    } 
    if (ins_disc)
    {
      ins_disc = false; 
      if (mode > 0)
      {
        mem[tail].hh.v.RH = new_disc(); 
        tail = mem[tail].hh.v.RH; 
      } 
    } 
  } 
  cur_q = tail; 
  cur_l = main_j.b3; 
  main_i = font_info[char_base[main_f]+ cur_l].qqqq; 
  ligature_present = true; 
      } 
      break; 
      default: 
      {
  cur_l = main_j.b3; 
  ligature_present = true; 
  if (lig_stack == 0)
  goto lab80; 
  else goto lab91; 
      } 
      break; 
    } 
    if (main_j.b2 > 4)
    if (main_j.b2 != 7)
    goto lab80; 
    if (cur_l < 256)
    goto lab110; 
    main_k = bchar_label[main_f]; 
    goto lab111; 
  } 
  if (main_j.b0 == 0)
  incr(main_k); 
  else {
    if (main_j.b0 >= 128)
    goto lab80; 
    main_k = main_k + main_j.b0 + 1; 
  } 
  goto lab111; 

/*  main_move_log */
lab95: main_p = mem[lig_stack + 1].hh.v.RH; 
/* if main_p>null then tail_append(main_p); l.20137 */
/*  if (main_p > 0) */ /* 92/Mar/22 */
  if (main_p != 0)     /* BUG FIX */
  {
    mem[tail].hh.v.RH = main_p; 
    tail = mem[tail].hh.v.RH; 
  } 
  temp_ptr = lig_stack; 
  lig_stack = mem[temp_ptr].hh.v.RH; 
  free_node(temp_ptr, 2); 
  main_i = font_info[char_base[main_f]+ cur_l].qqqq; 
  ligature_present = true; 
  if (lig_stack == 0)
/*   if main_p>null then goto main_loop_lookahead l.20142 */
/*  if (main_p > 0) */ /* 94/Mar/2 */
  if (main_p != 0)   /* BUG FIX */
  goto lab100; 
  else cur_r = bchar; 
  else cur_r = mem[lig_stack].hh.b1; 
  goto lab110; 

/*  append_normal_space */
lab120: if (eqtb[(hash_size + 794)].hh.v.RH == 0)
  {
    {
      main_p = font_glue[eqtb[(hash_size + 1834)].hh.v.RH]; 
      if (main_p == 0)
      {
  main_p = new_spec(0); 
  main_k = param_base[eqtb[(hash_size + 1834)].hh.v.RH]+ 2; 
  mem[main_p + 1].cint = font_info[main_k].cint; 
  mem[main_p + 2].cint = font_info[main_k + 1].cint; 
  mem[main_p + 3].cint = font_info[main_k + 2].cint; 
  font_glue[eqtb[(hash_size + 1834)].hh.v.RH]= main_p; 
      } 
    } 
    temp_ptr = new_glue(main_p); 
  } 
  else temp_ptr = new_param_glue(12); 
  mem[tail].hh.v.RH = temp_ptr; 
  tail = temp_ptr; 
  goto lab60; 
} /* end of main_control */
/* give_err_help etc followed here in the old tex8.c */
