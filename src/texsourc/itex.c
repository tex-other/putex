/* Copyright 1992 Karl Berry
   Copyright 2007 TeX Users Group
   Copyright 2014 Clerk Ma

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA.  */

#ifdef _WINDOWS
  #define NOCOMM
  #define NOSOUND
  #define NODRIVERS
  #define STRICT
  #pragma warning(disable:4115)	// kill rpcasync.h complaint
  #include <windows.h>
  #define MYLIBAPI __declspec(dllexport)
#endif

#include "texwin.h"

#pragma warning(disable:4996)
#pragma warning(disable:4131)	// old style declarator
#pragma warning(disable:4135)	// conversion between different integral types 
#pragma warning(disable:4127)	// conditional expression is constant

#include <setjmp.h>

#define EXTERN extern

#include "texd.h"

#pragma warning(disable:4244)				/* 96/Jan/10 */

#include <time.h>							// needed for clock_t

/* #define CHECKPOOL */						/* debugging only */

#define ENDFMTCHECKSUM 69069L

#define BEGINFMTCHECKSUM 367403084L

extern clock_t start_time, main_time, finish_time;	/* in local.c */

/* imported from pascal.h */
/* localized here to avoid conflict with io.h in other code */

#define read(f, b)	((b) = getc (f)) 
#define	readln(f)   { register int c; while ((c = getc (f)) != '\n' && c != EOF); } 
/* #define	readln(f)	{ while (getc (f) >= ' '); } ? use fgets ? */

#ifdef INITEX
  void do_initex (void);					/* later in this file */
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

void initialize (void) 
{
  integer i; 
  integer k; 
  integer flag;						/* bkph */
#ifndef ALLOCATEHYPHEN
  hyph_pointer z;  
#endif

/* We already initialized xchr if we are using non_ascii setup */

if (!non_ascii) {

/*	Normally, there is no translation of character code numbers */

  {
	  register integer for_end;
	  i = 0;
	  for_end = 255;
	  if(i <= for_end)
		  do 
/*			  xchr[i]= chr(i);  */
			  xchr[i]= (char) i;						/* 95/Jan/7 */
	  while(i++ < for_end);
  } 

/* This is a kind of joke, since the characters are given as numeric codes ! */

#ifdef JOKE
  xchr[32]= ' '; 
  xchr[33]= '!'; 
  xchr[34]= '"'; 
  xchr[35]= '#'; 
  xchr[36]= '$'; 
  xchr[37]= '%'; 
  xchr[38]= '&'; 
  xchr[39]= '\''; 
  xchr[40]= '('; 
  xchr[41]= ')'; 
  xchr[42]= '*'; 
  xchr[43]= '+'; 
  xchr[44]= ','; 
  xchr[45]= '-'; 
  xchr[46]= '.'; 
  xchr[47]= '/'; 
  xchr[48]= '0'; 
  xchr[49]= '1'; 
  xchr[50]= '2'; 
  xchr[51]= '3'; 
  xchr[52]= '4'; 
  xchr[53]= '5'; 
  xchr[54]= '6'; 
  xchr[55]= '7'; 
  xchr[56]= '8'; 
  xchr[57]= '9'; 
  xchr[58]= ':'; 
  xchr[59]= ';'; 
  xchr[60]= '<'; 
  xchr[61]= '='; 
  xchr[62]= '>'; 
  xchr[63]= '?'; 
  xchr[64]= '@'; 
  xchr[65]= 'A'; 
  xchr[66]= 'B'; 
  xchr[67]= 'C'; 
  xchr[68]= 'D'; 
  xchr[69]= 'E'; 
  xchr[70]= 'F'; 
  xchr[71]= 'G'; 
  xchr[72]= 'H'; 
  xchr[73]= 'I'; 
  xchr[74]= 'J'; 
  xchr[75]= 'K'; 
  xchr[76]= 'L'; 
  xchr[77]= 'M'; 
  xchr[78]= 'N'; 
  xchr[79]= 'O'; 
  xchr[80]= 'P'; 
  xchr[81]= 'Q'; 
  xchr[82]= 'R'; 
  xchr[83]= 'S'; 
  xchr[84]= 'T'; 
  xchr[85]= 'U'; 
  xchr[86]= 'V'; 
  xchr[87]= 'W'; 
  xchr[88]= 'X'; 
  xchr[89]= 'Y'; 
  xchr[90]= 'Z'; 
  xchr[91]= '['; 
  xchr[92]= '\\'; 
  xchr[93]= ']'; 
  xchr[94]= '^'; 
  xchr[95]= '_'; 
  xchr[96]= '`'; 
  xchr[97]= 'a'; 
  xchr[98]= 'b'; 
  xchr[99]= 'c'; 
  xchr[100]= 'd'; 
  xchr[101]= 'e'; 
  xchr[102]= 'f'; 
  xchr[103]= 'g'; 
  xchr[104]= 'h'; 
  xchr[105]= 'i'; 
  xchr[106]= 'j'; 
  xchr[107]= 'k'; 
  xchr[108]= 'l'; 
  xchr[109]= 'm'; 
  xchr[110]= 'n'; 
  xchr[111]= 'o'; 
  xchr[112]= 'p'; 
  xchr[113]= 'q'; 
  xchr[114]= 'r'; 
  xchr[115]= 's'; 
  xchr[116]= 't'; 
  xchr[117]= 'u'; 
  xchr[118]= 'v'; 
  xchr[119]= 'w'; 
  xchr[120]= 'x'; 
  xchr[121]= 'y'; 
  xchr[122]= 'z'; 
  xchr[123]= '{'; 
  xchr[124]= '|'; 
  xchr[125]= '}'; 
  xchr[126]= '~'; 

  {
	  register integer for_end;
	  i = 0;
	  for_end = 31;
	  if(i <= for_end)
		  do  xchr[i]= chr(i); 
	  while(i++ < for_end);
  } 

  {
	  register integer for_end;
	  i = 127;
	  for_end = 255;
	  if(i <= for_end)
		  do xchr[i]= chr(i); 
	  while(i++ < for_end);
  }
  #endif /* end of JOKE */
} /* end of plain ASCII case (otherwise have read in xchr vector before) */

/*	Fill in background of `delete' for inverse mapping */

  {
	  register integer for_end;
	  i = 0;
	  for_end = 255;
	  if(i <= for_end)
		  do xord[chr(i)] = 127; 
	  while(i++ < for_end);
  } 

#ifdef JOKE
  {
	  register integer for_end;
	  i = 128;
	  for_end = 255;
	  if(i <= for_end)
		  do xord[xchr[i]] = i; 
	  while(i++ < for_end);
  } 

  {
	  register integer for_end;
	  i = 0;
	  for_end = 126;
	  if(i <= for_end)
		  do xord[xchr[i]] = i; 
	  while(i++ < for_end);
  } 
#endif
  
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*	Now invert the xchr mapping to get xord mapping */

  {
	  register integer for_end;
	  i = 0;
	  for_end = 255;
	  if(i <= for_end)
		  do xord[xchr[i]] = (char) i;  
	  while(i++ < for_end);
  } 

  xord[127]= 127;		/* hmm, a fixed point ? why ? */

  flag = 0;					/* 93/Dec/28 - bkph */
  if (trace_flag != 0) {
	  for (k = 0; k < 256; k++)		/*  entries in xord / xchr */
		  if (xord[k]!= k){
			  flag = 1;
			  break;
		  }
	  if (flag) {		/* 127 here means mapping undefined */
		  show_line("Inverted mapping xord[] pairs:\n", 0);
		  for (k = 0; k < 256; k++) {	/*  entries in xord / xchr */
			  if (xord[k]!= 127) {
				  sprintf(log_line, "%d => %d\n", k, xord[k]);
				  show_line(log_line, 0);
			  }
		  }
	  }
  }
  if (interaction < 0)				/* may now set in local.c bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  interaction = 3; 
  deletions_allowed = true; 
  set_box_allowed = true; 
  error_count = 0; 
  help_ptr = 0; 
  use_err_help = false; 
  interrupt = 0; 
  OK_to_interrupt = true; 
	;
/* darn, need to know mem_top, mem_max etc for the following ... */
#ifdef DEBUG
  wasmemend = mem_min; 
/*  waslomax = mem_min;  */
  waslomax = mem_bot; 
/*  washimin = mem_max; */
  washimin = mem_top; 
  panicking = false; 
#endif /* DEBUG */
  nest_ptr = 0; 
  max_nest_stack = 0; 
  mode = 1; 
  head = mem_top - 1; 
  tail = mem_top - 1; 
  cur_list.aux_field.cint = ignore_depth; 
  mode_line = 0; 
/* *********************************************************************** */
/*  cur_list .lhmfield = 0; */ /* removed in 3.14159 */
/*  cur_list .rhmfield = 0; */ /* removed in 3.14159 */
/* *********************************************************************** */
  prev_graf = 0; 
  shown_mode = 0; 
  page_contents = 0; 
  page_tail = mem_top - 2; 
#ifdef ALLOCATEMAIN
  if (is_initex)			/* in iniTeX we did already allocate mem [] */
#endif
	  mem[mem_top - 2].hh.v.RH = 0; 
/*  last_glue = 262143L;  */ /* NO! */
  last_glue = empty_flag; 
  last_penalty = 0; 
  last_kern = 0; 
  page_so_far[7]= 0; 
  page_max_depth = 0; 
/* from int_base to eqtb_size */
  {
	  register integer for_end;
	  k = (hash_size + 3163);
	  for_end = (hash_size + 4006);
	  if(k <= for_end) 
		  do  xeq_level[k]= 1; 
	  while(k++ < for_end);
  } 
  no_new_control_sequence = true; 
  hash[514].v.LH = 0;			/* next(hash_base):= 0 */
  hash[514].v.RH = 0;			/* text(hash_base):= 0 */
/* 514 + hash_size + 266 = hashbase + hashsize + 10 + font_max  */
/* for k <- hashbase+1 to undefined_control_sequence - 1 do ... p.257 */
  {
	  register integer for_end;
	  k = 515;
	  for_end = (hash_size + 780);
	  if(k <= for_end) 
		  do hash[k]= hash[514]; 
	  while(k++ < for_end);
  } 
  save_ptr = 0; 
  cur_level = 1; 
  cur_group = 0; 
  cur_boundary = 0; 
  max_save_stack = 0; 
  mag_set = 0; 
  cur_mark[0]= 0; 
  cur_mark[1]= 0; 
  cur_mark[2]= 0; 
  cur_mark[3]= 0; 
  cur_mark[4]= 0; 
  cur_val = 0; 
  cur_val_level = 0; 
  radix = 0; 
  cur_order = 0; 
  {
	  register integer for_end;
	  k = 0;
	  for_end = 16;
	  if(k <= for_end)
		  do read_open[k]= 2; 
	  while(k++ < for_end);
  } 
  cond_ptr = 0; 
  if_limit = 0; 
  cur_if = 0; 
  if_line = 0; 
  {
	  register integer for_end;
	  k = 0;
	  for_end = font_max;
	  if(k <= for_end) 
		  do font_used[k]= false; 
	  while(k++ < for_end);
  } 
  null_character.b0 = 0; 
  null_character.b1 = 0; 
  null_character.b2 = 0; 
  null_character.b3 = 0; 
  total_pages = 0; 
  max_v = 0; 
  max_h = 0; 
  max_push = 0; 
  last_bop = -1; 
  doing_leaders = false; 
  dead_cycles = 0; 
  cur_s = -1; 
  half_buf = dvi_buf_size / 2; 
  dvi_limit = dvi_buf_size; 
  dvi_ptr = 0; 
  dvi_offset = 0; 
  dvi_gone = 0; 
/* down_ptr:=null; right_ptr:=null; l.12027 */
  down_ptr = 0; 
  right_ptr = 0; 
  adjust_tail = 0; 
  last_badness = 0; 
  pack_begin_line = 0; 
  empty_field.v.RH = 0; 
  empty_field.v.LH = 0; 
  null_delimiter.b0 = 0; 
  null_delimiter.b1 = 0; 
  null_delimiter.b2 = 0; 
  null_delimiter.b3 = 0; 
  align_ptr = 0; 
  cur_align = 0; 
  cur_span = 0; 
  cur_loop = 0; 
  cur_head = 0; 
  cur_tail = 0; 
/*	*not* OK with ALLOCATEHYPHEN, since may not be allocated yet */
#ifndef ALLOCATEHYPHEN
/*  {register integer for_end; z = 0; for_end = 607; if(z <= for_end) do */
  {
	  register integer for_end;
	  z = 0;
	  for_end = hyphen_prime;
	  if(z <= for_end) do 
	  {
		  hyph_word[z]= 0; 
		  hyph_list[z]= 0; 
	  } 
	  while(z++ < for_end);
  } 
#endif
  hyph_count = 0; 
  output_active = false; 
  insert_penalties = 0; 
  ligature_present = false; 
  cancel_boundary = false; 
  lft_hit = false; 
  rt_hit = false; 
  ins_disc = false; 
  after_token = 0; 
  long_help_seen = false; 
  format_ident = 0; 
  {
	  register integer for_end;
	  k = 0;
	  for_end = 17;
	  if(k <= for_end)
		  do  write_open[k]= false; 
	  while(k++ < for_end);
  } 
  edit_name_start = 0; 
	;
#ifdef INITEX
/* initex stuff split off for convenience of optimization adjustments */
  if (is_initex)  do_initex ();
#else
/* trap the -i on command line situation if INITEX was NOT defined */
  if (is_initex) {
	  show_line("Sorry, somebody forgot to make an INITEX!\n", 1);
  }
#endif /* not INITEX */
} 

/* do the part of initialize() that requires mem_top, mem_max or mem[] */
/* do this part after allocating main memory */

#ifdef ALLOCATEMAIN
void initialize_aux(void) {
								/* ??? */
#ifdef DEBUG
  wasmemend = mem_min; 
/*  waslomax = mem_min; */
  waslomax = mem_bot; 
/*  washimin = mem_max;  */
  washimin = mem_top; 
  panicking = false; 
#endif /* DEBUG */
/*  nest_ptr = 0; */
/*  max_nest_stack = 0; */
  mode = 1; 
  head = mem_top - 1; 
  tail = mem_top - 1; 
  cur_list.aux_field.cint = ignore_depth; 
  mode_line = 0; 
  prev_graf = 0; 
/*  shown_mode = 0;  */
/*  page_contents = 0;  */
  page_tail = mem_top - 2; 
  //mem[mem_top - 2].hh.v.RH = 0;
  link(mem_top-2) = 0;
}
#endif	// end of ifdef ALLOCATEMAIN

void line_break_ (integer finalwidowpenalty)
{/* 30 31 32 33 34 35 22 */
  bool autobreaking; 
  halfword prevp; 
  halfword q, r, s, prevs; 
  internal_font_number f; 
/*  small_number j;  */
  int j;								/* 95/Jan/7 */
/*  unsigned char c;  */
  unsigned int c;						/* 95/Jan/7 */

/*  savedbadness = 0; */				/* 96/Feb/9 */
  pack_begin_line = mode_line; 
  mem[mem_top - 3].hh.v.RH = mem[head].hh.v.RH; 
  if((tail >= hi_mem_min)) 
  {
    mem[tail].hh.v.RH = new_penalty(10000); 
    tail = mem[tail].hh.v.RH; 
  } 
  else if(mem[tail].hh.b0 != 10)
  {
    mem[tail].hh.v.RH = new_penalty(10000); 
    tail = mem[tail].hh.v.RH; 
  } 
  else {
      
    mem[tail].hh.b0 = 12; 
    delete_glue_ref(mem[tail + 1].hh.v.LH); 
    flush_node_list(mem[tail + 1].hh.v.RH); 
    mem[tail + 1].cint = 10000; 
  } 
  mem[tail].hh.v.RH = new_param_glue(14); 
/* *********************************************************************** */
/* following is new in 3.14159 */
  init_cur_lang = prev_graf % 65536L; 
/*  init_cur_lang = prev_graf & 65535L; */	/* last 16 bits */
  init_l_hyf = prev_graf / 4194304L;			/* 2^22 */
/*  init_l_hyf = prev_graf >> 22; */			/* top 10 bits */
  init_r_hyf =(prev_graf / 65536L)% 64; 
/*  init_r_hyf =(prev_graf >> 16)& 65; */ /* next to last 6 bits */
/* *********************************************************************** */
  pop_nest (); 
  noshrinkerroryet = true; 
  if((mem[eqtb[(hash_size + 789)].hh.v.RH].hh.b1 != 0)&&
	  (mem[eqtb[(hash_size + 789)].hh.v.RH + 3].cint != 0)) 
  {
    eqtb[(hash_size + 789)].hh.v.RH =
		finite_shrink(eqtb[(hash_size + 789)].hh.v.RH); 
  } 
  if((mem[eqtb[(hash_size + 790)].hh.v.RH].hh.b1 != 0)&&
	  (mem[eqtb[(hash_size + 790)].hh.v.RH + 3].cint != 0)) 
  {
    eqtb[(hash_size + 790)].hh.v.RH =
		finite_shrink(eqtb[(hash_size + 790)].hh.v.RH); 
  } 
  q = eqtb[(hash_size + 789)].hh.v.RH; 
  r = eqtb[(hash_size + 790)].hh.v.RH; 
  background[1]= mem[q + 1].cint + mem[r + 1].cint; 
  background[2]= 0; 
  background[3]= 0; 
  background[4]= 0; 
  background[5]= 0; 
  background[2 + mem[q].hh.b0]= mem[q + 2].cint; 
  background[2 + mem[r].hh.b0]= background[2 + mem[r].hh.b0]+ 
  mem[r + 2].cint; 
  background[6]= mem[q + 3].cint + mem[r + 3].cint; 
  minimum_demerits = 1073741823L;		/* 2^30 - 1 */	/* 40000000 hex - 1 */
  minimal_demerits[3]= 1073741823L; 
  minimal_demerits[2]= 1073741823L; 
  minimal_demerits[1]= 1073741823L; 
  minimal_demerits[0]= 1073741823L; 
  if(eqtb[(hash_size + 1312)].hh.v.RH == 0)
  if(eqtb[(hash_size + 3747)].cint == 0)
  {
    last_special_line = 0; 
    second_width = eqtb[(hash_size + 3733)].cint; 
    second_indent = 0; 
  } 
  else {
      
    last_special_line = abs(eqtb[(hash_size + 3204)].cint); 
    if(eqtb[(hash_size + 3204)].cint < 0)
    {
      first_width = eqtb[(hash_size + 3733)].cint -
		  abs(eqtb[(hash_size + 3747)].cint); 
      if(eqtb[(hash_size + 3747)].cint >= 0)
      first_indent = eqtb[(hash_size + 3747)].cint; 
      else first_indent = 0; 
      second_width = eqtb[(hash_size + 3733)].cint; 
      second_indent = 0; 
    } 
    else {
	
      first_width = eqtb[(hash_size + 3733)].cint; 
      first_indent = 0; 
      second_width = eqtb[(hash_size + 3733)].cint -
		   abs(eqtb[(hash_size + 3747)].cint); 
      if(eqtb[(hash_size + 3747)].cint >= 0)
      second_indent = eqtb[(hash_size + 3747)].cint; 
      else second_indent = 0; 
    } 
  } 
  else {
      
    last_special_line = mem[eqtb[(hash_size + 1312)].hh.v.RH].hh.v.LH - 1; 
    second_width = mem[eqtb[(hash_size + 1312)].hh.v.RH + 2 *(last_special_line + 1)
  ].cint; 
    second_indent = mem[eqtb[(hash_size + 1312)].hh.v.RH + 2 * last_special_line + 1]
   .cint; 
  } 
/* if looseness=0 then easy_line:=last_special_line */
  if(eqtb[(hash_size + 3182)].cint == 0)
  easyline = last_special_line; 
/*  else easyline = 262143L;  */ /* NO! */
  else easyline = empty_flag; 
/* threshold:=pretolerance; */
  threshold = eqtb[(hash_size + 3163)].cint; 
  if(threshold >= 0)
  {
	;
#ifdef STAT
/*   if tracing_paragraphs>0 then */
    if(eqtb[(hash_size + 3195)].cint > 0)
    {
      begin_diagnostic (); 
      print_nl("@firstpass");
    } 
#endif /* STAT */
    second_pass = false; 
    final_pass = false; 
	first_pass_count++;					/* 96 Feb 9 */
  } 
  else {
/*  threshold:=tolerance; second_pass:=true; */      
    threshold = eqtb[(hash_size + 3164)].cint; 
    second_pass = true; 
/*   final_pass:=(emergency_stretch<=0); */
    final_pass =(eqtb[(hash_size + 3750)].cint <= 0); 
	;
#ifdef STAT
    if(eqtb[(hash_size + 3195)].cint > 0)
    begin_diagnostic (); 
#endif /* STAT */
  } 
  while(true){
/*  if threshold>inf_bad then threshold:=inf_bad; */
    if(threshold > 10000)
		threshold = 10000; 
    if(second_pass)
    {
	;
#ifdef INITEX
  if (is_initex)  {	/* bkph */
      if(trie_not_ready)init_trie (); 
  }					/* bkph */
#endif /* INITEX */
/* ********************************************************************* */
/* following has changed in 3.14159 */
/*      cur_lang = 0;  */
      cur_lang = init_cur_lang; 
/*      lhyf = cur_list .lhmfield; */
      lhyf = init_l_hyf; 
/*      rhyf = cur_list .rhmfield;  */
      rhyf = init_r_hyf; 
/* ********************************************************************* */
    } 
    q = get_node(3); 
    mem[q].hh.b0 = 0; 
    mem[q].hh.b1 = 2; 
    mem[q].hh.v.RH = mem_top - 7; 
    mem[q + 1].hh.v.RH = 0; 
    mem[q + 1].hh.v.LH = prev_graf + 1; 
    mem[q + 2].cint = 0; 
    mem[mem_top - 7].hh.v.RH = q; 
    active_width[1]= background[1]; 
    active_width[2]= background[2]; 
    active_width[3]= background[3]; 
    active_width[4]= background[4]; 
    active_width[5]= background[5]; 
    active_width[6]= background[6]; 
/*	passive:=null; printed_node:=temp_head; pass_number:=0; */
    passive = 0; 
    printed_node = mem_top - 3; 
    pass_number = 0; 
    font_in_short_display = 0; 
    cur_p = mem[mem_top - 3].hh.v.RH; 
    autobreaking = true; 
    prevp = cur_p; 
/*  while (cur_p<>null)and(link(active)<>last_active) do */
    while((cur_p != 0)&&(mem[mem_top - 7].hh.v.RH != mem_top - 7)) 
    {
      if((cur_p >= hi_mem_min)) 
      {
	prevp = cur_p; 
	do {
	    f = mem[cur_p].hh.b0; 
	  active_width[1]= active_width[1]+ font_info[width_base[f]+ 
	  font_info[char_base[f]+ mem[cur_p].hh.b1].qqqq.b0].cint 
	; 
	  cur_p = mem[cur_p].hh.v.RH; 
	} while(!(!(cur_p >= hi_mem_min))); 
      } 
      switch(mem[cur_p].hh.b0)
      {case 0 : 
      case 1 : 
      case 2 : 
	active_width[1]= active_width[1]+ mem[cur_p + 1].cint; 
	break; 
      case 8 : 
	if(mem[cur_p].hh.b1 == 4)
	{
	  cur_lang = mem[cur_p + 1].hh.v.RH;	/* ASCII_code */
	  lhyf = mem[cur_p + 1].hh.b0; 
	  rhyf = mem[cur_p + 1].hh.b1; 
	} 
	break; 
      case 10 : 
	{
	  if(autobreaking)
	  {
	    if((prevp >= hi_mem_min)) 
	    try_break(0, 0); 
	    else if((mem[prevp].hh.b0 < 9)) 
	    try_break(0, 0); 
/* *********************************************************************** */
/* following is new in 3.14159 */
	    else if((mem[prevp].hh.b0 == 11)&&(mem[prevp].hh.b1 
	    != 1)) 
	    try_break(0, 0); 
/* *********************************************************************** */
	  } 
	  if((mem[mem[cur_p + 1].hh.v.LH].hh.b1 != 0)&&(mem[
	  mem[cur_p + 1].hh.v.LH + 3].cint != 0)) 
	  {
	    mem[cur_p + 1].hh.v.LH = finite_shrink(mem[cur_p + 1].hh.v.LH); 
	  } 
	  q = mem[cur_p + 1].hh.v.LH; 
	  active_width[1]= active_width[1]+ mem[q + 1].cint; 
	  active_width[2 + mem[q].hh.b0]= active_width[2 + mem[q]
	  .hh.b0]+ mem[q + 2].cint; 
	  active_width[6]= active_width[6]+ mem[q + 3].cint; 
	  if(second_pass && autobreaking)
	  {
	    prevs = cur_p; 
	    s = mem[prevs].hh.v.RH; 
	    if(s != 0)
	    {
	      while(true){
		if((s >= hi_mem_min)) 
		{
		  c = mem[s].hh.b1; 
		  hf = mem[s].hh.b0; 
		} 
		else if(mem[s].hh.b0 == 6)
		if(mem[s + 1].hh.v.RH == 0)
		goto lab22; 
		else {
		    
		  q = mem[s + 1].hh.v.RH; 
		  c = mem[q].hh.b1; 
		  hf = mem[q].hh.b0; 
		} 
		else if((mem[s].hh.b0 == 11)&&(mem[s].hh.b1 == 
		0)) 
		goto lab22; 
		else if(mem[s].hh.b0 == 8)
		{
		  if(mem[s].hh.b1 == 4)
		  {
		    cur_lang = mem[s + 1].hh.v.RH; 	/* ASCII_code */
		    lhyf = mem[s + 1].hh.b0; 
		    rhyf = mem[s + 1].hh.b1; 
		  } 
		  goto lab22; 
		} 
		else goto lab31; 
		if(eqtb[(hash_size + 2139) + c].hh.v.RH != 0)
/*		signed unsigned mismatch ? (c is unsigned) */
		if((eqtb[(hash_size + 2139) + c].hh.v.RH == c)|| 
			(eqtb[(hash_size + 3201)].cint > 0)) 
		goto lab32; 
		else goto lab31; 
		lab22: prevs = s; 
		s = mem[prevs].hh.v.RH; 
	      } 
	      lab32: hyf_char = hyphen_char[hf]; 
/* if hyf_char<0 then goto done1; */
	      if(hyf_char < 0) goto lab31; 
/* if hyf_char>255 then goto done1; */
	      if(hyf_char > 255) goto lab31; /* ? */
	      ha = prevs; 
	      if(lhyf + rhyf > 63) goto lab31; 
	      hn = 0; 
	      while(true){
		if((s >= hi_mem_min)) {
		  if(mem[s].hh.b0 != hf)
		  goto lab33; 
		  hyfbchar = mem[s].hh.b1; 
		  c = hyfbchar;			/*  unsigned char c;  */
		  if(eqtb[(hash_size + 2139) + c].hh.v.RH == 0)
		  goto lab33; 
		  if(hn == 63)
		  goto lab33; 
		  hb = s; 
		  incr(hn); 
		  hu[hn]= c; 
/*		  long to short ... */
		  hc[hn]= eqtb[(hash_size + 2139) + c].hh.v.RH; 
		  hyfbchar = 256;		/* special mark */
		} 
		else if(mem[s].hh.b0 == 6)
		{
		  if(mem[s + 1].hh.b0 != hf)
		  goto lab33; 
/* j:=hn; q:=lig_ptr(s); l.17554 */ 
		  j = hn; 
		  q = mem[s + 1].hh.v.RH; 
/* if q>null then hyf_bchar:=character(q); l.17554  BUG ??? */
/*		  if(q > 0) */
		  if(q != 0)						/* 94/Mar/23 BUG FIX */
		  hyfbchar = mem[q].hh.b1; 
/* while q>null do l.17555 BUG ??? */
/*		  while(q > 0)*/
		  while(q != 0){				/* 94/Mar/23 BUG FIX */
		    c = mem[q].hh.b1; 
		    if(eqtb[(hash_size + 2139) + c].hh.v.RH == 0)
		    goto lab33; 
		    if(j == 63)
		    goto lab33; 
		    incr(j); 
		    hu[j]= c; 
/*			long to short ... */
		    hc[j]= eqtb[(hash_size + 2139) + c].hh.v.RH; 
		    q = mem[q].hh.v.RH; 
		  } 
		  hb = s; 
		  hn = j; 
		  if(odd(mem[s].hh.b1)) 
		  hyfbchar = font_bchar[hf]; 
		  else hyfbchar = 256; 		/* special mark */
		} 
		else if((mem[s].hh.b0 == 11)&&(mem[s].hh.b1 == 
		0)) 
		{	
		  hb = s; 
/* ******************************************************************** */
		  hyfbchar = font_bchar[hf]; 		/* new code in 3.14159 */
/* ******************************************************************** */
		} 
		else goto lab33; 
		s = mem[s].hh.v.RH; 
	      } 
	      lab33:; 
	      if(hn < lhyf + rhyf)
	      goto lab31; 
	      while(true){
		if(!((s >= hi_mem_min)))
		switch(mem[s].hh.b0)
		{case 6 : 
		; 
		  break; 
		case 11 : 
		  if(mem[s].hh.b1 != 0)
		  goto lab34; 
		  break; 
		case 8 : 
		case 10 : 
		case 12 : 
		case 3 : 
		case 5 : 
		case 4 : 
		  goto lab34; 
		  break; 
		  default: 
		  goto lab31; 
		  break; 
		} 
		s = mem[s].hh.v.RH; 
	      } 
	      lab34:; 
	      hyphenate (); 
	    } 
	    lab31:; 
	  } 
	} 
	break; 
      case 11 : 
/* ******************************************************************* */
	if(mem[cur_p].hh.b1 == 1)/*  change in 3.14159 */
/* ******************************************************************* */
	{
	  if(!(mem[cur_p].hh.v.RH >= hi_mem_min)&& autobreaking)
	  if(mem[mem[cur_p].hh.v.RH].hh.b0 == 10)
	  try_break(0, 0); 
	  active_width[1]= active_width[1]+ mem[cur_p + 1].cint; 
	} 
/* ******************************************************************* */
/*  change in 3.14159 */
	else active_width[1]= active_width[1]+ mem[cur_p + 1].cint; 
/* ******************************************************************* */
	break; 
      case 6 : 
	{
	  f = mem[cur_p + 1].hh.b0; 
	  active_width[1]= active_width[1]+ font_info[width_base[f]+ 
	  font_info[char_base[f]+ mem[cur_p + 1].hh.b1].qqqq.b0]
	 .cint; 
	} 
	break; 
      case 7 : 
	{
	  s = mem[cur_p + 1].hh.v.LH; 
	  disc_width = 0; 
	  if(s == 0)
	  try_break(eqtb[(hash_size + 3167)].cint, 1); 
	  else {
	      
	    do {
		if((s >= hi_mem_min)) 
	      {
		f = mem[s].hh.b0; 
		disc_width = disc_width + font_info[width_base[f]+ font_info 
		[ char_base[f]+ mem[s].hh.b1].qqqq.b0].cint; 
	      } 
	      else switch(mem[s].hh.b0)
	      {case 6 : 
		{
		  f = mem[s + 1].hh.b0; 
		  disc_width = disc_width + font_info[width_base[f]+ 
		  font_info[char_base[f]+ mem[s + 1].hh.b1].qqqq.b0 
		].cint; 
		} 
		break; 
	      case 0 : 
	      case 1 : 
	      case 2 : 
	      case 11 : 
		disc_width = disc_width + mem[s + 1].cint; 
		break; 
		default: 
			{
				confusion(931);		/* disc3 */
				return;				// abort_flag set
			}
			break; 
	      } 
	      s = mem[s].hh.v.RH; 
	    } while(!(s == 0)); 
	    active_width[1]= active_width[1]+ disc_width; 
	    try_break(eqtb[(hash_size + 3166)].cint, 1); 
	    active_width[1]= active_width[1]- disc_width; 
	  } 
	  r = mem[cur_p].hh.b1; 
	  s = mem[cur_p].hh.v.RH; 
	  while(r > 0){
	    if((s >= hi_mem_min)) 
	    {
	      f = mem[s].hh.b0; 
	      active_width[1]= active_width[1]+ font_info[width_base[f 
	    ]+ font_info[char_base[f]+ mem[s].hh.b1].qqqq.b0]
	     .cint; 
	    } 
	    else switch(mem[s].hh.b0)
	    {case 6 : 
	      {
		f = mem[s + 1].hh.b0; 
		active_width[1]= active_width[1]+ font_info[width_base[
		f]+ font_info[char_base[f]+ mem[s + 1].hh.b1].qqqq 
		.b0].cint; 
	      } 
	      break; 
	    case 0 : 
	    case 1 : 
	    case 2 : 
	    case 11 : 
	      active_width[1]= active_width[1]+ mem[s + 1].cint; 
	      break; 
	      default:
			  {
				  confusion(932);	/* disc4 */
				  return;				// abort_flag set
			  }
			  break; 
	    } 
	    decr(r); 
	    s = mem[s].hh.v.RH; 
	  } 
	  prevp = cur_p; 
	  cur_p = s; 
	  goto lab35; 
	} 
	break; 
      case 9 : 
	{
	  autobreaking =(mem[cur_p].hh.b1 == 1); 
	  {
	    if(!(mem[cur_p].hh.v.RH >= hi_mem_min)&& autobreaking)
	    if(mem[mem[cur_p].hh.v.RH].hh.b0 == 10)
	    try_break(0, 0); 
	    active_width[1]= active_width[1]+ mem[cur_p + 1].cint; 
	  } 
	} 
	break; 
      case 12 : 
	try_break(mem[cur_p + 1].cint, 0); 
	break; 
      case 4 : 
      case 3 : 
      case 5 : 
	; 
	break; 
	default: 
		{
			confusion(930);		/* paragraph */
			return;				// abort_flag set
		}
		break; 
      } 
      prevp = cur_p; 
      cur_p = mem[cur_p].hh.v.RH; 
      lab35:; 
    } 
    if(cur_p == 0)
    {
      try_break(-10000, 1); 
      if(mem[mem_top - 7].hh.v.RH != mem_top - 7)
      {
	r = mem[mem_top - 7].hh.v.RH; 
	fewest_demerits = 1073741823L; /* 2^30 - 1 */
	do {
	    if(mem[r].hh.b0 != 2)
	  if(mem[r + 2].cint < fewest_demerits)
	  {
	    fewest_demerits = mem[r + 2].cint; 
	    best_bet = r; 
	  } 
	  r = mem[r].hh.v.RH; 
	} while(!(r == mem_top - 7)); 
	best_line = mem[best_bet + 1].hh.v.LH; 
/*  if looseness=0 then goto done; */
	if(eqtb[(hash_size + 3182)].cint == 0){
/*		if (final_pass && eqtb[(hash_size + 3750)].cint > 0){ */
/*			paragraph_failed++; */
/*		} */
		goto lab30;						/* normal exit */
	}
	{
/* r:=link(active); actual_looseness:=0; */
	  r = mem[mem_top - 7].hh.v.RH; 
	  actual_looseness = 0; 
	  do {
	      if(mem[r].hh.b0 != 2)
	    {
/*   line_diff:=line_number(r)-best_line; */
	      line_diff = toint(mem[r + 1].hh.v.LH)- toint(best_line 
	    ); 
/*   if ((line_diff<actual_looseness)and(looseness<=line_diff))or@|
        ((line_diff>actual_looseness)and(looseness>=line_diff)) then */
	      if(((line_diff < actual_looseness)&&
				(eqtb[(hash_size + 3182)].cint 
	      <= line_diff)) ||((line_diff > actual_looseness)&&
				(eqtb[(hash_size + 3182)].cint >= line_diff)))
	      {
		best_bet = r; 
		actual_looseness = line_diff; 
		fewest_demerits = mem[r + 2].cint; 
	      } 
	      else if((line_diff == actual_looseness)&&(mem[r + 2]
	     .cint < fewest_demerits)) 
	      {
		best_bet = r; 
		fewest_demerits = mem[r + 2].cint; 
	      } 
	    } 
	    r = mem[r].hh.v.RH; 
	  } while(!(r == mem_top - 7)); 
	  best_line = mem[best_bet + 1].hh.v.LH; 
	} 
/*  if (actual_looseness=looseness)or final_pass then goto done; */
/*	if((actual_looseness == eqtb[(hash_size + 3182)].cint)|| final_pass)*/
	if((actual_looseness == eqtb[(hash_size + 3182)].cint)) {
		goto lab30;
	}
	if(final_pass){
		goto lab30;
	}
      } 
    } 
    q = mem[mem_top - 7].hh.v.RH; 
    while(q != mem_top - 7){
      cur_p = mem[q].hh.v.RH; 
      if(mem[q].hh.b0 == 2)
      free_node(q, 7); 
      else free_node(q, 3); 
      q = cur_p; 
    } 
    q = passive; 
    while(q != 0){
      cur_p = mem[q].hh.v.RH; 
      free_node(q, 2); 
      q = cur_p; 
    } 
    if(! second_pass)
    {
	;
#ifdef STAT
      if(eqtb[(hash_size + 3195)].cint > 0)
      print_nl("@second_pass");			/*  */
#endif /* STAT */
/* threshold:=tolerance; */
      threshold = eqtb[(hash_size + 3164)].cint; 
      second_pass = true; 
	  second_pass_count++;					/* 96 Feb 9 */
/*   final_pass:=(emergency_stretch<=0); */
      final_pass =(eqtb[(hash_size + 3750)].cint <= 0); 
    } 
    else {
	
	;
#ifdef STAT
      if(eqtb[(hash_size + 3195)].cint > 0)	/* tracing_paragraphs */
      print_nl("@emergencypass");			/*  */
#endif /* STAT */
/*     can only get here is \emergencystretch has been set positive */
/*     background[2]:=background[2]+emergency_stretch; final_pass:=true; */
      background[2]= background[2]+ eqtb[(hash_size + 3750)].cint;
	  final_pass = true; 
	  final_pass_count++;					/* 96 Feb 9 */
	} /* end of if second_pass */
  } /* end of while(true)do */
/* cannot drop through from above loop */
  lab30:								/* common exit point */
/*  if (badness > (pre)tolerance) */
/*    if(last_badness > threshold){ */
/*    if(savedbadness > threshold){ */	/* do we believe this ??? */ 
/*		paragraph_failed++;	*/	/* 96/Feb/9 */
/*	} */
/*	  if (prevgraf == 1) singleline++; */
/*	  if (nest[nest_ptr].pg_field == 1) singleline++; */
/*	  At this point best_line is number of lines in paragraph + 1 */
	  if (best_line == 2) singleline++;
	;
#ifdef STAT
  if(eqtb[(hash_size + 3195)].cint > 0)
  {
    end_diagnostic(true); 
    normalize_selector (); 
  } 
#endif /* STAT */
  post_line_break(finalwidowpenalty); 
  q = mem[mem_top - 7].hh.v.RH; 
  while(q != mem_top - 7){
    cur_p = mem[q].hh.v.RH; 
    if(mem[q].hh.b0 == 2)
    free_node(q, 7); 
    else free_node(q, 3); 
    q = cur_p; 
  } 
  q = passive; 
  while(q != 0){ /* while q<>null do l.16979 */
    cur_p = mem[q].hh.v.RH; 
    free_node(q, 2); 
    q = cur_p; 
  } 
  pack_begin_line = 0; 
} 

void prefixed_command (void) 
{/* 30 10 */
  small_number a; 
  internal_font_number f; 
  halfword j; 
  font_index k; 
  halfword p, q; 
  integer n; 
  bool e; 
  a = 0; 
  while(cur_cmd == 93){
    if(! odd(a / cur_chr)) 
    a = a + cur_chr;			/*   small_number a;  */
    do {
	get_x_token (); 
    } while(!((cur_cmd != 10)&&(cur_cmd != 0))); 
    if(cur_cmd <= 70)
    {
      print_err("You can't use a prefix with `");
      print_cmd_chr(cur_cmd, cur_chr); 
      print_char(39);	/* ' */
	  help1("I'll pretend you didn't say \\long or \\outer or \\global.");
      back_error (); 
      return; 
    } 
  } 
  if((cur_cmd != 97)&&(a % 4 != 0)) {
    print_err("You can't use `");
    print_esc(1165);		/* long */
    print(1175);		/* ' or ` */
    print_esc(1166);		/* outer */
    print(1176);		/* ' with ` */
    print_cmd_chr(cur_cmd, cur_chr); 
    print_char(39);		/* ' */
	help1("I'll pretend you didn't say \\long or \\outer here.");
    error (); 
  } 
  if(eqtb[(hash_size + 3206)].cint != 0)
  if(eqtb[(hash_size + 3206)].cint < 0)
  {
    if((a >= 4)) 
    a = a - 4; 
  } 
  else {
      
    if(!(a >= 4)) 
    a = a + 4; 
  } 

  switch(cur_cmd){
	  case 87 : 
/* set_font: define(cur_font_loc,data,cur_chr); */
    if((a >= 4)) 
    geq_define((hash_size + 1834), 120, cur_chr); 
    else eq_define((hash_size + 1834), 120, cur_chr); 
    break; 
  case 97 : 
    {
      if(odd(cur_chr)&& !(a >= 4)&& 
		  (eqtb[(hash_size + 3206)].cint >= 0)) 
      a = a + 4; 
      e =(cur_chr >= 2); 
      get_r_token (); 
      p = cur_cs; 
      q = scan_toks(true, e); 
      if((a >= 4)) geq_define(p, 111 +(a % 4), def_ref); 
      else eq_define(p, 111 +(a % 4), def_ref); 
    } 
    break; 
  case 94 : 
    {
      n = cur_chr; 
      get_r_token (); 
      p = cur_cs; 
      if(n == 0)
      {
	do {
	    get_token (); 
	} while(!(cur_cmd != 10)); 
	if(cur_tok == 3133)
	{
	  get_token (); 
	  if(cur_cmd == 10)
	  get_token (); 
	} 
      } 
      else {
	  
	get_token (); 
	q = cur_tok; 
	get_token (); 
	back_input (); 
	cur_tok = q; 
	back_input (); 
      } 
      if(cur_cmd >= 111)
      incr(mem[cur_chr].hh.v.LH); 
      if((a >= 4)) 
      geq_define(p, cur_cmd, cur_chr); 
      else eq_define(p, cur_cmd, cur_chr); 
    } 
    break; 
  case 95 : 
    {
      n = cur_chr; 
      get_r_token (); 
      p = cur_cs; 
      if((a >= 4))  geq_define(p, 0, 256); 
      else eq_define(p, 0, 256); 
      scan_optional_equals (); 
      switch(n){
		  case 0 : 
	{
	  scan_char_num (); 
	  if((a >= 4)) 
	  geq_define(p, 68, cur_val); 
	  else eq_define(p, 68, cur_val); 
	} 
	break; 
      case 1 : 
	{
	  scan_fifteen_bit_int (); 
	  if((a >= 4)) 
	  geq_define(p, 69, cur_val); 
	  else eq_define(p, 69, cur_val); 
	} 
	break; 
	default: 
	{
	  scan_eight_bit_int (); 
	  switch(n){
		  case 2 : 
	    if((a >= 4)) 
	    geq_define(p, 73, (hash_size + 3218) + cur_val); 
	    else eq_define(p, 73, (hash_size + 3218) + cur_val); 
	    break; 
	  case 3 : 
	    if((a >= 4)) 
	    geq_define(p, 74, (hash_size + 3751) + cur_val); 
	    else eq_define(p, 74, (hash_size + 3751) + cur_val); 
	    break; 
	  case 4 : 
	    if((a >= 4)) 
	    geq_define(p, 75, (hash_size + 800) + cur_val); 
	    else eq_define(p, 75, (hash_size + 800) + cur_val); 
	    break; 
	  case 5 : 
	    if((a >= 4)) 
	    geq_define(p, 76, (hash_size + 1056) + cur_val); 
	    else eq_define(p, 76, (hash_size + 1056) + cur_val); 
	    break; 
	  case 6 : 
	    if((a >= 4)) 
	    geq_define(p, 72, (hash_size + 1322) + cur_val); 
	    else eq_define(p, 72, (hash_size + 1322) + cur_val); 
	    break; 
	  } 
	} 
	break; 
      } 
    } 
    break; 
  case 96 : 
    {
      scan_int (); 
      n = cur_val; 
      if(! scan_keyword(836))	/* to */
      {
      print_err("Missing `to' inserted");
	  help2("You should have said `\\read<number> to \\cs'.", 
		  "I'm going to look for the \\cs now.");
	error (); 
      } 
      get_r_token (); 
      p = cur_cs; 
	  read_toks(n, p); 
      if((a >= 4)) geq_define(p, 111, cur_val); 
      else eq_define(p, 111, cur_val); 
    } 
    break; 
  case 71 : 
  case 72 : 
    {
      q = cur_cs; 
      if(cur_cmd == 71)
      {
	scan_eight_bit_int (); 
	p = (hash_size + 1322) + cur_val; 
      } 
      else p = cur_chr; 
      scan_optional_equals (); 
      do {
	  get_x_token (); 
      } while(!((cur_cmd != 10)&&(cur_cmd != 0))); 
      if(cur_cmd != 1)
      {
	if(cur_cmd == 71)
	{
	  scan_eight_bit_int (); 
	  cur_cmd = 72; 
	  cur_chr = (hash_size + 1322) + cur_val; 
	} 
	if(cur_cmd == 72)
	{
	  q = eqtb[cur_chr].hh.v.RH; 
	  if(q == 0)
	  if((a >= 4)) 
	  geq_define(p, 101, 0); 
	  else eq_define(p, 101, 0); 
	  else {
	      
	    incr(mem[q].hh.v.LH); 
	    if((a >= 4)) 
	    geq_define(p, 111, q); 
	    else eq_define(p, 111, q); 
	  } 
	  goto lab30; 
	} 
      } 
      back_input (); 
      cur_cs = q; 
      q = scan_toks(false, false); 
      if(mem[def_ref].hh.v.RH == 0){
	if((a >= 4)) 
	geq_define(p, 101, 0); 
	else eq_define(p, 101, 0); 
	{
	  mem[def_ref].hh.v.RH = avail; 
	  avail = def_ref; 
	;
#ifdef STAT
	  decr(dyn_used); 
#endif /* STAT */
	} 
      } 
      else {
	  
	if(p == (hash_size + 1313))			/* output ? */
	{
	  mem[q].hh.v.RH = get_avail (); 
	  q = mem[q].hh.v.RH; 
	  mem[q].hh.v.LH = 637; 
	  q = get_avail (); 
	  mem[q].hh.v.LH = 379;
	  mem[q].hh.v.RH = mem[def_ref].hh.v.RH; 
	  mem[def_ref].hh.v.RH = q; 
	} 
	if((a >= 4)) 
	geq_define(p, 111, def_ref); 
	else eq_define(p, 111, def_ref); 
      } 
    } 
    break; 
  case 73 : 
    {
      p = cur_chr; 
      scan_optional_equals (); 
      scan_int (); 
      if((a >= 4)) 
      geq_word_define(p, cur_val); 
      else eq_word_define(p, cur_val); 
    } 
    break; 
  case 74 : 
    {
      p = cur_chr; 
      scan_optional_equals (); 
      scan_dimen(false, false, false); 
      if((a >= 4)) 
      geq_word_define(p, cur_val); 
      else eq_word_define(p, cur_val); 
    } 
    break; 
  case 75 : 
  case 76 : 
    {
      p = cur_chr; 
      n = cur_cmd; 
      scan_optional_equals (); 
      if(n == 76) scan_glue(3);
      else scan_glue(2);
      trap_zero_glue (); 
      if((a >= 4)) 
      geq_define(p, 117, cur_val); 
      else eq_define(p, 117, cur_val); 
    } 
    break; 
  case 85 : 
    {
      if(cur_chr == (hash_size + 1883))
      n = 15; 
      else if(cur_chr == (hash_size + 2907))
      n = 32768L; 								/* 2^15 */
      else if(cur_chr == (hash_size + 2651))
      n = 32767; 								/* 2^15 - 1*/
      else if(cur_chr == (hash_size + 3474))
      n = 16777215L; 							/* 2^24 - 1 */
      else n = 255;							/* 2^8 - 1 */
      p = cur_chr; 
      scan_char_num (); 
      p = p + cur_val; 
      scan_optional_equals (); 
      scan_int (); 
      if(((cur_val < 0)&&(p < (hash_size + 3474))) ||(cur_val > n)) 
      {
		  print_err("Invalid code(");
	print_int(cur_val); 
	if(p < (hash_size + 3474))
		print(1197);	/*), should be in the range 0.. */
	else print(1198);	/*), should be at most  */
	print_int(n); 
	help1("I'm going to use 0 instead of that illegal code value.");
	error (); 
	cur_val = 0; 
      } 
      if(p < (hash_size + 2907))
      if((a >= 4)) 
      geq_define(p, 120, cur_val); 
      else eq_define(p, 120, cur_val); 
      else if(p < (hash_size + 3474))
      if((a >= 4)) 
      geq_define(p, 120, cur_val); 
      else eq_define(p, 120, cur_val); 
      else if((a >= 4)) 
      geq_word_define(p, cur_val); 
      else eq_word_define(p, cur_val); 
    } 
    break; 
  case 86 : 
    {
      p = cur_chr; 
      scan_four_bit_int (); 
      p = p + cur_val; 
      scan_optional_equals (); 
      scan_font_ident (); 
      if((a >= 4)) 
      geq_define(p, 120, cur_val); 
      else eq_define(p, 120, cur_val); 
    } 
    break; 
  case 89 : 
  case 90 : 
  case 91 : 
  case 92 : 
    do_register_command(a); 
    break; 
  case 98 : 
    {
      scan_eight_bit_int (); 
      if((a >= 4)) 
      n = 256 + cur_val; 
      else n = cur_val; 
      scan_optional_equals (); 
      if(set_box_allowed){
		  scan_box(1073741824L + n); /* 2^30 + n */ /* box_flag + n */
	  }
      else {
	  print_err("Improper ");
	print_esc(533);		/* setbox */
	help2("Sorry, \\setbox is not allowed after \\halign in a display,",
		"or between \\accent and an accented character.");
	error (); 
      } 
    } 
    break; 
  case 79 : 
    alter_aux (); 
    break; 
  case 80 : 
    alter_prev_graf (); 
    break; 
  case 81 : 
    alter_page_so_far (); 
    break; 
  case 82 : 
    alter_integer (); 
    break; 
  case 83 : 
    alter_box_dimen (); 
    break; 
  case 84 : 
    {
      scan_optional_equals (); 
      scan_int (); 
      n = cur_val; 
      if(n <= 0)
      p = 0; 
      else {
	  
	p = get_node(2 * n + 1); 
	mem[p].hh.v.LH = n; 
	{
		register integer for_end;
		j = 1;
		for_end = n;
		if(j <= for_end) do 
		{
			scan_dimen(false, false, false); 
			mem[p + 2 * j - 1].cint = cur_val; 
			scan_dimen(false, false, false); 
			mem[p + 2 * j].cint = cur_val; 
		} 
		while(j++ < for_end);
	} 
      } 
      if((a >= 4)) 
      geq_define((hash_size + 1312), 118, p); 
      else eq_define((hash_size + 1312), 118, p); 
    } 
    break; 
  case 99 : 
    if(cur_chr == 1)
    {
	;
#ifdef INITEX
  if (is_initex) {		/* bkph */
      new_patterns (); 
      goto lab30; 
  }						/* bkph */
#endif /* INITEX */
     print_err("Patterns can be loaded only by INITEX");
      help_ptr = 0; 
      error (); 
      do {
		  get_token (); 
      } while(!(cur_cmd == 2)); 
      return; 
    } 
    else {
	
      new_hyph_exceptions (); 
      goto lab30; 
    } 
    break; 
  case 77 : 
    {
      find_font_dimen(true); 
      k = cur_val; 
      scan_optional_equals (); 
      scan_dimen(false, false, false); 
      font_info[k].cint = cur_val; 
    } 
    break; 
  case 78 : 
    {
      n = cur_chr; 
      scan_font_ident (); 
      f = cur_val; 
      scan_optional_equals (); 
      scan_int (); 
      if(n == 0)hyphen_char[f]= cur_val; 
      else skew_char[f]= cur_val; 
    } 
    break; 
  case 88 : 
    new_font(a); 
    break; 
  case 100 : 
    new_interaction (); 
    break; 
    default: 
	{
		confusion(1172);	/* prefix */
		return;				// abort_flag set
	}
	break; 
  } /* end of cur_cmd switch */


lab30:
  if(after_token != 0) {
	  cur_tok = after_token; 
	  back_input (); 
	  after_token = 0; 
  } 
} 

/* added following explanations 96/Jan/10 */

void bad_formator_pool (char *name, char *defaultname, char *envvar)
{
	if (name == NULL) name = defaultname;
	sprintf(log_line, "(Perhaps %s is for an older version of TeX)\n", name); 
	show_line(log_line, 0);
	name_of_file[name_length + 1] = '\0';	/* null terminate */
	sprintf(log_line, "(Alternatively, %s may have been corrupted)\n", name_of_file+1);
	show_line(log_line, 0);
	name_of_file[name_length + 1] = ' ';	/* space terminate */
	sprintf(log_line,
		"(Perhaps your %s environment variable is not set correctly)\n", envvar);
	show_line(log_line, 0);
	{
		char *s;						/* extra info 99/April/28 */
		if ((s = grabenv(envvar)) != NULL) {
			sprintf(log_line, "(%s=%s)\n", envvar, s);
			show_line(log_line, 0);
		}
		else {
			sprintf(log_line, "%s environment variable not set\n", envvar);
			show_line(log_line, 0);
		}
	}
#ifndef _WINDOWS
	fflush(stdout); 
#endif
} 

bool load_fmt_file (void) 
{/* 6666 10 */ register bool Result; 
  integer j, k; 
  halfword p, q; 
  integer x; 

  undump_int(x);				/* CHECKSUM */
  if(x != BEGINFMTCHECKSUM) /* magic FMT file start 4C 20 E6 15 hex */
  goto lab6666; 

  undump_int(x);				/* mem_bot */
/*  if(x != 0) */
  if(x != mem_bot)
  goto lab6666; 

  undump_int(x);				/* mem_top */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEMAIN
/*	we already read this once earlier to grab mem_top */
	if (trace_flag) {
		sprintf(log_line, "Read from fmt file mem_top = %d TeX words\n", x);
		show_line(log_line, 0);
	}
/*    allocate_main_memory (x); */	/* allocate main memory at this point */
    mem = allocate_main_memory(x);	/* allocate main memory at this point */
	if (mem == NULL) exit(1);		/* redundant sanity test ! */
	initialize_aux();				/* do `mem' part of initialize */
/*	mem = zmem; */					/* update pointer to main memory */
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(x != mem_top)
  goto lab6666; 

  undump_int(x);					/* eqtbsize */
  if(x != (hash_size + 4006))	/* eqtbsize */
  goto lab6666; 
  undump_int(x);					/* hash_prime */
  if(x != hash_prime)
  goto lab6666; 
  undump_int(x);					/* hyphen_prime */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEHYPHEN
/*  allow format files dumped with arbitrary (prime) hyphenation exceptions */
	realloc_hyphen (x);				/*	reset_hyphen(); */
	hyphen_prime = x;
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  if(x != 607)*/
  if(x != hyphen_prime)
  goto lab6666; 
  {
    undump_int(x);				/* pool_size */
    if(x < 0)
    goto lab6666; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
	if(x > current_pool_size) {
		if (trace_flag) {
			sprintf(log_line, "undump string pool reallocation (%d > %d)\n",
			   x, current_pool_size);
			show_line(log_line, 0);
		}
		str_pool = realloc_str_pool (x - current_pool_size + increment_pool_size);
	}
	if(x > current_pool_size)						/* 94/Jan/24 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if(x > pool_size)
#endif
    {
   ; 
      sprintf(log_line, "%s%s\n",  "---! Must increase the ", "string pool size"); 
	  show_line(log_line, 0);
      goto lab6666; 
    } 
    else pool_ptr = x; 
  } 
  {
    undump_int(x);				/* max_strings */
    if(x < 0)
		goto lab6666; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
    if(x > current_max_strings){
		if (trace_flag) {
			sprintf(log_line, "undump string pointer reallocation (%d > %d)\n",
				  x, current_max_strings);
			show_line(log_line, 0);
		}
		str_start = realloc_str_start(x - current_max_strings + increment_max_strings);
	}
    if(x > current_max_strings)					/* 94/Jan/24 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if(x > max_strings)
#endif
    {
   ; 
      sprintf(log_line,  "%s%s\n",  "---! Must increase the ", "max strings"); 
	  show_line(log_line, 0);
      goto lab6666; 
    } 
    else str_ptr = x; 
  } 

  if (undumpthings(str_start[0], str_ptr + 1) /* undump string ptrs */
	)return -1;
  if (undumpthings(str_pool[0], pool_ptr) /*	undump string pool */
	)return -1;

  init_str_ptr = str_ptr; 
  init_pool_ptr = pool_ptr; 
/*	undump the dynamic memory - paragraph 1312 in the book */
  {
    undump_int(x); 
    if((x < 1019)||(x > mem_top - 14)) 
    goto lab6666; 
    else lo_mem_max = x; 
  } 
  {
    undump_int(x); 
    if((x < 20)||(x > lo_mem_max)) 
    goto lab6666; 
    else rover = x; 
  } 
  p = 0;									/* mem_bot */
  q = rover; 
  do {
      if (undumpthings(mem[p], q + 2 - p) 
		)return -1;
    p = q + mem[q].hh.v.LH; 
    if((p > lo_mem_max)||((q >= mem[q + 1].hh.v.RH)&&(mem[q + 
    1].hh.v.RH != rover)))
    goto lab6666; 
    q = mem[q + 1].hh.v.RH; 
  } while(!(q == rover)); 
  if (undumpthings(mem[p], lo_mem_max + 1 - p) 
	)return -1;
/*  if(mem_min < -2) */
  if(mem_min < mem_bot - 2)					/*  ? splice in block below */
  {
/*	or call add_variable_space(mem_bot - (mem_min + 1)) */

    if (trace_flag) show_line("Splicing in mem_min space in undump!\n", 0);

    p = mem[rover + 1].hh.v.LH; 
    q = mem_min + 1; 
    mem[mem_min].hh.v.RH = 0;				/* null */
    mem[mem_min].hh.v.LH = 0;				/* null */
    mem[p + 1].hh.v.RH = q; 
    mem[rover + 1].hh.v.LH = q; 
    mem[q + 1].hh.v.RH = rover; 
    mem[q + 1].hh.v.LH = p; 
/*    mem[q].hh.v.RH = 262143L;  */	/* NO! */
    mem[q].hh.v.RH = empty_flag; 
/*    mem[q].hh.v.LH = -0 - q;  */
    mem[q].hh.v.LH = mem_bot - q; 		/* ? size of block  */
  } 
  {
    undump_int(x); 
    if((x < lo_mem_max + 1)||(x > mem_top - 13)) 
    goto lab6666; 
    else hi_mem_min = x; 
  } 
  {
    undump_int(x); 
/*    if((x < 0)||(x > mem_top))  */
    if((x < mem_bot)||(x > mem_top)) 
    goto lab6666; 
    else avail = x; 
  } 
  mem_end = mem_top; 
  if (undumpthings(mem[hi_mem_min], mem_end + 1 - hi_mem_min) 
	)return -1;
  undump_int(var_used); 
  undump_int(dyn_used); 
  k = 1; 
  do {
      undump_int(x); 
    if((x < 1)||(k + x > (hash_size + 4007))) 
    goto lab6666; 
    if (undumpthings(eqtb[k], x) 
	 )return -1;
    k = k + x; 
    undump_int(x); 
    if((x < 0)||(k + x > (hash_size + 4007))) 
    goto lab6666; 
    {
		register integer for_end;
		j = k;
		for_end = k + x - 1;
		if(j <= for_end) do 
			eqtb[j]= eqtb[k - 1]; 
		while(j++ < for_end);
	} 
    k = k + x; 
  } while(!(k > (hash_size + 4006))); 
  {
    undump_int(x); 
/*	if((x < hashbase)||(x > hashbase + hashsize)) */
/*    if((x < 514)||(x > (hash_size + 514)))  */
    if((x < 514)||(x > (hash_size + hash_extra + 514)))  /*96/Jan/10*/
    goto lab6666; 
    else par_loc = x; 
  } 
  par_token = 4095 + par_loc; 
  {
    undump_int(x); 
/*  if((x < hashbase)||(x > hashbase + hashsize)) */
/*    if((x < 514)||(x > (hash_size + 514)))  */
    if((x < 514)||(x > (hash_size + hash_extra + 514))) /*96/Jan/10*/
    goto lab6666; 
    else
    write_loc = x; 
  } 
  {
    undump_int(x); 
/*  if((x < hashbase)||(x > hashbase + hashsize)) */
/*    if((x < 514)||(x > (hash_size + 514)))  */
    if((x < 514)||(x > (hash_size + hash_extra + 514))) /*96/Jan/10*/
    goto lab6666; 
    else hash_used = x; 
  } 
  p = 513; 
  do {
      { 
      undump_int(x); 
      if((x < p + 1)||(x > hash_used)) 
      goto lab6666; 
      else p = x; 
    } 
    undump_hh(hash[p]); 
  } while(!(p == hash_used)); 
  if (undumpthings(hash[hash_used + 1], (hash_size + 780) - hash_used) 
	)return -1;

  undump_int(cs_count);			/* cs_count */
  if (trace_flag) {
	  sprintf(log_line, "itex undump cs_count %d ", cs_count); /* debugging */
	  show_line(log_line, 0);
  }
  {
    undump_int(x);				/* font_mem_size */
    if(x < 7)
    goto lab6666; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEFONT
	if (trace_flag) {
		sprintf(log_line, "Read from fmt fmem_ptr = %d\n", x);
		show_line(log_line, 0);
	}
    if(x > current_font_mem_size){	/* 93/Nov/28 dynamic allocate font_info */
		if (trace_flag) {
			sprintf(log_line, "Undump realloc font_info (%d > %d)\n",
				   x, current_font_mem_size);
			show_line(log_line, 0);
		}
		font_info = realloc_font_info(x - current_font_mem_size + increment_font_mem_size);
	}
    if(x > current_font_mem_size)  /* in case allocation failed 94/Jan/24 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if(x > font_mem_size)
#endif
    {
   ; 
      sprintf(log_line, "%s%s\n",  "---! Must increase the ", "font mem size"); 
	  show_line(log_line, 0);
      goto lab6666; 
    } 
    else fmem_ptr = x; 
  } 
  {
    if (undumpthings(font_info[0], fmem_ptr) 
	 )return -1;
    {
      undump_int(x);			/* font_max */
      if(x < 0)
		  goto lab6666; 
      if(x > font_max)
      {
	; 
		sprintf(log_line, "%s%s\n",  "---! Must increase the ", "font max"); 
		show_line(log_line, 0);
		goto lab6666; 
      } 
      else font_ptr = x; 
    } 
	frozenfontptr = font_ptr;	/* remember number of fonts frozen into format */
    if (undumpthings(font_check[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_size[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_dsize[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_params[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(hyphen_char[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(skew_char[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_name[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_area[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_bc[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_ec[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(char_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(width_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(height_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(depth_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(italic_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(lig_kern_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(kern_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(exten_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(param_base[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_glue[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(bchar_label[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_bchar[0], font_ptr + 1) 
	 )return -1;
    if (undumpthings(font_false_bchar[0], font_ptr + 1) 
	 )return -1;
  } 

/*	log not opened yet, so can't show fonts frozen into format */
  
/* May be able to avoid the following since we switched to */
/* non_address from font_mem_size to 0 96/Jan/15 ??? */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEFONT
/* deal with fmt files dumped with *different* font_mem_size 93/Nov/29 */
 { int count = 0, oldfont_mem_size = 0;
	  for (x = 0; x <= font_ptr; x++) {
		  if(bchar_label[x] > oldfont_mem_size) oldfont_mem_size = bchar_label[x];
	  }
/* somewhat arbitrary sanity check ... */
/*	  if (oldfont_mem_size != font_mem_size && oldfont_mem_size > font_max) { */
	  if (oldfont_mem_size != non_address && oldfont_mem_size > font_max) {	/* 96/Jan/16 */
		  for (x = 0; x <= font_ptr; x++) {
			  if(bchar_label[x] == oldfont_mem_size) {
/*				  bchar_label[x] = font_mem_size; */
				  bchar_label[x] = non_address;		/* 96/Jan/16 */
				  count++;
			  }
		  }

		  if (trace_flag) {
			  sprintf(log_line,
					  "oldfont_mem_size is %d --- hit %d times. Using non_address %d\n",
					  oldfont_mem_size, count, non_address);
			  show_line(log_line, 0);
		  }

	  }
 }
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* undump(0)(hyph_size)(hyph_count); */
  {
    undump_int(x); 
/*    if((x < 0)||(x > 607))  */
    if((x < 0)||(x > hyphen_prime)) 
    goto lab6666; 
    else hyph_count = x; 
  } 
/* undump hypenation exception tables p.1325 */
  {
	  register integer for_end;
	  k = 1;
	  for_end = hyph_count;
	  if(k <= for_end) 
		  do 
		  {
			  {
	undump_int(x); 
/*	if((x < 0)||(x > 607))  */
	if((x < 0)||(x > hyphen_prime)) 
	goto lab6666; 
	else j = x; 
      } 
/*   undump(0)(str_ptr)(hyph_word[j]); */
      {
	undump_int(x); 
	if((x < 0)||(x > str_ptr)) 
	goto lab6666; 
	else hyph_word[j]= x; 
      } 
/*   undump(min_halfword)(max_halfword)(hyph_list[j]); */
      {
	undump_int(x); 
/*	if((x < 0)||(x > 262143L)) */
	if((x < 0)||(x > max_halfword))		/* mem_top ? no p.1325 */
	goto lab6666; 
	else hyph_list[j]= x; 
      } 
    } 
  while(k++ < for_end);
  } 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEHYPHEN
/* if user specified new hyphen prime - flush existing exception patterns ! */
/* but, we can reclaim the string storage wasted ... */
	if (is_initex) {
		if (new_hyphen_prime != 0) {
			realloc_hyphen(new_hyphen_prime);	/*	reset_hyphen(); */
			hyphen_prime = new_hyphen_prime;
		}
	}
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  {
    undump_int(x); 
    if(x < 0)
    goto lab6666; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATETRIES
	if (!is_initex) {
		ALLOCATETRIES(x);	/* allocate only as much as is needed */
/*		trie_size = x; */	/* ??? */
	}
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if(x > trie_size)
    {
   ; 
      sprintf(log_line, "%s%s\n",  "---! Must increase the ", "trie size"); 
	  show_line(log_line, 0);
      goto lab6666; 
    } 
    else j = x; 
  } 
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex)			/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  trie_max = j; 
#endif /* INITEX */
  if (undumpthings(trietrl[0], j + 1) 
	)return -1;
  if (undumpthings(trietro[0], j + 1) 
	)return -1;
  if (undumpthings(trietrc[0], j + 1) 
	)return -1;
  {
    undump_int(x); 
    if(x < 0)
    goto lab6666; 
    if(x > trie_op_size)
    {
   ; 
      sprintf(log_line, "%s%s\n",  "---! Must increase the ", "trie op size"); 
	  show_line(log_line, 0);
      goto lab6666; 
    } 
    else j = x; 
  } 
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex)				/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  trie_op_ptr = j; 
#endif /* INITEX */
/* for k:=1 to j do
  begin undump(0)(63)(hyf_distance[k]); {a |small_number|}
  undump(0)(63)(hyf_num[k]);
  undump(min_quarterword)(max_quarterword)(hyf_next[k]); end; */
  if (undumpthings(hyf_distance[1], j) 
	)return -1;
  if (undumpthings(hyf_num[1], j) 
	)return -1;
  if (undumpthings(hyf_next[1], j) 
	)return -1;
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex) {					/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  {
	  register integer for_end;
	  k = 0;
	  for_end = 255;
	  if(k <= for_end) do 
		  trie_used[k]= 0; 
	  while(k++ < for_end);
  } 
 }
#endif /* INITEX */
  k = 256; 
  while(j > 0){
/* undump(0)(k-1)(k) */      
    {
      undump_int(x); 
      if((x < 0)||(x > k - 1)) 
      goto lab6666; 
      else k = x; 
    } 
/* undump(1)(j)(x) */
    {
      undump_int(x); 
      if((x < 1)||(x > j)) 
      goto lab6666; 
      else x = x; 
    } 
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex)					/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    trie_used[k]= x; 
#endif /* INITEX */
/*   j:=j-x; op_start[k]:=qo(j); */
    j = j - x; 
    op_start[k]= j; 
  } 
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex)					/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  trie_not_ready = false 
#endif /* INITEX */
; 
  {
    undump_int(x); 
    if((x < 0)||(x > 3)) 
    goto lab6666; 
/*    else interaction = x;  */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (interaction < 0)		/* may now set in local.c bkph 94/Jan/8 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
		interaction = x;
  } 
  {
    undump_int(x); 
    if((x < 0)||(x > str_ptr)) 
    goto lab6666; 
    else format_ident = x; 
  } 
  undump_int(x); 
/*	test_eof(fmt_file)? */
  if((x != ENDFMTCHECKSUM)|| feof(fmt_file)) /* magic checksum --- change ? */
  goto lab6666; 

  Result = true; 
  return(Result); 

  lab6666:; 
  sprintf(log_line, "(Fatal format file error; I'm stymied)\n"); 
  show_line(log_line, 1);
/* added following bit of explanation 96/Jan/10 */
  if (! knuth_flag)
	  bad_formator_pool(format_file, "the format file", "TEXFORMATS");
  Result = false; 
  return Result; 
} 

void final_clean_up (void) 
{/* 10 */ 
  small_number c; 
  c = cur_chr;					/* small_number  c */
  if(job_name == 0)open_log_file (); 
/* ******************************** 3.14159 ***************************** */
  while(input_ptr > 0)
	  if(cur_input.state_field == 0){
		  end_token_list ();
	  }
	  else end_file_reading (); 
/* *********************************************************************** */
  while(open_parens > 0){
	  print(1270);	/* )*/
	  decr(open_parens); 
  } 
  if(cur_level > 1)  {
    print_nl("(");		/*(*/
    print_esc(1271);		/* end occurred  */
    print(1272);		/* inside a group at level  */
    print_int(cur_level - 1); 
    print_char(41);		/*)*/
  } 
  while(cond_ptr != 0){
    print_nl("(");		/*(*/
    print_esc(1271);		/* end occurred  */
    print(1273);		/* when  */
    print_cmd_chr(105, cur_if);	/* i */
    if(if_line != 0)
    {
      print(1274);		/* on line  */
      print_int(if_line); 
    } 
    print(1275);		/*  was incomplete) */
    if_line = mem[cond_ptr + 1].cint; 
    cur_if = mem[cond_ptr].hh.b1; 
/* *********************************************************************** */
    temp_ptr = cond_ptr;						/* new in 3.14159 */
    cond_ptr = mem[cond_ptr].hh.v.RH; 
	free_node(temp_ptr, 2); 				/* new in 3.14159 */
/* *********************************************************************** */
  } 
  if(history != 0)
  if(((history == 1)||(interaction < 3)))
  if(selector == 19)
  {
    selector = 17; 
    print_nl(" (see the transcript file for additional information)");	/* 	 */
    selector = 19; 
  } 
  if(c == 1){
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (is_initex) {					/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* *************************** new in 3.14159 ***************************** */
    {
		register integer for_end;
		c = 0;
		for_end = 4;
		if(c <= for_end) do 
			if(cur_mark[c]!= 0)
				delete_token_ref(cur_mark[c]); 
		while(c++ < for_end);
	} 
/* *********************************************************************** */
		store_fmt_file ();	// returns a value ?
	}
#endif /* INITEX */
	if (!is_initex) 			/* 2000/March/3 */
		print_nl("	(\\dump is performed only by INITEX)");		/*  */
  } 
}

// debugging code for checking the string pool

#ifdef CHECKPOOL
int checkpool (char *task)
{	
	int c, i, k, n, st, flag, bad=0;

	if (task != NULL) {
		sprintf(log_line, "Check on string pool start (%s)\n", task);
		show_line(log_line, 0);
	}
	for (k = 0; k < 32; k++) {
		if (str_start[k] != 3 * k) {
			sprintf(log_line, "k %d str_start[k] %d\n", k, str_start[k]);
			show_line(log_line, 0);
		}
	}
	for (k = 32; k < 128; k++) {
		if (str_start[k] != 96 + (k - 32)) {
			sprintf(log_line, "k %d str_start[k] %d\n", k, str_start[k]);
			show_line(log_line, 0);
		}
	}
	for (k = 128; k < 256; k++) {
		if (str_start[k] != 194 + 4 * (k - 128)){
			sprintf(log_line, "k %d str_start[k] %d\n", k, str_start[k]);
			show_line(log_line, 0);
		}
	}
	if (task != NULL) {
		sprintf(log_line, "Check on string pool (%s)\n", task);
		show_line(log_line, 0);
	}
	for (k = 0; k < str_ptr; k++) {
		if (str_start[k+1] == 0) break;
		n = str_start[k+1] - str_start[k];
		if (n < 0) break;
		st = str_start[k];
		flag = 0;
		for (i = 0; i < n; i++) {
			if (str_pool[st + i] == 0) {
				flag = 1; break;
			}
			if (str_pool[st + i] > 255) {
				flag = 1; break;
			}
		}
		if (flag) {
			bad++;
			sprintf(log_line, "BAD %d (start at %d): ", k, st);
			show_line(log_line, 0);
			s = log_line;
			for (i = 0; i < n; i++) {
				c = str_pool[st + i];
				if (c > 255) {
					sprintf(s, "%d ", c);
					s += strlen(s);
					continue;
				}
				if (c >= 128) {
					c -= 128;
					sprintf(s, "M-");
					s += strlen(s);
				}
				if (c < 32) {
					c += 64;
					sprintf(s, "C-");
					s += strlen(s);
				}
//				putc(c, stdout);
				*s++ = c;		// ???
			}
			*s++ = '\n';
			*s++ = '\0';
			show_line(log_line, stdout);
		}
	}
	sprintf(log_line, "end of check (%s)\n", bad ? "BAD" : "OK");
	show_line(log_line, 0);
	if (bad) {
		if (task == NULL) return bad;
		else exit(1);
	}
	return bad;
}  /* DEBUGGING ONLY */
#endif

void show_frozen (void)
{
	int i, j, n;
	fprintf(log_file, "\n");
	fprintf(log_file, "(%d fonts frozen in format file:\n", font_ptr);
/*	ignore font 0 which is nullfont */
/*	for (i = 1; i < font_ptr+1; i++) */
	for (i = 1; i <= font_ptr; i++) {
		if (i > 1) fprintf(log_file, ", ");
		if ((i % 8) == 0) fprintf(log_file, "\n");
		n = str_start[font_name[i]+1]-str_start[font_name[i]];
		for (j = 0; j < n; j++) {
			putc(str_pool[str_start[font_name[i]]+j], log_file);
/*				 str_pool[str_start[font_name[i]]+j] = '?'; */
		}
	}
	fprintf(log_file, ") ");
}

/* Main entry point called from texmf.c int main(int ac, char *av[]) */
/* call from main_program in texmf.c */
/* This in turn calls initialize() */

#pragma warning(disable:4127)		// conditional expression is constant

/* void texbody()  */
int texbody (void)					/* now returns a value --- bkph */
{ 
  history = 3; 

  set_paths(TEXFORMATPATHBIT + TEXINPUTPATHBIT + TEXPOOLPATHBIT + TFMFILEPATHBIT); 

  if(ready_already == 314159L)goto lab1; /* magic number */
/*	These tests are almost all compile time tests and so could be eliminated */
  bad = 0; 
  if((half_error_line < 30)||(half_error_line > error_line - 15)) 
	  bad = 1; 
  if(max_print_line < 60)	bad = 2; 
  if(dvi_buf_size % 8 != 0) bad = 3; 
  if(1100 > mem_top)bad = 4;		/* not compile time */
/*  if(hash_prime > hash_size) */
  if(hash_prime > (hash_size + hash_extra)) bad = 5; 
  if(max_in_open >= 128)bad = 6;		/* p.14 */
  if(mem_top < 267)bad = 7;		/* where from ? *//* not compile time */
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex) {					/* bkph */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  if((mem_min != 0)||(mem_max != mem_top))   bad = 10;
  }
#endif /* INITEX */
/*  if((mem_min > 0)||(mem_max < mem_top))  */
  if((mem_min > mem_bot)||(mem_max < mem_top))  bad = 10;
/*  if((0 > 0)||(255 < 127))  */
  if((min_quarterword > 0)||(max_quarterword < 255))  bad = 11;
/*  if((0 > 0)||(262143L < 32767))  */
  if((min_halfword > 0)||(max_halfword < 32767))  bad = 12;
/*  if((0 < 0)||(255 > 262143L))  */
  if((min_quarterword < min_halfword)||(max_quarterword > max_halfword)) 
	  bad = 13; 
/*  if((mem_min < 0)||(mem_max >= 262143L)||(-0 - mem_min > 262144L))  */
  if((mem_min < min_halfword)||(mem_max >= max_halfword)||(mem_bot - mem_min >= max_halfword)) 
	  bad = 14; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(mem_max > mem_top + mem_extra_high)			/* not compile time */
	  bad = 14;				/* ha ha */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  if((0 < 0)||(font_max > 255)) */
  if((0 < min_quarterword)||(font_max > max_quarterword))  /* 93/Dec/3 */
	  bad = 15; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
/*  if(font_max > 512)*/							/* 93/Dec/3 */
  if(font_max > 1024)								/* 96/Jan/17 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(font_max > 256) 
#endif
	  bad = 16; 
/*  if((save_size > 262143L)||(max_strings > 262143L)) */
  if((save_size > max_halfword)||(max_strings > max_halfword)) 
	  bad = 17; 
/*  if(buf_size > 262143L)*/
  if(buf_size > max_halfword)bad = 18; 
/*  if(255 < 255) */
  if(max_quarterword - min_quarterword < 255) bad = 19; 
/* if cs_token_flag + undefined_control_sequence > max_halfword then bad <- 21;*/
/*  if((hash_size + 4876) > 262143L)*/
/*  if((hash_size + 4876) > max_halfword) */
  if((hash_size + 4095 + 781) > max_halfword) bad = 21; 
  if(format_default_length > PATHMAX)	bad = 31; 
/*  if(2 * 262143L < mem_top - mem_min) */
  if(max_halfword < (mem_top - mem_min) / 2) bad = 41; 
  if(bad > 0) {
    sprintf(log_line,  "%s%s%ld\n",  "Ouch---my internal constants have been clobbered!",
			"---case ", (long) bad); 
	show_line(log_line, 1);
    if (! knuth_flag) 
		bad_formator_pool(format_file, "the format file", "TEXFORMATS");	/* 96/Jan/17 */
    goto lab9999;			// abort
  } 
  initialize (); 
	;
#ifdef INITEX
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (is_initex) {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  if(! get_strings_started ()) goto lab9999;		// abort
	  init_prim (); 
	  init_str_ptr = str_ptr; 
	  init_pool_ptr = pool_ptr; 
	  dateandtime(eqtb[(hash_size + 3183)].cint,
			eqtb[(hash_size + 3184)].cint,
				eqtb[(hash_size + 3185)].cint,
					eqtb[(hash_size + 3186)].cint); 
  }
#endif /* INITEX */
  ready_already = 314159L;			/* magic number */

lab1:			/* get here directly if ready_already already set ... */
  selector = 17; 
  tally = 0; 
  term_offset = 0; 
  file_offset = 0; 
  show_line(tex_version, 0); 
  sprintf(log_line, " (%s %s)", application, yandyversion);
  show_line(log_line, 0);
  if(format_ident > 0)slow_print(format_ident); 
  print_ln (); 
#ifndef _WINDOWS
  fflush(stdout); 
#endif
  job_name = 0; 
  name_in_progress = false; 
  log_opened = false; 
  output_file_name = 0; 
  {
    {
      input_ptr = 0; 
      max_in_stack = 0; 
      in_open = 0; 
	  high_in_open = 0;	/* max_in_open name already used 1999 Jan 17 */
      open_parens = 0; 
	  max_open_parens = 0;	/* max_open_parens */
      max_buf_stack = 0; 
      param_ptr = 0; 
      max_param_stack = 0; 
#ifdef ALLOCATEBUFFER
/*	  first = current_buf_size; */
	  memset (buffer, 0, current_buf_size);	/* redundant */
#else
/*	  first = buf_size; */
	  memset (buffer, 0, buf_size);			/* redundant ? */
#endif
/*      do {
		  buffer[first]= 0; 
		  decr(first); 
      } while(!(first == 0)); */
	  first = 0;							/* 1999/Jan/22 */

      scanner_status = 0; 
      warning_index = 0; /* warning_index:=null; l.7068 */
      first = 1; 
      cur_input.state_field = 33; 
      cur_input.start_field = 1; 
      cur_input.index_field = 0; 
      line = 0; 
      cur_input.name_field = 0; 
      force_eof = false; 
      align_state = 1000000L; 
      if(! init_terminal ())goto lab9999;	// abort
      cur_input.limit_field = last; 
      first = last + 1; 
    } 
/*    if((format_ident == 0)||(buffer[cur_input.loc_field]== 38)) */
/*		For Windows NT, lets allow + instead of & for format specification */
    if((format_ident == 0)||
		(buffer[cur_input.loc_field]== '&')||
		(buffer[cur_input.loc_field]==  '+')) 
    {
      if(format_ident != 0)initialize (); 
      if(! open_fmt_file ())goto lab9999; // abort
      if(! load_fmt_file ()){
		  w_close(fmt_file); 
		  goto lab9999;	// abort
      } 
      w_close(fmt_file); 
      while((cur_input.loc_field < cur_input.limit_field)&&
			 (buffer[cur_input.loc_field]== 32))
		  incr(cur_input.loc_field); 
    } 
#ifdef CHECKEQTB
	if (debug_flag) check_eqtb("after format");	/* debugging 94/Apr/5 */
#endif
    if((eqtb[(hash_size + 3211)].cint < 0)|| 
		(eqtb[(hash_size + 3211)].cint > 255)) 
		decr(cur_input.limit_field); 
/*	long to unsigned char ... */
    else buffer[cur_input.limit_field]= eqtb[(hash_size + 3211)].cint; 
    dateandtime(eqtb[(hash_size + 3183)].cint,
		eqtb[(hash_size + 3184)].cint,
			  eqtb[(hash_size + 3185)].cint,
				  eqtb[(hash_size + 3186)].cint); 
    magic_offset = str_start[886]- 9 * 16;	/* following: */
/*	"0234000122*4000133**3**344*0400400*000000234000111*1111112341011" */
    if(interaction == 0)	selector = 16; 
    else selector = 17; 
     if((cur_input.loc_field < cur_input.limit_field)&&
		(eqtb[(hash_size + 1883) + 
		   buffer[cur_input.loc_field]].hh.v.RH != 0)) 
    start_input (); 
  }

/*	show font TFMs frozen into format file */
/*	moved here after start_input to ensure the log file is open */
  if (show_tfm_flag && log_opened && font_ptr > 0) 		/* 98/Sep/28 */
	  show_frozen();


/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  main_time = clock();
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  history = 0; 

  if (show_cs_names) print_cs_names(stdout, 0);		/* 98/Mar/31 */

  main_control();			/* read-eval-print loop :-) in tex8.c */

//	what if abort_flag is set now ?

  if (show_cs_names) print_cs_names(stdout, 1);		/* 98/Mar/31 */

//	what if abort_flag is set now ?

  final_clean_up();

//  if (abort_flag) return -1;

  close_files_and_terminate(); 

  lab9999:
  {
	  int code;
#ifndef _WINDOWS
	  fflush(stdout); 
#endif
	  ready_already = 0; 
	  if((history != 0)&&(history != 1)) code = 1;
	  else code = 0;
//	  now return instead of exit to allow cleanup in local.c
	  return code;
//	  uexit(code);  
  } 
}	/* end of texbody */

#ifdef ALLOCATEMAIN
/* add a block of variable size node space below mem_bot */
void add_variable_space(int size)
{	/* used in tex0.c, local.c, itex.c */
	
	halfword p;
	halfword q;
	integer t;

	if (mem_min == 0) t = mem_min;	/* bottom of present block */
	else t = mem_min + 1; 
	mem_min = t - (size + 1);			/* first word in new block - 1 */
/*	mem_min = mem_start; */				/* allocate all of it at once */
	if (mem_min < mem_start) {			/* sanity test */
		if (trace_flag) show_line("WARNING: mem_min < mem_start!\n", 0);
		mem_min = mem_start;
	}
	p = mem[rover + 1].hh.v.LH; 
	q = mem_min + 1; 
	mem[mem_min].hh.v.RH = 0;	/* insert blank word below ??? */
	mem[mem_min].hh.v.LH = 0;	/* insert blank word below ??? */
	mem[p + 1].hh.v.RH = q;
	mem[rover + 1].hh.v.LH = q; 
	mem[q + 1].hh.v.RH = rover; 
	mem[q + 1].hh.v.LH = p; 
	mem[q].hh.v.RH = empty_flag; 
	mem[q].hh.v.LH = t - q;	/* block size */
	rover = q; 
}
#endif

/*************************************************************************/

/* All ini TeX code is here at end so can use same pragma optimize for it */

/* Ini-TeX code is rarely needed/used so make it small rather than fast */

#pragma optimize("t", off)
/* #pragma optimize("2", off) */
#pragma optimize("s", on)
/* #pragma optimize("1", on) */

#ifdef INITEX
/* split out to allow sharing of code from do_initex and newpattern */
void reset_trie (void)
{
	integer k;
	{
		register integer for_end;
		k = - (integer) trie_op_size;
		for_end = trie_op_size;
		if(k <= for_end)
			do trieophash[k]= 0; 
		while(k++ < for_end);
	} 
	{
		register integer for_end;
		k = 0; for_end = 255;
		if(k <= for_end)
			do   trie_used[k]= min_trie_op; 
		while(k++ < for_end);
	} 
	max_op_used = min_trie_op; 
	trie_op_ptr = 0; 
	trie_not_ready = true; 
	trie_l[0]= 0; 
	trie_c[0]= 0; 
	trie_ptr = 0; 
	trie_not_ready = true;
}

void reset_hyphen (void)
{	/* borrowed code from initialize() */
	hyph_pointer z;
  {
	  register integer for_end;
	  z = 0;
	  for_end = hyphen_prime;
	  if(z <= for_end) do 
	  {
		  hyph_word[z]= 0; 
		  hyph_list[z]= 0; /* hyph_list[z]:=null; l.18131 */
	  } 
	  while(z++ < for_end);
  } 
  hyph_count = 0;
}

void do_initex (void)
{	/* split out to allow optimize for space, not time */
	
/*  integer i; */
  integer k; 
/*  hyph_pointer z; */

/* for k:=mem_bot+1 to lo_mem_stat_max do mem[k].sc:=0; p.164 */
  {
	  register integer for_end;
	  k = 1;
	  for_end = 19;
	  if(k <= for_end)
		  do mem[k].cint = 0; 
	  while(k++ < for_end);
  } 
  k = 0; 
  while(k <= 19){			/* while k <= lo_mem_stat-max ... */
/*  glue_ref_count(k):=null+1; */
    mem[k].hh.v.RH = 1; 
    mem[k].hh.b0 = 0; 
    mem[k].hh.b1 = 0; 
    k = k + 4; 
  } 
  mem[6].cint = 65536L; 
  mem[4].hh.b0 = 1; 
  mem[10].cint = 65536L; 
  mem[8].hh.b0 = 2; 
  mem[14].cint = 65536L; 
  mem[12].hh.b0 = 1; 
  mem[15].cint = 65536L; 
  mem[12].hh.b1 = 1; 
  mem[18].cint = -65536L; 
  mem[16].hh.b0 = 1; 
  rover = 20;					/* rover = lo_mem_stat-max + 1 ... */
/*  mem[rover].hh.v.RH = 262143L;  */
  mem[rover].hh.v.RH = empty_flag; 
/*  mem[rover].hh.v.LH = 1000; */
  mem[rover].hh.v.LH = block_size; 
  mem[rover + 1].hh.v.LH = rover; 
  mem[rover + 1].hh.v.RH = rover; 
/*  lo_mem_max = rover + 1000; */
  lo_mem_max = rover + block_size;
  mem[lo_mem_max].hh.v.RH = 0; 
  mem[lo_mem_max].hh.v.LH = 0; 
/* for k <- hi_mem_stat_min to mem_top do mem[k] = mem[lo_mem_max]; */
  {
	  register integer for_end;
	  k = mem_top - 13;
	  for_end = mem_top;
	  if(k <= for_end)
		  do  mem[k]= mem[lo_mem_max]; 
	  while(k++ < for_end);
  } 
/* info(omit_template) <- end_template_token; p.790 */
/* @d end_template_token==cs_token_flag+frozen_end_template */
/*  mem[mem_top - 10].hh.v.LH = 14114;  */
/*  mem[mem_top - 10].hh.v.LH = 10019 + 4095; */  /* + eqtbextra ? NO */
/*  mem[mem_top - 10].hh.v.LH = (hash_size + 4614); */
/*  mem[mem_top - 10].hh.v.LH = (hash_size + 4095 + 519); */
  mem[mem_top - 10].hh.v.LH = (hash_size + hash_extra + 4095 + 519); 
/* link(end_span) <- max_quarterword + 1 p.797 */
/*  mem[mem_top - 9].hh.v.RH = 256;  */			/* 94/Apr/4 ? */
  mem[mem_top - 9].hh.v.RH = max_quarterword + 1;   /* 96/Oct/12 ??? */
/* info(end_span) <- null p.797 */
  mem[mem_top - 9].hh.v.LH = 0; 
/* type(last_active) <- hyphenated; p.820 */
  mem[mem_top - 7].hh.b0 = 1; 
/* line_number(last_active) <- max_halfword; p.820 */
/*  mem[mem_top - 6].hh.v.LH = 262143L;  */
  mem[mem_top - 6].hh.v.LH = empty_flag;	/* max_halfword ? */
/* subtype(last_active) <- 0; p.820 */
  mem[mem_top - 7].hh.b1 = 0; 
/* subtype(page_ins_head) <- 255; p.981 */
  mem[mem_top].hh.b1 = 255;	/* subtype(page_ins_head) = qi(255)  p.981 */ 
/* type(page_ins_head) <- split_up; p.981 */
  mem[mem_top].hh.b0 = 1; 
/* link(page_ins_head) <- page_ins_head; p.981 */
  mem[mem_top].hh.v.RH = mem_top; 
/* type(page_head) <- glue_node; p. 988 */
  mem[mem_top - 2].hh.b0 = 10; 
/* subtype(page_head) <- normal; p. 988 */
  mem[mem_top - 2].hh.b1 = 0; 
  avail = 0;							/* avail <- null p.164 */
  mem_end = mem_top;						/* mem_end <- mem_top */
  hi_mem_min = mem_top - 13;				/* hi_mem_min <- hi_mem_stat_min */
  var_used = 20;						/* var_used <- lo_mem_stat_max */
  dyn_used = 14;						/* dyn_used <- hi_mem_stat_usage */
/* eq_type(undefined_control_sequence) <- undefined_cs; */
/* equiv(undefined_control_sequence) <- null; */
/* eq_level(undefined_control_sequence) <- level_zero; */
  eqtb[(hash_size + 781)].hh.b0 = 101; 
  eqtb[(hash_size + 781)].hh.v.RH = 0; 
  eqtb[(hash_size + 781)].hh.b1 = 0; 
/* for k <- active_base to undefined_control_sequence -1 do */
/*				eqtb[k] <- eqtb(undefined_control_sequence); */
/*  {register integer for_end; k = 1; for_end = 10280; if(k <= for_end) do */
  {
	  register integer for_end;
	  k = 1;
	  for_end = (hash_size + 780);
	  if(k <= for_end) do 
		  eqtb[k]= eqtb[(hash_size + 781)]; 
	  while(k++ < for_end);
  } 
  eqtb[(hash_size + 782)].hh.v.RH = 0; /* glue_base (hash_size + 782) */
  eqtb[(hash_size + 782)].hh.b1 = 1; 
  eqtb[(hash_size + 782)].hh.b0 = 117; 
/* equiv(glue_base):=zero_glue; eq_level(glue_base):=level_one; */
/* eq_type(glue_base):=glue_ref; */
/*  {register integer for_end; k = 10283; for_end = 10811; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 783);
	  for_end = (hash_size + 1311);
	  if(k <= for_end) 
		  do eqtb[k]= eqtb[(hash_size + 782)]; 
	  while(k++ < for_end);
  } 
/* glue_ref_count(zero_glue):=glue_ref_count(zero_glue)+local_base-glue_base; */
/* local_base - glue_base = 530 = 17 glue_pars + 256 skip + 256 mu_skip */
  mem[0].hh.v.RH = mem[0].hh.v.RH + 530; /* mem[mem_bot]? */
/* box(0):=null; eq_type(box_base):=box_ref; eq_level(box_base):=level_one; */
  eqtb[(hash_size + 1312)].hh.v.RH = 0; 
  eqtb[(hash_size + 1312)].hh.b0 = 118; 
  eqtb[(hash_size + 1312)].hh.b1 = 1; 
/* for k:=box_base+1 to box_base+255 do eqtb[k]:=eqtb[box_base]; */
/*  {register integer for_end; k = 10813; for_end = 11077; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 1313);
	  for_end = (hash_size + 1577);
	  if(k <= for_end) 
		  do  eqtb[k]= eqtb[(hash_size + 781)]; 
	  while(k++ < for_end);
  } 
  eqtb[(hash_size + 1578)].hh.v.RH = 0; 
  eqtb[(hash_size + 1578)].hh.b0 = 119; 
  eqtb[(hash_size + 1578)].hh.b1 = 1; 
/*  {register integer for_end; k = 11079; for_end = 11333; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 1579);
	  for_end = (hash_size + 1833);
	  if(k <= for_end) 
		  do eqtb[k]= eqtb[(hash_size + 1578)]; 
	  while(k++ < for_end);
  } 
  eqtb[(hash_size + 1834)].hh.v.RH = 0; 
  eqtb[(hash_size + 1834)].hh.b0 = 120; 
  eqtb[(hash_size + 1834)].hh.b1 = 1; 
/*  {register integer for_end; k = 11335; for_end = 11382; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 1835);
	  for_end = (hash_size + 1882);
	  if(k <= for_end) 
		  do  eqtb[k]= eqtb[(hash_size + 1834)]; 
	  while(k++ < for_end);
  } 
  eqtb[(hash_size + 1883)].hh.v.RH = 0; 
  eqtb[(hash_size + 1883)].hh.b0 = 120; 
  eqtb[(hash_size + 1883)].hh.b1 = 1; 
/* initialize cat_code, lc_code, uc_code, sf_code, math_code 256 * 5 */
/*  {register integer for_end; k = 11384; for_end = 12662; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 1884);
	  for_end = (hash_size + 3162);
	  if(k <= for_end) 
		  do eqtb[k]= eqtb[(hash_size + 1883)]; 
	  while(k++ < for_end);
  } 
/* cat_code(k) <- other_char; math_code(k) <- hi(k); sf_code(k) = 1000; */
  {
	  register integer for_end;
	  k = 0;
	  for_end = 255;
	  if(k <= for_end) do 
	  {
		  eqtb[(hash_size + 1883) + k].hh.v.RH = 12; 
		  eqtb[(hash_size + 2907) + k].hh.v.RH = k; 
		  eqtb[(hash_size + 2651) + k].hh.v.RH = 1000; 
	  } 
	  while(k++ < for_end);
  } 
/* cat_base == 11383 */
/* cat_code(carriage_return) <- car_ret; cat_code(" ") <- space */
/* cat_code("\") <- escape; cat_code("%") <- comment; ... */  
  eqtb[(hash_size + 1896)].hh.v.RH = 5; 
  eqtb[(hash_size + 1915)].hh.v.RH = 10; 
  eqtb[(hash_size + 1975)].hh.v.RH = 0; 
  eqtb[(hash_size + 1920)].hh.v.RH = 14; 
  eqtb[(hash_size + 2010)].hh.v.RH = 15; 
  eqtb[(hash_size + 1883)].hh.v.RH = 9; 
/* for k:="0" to "9" do math_code(k):=hi(k+var_code); */
  {
	  register integer for_end;
	  k = 48;
	  for_end = 57;
	  if(k <= for_end) do 
		  eqtb[(hash_size + 2907) + k].hh.v.RH = k + 28672; /* '70000 */
	  while(k++ < for_end);
  } 
/* cat_code of uppercase and lowercase letters ... */
  {
	  register integer for_end;
	  k = 65;
	  for_end = 90;
	  if(k <= for_end) do 
    {
/* cat_code ... */
      eqtb[(hash_size + 1883) + k].hh.v.RH = 11; 
      eqtb[(hash_size + 1883) + k + 32].hh.v.RH = 11; 
/* mathcode(k) = hi(k + var_code + "100); */ /* '70000 + 256 */
      eqtb[(hash_size + 2907) + k].hh.v.RH =
		  k + 28928; 	/* '70000 + 256 */
/* mathcode(k + "a" - "A") = hi(k + "a" - "A" + var_code + "100); */
      eqtb[(hash_size + 2907) + k + 32].hh.v.RH =
		  k + 28960;  	/* '70000 + 256 + 32 */
/* lc_code ... */
      eqtb[(hash_size + 2139) + k].hh.v.RH = k + 32; 
      eqtb[(hash_size + 2139) + k + 32].hh.v.RH = k + 32; 
/* uc_code ... */
      eqtb[(hash_size + 2395) + k].hh.v.RH = k; 
      eqtb[(hash_size + 2395) + k + 32].hh.v.RH = k; 
/* sf_code */
      eqtb[(hash_size + 2651) + k].hh.v.RH = 999; 
    } 
  while(k++ < for_end);
  } 
/*  {register integer for_end; k = 12663; for_end = 12973; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 3163);
	  for_end = (hash_size + 3473);
	  if(k <= for_end) 
		  do 
			  eqtb[k].cint = 0; 
	  while(k++ < for_end);
  } 
  eqtb[(hash_size + 3180)].cint = 1000; 
  eqtb[(hash_size + 3164)].cint = 10000; 
  eqtb[(hash_size + 3204)].cint = 1; 
  eqtb[(hash_size + 3203)].cint = 25; 
  eqtb[(hash_size + 3208)].cint = 92; 
  eqtb[(hash_size + 3211)].cint = 13; 
/*  {register integer for_end; k = 13230; for_end = 13506; if(k <= for_end) */
  {
	  register integer for_end;
	  k = 0;
	  for_end = 255;
	  if(k <= for_end) do 
		  eqtb[(hash_size + 3474) + k].cint = -1; 
	  while(k++ < for_end);
  } 
  eqtb[(hash_size + 3520)].cint = 0; 
/*  {register integer for_end; k = 13230; for_end = 13506; if(k <= for_end) */
  {
	  register integer for_end;
	  k = (hash_size + 3730);
	  for_end = (hash_size + 4006);
	  if(k <= for_end) 
		  do  eqtb[k].cint = 0; 
	  while(k++ < for_end);
  } 
/*  hash_used = 10014;  */ /*   hash_used = frozen_control_sequence */
/* frozen_control_sequence =  hashsize + hashbase p.222 */
/*  hash_used = (hash_size + 514);  */
  hash_used = (hash_size + hash_extra + 514);	/* 96/Jan/10 */
  cs_count = 0; 
  if (trace_flag) show_line("itex cs_count=0 ", 0);		/* debugging */
/* eq_type(frozen_dont_expand) <- dont_expand; */
/*  eqtb[10023].hh.b0 = 116;  */
/*  eqtb[(hash_size + 523)].hh.b0 = 116;  */
  eqtb[(hash_size + hash_extra + 523)].hh.b0 = 116; 
/*  hash[(hash_size + 523)].v.RH = 499;  */
  hash[(hash_size + hash_extra + 523)].v.RH = 499;	/* notexpanded */
/* @<Initialize table...@>= l.10750 */
  font_ptr = 0;				/* font_ptr:=null_font; */
  fmem_ptr = 7;				/* fmem_ptr:=7; */
  font_name[0]= 795;	/* nullfont */
  font_area[0]= 335;	/* "" */
  hyphen_char[0]= 45;	/* - */
  skew_char[0]= -1; 
/* ************************************************************************ */
/* bchar_label[null_font]:=non_address; */ /* 3.14159 */
/*  bchar_label[0]= font_mem_size; */	/* OK ? 93/Nov/26 */
  bchar_label[0]= non_address;	/* i.e. 0 --- 96/Jan/16  */
/* ************************************************************************ */
  font_bchar[0]= 256;	/* font_bchar[null_font]:=non_char; */
  font_false_bchar[0]= 256; /* font_false_bchar[null_font]:=non_char; */
  font_bc[0]= 1; 
  font_ec[0]= 0; 
  font_size[0]= 0; 
  font_dsize[0]= 0; 
  char_base[0]= 0; 
  width_base[0]= 0; 
  height_base[0]= 0; 
  depth_base[0]= 0; 
  italic_base[0]= 0; 
  lig_kern_base[0]= 0; 
  kern_base[0]= 0; 
  exten_base[0]= 0; 
  font_glue[0]= 0; 
  font_params[0]= 7; 
  param_base[0]= -1; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***  */
  reset_trie ();					/* shared 93/Nov/26 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***  */
/*  {register integer for_end; k = 0; for_end = 6; if(k <= for_end) do 
    font_info[k].cint = 0; 
  while(k++ < for_end); } 
  {register integer for_end; k = - (integer) trie_op_size; for_end = 
  trie_op_size; if(k <= for_end) do 
    trieophash[k]= 0; 
  while(k++ < for_end); } 
  {register integer for_end; k = 0; for_end = 255; if(k <= for_end) do 
    trie_used[k]= min_trie_op; 
  while(k++ < for_end); } 
  max_op_used = min_trie_op; 
  trie_op_ptr = 0; 
  trie_not_ready = true; 
  trie_l[0]= 0; 
  trie_c[0]= 0; 
  trie_ptr = 0; */
/* text(frozen_protection):="inaccessible"; */
/*  hash[10014].v.RH = 1183; */
/*  hash[(hash_size + 514)].v.RH = 1184;  */
  hash[(hash_size + hash_extra + 514)].v.RH = 1184; /* 1183 */
  format_ident = 1251;	/* 1250 */
/*  hash[(hash_size + 522)].v.RH = 1290; */ /* 1288 */
  hash[(hash_size + hash_extra + 522)].v.RH = 1290; /* 1288 */
/*  eqtb[(hash_size + 522)].hh.b1 = 1;  */
  eqtb[(hash_size + hash_extra + 522)].hh.b1 = 1; 
/*  eqtb[(hash_size + 522)].hh.b0 = 113;  */
  eqtb[(hash_size + hash_extra + 522)].hh.b0 = 113; 
/*  eqtb[(hash_size + 522)].hh.v.RH = 0;  */
  eqtb[(hash_size + hash_extra + 522)].hh.v.RH = 0; 
} 
#endif /* INITEX */

#ifdef INITEX
/* starts string pool with strings for 256 chars and then reads tex.pool */
/* adjusted to try both "tex.pool" and "tex.poo" 95/Feb/19 */
bool get_strings_started (void) 
{/* 30 10 */ register bool Result;
  unsigned char k, l; 
  ASCII_code m, n; 
  str_number g; 
  integer a; 
  bool c; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  int flag;										/* 95/Feb/19 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  pool_ptr = 0; 
  str_ptr = 0; 
  str_start[0]= 0; 
  {
	  register integer for_end;
	  k = 0;
	  for_end = 255;
	  if(k <= for_end) do 
	  {
		  if(((k < 32)||(k > 126)))
		  {
			  {
				  str_pool[pool_ptr]= 94; /* ^ */
				  incr(pool_ptr); 
			  } 
			  {
				  str_pool[pool_ptr]= 94; /* ^ */
				  incr(pool_ptr); 
			  } 
			  if(k < 64)
			  {
				  str_pool[pool_ptr]= k + 64; 
				  incr(pool_ptr); 
			  } 
			  else if(k < 128)
			  {
				  str_pool[pool_ptr]= k - 64; 
				  incr(pool_ptr); 
			  } 
			  else {
				  l = k / 16;  
/*					l = k >> 4;  */
				  if(l < 10)
				  {
					  str_pool[pool_ptr]= l + 48;	/* '0' */
					  incr(pool_ptr); 
				  } 
				  else {
					  str_pool[pool_ptr]= l + 87;	/* 'a' - 10 */
					  incr(pool_ptr); 
				  } 
				  l = k % 16;  
/*					l = k & 15; */
				  if(l < 10)
				  {
					  str_pool[pool_ptr]= l + 48;	/* '0' */
					  incr(pool_ptr); 
				  } 
				  else {
					  str_pool[pool_ptr]= l + 87;	/* 'a' - 10 */
					  incr(pool_ptr); 
				  } 
			  } 
		  } 
		  else {
			  str_pool[pool_ptr]= k; 
			  incr(pool_ptr); 
		  } 
		  g = make_string (); 
	  } 
	  while(k++ < for_end);
  } 
  vstrcpy(name_of_file + 1, poolname); 
  name_of_file[0]= ' '; 
  name_of_file[strlen(poolname)+ 1]= ' '; 
  name_length = strlen(poolname); 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  if(a_open_in(pool_file, TEXPOOLPATH))  */
  flag = a_open_in(pool_file, TEXPOOLPATH);
  if (flag == 0) {							/* 95/Feb/19 */
	  poolname [name_length - 1] = '\0'; 	/* `tex.pool' => `tex.poo' */
	  vstrcpy(name_of_file + 1, poolname); 
	  name_of_file[0]= ' '; 
	  name_of_file[strlen(poolname)+ 1]= ' '; 
	  name_length = strlen(poolname);	  
	  flag = a_open_in(pool_file, TEXPOOLPATH);
  }
  if (flag) 
  {
    c = false; 
    do {
	{ 
/*	if(eof(pool_file))	*/			/* feof(pool_file)??? */
	if(test_eof(pool_file)) 
	{
	; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  show_line("! string pool file has no check sum.\n", 1); 
/*	  added following bit of explanation 96/Jan/16 */
	  if (! knuth_flag) 
	      bad_formator_pool(string_file, "the pool file", "TEXPOOL");
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  (void) a_close(pool_file); 
	  Result = false; 
	  return(Result); 
	} 
	read(pool_file, m); 
	read(pool_file, n); 
	if(m == '*')					/* last line starts with * */
	{
	  a = 0; 
	  k = 1; 
	  while(true){
	    if((xord[n]< 48)||(xord[n]> 57)) 
	    {
	   ; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
		  show_line("! string pool file check sum doesn't have nine digits.\n", 1);
/*		  added following bit of explanation 96/Jan/16 */
		  if (! knuth_flag) 
		      bad_formator_pool(string_file, "the pool file", "TEXPOOL");
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	      (void) a_close(pool_file); 
	      Result = false; 
	      return(Result); 
	    } 
	    a = 10 * a + xord[n]- 48; 
	    if(k == 9)
	    goto lab30; 
	    incr(k); 
	    read(pool_file, n); 
	  } 
/*	  tex.pool file check sum *367403084 */
	  lab30: if(a != BEGINFMTCHECKSUM){
	 ; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
		show_line("! string pool check sum doesn't match; tangle me again.\n", 1);
/*	    added following bit of explanation 96/Jan/16 */
		if (! knuth_flag) 
		    bad_formator_pool(string_file, "the pool file", "TEXPOOL");
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	    (void) a_close(pool_file); 
	    Result = false; 
	    return(Result); 
	  } 
	  c = true; 
	} 
	else {
	    
	  if((xord[m]< 48)||(xord[m]> 57)||(xord[n]< 48 
	)||(xord[n]> 57)) 
	  {
	 ; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
		show_line("! string pool line doesn't begin with two digits.\n", 1);
/*		added following bit of explanation 96/Jan/16 */
		if (! knuth_flag) 
		    bad_formator_pool(string_file, "the pool file", "TEXPOOL");
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	    (void) a_close(pool_file); 
	    Result = false; 
	    return(Result); 
	  } 
	  l = xord[m]* 10 + xord[n]- 48 * 11; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
/* can freely extend memory, so we need not be paranoid - stringvacancies */
/*	  if(pool_ptr + l + stringvacancies > current_pool_size)*/
	  if(pool_ptr + l + stringmargin > current_pool_size){
		  if (trace_flag) show_line("String margin reallocation\n", 0);
/*		  str_pool =  realloc_str_pool (pool_ptr + l + stringvacancies */
		  str_pool =  realloc_str_pool (pool_ptr + l + stringmargin 
					- current_pool_size  + increment_pool_size);
	  }
	  if(pool_ptr + l + stringmargin > current_pool_size)	/* 94/Jan/24 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
	  if(pool_ptr + l + stringvacancies > pool_size)
#endif
	  {
	 ; 
	    show_line("! You have to increase POOLSIZE.\n", 1);
	    (void) a_close(pool_file); 
	    Result = false; 
	    return(Result); 
	  } 
	  {
		  register integer for_end;
		  k = 1;
		  for_end = l;
		  if(k <= for_end) 
			  do 
			  {
				  if(eoln(pool_file)) 
					  m = ' '; 
				  else read(pool_file, m); 
				  {
					  str_pool[pool_ptr]= xord[m]; 
					  incr(pool_ptr); 
				  } 
			  } 
		  while(k++ < for_end);
	  } 
	  readln(pool_file);		/* flush rest to end of file / end of line */
	  g = make_string (); 
	} 
      } 
    } while(!(c)); 
    (void) a_close(pool_file); 
    Result = true; 
  }
  else {
      
 ; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    sprintf(log_line, "! I can't read %s.\n", poolname); 
	show_line(log_line, 1);
	if (! knuth_flag)
	show_line("  (Was unable to find tex.poo or tex.pool)\n", 0); /* extra explain */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    Result = false; 
    return(Result); 
  } 
  return Result; 
} 
#endif /* INITEX */

#ifdef INITEX
void sort_avail (void) 
{ 
  halfword p, q, r; 
  halfword oldrover; 
  p = get_node(1073741824L);			/* 2^30 merge adjacent free nodes */
  p = mem[rover + 1].hh.v.RH; 
/*  mem[rover + 1].hh.v.RH = 262143L;  */ /* NO! */
  mem[rover + 1].hh.v.RH = empty_flag; 
  oldrover = rover; 
  while(p != oldrover)if(p < rover)
  {
    q = p; 
    p = mem[q + 1].hh.v.RH; 
    mem[q + 1].hh.v.RH = rover; 
    rover = q; 
  } 
  else {
      
    q = rover; 
    while(mem[q + 1].hh.v.RH < p)q = mem[q + 1].hh.v.RH; 
    r = mem[p + 1].hh.v.RH; 
    mem[p + 1].hh.v.RH = mem[q + 1].hh.v.RH; 
    mem[q + 1].hh.v.RH = p; 
    p = r; 
  } 
  p = rover; 
/*  while(mem[p + 1].hh.v.RH != 262143L){ */	/* NO! */
  while(mem[p + 1].hh.v.RH != empty_flag){
      
    mem[mem[p + 1].hh.v.RH + 1].hh.v.LH = p; 
    p = mem[p + 1].hh.v.RH; 
  } 
  mem[p + 1].hh.v.RH = rover; 
  mem[rover + 1].hh.v.LH = p; 
} 
#endif /* INITEX */

#ifdef INITEX
void primitive_ (str_number s, quarterword c, halfword o)
{ 
  pool_pointer k; 
  small_number j; 
/*  small_number l;  */
  int l;						/* 95/Jan/7 */
  if(s < 256)
	  cur_val = s + 257;	/* cur_val <- s + single_base; p.264 */
  else {
    k = str_start[s]; 
    l = str_start[s + 1]- k;			/* small_number l */
    {
		register integer for_end;
		j = 0;
		for_end = l - 1;
		if(j <= for_end) 
			do buffer[j]= str_pool[k + j]; 
		while(j++ < for_end);
	} 
    cur_val = id_lookup(0, l); 
    {
      decr(str_ptr); 
      pool_ptr = str_start[str_ptr]; 
    } 
/*	**********************  debugging only  96/Jan/20 should not happen */
#ifdef SHORTHASH
	if (s > 65535L) show_line("ERROR: hash entry too large\n", 1);
#endif
    hash[cur_val].v.RH = s;
  }
  eqtb[cur_val].hh.b1 = 1; 
  eqtb[cur_val].hh.b0 = c; 
  eqtb[cur_val].hh.v.RH = o; 
} 
#endif /* INITEX */

/* more weird constants ? page 394 */

#ifdef INITEX
trie_op_code new_trie_op_ (small_number d, small_number n, trie_op_code v)
{/* 10 */ register trie_op_code Result; 
  integer h; 
  trie_op_code u; 
  integer l; 
/* the 313, 361 and 1009 are hard-wired constants here p.944 */
/* begin h:=abs(n+313*d+361*v+1009*cur_lang) mod (trie_op_size+trie_op_size) */
  h = abs(toint(n)+ 313 * toint(d)+ 361 * toint(v)+ 1009 *
		toint(cur_lang)) %(trie_op_size - neg_trie_op_size)+ neg_trie_op_size; 
  while(true){

/*  if l=0 then {empty position found for a new op} */
    l = trieophash[h]; 
    if(l == 0)
    {
      if(trie_op_ptr == trie_op_size){
		  overflow(943, trie_op_size);	/* pattern memory ops  - NOT DYNAMIC */
		  return 0;			// abort_flag set
	  }
      u = trie_used[cur_lang]; 
/*    if u=max_quarterword then ??? */
      if(u == max_trie_op){
/*		  overflow("pattern memory ops per language",
		  max_quarterword-min_quarterword); ??? */
		  overflow(944, max_trie_op - min_trie_op); /* pattern memory ops per language */
		  return 0;			// abort_flag set
	  }
      incr(trie_op_ptr); 
      incr(u); 
      trie_used[cur_lang]= u; 
      if(u > max_op_used)
      max_op_used = u; 
      hyf_distance[trie_op_ptr]= d; 
      hyf_num[trie_op_ptr]= n; 
      hyf_next[trie_op_ptr]= v; 
      trie_op_lang[trie_op_ptr]= cur_lang; 
      trieophash[h]= trie_op_ptr; 
      trie_op_val[trie_op_ptr]= u; 
      Result = u; 
      return(Result); 
    } 
    if((hyf_distance[l]== d)&&(hyf_num[l]== n)&&(hyf_next[l 
  ]== v)&&(trie_op_lang[l]== cur_lang)) 
    {
      Result = trie_op_val[l]; 
      return(Result); 
    } 
    if(h > - (integer) trie_op_size)
		decr(h); 
    else h = trie_op_size; 
  } 
/*  return Result;  */	/* unreachable code */
} 
/* what are those horrible constants there ? page 395 */
trie_pointer trie_node_ (trie_pointer p)
{/* 10 */ register trie_pointer Result; 
  trie_pointer h; 
  trie_pointer q; 
/* the 1009, 2718, 3142 are hard-wired constants here (not hyphen_prime) */
  h = abs(toint(trie_c[p])+ 1009 * toint(trie_o[p])+ 2718 * 
  toint(trie_l[p])+ 3142 * toint(trie_r[p])) % trie_size; 
  while(true){
    q = trie_hash[h]; 
    if(q == 0)
    {
      trie_hash[h]= p; 
      Result = p; 
      return(Result); 
    } 
    if((trie_c[q]== trie_c[p])&&(trie_o[q]== trie_o[p])&&(
    trie_l[q]== trie_l[p])&&(trie_r[q]== trie_r[p])) 
    {
      Result = q; 
      return(Result); 
    } 
    if(h > 0)
		decr(h); 
    else h = trie_size; 
  } 
/*  return Result;  */	/* unreachable code */
} 
trie_pointer compress_trie_ (trie_pointer p)
{register trie_pointer Result; 
  if(p == 0)
  Result = 0; 
  else {
      
    trie_l[p]= compress_trie(trie_l[p]); 
    trie_r[p]= compress_trie(trie_r[p]); 
    Result = trie_node(p); 
  } 
  return Result; 
} 
void first_fit_ (trie_pointer p)
{/* 45 40 */ 
  trie_pointer h; 
  trie_pointer z; 
  trie_pointer q; 
  ASCII_code c; 
  trie_pointer l, r; 
  short ll; 
  c = trie_c[p]; 
  z = trie_min[c]; 
  while(true){
    h = z - c; 
    if(trie_max < h + 256)
    {
      if(trie_size <= h + 256){ 	
		  overflow(945, trie_size);  /* pattern memory - NOT DYNAMIC */
/*		  not dynamic ---- but can be set -h=... from command line in ini-TeX */
		  return;			// abort_flag set
	  }
      do {
	  incr(trie_max); 
	trie_taken[trie_max]= false; 
	trietrl[trie_max]= trie_max + 1; 
	trietro[trie_max]= trie_max - 1; 
      } while(!(trie_max == h + 256)); 
    } 
    if(trie_taken[h])
    goto lab45; 
    q = trie_r[p]; 
    while(q > 0){
      if(trietrl[h + trie_c[q]] == 0)
      goto lab45; 
      q = trie_r[q]; 
    } 
    goto lab40; 
    lab45: z = trietrl[z]; 
  } 
lab40:
  trie_taken[h]= true; /* h may be used without ... */
  trie_hash[p]= h; 
  q = p; 
  do {
      z = h + trie_c[q]; 
    l = trietro[z]; 
    r = trietrl[z]; 
    trietro[r]= l; 
    trietrl[l]= r; 
    trietrl[z]= 0; 
    if(l < 256)
    {
      if(z < 256)
      ll = z;					/* short ll */
      else ll = 256; 
      do {
	  trie_min[l]= r; 
	incr(l); 
      } while(!(l == ll)); 
    } 
    q = trie_r[q]; 
  } while(!(q == 0)); 
} 
void trie_pack_ (trie_pointer p) 
{
  trie_pointer q; 
  do {
      q = trie_l[p]; 
    if((q > 0)&&(trie_hash[q]== 0)) 
    {
      first_fit(q); 
      trie_pack(q); 
    } 
    p = trie_r[p]; 
  } while(!(p == 0)); 
} 
void trie_fix_ (trie_pointer p)
{ 
  trie_pointer q; 
  ASCII_code c; 
  trie_pointer z; 
  z = trie_hash[p]; 
  do {
      q = trie_l[p]; 
    c = trie_c[p]; 
    trietrl[z + c]= trie_hash[q]; 
    trietrc[z + c]= c; 
    trietro[z + c]= trie_o[p]; 
    if(q > 0)
    trie_fix(q); 
    p = trie_r[p]; 
  } while(!(p == 0)); 
} 
void new_patterns (void) 
{/* 30 31 */ 
/* ******************************************************************* */
/*  was small_number k, l;  in 3.141 */
  char k, l; 
/* ******************************************************************* */
  bool digitsensed; 
  trie_op_code v; 
  trie_pointer p, q; 
  bool firstchild; 
/*  ASCII_code c;  */
  int c;								/* 95/Jan/7 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (! trie_not_ready) {					/* new stuff */
	  if (allow_patterns) {	
		  if (trace_flag) show_line("Resetting patterns\n", 0);
		  reset_trie();					/* RESET PATTERNS -  93/Nov/26 */
		  if (reset_exceptions) {
			  if (trace_flag) show_line("Resetting exceptions\n", 0);
			  reset_hyphen();			/* RESET HYPHENEXCEPTIONS -  93/Nov/26 */
		  }
	  }
  }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(trie_not_ready)
  {
    if(eqtb[(hash_size + 3213)].cint <= 0)
    cur_lang = 0; 
    else if(eqtb[(hash_size + 3213)].cint > 255)
    cur_lang = 0; 
    else cur_lang = eqtb[(hash_size + 3213)].cint; 
    scan_left_brace (); 
    k = 0; 
    hyf[0]= 0; 
    digitsensed = false; 
    while(true){
      get_x_token (); 
      switch(cur_cmd)
      {case 11 : 
      case 12 : 
	if(digitsensed ||(cur_chr < 48)||(cur_chr > 57)) 
	{
	  if(cur_chr == 46)		/* . */
	  cur_chr = 0; 
	  else {
	      
	    cur_chr = eqtb[(hash_size + 2139) + cur_chr].hh.v.RH; 
	    if(cur_chr == 0)
	    {
			print_err("Nonletter");
			help1("(See Appendix H.)");
	      error (); 
	    } 
	  } 
	  if(k < 63)
	  {
	    incr(k); 
	    hc[k]= cur_chr; 
	    hyf[k]= 0; 
	    digitsensed = false; 
	  } 
	} 
	else if(k < 63)
	{
	  hyf[k]= cur_chr - 48; 
	  digitsensed = true; 
	} 
	break; 
      case 10 : 
      case 2 : 
	{
	  if(k > 0)
	  {
	    if(hc[1]== 0)
	    hyf[0]= 0; 
	    if(hc[k]== 0)
	    hyf[k]= 0; 
	    l = k; 
	    v = min_trie_op; 
	    while(true){
	      if(hyf[l]!= 0)
	      v = new_trie_op(k - l, hyf[l], v); 
	      if(l > 0)
	      decr(l); 
	      else goto lab31; 
	    } 
	    lab31:; 
	    q = 0; 
	    hc[0]= cur_lang; 
	    while(l <= k){
	      c = hc[l]; 
	      incr(l); 
	      p = trie_l[q]; 
	      firstchild = true; 
	      while((p > 0)&&(c > trie_c[p])) {
		q = p; 
		p = trie_r[q]; 
		firstchild = false; 
	      } 
	      if((p == 0)||(c < trie_c[p])) 
	      {
		if(trie_ptr == trie_size){
			overflow(945, trie_size);	/* pattern memory - NOT DYNAMIC */
/*			not dynamic ---- but can be set -h=... from command line in ini-TeX */
			return;			// abort_flag set
		}
		incr(trie_ptr); 
		trie_r[trie_ptr]= p; 
		p = trie_ptr; 
		trie_l[p]= 0; 
		if(firstchild)
		trie_l[q]= p; 
		else trie_r[q]= p; 
		trie_c[p]= c; 
		trie_o[p]= min_trie_op; 
	      } 
	      q = p; 
	    } 
	    if(trie_o[q]!= min_trie_op)
	    {
			print_err("Duplicate pattern");
			help1("(See Appendix H.)");
	      error (); 
	    } 
	    trie_o[q]= v; 
	  } 
	  if(cur_cmd == 2)
	  goto lab30; 
	  k = 0; 
	  hyf[0]= 0; 
	  digitsensed = false; 
	} 
	break; 
	default: 
	{
		print_err("Bad ");
	  print_esc(947);	/* patterns */
	  help1("(See Appendix H.)");
	  error (); 
	} 
	break; 
      } 
    } 
    lab30:; 
  } 
  else {
      print_err("Too late for ");
    print_esc(947);	/* patterns */
	help1("All patterns must be given before typesetting begins.");
    error (); 
    mem[mem_top - 12].hh.v.RH = scan_toks(false, false); 
    flush_list(def_ref); 
  } 
} 
void init_trie (void) 
{
  trie_pointer p; 
/*  integer j, k, t;  */
  integer j, k; 
  int t;									/* 95/Jan/7 */
  trie_pointer r, s; 
  op_start[0]= - (integer) min_trie_op; 
  {
	  register integer for_end;
	  j = 1;
	  for_end = 255;
	  if(j <= for_end) do 
		  op_start[j]= op_start[j - 1]+ trie_used[j - 1]; 
	  while(j++ < for_end);
  } 
  {
	  register integer for_end;
	  j = 1;
	  for_end = trie_op_ptr;
	  if(j <= for_end) 
		  do 
			  trieophash[j]= op_start[trie_op_lang[j]] + trie_op_val[j]; 
	  while(j++ < for_end);
  } 
  {
	  register integer for_end;
	  j = 1;
	  for_end = trie_op_ptr;
	  if(j <= for_end) 
		  do 
	  while(trieophash[j]> j){
		  k = trieophash[j]; 
		  t = hyf_distance[k]; 
		  hyf_distance[k]= hyf_distance[j]; 
		  hyf_distance[j]= t; 
		  t = hyf_num[k]; 
		  hyf_num[k]= hyf_num[j]; 
		  hyf_num[j]= t; 
		  t = hyf_next[k]; 
		  hyf_next[k]= hyf_next[j]; 
		  hyf_next[j]= t; 
		  trieophash[j]= trieophash[k]; 
		  trieophash[k]= k; 
	  } 
	  while(j++ < for_end);
  } 
  {
	  register integer for_end;
	  p = 0;
	  for_end = trie_size;
	  if(p <= for_end) 
		  do trie_hash[p]= 0; 
	  while(p++ < for_end);
  } 
  trie_l[0]= compress_trie(trie_l[0]); 
  {
	  register integer for_end;
	  p = 0;
	  for_end = trie_ptr;
	  if(p <= for_end) 
		  do trie_hash[p]= 0; 
	  while(p++ < for_end);
  } 
  {
	  register integer for_end;
	  p = 0;
	  for_end = 255;
	  if(p <= for_end) do 
		  trie_min[p]= p + 1; 
	  while(p++ < for_end);
  } 
  trietrl[0]= 1; 
  trie_max = 0; 
  if(trie_l[0]!= 0)
  {
    first_fit(trie_l[0]); 
    trie_pack(trie_l[0]); 
  } 
  if(trie_l[0]== 0)
  {
    {
		register integer for_end;
		r = 0;
		for_end = 256;
		if(r <= for_end) do 
		{
			trietrl[r]= 0; 
			trietro[r]= min_trie_op; 
			trietrc[r]= 0; 
		} 
		while(r++ < for_end);
	} 
    trie_max = 256; 
  } 
  else {
    trie_fix(trie_l[0]); 
    r = 0; 
    do {
	s = trietrl[r]; 
      {
	trietrl[r]= 0; 
	trietro[r]= min_trie_op; 
	trietrc[r]= 0; 
      } 
      r = s; 
    } while(!(r > trie_max)); 
  } 
  trietrc[0]= 63; 
  trie_not_ready = false; 
} 
#endif /* INITEX */

#ifdef INITEX
void store_fmt_file (void) 
{/* 41 42 31 32 */
  integer j, k, l; 
  halfword p, q; 
  integer x; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (!is_initex) {			/* redundant check 94/Feb/14 */
	  show_line("! \\dump is performed only by INITEX\n", 1);
	  if (! knuth_flag)
		  show_line("  (Use -i on the command line)\n", 0);
	  abort_flag++;
	  return;
  }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(save_ptr != 0) {
	  print_err("You can't dump inside a group");
	  help1("`{...\\dump}' is a no-no..");
    {
      if(interaction == 3) interaction = 2; 
      if(log_opened){
		  error ();
	  }
	;
#ifdef DEBUG
      if(interaction > 0) debug_help (); 
#endif /* DEBUG */
      history = 3; 
      jump_out ();		// trying to \dump inside a group
//	  return;			// drops through now
	}
  } /* end of if save_ptr != 0 */

  selector = 21; 
  print(1266);		/*  (format= */
  print(job_name); 
  print_char(32);	/*   */
/*  print_int(eqtb[(hash_size + 3186)].cint % 100);  */	/* Y2K */
  print_int(eqtb[(hash_size + 3186)].cint);		/* 98/Oct/12 */
/*  {
	  int n= eqtb[(hash_size + 3186)].cint;
	  sprintf(log_line, "YEAR: %ld\n", n);
	  show_line(log_line, 0);
  } */
  print_char(46); /* . */
  print_int(eqtb[(hash_size + 3185)].cint); 
  print_char(46); /* . */
  print_int(eqtb[(hash_size + 3184)].cint); 
  print_char(41); /*)*/
  if(interaction == 0)selector = 18; 
  else selector = 19; 
  {
#ifdef ALLOCATESTRING
    if(pool_ptr + 1 > current_pool_size)
		str_pool = realloc_str_pool (increment_pool_size);
    if(pool_ptr + 1 > current_pool_size){ /* in case it failed 94/Jan/24 */
		overflow(257, current_pool_size - init_pool_ptr); /* 97/Mar/9 */
		return;			// abort_flag set
	}
#else
    if(pool_ptr + 1 > pool_size){
		overflow(257, pool_size - init_pool_ptr); /* pool size */
		return;			// abort_flag set
	}
#endif
  } 
  format_ident = make_string (); 
  pack_job_name(780);		/* .fmt */
  while(! w_open_out(fmt_file)) {
	  prompt_file_name(1267, 780); /* format file name  .fmt */
  }
  print_nl(" Beginning to dump on file ");		/*  */
  slow_print(w_make_name_string(fmt_file)); 
  {
    decr(str_ptr); 
    pool_ptr = str_start[str_ptr]; 
  } 
  print_nl("");		/* */
  slow_print(format_ident); 
  dump_int(BEGINFMTCHECKSUM); /* magic FMT file start 4C 20 E6 15 hex */
/*  dump_int(0); */
  dump_int(mem_bot); 
  dump_int(mem_top); 
  dump_int((hash_size + 4006));	/* eqtbsize */
  dump_int(hash_prime); 
/*  dump_int(607);  */
  dump_int(hyphen_prime);		/* bkph */
  dump_int(pool_ptr); 
  dump_int(str_ptr); 
  if (dumpthings(str_start[0], str_ptr + 1) 
	)return;
  if (dumpthings(str_pool[0], pool_ptr) 
	)return;
  print_ln (); 
  print_int(str_ptr); 
  print(1254);	/* strings of total length  */
  print_int(pool_ptr); 
  sort_avail (); 
  var_used = 0; 
  dump_int(lo_mem_max); 
  dump_int(rover); 
  p = 0; 
  q = rover; 
  x = 0; 
  do {
      if (dumpthings(mem[p], q + 2 - p) 
		)return;
    x = x + q + 2 - p; 
    var_used = var_used + q - p; 
    p = q + mem[q].hh.v.LH; 
    q = mem[q + 1].hh.v.RH; 
  } while(!(q == rover)); 
  var_used = var_used + lo_mem_max - p; 
  dyn_used = mem_end + 1 - hi_mem_min; 
  if (dumpthings(mem[p], lo_mem_max + 1 - p) 
	)return;
  x = x + lo_mem_max + 1 - p; 
  dump_int(hi_mem_min); 
  dump_int(avail); 
  if (dumpthings(mem[hi_mem_min], mem_end + 1 - hi_mem_min) 
	)return;
  x = x + mem_end + 1 - hi_mem_min; 
  p = avail; 
  while(p != 0){
    decr(dyn_used); 
    p = mem[p].hh.v.RH; 
  } 
  dump_int(var_used); 
  dump_int(dyn_used); 
  print_ln (); 
  print_int(x); 
  print(1255);	/* memory locations dumped; current usage is  */
  print_int(var_used); 
  print_char(38);	/* & */
  print_int(dyn_used); 
  k = 1; 
  do {
      j = k; 
    while(j < (hash_size + 3162)){
      if((eqtb[j].hh.v.RH == eqtb[j + 1].hh.v.RH)&&(eqtb[j 
    ].hh.b0 == eqtb[j + 1].hh.b0)&&(eqtb[j].hh.b1 == eqtb[j + 
      1].hh.b1)) 
      goto lab41; 
      incr(j); 
    } 
    l = (hash_size + 3163); 
    goto lab31; 
    lab41: incr(j); 
    l = j; 
    while(j < (hash_size + 3162)){
      if((eqtb[j].hh.v.RH != eqtb[j + 1].hh.v.RH)||(eqtb[j 
    ].hh.b0 != eqtb[j + 1].hh.b0)||(eqtb[j].hh.b1 != eqtb[j + 
      1].hh.b1)) 
      goto lab31; 
      incr(j); 
    } 
    lab31: dump_int(l - k); 
    if (dumpthings(eqtb[k], l - k) 
	 )return;
    k = j + 1; 
    dump_int(k - l); 
  } while(!(k == (hash_size + 3163))); 
  do {
      j = k; 
    while(j < (hash_size + 4006)){
      if(eqtb[j].cint == eqtb[j + 1].cint)
      goto lab42; 
      incr(j); 
    } 
    l = (hash_size + 4007); 
    goto lab32; 
    lab42: incr(j); 
    l = j; 
    while(j < (hash_size + 4006)){
      if(eqtb[j].cint != eqtb[j + 1].cint)
      goto lab32; 
      incr(j); 
    } 
    lab32: dump_int(l - k); 
    if (dumpthings(eqtb[k], l - k) 
	 )return;
    k = j + 1; 
    dump_int(k - l); 
  } while(!(k > (hash_size + 4006))); 
  dump_int(par_loc); 
  dump_int(write_loc); 
  dump_int(hash_used); 
/*  cs_count:=frozen_control_sequence-1-hash_used; */
/*  cs_count = (hash_size + 513) - hash_used;  */
  cs_count = (hash_size + hash_extra + 513) - hash_used; 
/*  cs_count = (hash_size + 780) - hash_used;  */ /* ??? */
  if (trace_flag) {
	  sprintf(log_line, "itex cs_count %d hash_size %d hash_extra %d hash_used %d",
		 cs_count, hash_size, hash_extra, hash_used);			/* debugging */
	  show_line(log_line, 0);
  }
/*	for p <- hash_base to hash_used do */
  {
	  register integer for_end;
	  p = 514;
	  for_end = hash_used;
	  if(p <= for_end) do 
		  if(hash[p].v.RH != 0){
			  dump_int(p); 
			  dump_hh(hash[p]); 
			  incr(cs_count); 
			  if (trace_flag) {
				  sprintf(log_line, "itex cs_count++ ");
				  show_line(log_line, 0); /* debugging */
			  }
		  } 
	  while(p++ < for_end);
  } 
/*	??? */
/* for p <- hash_used+1 to undefined_control_sequence-1 do dump_hh(hash[p]) */
  if (dumpthings(hash[hash_used + 1], (hash_size + 780) - hash_used) 
	)return;
  dump_int(cs_count); 
  print_ln (); 
  print_int(cs_count); 
  print(1256);	/*   multiletter control sequences */
  dump_int(fmem_ptr); 
  {
    if (dumpthings(font_info[0], fmem_ptr) 
	 )return;
/*	frozenfontptr = font_ptr; */		/* number of fonts frozen into format */
    dump_int(font_ptr); 
    if (dumpthings(font_check[0], font_ptr + 1))return;
    if (dumpthings(font_size[0], font_ptr + 1))return;
    if (dumpthings(font_dsize[0], font_ptr + 1))return;
    if (dumpthings(font_params[0], font_ptr + 1))return;
    if (dumpthings(hyphen_char[0], font_ptr + 1))return;
    if (dumpthings(skew_char[0], font_ptr + 1))return;
    if (dumpthings(font_name[0], font_ptr + 1))return;
    if (dumpthings(font_area[0], font_ptr + 1))return;
    if (dumpthings(font_bc[0], font_ptr + 1))return;
    if (dumpthings(font_ec[0], font_ptr + 1))return;
    if (dumpthings(char_base[0], font_ptr + 1))return;
    if (dumpthings(width_base[0], font_ptr + 1))return;
    if (dumpthings(height_base[0], font_ptr + 1))return;
    if (dumpthings(depth_base[0], font_ptr + 1))return;
    if (dumpthings(italic_base[0], font_ptr + 1))return;
    if (dumpthings(lig_kern_base[0], font_ptr + 1))return;
    if (dumpthings(kern_base[0], font_ptr + 1))return;
    if (dumpthings(exten_base[0], font_ptr + 1))return;
    if (dumpthings(param_base[0], font_ptr + 1))return;
    if (dumpthings(font_glue[0], font_ptr + 1))return;
    if (dumpthings(bchar_label[0], font_ptr + 1))return;
    if (dumpthings(font_bchar[0], font_ptr + 1))return;
    if (dumpthings(font_false_bchar[0], font_ptr + 1))return;
    {
		register integer for_end;
		k = 0;
		for_end = font_ptr;
		if(k <= for_end) 
			do 
      {
	print_nl("\\font");	/* \font */
/*	print_esc(hash[(hash_size + 524) + k].v.RH);  */
	print_esc(hash[(hash_size + hash_extra + 524) + k].v.RH); 
	print_char(61); /* = */
	print_file_name(font_name[k], font_area[k], 335); 
	if(font_size[k]!= font_dsize[k])
	{
	  print(738);	/*   at  */
	  print_scaled(font_size[k]); 
	  print(394);	/* pt */
	} 
      } 
    while(k++ < for_end);
	} 
  } 
  print_ln (); 
  print_int(fmem_ptr - 7); 
  print(1257);		/* words of font info for */
  print_int(font_ptr - 0); 
  print(1258);		/*  preloaded font */
  if(font_ptr != 1)
	  print_char(115);	/* s */
  dump_int(hyph_count); 
/*  {register integer for_end; k = 0; for_end = 607; if(k <= for_end) do */
  {
	  register integer for_end;
	  k = 0;
	  for_end = hyphen_prime;
	  if(k <= for_end) do 
		  if(hyph_word[k]!= 0)
		  {
			  dump_int(k); 
			  dump_int(hyph_word[k]); 
			  dump_int(hyph_list[k]); 
		  } 
	  while(k++ < for_end);
  } 
  print_ln (); 
  print_int(hyph_count); 
  print(1260);	/*  hyphenation exception */
  if(hyph_count != 1)
  print_char(115);	/* s */
  if(trie_not_ready)
  init_trie (); 
  dump_int(trie_max); 
  if (dumpthings(trietrl[0], trie_max + 1) 
	)return;
  if (dumpthings(trietro[0], trie_max + 1) 
	)return;
  if (dumpthings(trietrc[0], trie_max + 1) 
	)return;
  dump_int(trie_op_ptr); 
  if (dumpthings(hyf_distance[1], trie_op_ptr) 
	)return;
  if (dumpthings(hyf_num[1], trie_op_ptr) 
	)return;
  if (dumpthings(hyf_next[1], trie_op_ptr) 
	)return;
  print_nl("Hyphenation trie of length ");	/*  */
  print_int(trie_max); 
  print(1262);		/* has */
  print_int(trie_op_ptr); 
  print(1263);		/* op */
  if(trie_op_ptr != 1)
  print_char(115);	/* s */
  print(1264);		/* out of */
  print_int(trie_op_size); 
  {
	  register integer for_end;
	  k = 255;
	  for_end = 0;
	  if(k >= for_end) do 
		  if(trie_used[k]> 0)
		  {
			  print_nl("  ");		/*    */
			  print_int(trie_used[k]); 
			  print(1265);		/* for language */
			  print_int(k); 
			  dump_int(k); 
			  dump_int(trie_used[k]); 
		  } 
	  while(k-- > for_end);
  } 
  dump_int(interaction); 
  dump_int(format_ident); 
  dump_int(ENDFMTCHECKSUM);	/* magic checksum end of FMT file --- change ??? */ 
  eqtb[(hash_size + 3194)].cint = 0;	/* tracingstats  */
  w_close(fmt_file); 
//  return 0;
} /* end of store_fmt_file */
#endif /* INITEX */

#ifdef INITEX
void init_prim (void) 
{ 
  no_new_control_sequence = false; 
  primitive(373, 75, (hash_size + 782)); /* lineskip */
  primitive(374, 75, (hash_size + 783)); /* baselineskip */
  primitive(375, 75, (hash_size + 784)); /* parskip */
  primitive(376, 75, (hash_size + 785)); /* abovedisplayskip */
  primitive(377, 75, (hash_size + 786)); /* belowdisplayskip */
  primitive(378, 75, (hash_size + 787)); /* abovedisplayshortskip */
  primitive(379, 75, (hash_size + 788)); /* belowdisplayshortskip */
  primitive(380, 75, (hash_size + 789)); /* leftskip */
  primitive(381, 75, (hash_size + 790)); /* rightskip */
  primitive(382, 75, (hash_size + 791)); /* topskip */
  primitive(383, 75, (hash_size + 792)); /* splittopskip */
  primitive(384, 75, (hash_size + 793)); /* tabskip */
  primitive(385, 75, (hash_size + 794)); /* spaceskip */
  primitive(386, 75, (hash_size + 795)); /* xspaceskip */
  primitive(387, 75, (hash_size + 796)); /* parfillskip */
  primitive(388, 76, (hash_size + 797)); /* thinmuskip */
  primitive(389, 76, (hash_size + 798)); /* medmuskip */
  primitive(390, 76, (hash_size + 799)); /* thickmuskip */
  primitive(395, 72, (hash_size + 1313)); /* output */
  primitive(396, 72, (hash_size + 1314)); /* everypar */
  primitive(397, 72, (hash_size + 1315)); /* everymath */
  primitive(398, 72, (hash_size + 1316)); /* everydisplay */
  primitive(399, 72, (hash_size + 1317)); /* everyhbox */
  primitive(400, 72, (hash_size + 1318)); /* everyvbox */
  primitive(401, 72, (hash_size + 1319)); /* everyjob */
  primitive(402, 72, (hash_size + 1320)); /* everycr */
  primitive(403, 72, (hash_size + 1321)); /* errhelp */
  primitive(417, 73, (hash_size + 3163)); /* pretolerance */
  primitive(418, 73, (hash_size + 3164)); /* tolerance */
  primitive(419, 73, (hash_size + 3165)); /* linepenalty */
  primitive(420, 73, (hash_size + 3166)); /* hyphenpenalty */
  primitive(421, 73, (hash_size + 3167)); /* exhyphenpenalty */
  primitive(422, 73, (hash_size + 3168)); /* clubpenalty */
  primitive(423, 73, (hash_size + 3169)); /* widowpenalty */
  primitive(424, 73, (hash_size + 3170)); /* displaywidowpenalty */
  primitive(425, 73, (hash_size + 3171)); /* brokenpenalty */
  primitive(426, 73, (hash_size + 3172)); /* binoppenalty */
  primitive(427, 73, (hash_size + 3173)); /* relpenalty */
  primitive(428, 73, (hash_size + 3174)); /* predisplaypenalty */
  primitive(429, 73, (hash_size + 3175)); /* postdisplaypenalty */
  primitive(430, 73, (hash_size + 3176)); /* interlinepenalty */
  primitive(431, 73, (hash_size + 3177)); /* doublehyphendemerits */
  primitive(432, 73, (hash_size + 3178)); /* finalhyphendemerits */
  primitive(433, 73, (hash_size + 3179)); /* adjdemerits */
  primitive(434, 73, (hash_size + 3180)); /* mag */
  primitive(435, 73, (hash_size + 3181)); /* delimiterfactor */
  primitive(436, 73, (hash_size + 3182)); /* looseness */
  primitive(437, 73, (hash_size + 3183)); /* time */
  primitive(438, 73, (hash_size + 3184)); /* day */
  primitive(439, 73, (hash_size + 3185)); /* month */
  primitive(440, 73, (hash_size + 3186)); /* year */
  primitive(441, 73, (hash_size + 3187)); /* showboxbreadth */
  primitive(442, 73, (hash_size + 3188)); /* showboxdepth */
  primitive(443, 73, (hash_size + 3189)); /* hbadness */
  primitive(444, 73, (hash_size + 3190)); /* vbadness */
  primitive(445, 73, (hash_size + 3191)); /* pausing */
  primitive(446, 73, (hash_size + 3192)); /* tracingonline */
  primitive(447, 73, (hash_size + 3193)); /* tracingmacros */
  primitive(448, 73, (hash_size + 3194)); /* tracingstats */
  primitive(449, 73, (hash_size + 3195)); /* tracingparagraphs */
  primitive(450, 73, (hash_size + 3196)); /* tracingpages */
  primitive(451, 73, (hash_size + 3197)); /* tracingoutput */
  primitive(452, 73, (hash_size + 3198)); /* tracinglostchars */
  primitive(453, 73, (hash_size + 3199)); /* tracingcommands */
  primitive(454, 73, (hash_size + 3200)); /* tracingrestores */
  primitive(455, 73, (hash_size + 3201)); /* uchyph */
  primitive(456, 73, (hash_size + 3202)); /* outputpenalty */
  primitive(457, 73, (hash_size + 3203)); /* maxdeadcycles */
  primitive(458, 73, (hash_size + 3204)); /* hangafter */
  primitive(459, 73, (hash_size + 3205)); /* floatingpenalty */
  primitive(460, 73, (hash_size + 3206)); /* globaldefs */
  primitive(461, 73, (hash_size + 3207)); /* fam */
  primitive(462, 73, (hash_size + 3208)); /* escapechar */
  primitive(463, 73, (hash_size + 3209)); /* defaulthyphenchar */
  primitive(464, 73, (hash_size + 3210)); /* defaultskewchar */
  primitive(465, 73, (hash_size + 3211)); /* endlinechar */
  primitive(466, 73, (hash_size + 3212)); /* newlinechar */
  primitive(467, 73, (hash_size + 3213)); /* language */
  primitive(468, 73, (hash_size + 3214)); /* lefthyphenmin */
  primitive(469, 73, (hash_size + 3215)); /* righthyphenmin */
  primitive(470, 73, (hash_size + 3216)); /* holdinginserts */
  primitive(471, 73, (hash_size + 3217)); /* errorcontextlines */
  primitive(475, 74, (hash_size + 3730)); /* parindent */
  primitive(476, 74, (hash_size + 3731)); /* mathsurround */
  primitive(477, 74, (hash_size + 3732)); /* lineskiplimit */
  primitive(478, 74, (hash_size + 3733)); /* hsize */
  primitive(479, 74, (hash_size + 3734)); /* vsize */
  primitive(480, 74, (hash_size + 3735)); /* maxdepth */
  primitive(481, 74, (hash_size + 3736)); /* splitmaxdepth */
  primitive(482, 74, (hash_size + 3737)); /* boxmaxdepth */
  primitive(483, 74, (hash_size + 3738)); /* hfuzz */
  primitive(484, 74, (hash_size + 3739)); /* vfuzz */
  primitive(485, 74, (hash_size + 3740)); /* delimitershortfall */
  primitive(486, 74, (hash_size + 3741)); /* nulldelimiterspace */
  primitive(487, 74, (hash_size + 3742)); /* scriptspace */
  primitive(488, 74, (hash_size + 3743)); /* predisplaysize */
  primitive(489, 74, (hash_size + 3744)); /* displaywidth */
  primitive(490, 74, (hash_size + 3745)); /* displayindent */
  primitive(491, 74, (hash_size + 3746)); /* overfullrule */
  primitive(492, 74, (hash_size + 3747)); /* hangindent */
  primitive(493, 74, (hash_size + 3748)); /* hoffset */
  primitive(494, 74, (hash_size + 3749)); /* voffset */
  primitive(495, 74, (hash_size + 3750));	/* emergencystretch */
  primitive(32, 64, 0);	/*   */
  primitive(47, 44, 0);	/* / */
  primitive(505, 45, 0);	/* accent */
  primitive(506, 90, 0);	/* advance */
  primitive(507, 40, 0);	/* afterassignment */
  primitive(508, 41, 0);	/* aftergroup */
  primitive(509, 61, 0);	/* begingroup */
  primitive(510, 16, 0);	/* char */
  primitive(501, 107, 0);		/* csname */
  primitive(511, 15, 0);	/* delimiter */
  primitive(512, 92, 0);	/* divide */
  primitive(502, 67, 0);	/* endcsname */
  primitive(513, 62, 0);	/* endgroup */
/*  hash[(hash_size + 516)].v.RH = 513; */	
  hash[(hash_size + hash_extra + 516)].v.RH = 513;	/* endgroup */
/*  eqtb[(hash_size + 516)]= eqtb[cur_val]; */
  eqtb[(hash_size + hash_extra + 516)]= eqtb[cur_val]; 
  primitive(514, 102, 0);		/* expandafter */
  primitive(515, 88, 0);	/* font */
  primitive(516, 77, 0);	/* fontdimen */
  primitive(517, 32, 0);	/* halign */
  primitive(518, 36, 0);	/* hrule */
  primitive(519, 39, 0);	/* ignorespaces */
  primitive(327, 37, 0); /* insert */
  primitive(348, 18, 0); /* mark */
  primitive(520, 46, 0);	/* mathaccent */
  primitive(521, 17, 0);	/* mathchar */
  primitive(522, 54, 0);	/* mathchoice */
  primitive(523, 91, 0);	/* multiply */
  primitive(524, 34, 0);	/* noalign */
  primitive(525, 65, 0);	/* noboundary */
  primitive(526, 103, 0);		/* noexpand */
  primitive(332, 55, 0); /* nonscript */
  primitive(527, 63, 0);	/* omit */
  primitive(405, 84, 0);	/* parshape */
  primitive(528, 42, 0);	/* penalty */
  primitive(529, 80, 0);	/* prevgraf */
  primitive(530, 66, 0);	/* radical */
  primitive(531, 96, 0);	/* read */
  primitive(532, 0, 256);		/* primitive("relax",relax,256); */
/*  hash[(hash_size + 521)].v.RH = 532; */ 
  hash[(hash_size + hash_extra + 521)].v.RH = 532;  /* read */
/*  eqtb[(hash_size + 521)]= eqtb[cur_val];  */
  eqtb[(hash_size + hash_extra + 521)]= eqtb[cur_val]; 
  primitive(533, 98, 0);	/* setbox */
  primitive(534, 109, 0);		/* the */
  primitive(404, 71, 0);	/* toks */
  primitive(349, 38, 0); /* vadjust */
  primitive(535, 33, 0);	/* valign */
  primitive(536, 56, 0);	/* vcenter */
  primitive(537, 35, 0);	/* vrule */
  primitive(594, 13, 256);	/* par */
  par_loc = cur_val; 
  par_token = 4095 + par_loc; 
  primitive(626, 104, 0);		/* input */
  primitive(627, 104, 1);		/* endinput */
  primitive(628, 110, 0);		/* topmark */
  primitive(629, 110, 1);		/* firstmark */
  primitive(630, 110, 2);		/* botmark */
  primitive(631, 110, 3);		/* splitfirstmark */
  primitive(632, 110, 4);		/* splitbotmark */
  primitive(473, 89, 0);	/* count */
  primitive(497, 89, 1);	/* dimen */
  primitive(392, 89, 2);	/* skip */
  primitive(393, 89, 3);	/* muskip */
  primitive(665, 79, 102);	/* spacefactor */
  primitive(666, 79, 1);	/* prevdepth */
  primitive(667, 82, 0);	/* dead_cycles */
  primitive(668, 82, 1);	/* insert_penalties */
  primitive(669, 83, 1);	/* wd */
  primitive(670, 83, 3);	/* ht */
  primitive(671, 83, 2);	/* dp */
  primitive(672, 70, 0);	/* last_penalty */
  primitive(673, 70, 1);	/* last_kern */
  primitive(674, 70, 2);	/* lastskip */
  primitive(675, 70, 3);	/* inputlineno */
  primitive(676, 70, 4);	/* badness */
  primitive(732, 108, 0);		/* number */
  primitive(733, 108, 1);		/* romannumeral */
  primitive(734, 108, 2);		/* string */
  primitive(735, 108, 3);		/* meaning */
  primitive(736, 108, 4);		/* font_name */
  primitive(737, 108, 5);		/* job_name */
  primitive(753, 105, 0);		/* if */
  primitive(754, 105, 1);		/* ifcat */
  primitive(755, 105, 2);		/* ifnum */
  primitive(756, 105, 3);		/* ifdim */
  primitive(757, 105, 4);		/* ifodd */
  primitive(758, 105, 5);		/* ifvmode */
  primitive(759, 105, 6);		/* ifhmode */
  primitive(760, 105, 7);		/* ifmmode */
  primitive(761, 105, 8);		/* ifinner */
  primitive(762, 105, 9);		/* ifvoid */
  primitive(763, 105, 10);	/* ifhbox */
  primitive(764, 105, 11);	/* ifvbox */
  primitive(765, 105, 12);	/* ifx */
  primitive(766, 105, 13);	/* ifeof */
  primitive(767, 105, 14);	/* iftrue */
  primitive(768, 105, 15);	/* iffalse */
  primitive(769, 105, 16);	/* ifcase */
  primitive(770, 106, 2);		/* fi */
/*  hash[(hash_size + 518)].v.RH = 770; */ 
  hash[(hash_size + hash_extra + 518)].v.RH = 770;		/* fi */
/*  eqtb[(hash_size + 518)]= eqtb[cur_val];  */
  eqtb[(hash_size + hash_extra + 518)]= eqtb[cur_val]; 
  primitive(771, 106, 4);		/* or */
  primitive(772, 106, 3);		/* else */
  primitive(795, 87, 0);		/* nullfont */
/*  hash[(hash_size + 524)].v.RH = 795; */	/* hash[frozen_null_font] */
  hash[(hash_size + hash_extra + 524)].v.RH = 795;	/* nullfont */
/*  eqtb[(hash_size + 524)]= eqtb[cur_val];  */
  eqtb[(hash_size + hash_extra + 524)]= eqtb[cur_val]; 
  primitive(892, 4, 256);		/* span */
		  /* primitive("span",tab_mark,span_code); */
  primitive(893, 5, 257);		/* cr */
		  /* primitive("cr",car_ret,cr_code); */
/*  hash[(hash_size + 515)].v.RH = 893; */	
  hash[(hash_size + hash_extra + 515)].v.RH = 893;		/* cr */
/*  eqtb[(hash_size + 515)]= eqtb[cur_val];  */
  eqtb[(hash_size + hash_extra + 515)]= eqtb[cur_val]; 
  primitive(894, 5, 258);			/* cr cr */
/*  hash[(hash_size + 519)].v.RH = 895;  */
  hash[(hash_size + hash_extra + 519)].v.RH = 895; /* endtemplate */
/*  hash[(hash_size + 520)].v.RH = 895;  */
  hash[(hash_size + hash_extra + 520)].v.RH = 895; /* endtemplate */
/*  eqtb[(hash_size + 520)].hh.b0 = 9;  */
  eqtb[(hash_size + hash_extra + 520)].hh.b0 = 9; 
/*  eqtb[(hash_size + 520)].hh.v.RH = mem_top - 11;  */
  eqtb[(hash_size + hash_extra + 520)].hh.v.RH = mem_top - 11; 
/*  eqtb[(hash_size + 520)].hh.b1 = 1;  */
  eqtb[(hash_size + hash_extra + 520)].hh.b1 = 1; 
/*  eqtb[(hash_size + 519)]= eqtb[(hash_size + 520)];  */
  eqtb[(hash_size + hash_extra + 519)]=
		eqtb[(hash_size + hash_extra + 520)]; 
/*  eqtb[(hash_size + 519)].hh.b0 = 115;  */
  eqtb[(hash_size + hash_extra + 519)].hh.b0 = 115; 
  primitive(964, 81, 0);	/* pagegoal */
  primitive(965, 81, 1);	/* pagetotal */
  primitive(966, 81, 2);	/* pagestretch */
  primitive(967, 81, 3);	/* pagefilstretch */
  primitive(968, 81, 4);	/* pagefillstretch */
  primitive(969, 81, 5);	/* pagefilllstretch */
  primitive(970, 81, 6);	/* pageshrink */
  primitive(971, 81, 7);	/* pagedepth */
  primitive(1019, 14, 0);		/* end */
  primitive(1020, 14, 1);		/* dump */
  primitive(1021, 26, 4);		/* hskip */
  primitive(1022, 26, 0);		/* hfil */
  primitive(1023, 26, 1);		/* hfill */
  primitive(1024, 26, 2);		/* hss */
  primitive(1025, 26, 3);		/* hfilneg */
  primitive(1026, 27, 4);		/* vskip */
  primitive(1027, 27, 0);		/* vfil */
  primitive(1028, 27, 1);		/* vfill */
  primitive(1029, 27, 2);		/* vss */
  primitive(1030, 27, 3);		/* vfilneg */
  primitive(333, 28, 5); /* mskip */
  primitive(337, 29, 1); /* kern */
  primitive(339, 30, 99); /* mkern */
  primitive(1048, 21, 1);		/* moveleft */
  primitive(1049, 21, 0);		/* moveright */
  primitive(1050, 22, 1);		/* raise */
  primitive(1051, 22, 0);		/* lower */
  primitive(406, 20, 0);	/* box */
  primitive(1052, 20, 1);		/* copy */
  primitive(1053, 20, 2);		/* lastbox */
  primitive(959, 20, 3);	/* vsplit */
  primitive(1054, 20, 4);		/* vtop */
  primitive(961, 20, 5);	/* vbox */
  primitive(1055, 20, 106);	/* hbox */
  primitive(1056, 31, 99);	/* ship_out */
  primitive(1057, 31, 100);	/* leaders */
  primitive(1058, 31, 101);	/* cleaders */
  primitive(1059, 31, 102);	/* xleaders */
  primitive(1074, 43, 1);		/* indent */
  primitive(1075, 43, 0);		/* noindent */
  primitive(1084, 25, 12);	/* unpenalty */
  primitive(1085, 25, 11);	/* unkern */
  primitive(1086, 25, 10);	/* unskip */
  primitive(1087, 23, 0);		/* unhbox */
  primitive(1088, 23, 1);		/* unhcopy */
  primitive(1089, 24, 0);		/* unvbox */
  primitive(1090, 24, 1);		/* unvcopy */
  primitive(45, 47, 1);		/* - */
  primitive(346, 47, 0); /* discretionary */
  primitive(1121, 48, 0);		/* eqno */
  primitive(1122, 48, 1);		/* leqno */
  primitive(860, 50, 16);		/* mathord */
  primitive(861, 50, 17); /* mathop */
  primitive(862, 50, 18); /* mathbin */
  primitive(863, 50, 19); /* mathrel */
  primitive(864, 50, 20); /* mathopen */
  primitive(865, 50, 21); /* mathclose */
  primitive(866, 50, 22); /* mathpunct */
  primitive(867, 50, 23); /* mathinner */
  primitive(869, 50, 26); /* underline */
  primitive(868, 50, 27); /* overline */
  primitive(1123, 51, 0); /* displaylimits */
  primitive(872, 51, 1); /* limits */
  primitive(873, 51, 2); /* nolimits */
  primitive(855, 53, 0); /* displaystyle */
  primitive(856, 53, 2); /* textstyle */
  primitive(857, 53, 4); /* scriptstyle */
  primitive(858, 53, 6); /* scriptscriptstyle */
  primitive(1141, 52, 0); /* above */
  primitive(1142, 52, 1); /* over */
  primitive(1143, 52, 2); /* atop */
  primitive(1144, 52, 3); /* abovewithdelims */
  primitive(1145, 52, 4); /* overwithdelims */
  primitive(1146, 52, 5); /* atopwithdelims */
  primitive(870, 49, 30); /* left */
  primitive(871, 49, 31); /* right */
/*  hash[(hash_size + 517)].v.RH = 871;  */
  hash[(hash_size + hash_extra + 517)].v.RH = 871;		/* right */
/*  eqtb[(hash_size + 517)]= eqtb[cur_val];  */
  eqtb[(hash_size + hash_extra + 517)]= eqtb[cur_val]; 
  primitive(1165, 93, 1); /* long */
  primitive(1166, 93, 2); /* outer */
  primitive(1167, 93, 4); /* global */
  primitive(1168, 97, 0); /* def */
  primitive(1169, 97, 1); /* gdef */
  primitive(1170, 97, 2); /* edef */
  primitive(1171, 97, 3); /* xdef */
  primitive(1185, 94, 0); /* let */
  primitive(1186, 94, 1); /* futurelet */
  primitive(1187, 95, 0); /* chardef */
  primitive(1188, 95, 1); /* mathchardef */
  primitive(1189, 95, 2); /* countdef */
  primitive(1190, 95, 3); /* dimendef */
  primitive(1191, 95, 4); /* skipdef */
  primitive(1192, 95, 5); /* muskipdef */
  primitive(1193, 95, 6); /* toksdef */
  primitive(412, 85, (hash_size + 1883)); /* catcode */
  primitive(416, 85, (hash_size + 2907)); /* mathcode */
  primitive(413, 85, (hash_size + 2139)); /* lccode */
  primitive(414, 85, (hash_size + 2395)); /* uccode */
  primitive(415, 85, (hash_size + 2651)); /* sfcode */
  primitive(474, 85, (hash_size + 3474)); /* delcode */
  primitive(409, 86, (hash_size + 1835)); /* textfont */
  primitive(410, 86, (hash_size + 1851)); /* scriptfont */
  primitive(411, 86, (hash_size + 1867)); /* scriptscriptfont */
  primitive(935, 99, 0); /* hyphenation */
  primitive(947, 99, 1); /* patterns */
  primitive(1211, 78, 0); /* hyphen_char */
  primitive(1212, 78, 1); /* skew_char */
  primitive(272, 100, 0); /* batchmode */
  primitive(273, 100, 1); /* nonstopmode */
  primitive(274, 100, 2); /* scrollmode */
  primitive(1221, 100, 3); /* errorstopmode */
  primitive(1222, 60, 1); /* openin */
  primitive(1223, 60, 0); /* closein */
  primitive(1224, 58, 0); /* message */
  primitive(1225, 58, 1); /* errmessage */
  primitive(1231, 57, (hash_size + 2139)); /* lowercase */
  primitive(1232, 57, (hash_size + 2395)); /* uppercase */
  primitive(1233, 19, 0); /* show */
  primitive(1234, 19, 1); /* show_box */
  primitive(1235, 19, 2); /* showthe */
  primitive(1236, 19, 3); /* showlists */
  primitive(1279, 59, 0); /* openout */
  primitive(591, 59, 1); /* write */
  write_loc = cur_val; 
  primitive(1280, 59, 2);		/* closeout */
  primitive(1281, 59, 3);		/* special */
  primitive(1282, 59, 4);		/* immediate */
  primitive(1283, 59, 5);		/* setlanguage */
  no_new_control_sequence = true; 
} 
#endif /* INITEX */

#pragma optimize("s", off)						/* 96/Sep/12 */
/* #pragma optimize("1", off) */
#pragma optimize("t", on)						/* 96/Sep/12 */
/* #pragma optimize("2", on) */
