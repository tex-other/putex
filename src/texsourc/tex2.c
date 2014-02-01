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

/* following bit used to be end of tex1.c */

#ifdef STAT
void restore_trace_(halfword p, str_number s)
{ 
  begin_diagnostic(); 
  print_char(123);  /* { */
  print(s); 
  print_char(32); /*   */
  show_eqtb(p); 
  print_char(125);  /* } */
  end_diagnostic(false); 
} 
#endif /* STAT */
void unsave (void) 
{/* 30 */  
  halfword p; 
  quarterword l; 
  halfword t; 
  if(cur_level > 1)
  {
    decr(cur_level); 
    while(true){
      decr(save_ptr); 
      if(save_stack[save_ptr].hh.b0 == 3)
      goto lab30; 
      p = save_stack[save_ptr].hh.v.RH; 
      if(save_stack[save_ptr].hh.b0 == 2)
      {
  t = cur_tok; 
  cur_tok = p; 
  back_input (); 
  cur_tok = t; 
      } 
      else {
    
  if(save_stack[save_ptr].hh.b0 == 0)
  {
    l = save_stack[save_ptr].hh.b1; 
    decr(save_ptr); 
  } 
  else save_stack[save_ptr]= eqtb[(hash_size + 781)]; 
                    /* undefine_control_sequence */
  if(p < (hash_size + 3163))
  if(eqtb[p].hh.b1 == 1)
  {
    eq_destroy(save_stack[save_ptr]); 
  ;
#ifdef STAT
    if(eqtb[(hash_size + 3200)].cint > 0)
    restore_trace(p, 541); /* retaining */
#endif /* STAT */
  } 
  else {
      
    eq_destroy(eqtb[p]); 
    eqtb[p]= save_stack[save_ptr]; 
  ;
#ifdef STAT
    if(eqtb[(hash_size + 3200)].cint > 0)
    restore_trace(p, 542); /* restoring */
#endif /* STAT */
  } 
  else if(xeq_level[p]!= 1)
  {
    eqtb[p]= save_stack[save_ptr]; 
    xeq_level[p]= l;     /* l may be used without having been ... */
  ;
#ifdef STAT
    if(eqtb[(hash_size + 3200)].cint > 0)
    restore_trace(p, 542); /* restoring */
#endif /* STAT */
  } 
  else {
      
  ;
#ifdef STAT
    if(eqtb[(hash_size + 3200)].cint > 0)
    restore_trace(p, 541); /* retaining */
#endif /* STAT */
  } 
      } 
    } 
    lab30: cur_group = save_stack[save_ptr].hh.b1; 
    cur_boundary = save_stack[save_ptr].hh.v.RH; 
  } 
  else {
    confusion(540); /* cur_level */
    return;       // abort_flag set
  }
} 
/* This is where the old tex2.c used to start */
void prepare_mag (void) 
{
  if((mag_set > 0)&&(eqtb[(hash_size + 3180)].cint != mag_set)) 
  {
	  print_err("Incompatible magnification(");
    print_int(eqtb[(hash_size + 3180)].cint); 
    print(545);     /*)*/
    print_nl(" the previous value will be retained");
	help2("I can handle only one magnification ratio per job.",
		"So I've reverted to the magnification you used earlier on this run.");
    int_error(mag_set); 
    geq_word_define((hash_size + 3180), mag_set); 
  } 
  if((eqtb[(hash_size + 3180)].cint <= 0)||
    (eqtb[(hash_size + 3180)].cint > 32768L)) 
  {
	  print_err("Illegal magnification has been changed to 1000");
	  help1("The magnification ratio must be between 1 and 32768.");
    int_error(eqtb[(hash_size + 3180)].cint); 
    geq_word_define((hash_size + 3180), 1000); 
  } 
  mag_set = eqtb[(hash_size + 3180)].cint; 
} 
void token_show_ (halfword p)  
{
/* begin if p<>null then show_token_list(link(p),null,10000000); l.6289 */
  if(p != 0)
  show_token_list(mem[p].hh.v.RH, 0, 10000000L); 
} 
void print_meaning (void) 
{
  print_cmd_chr(cur_cmd, cur_chr); 
  if(cur_cmd >= 111)
  {
    print_char(58); /* : */
    print_ln (); 
    token_show(cur_chr); 
  } 
  else if(cur_cmd == 110)
  {
    print_char(58); /* : */
    print_ln (); 
    token_show(cur_mark[cur_chr]); 
  } 
} 
void show_cur_cmd_chr (void) 
{ 
  begin_diagnostic (); 
  print_nl("{");    /* { */
  if(mode != shown_mode)
  {
    print_mode(mode); 
    print(565); /* :  */
    shown_mode = mode; 
  } 
  print_cmd_chr(cur_cmd, cur_chr); 
  print_char(125);  /* } */
  end_diagnostic(false); 
} 
void show_context (void) 
{/* 30 */ 
  char old_setting; 
  integer nn; 
  bool bottomline; 
  integer i; 
  integer j; 
  integer l; 
  integer m; 
  integer n; 
  integer p; 
  integer q; 
  base_ptr = input_ptr; 
  input_stack[base_ptr] = cur_input; 
  nn = -1; 
  bottomline = false; 
  while(true){
    cur_input = input_stack[base_ptr]; 
    if((cur_input.state_field != 0)) 
    if((cur_input.name_field > 17)||(base_ptr == 0)) 
    bottomline = true; 
    if((base_ptr == input_ptr)|| bottomline ||
    (nn < eqtb[(hash_size + 3217)].cint)) 
    {
/* begin if (base_ptr=input_ptr) or (state<>token_list) or
   (token_type<>backed_up) or (loc<>null) then
    {we omit backed-up token lists that have already been read} l.6761 */
      if((base_ptr == input_ptr)||(cur_input.state_field != 0)||(
      cur_input.index_field != 3)||(cur_input.loc_field != 0)) 
      {
  tally = 0; 
  old_setting = selector;  
  if(cur_input.state_field != 0)
  {
    if(cur_input.name_field <= 17)
    if((cur_input.name_field == 0)) 
      if(base_ptr == 0)print_nl("<*>");   /* <*> */
      else print_nl("<insert> "); /*  */
    else {
      print_nl("<read ");    /* <read  */
      if(cur_input.name_field == 17)
      print_char(42);   /* * */
      else print_int(cur_input.name_field - 1); 
      print_char(62);   /* > */
    } 
    else {
/*      print_nl(574); */
/*      print_int(line); */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      if (c_style_flag) {         /* 94/Mar/21 */
        print_ln();         /* new line */
        /* show current input file name - ignore if from terminal */
        if (cur_input.name_field > 17)  /* redundant ? */
          print(cur_input.name_field);
        print_char(40);       /*(*/
        print_int(line);      /* line number */
        print_char(41);       /*)*/
        print_char(32);       /*   */
        print_char(58);       /* : */
      }
      else {
        print_nl("l.");        /* l. ? 573 ????? 98/Dec/8 check ! */
        print_int(line);      /* line number */
      }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    } 
    print_char(32);   /*   */
    {
      l = tally; 
      tally = 0; 
      selector = 20; 
      trick_count = 1000000L; 
    } 
    if(buffer[cur_input.limit_field]== eqtb[(hash_size + 3211)].cint)
    j = cur_input.limit_field; 
    else j = cur_input.limit_field + 1; 
    if(j > 0)
    {
      register integer for_end; 
      i = cur_input.start_field; 
      for_end = j - 1; 
      if(i <= for_end) do 
      {
        if(i == cur_input.loc_field)
        {
          first_count = tally; 
          trick_count = tally + 1 + error_line - half_error_line; 
          if(trick_count < error_line)
            trick_count = error_line; 
        } 
        print(buffer[i]); 
      } 
      while(i++ < for_end);
    } 
  } 
  else {
      
    switch(cur_input.index_field)
    {case 0 : 
      print_nl("<argument> "); /*  */  
    break; 
    case 1 : 
    case 2 : 
      print_nl("<template> "); /*  */
      break; 
    case 3 : 
      if(cur_input.loc_field == 0)
      print_nl("<recently read> ");  /*   */
      else print_nl("<to be read again> "); /*  */
      break; 
    case 4 : 
      print_nl("<inserted text> "); /*  */
      break; 
    case 5 : 
      {
        print_ln (); 
        print_cs(cur_input.name_field); 
      } 
      break; 
    case 6 : 
      print_nl("<output> "); /*  */
      break; 
    case 7 : 
      print_nl("<everypar> "); /*  */
      break; 
    case 8 : 
      print_nl("<everymath> "); /*  */
      break; 
    case 9 : 
      print_nl("<everydisplay> "); /*  */
      break; 
    case 10 : 
      print_nl("<everyhbox> "); /*  */
      break; 
    case 11 : 
      print_nl("<everyvbox> "); /*  */
      break; 
    case 12 : 
      print_nl("<everyjob> "); /*  */
      break; 
    case 13 : 
      print_nl("<everycr> "); /*  */
      break; 
    case 14 : 
      print_nl("<mark> "); /*  */
      break; 
    case 15 : 
      print_nl("<write> "); /*  */
      break; 
      default: 
      print_nl("?");  /* ? */
      break; 
    } 
    {
      l = tally; 
      tally = 0; 
      selector = 20; 
      trick_count = 1000000L; 
    } 
    if(cur_input.index_field < 5)
    show_token_list(cur_input.start_field, cur_input.loc_field, 100000L 
  ); 
    else show_token_list(mem[cur_input.start_field].hh.v.RH, 
    cur_input.loc_field, 100000L); 
  } 
  selector = old_setting; 
  if(trick_count == 1000000L)
  {
    first_count = tally; 
    trick_count = tally + 1 + error_line - half_error_line; 
    if(trick_count < error_line)
      trick_count = error_line; 
  } 
  if(tally < trick_count)
    m = tally - first_count; 
  else m = trick_count - first_count; 
  if(l + first_count <= half_error_line){
    p = 0; 
    n = l + first_count; 
  } 
  else {
    print(275);       /* ... */
    p = l + first_count - half_error_line + 3; 
    n = half_error_line; 
  } 
  {
    register integer for_end; 
    q = p; 
    for_end = first_count - 1; 
    if(q  <= for_end) do 
      print_char(trick_buf[q % error_line]); 
    while(q++ < for_end);
  } 
  print_ln (); 
  {
    register integer for_end; 
    q = 1; 
    for_end = n; 
    if(q <= for_end) 
      do print_char(32);    /*   */
    while(q++ < for_end);
  } 
  if(m + n <= error_line)
  p = first_count + m; 
  else p = first_count +(error_line - n - 3); 
  {
    register integer for_end; 
    q = first_count; 
    for_end = p - 1; 
    if(q  <= for_end) do 
      print_char(trick_buf[q % error_line]); 
    while(q++ < for_end);
  } 
  if(m + n > error_line)
    print(275);       /* ... */
  incr(nn); 
      } 
    } 
    else if(nn == eqtb[(hash_size + 3217)].cint)
    {
      print_nl("...");      /*  */
      incr(nn); 
    } 
    if(bottomline)
    goto lab30; 
    decr(base_ptr); 
  } 
  lab30: cur_input = input_stack[input_ptr]; 
}
#pragma optimize("g", off)          /* 98/Dec/10 experiment */
void begin_token_list_ (halfword p, quarterword t)
{
  {
    if(input_ptr > max_in_stack)
    {
      max_in_stack = input_ptr; 
#ifdef ALLOCATEINPUTSTACK
    if(input_ptr == current_stack_size)
      input_stack = realloc_input_stack (increment_stack_size);
    if(input_ptr == current_stack_size){    /* check again after allocation */
      overflow(590, current_stack_size);
      return;     // abort_flag set
    }
#else
    if(input_ptr == stack_size) { /* input stack - not dynamic */
      overflow(590, stack_size);
      return;     // abort_flag set
    }
#endif
    } 
    input_stack[input_ptr]= cur_input; 
    incr(input_ptr); 
  } 
  cur_input.state_field = 0; 
  cur_input.start_field = p; 
  cur_input.index_field = t; 
  if(t >= 5)
  {
    incr(mem[p].hh.v.LH); 
    if(t == 5)
    cur_input.limit_field = param_ptr; 
    else {
  
      cur_input.loc_field = mem[p].hh.v.RH; 
      if(eqtb[(hash_size + 3193)].cint > 1)
      {
  begin_diagnostic (); 
  print_nl("");   /* */
  switch(t)
  {case 14 : 
    print_esc("mark");
    break; 
  case 15 : 
    print_esc("write");
    break; 
    default: 
    print_cmd_chr(72, t + (hash_size + 1307));  /* H */
    break; 
  } 
  print(553); /* -> */
  token_show(p); 
  end_diagnostic(false); 
      } 
    } 
  } 
  else cur_input.loc_field = p; 
} 
#pragma optimize("", on)          /* 98/Dec/10 experiment */
void end_token_list (void) 
{ 
  if(cur_input.index_field >= 3)
  {
    if(cur_input.index_field <= 4)
    flush_list(cur_input.start_field); 
    else {
      delete_token_ref(cur_input.start_field); 
      if(cur_input.index_field == 5)
      while(param_ptr > cur_input.limit_field){
      decr(param_ptr); 
      flush_list(param_stack[param_ptr]); 
      } 
    } 
  } 
  else if(cur_input.index_field == 1)
  if(align_state > 500000L)align_state = 0; 
  else {
    fatal_error("(interwoven alignment preambles are not allowed)"); /*  */
    return;     // abort_flag set
  }
  {
    decr(input_ptr); 
    cur_input = input_stack[input_ptr]; 
  } 
  {
    if(interrupt != 0){
    pause_for_instructions ();
  }
  } 
}
void back_input (void) 
{ 
  halfword p; 
  while((cur_input.state_field == 0)&&(cur_input.loc_field == 0)) {
    end_token_list ();
  }
  p = get_avail (); 
  mem[p].hh.v.LH = cur_tok; 
  if(cur_tok < 768)
    if(cur_tok < 512)
      decr(align_state); 
    else incr(align_state); 
  {
    if(input_ptr > max_in_stack)
    {
      max_in_stack = input_ptr; 
#ifdef ALLOCATEINPUTSTACK
      if(input_ptr == current_stack_size)
        input_stack = realloc_input_stack (increment_stack_size);
      if(input_ptr == current_stack_size){  /* check again after allocation */
        overflow(590, current_stack_size);
        return;     // abort_flag set
      }
#else
      if(input_ptr == stack_size) { /* stack size - not dynamic */
        overflow(590, stack_size);
        return;     // abort_flag set
      }
#endif
    } 
    input_stack[input_ptr]= cur_input; 
    incr(input_ptr); 
  } 
  cur_input.state_field = 0; 
  cur_input.start_field = p; 
  cur_input.index_field = 3; 
  cur_input.loc_field = p; 
} 
void back_error (void) 
{ 
  OK_to_interrupt = false; 
  back_input (); 
  OK_to_interrupt = true; 
  error (); 
} 
void ins_error (void) 
{ 
    OK_to_interrupt = false; 
  back_input (); 
  cur_input.index_field = 4; 
  OK_to_interrupt = true; 
  error (); 
} 
void begin_file_reading (void) 
{ 
    if(in_open == max_in_open){
      overflow(593, max_in_open); /* text input levels - NOT DYNAMIC */
    return;     // abort_flag set
  }
#ifdef ALLOCATEBUFFER
    if(first == current_buf_size)
    buffer = realloc_buffer (increment_buf_size);
  if(first == current_buf_size) {   /* check again after allocation */
    overflow(256, current_buf_size);
    return;     // abort_flag set
  }
#else
  if(first == buf_size){
    overflow(256, buf_size);  /* buffer size - not dynamic */
    return;     // abort_flag set
  }
#endif

  incr(in_open); 
  if (in_open > high_in_open)     /* 1999 Jan 17 */
    high_in_open = in_open;
  {
    if(input_ptr > max_in_stack)
    {
      max_in_stack = input_ptr; 
#ifdef ALLOCATEINPUTSTACK
    if(input_ptr == current_stack_size)
      input_stack = realloc_input_stack (increment_stack_size);
    if(input_ptr == current_stack_size){
      overflow(590, current_stack_size);  /* check again after allocation */
      return;     // abort_flag set
    }
#else
    if(input_ptr == stack_size){
      overflow(590, stack_size);    /* input stack - not dynamic */
      return;     // abort_flag set
    }
#endif
    } 
    input_stack[input_ptr]= cur_input; 
    incr(input_ptr); 
  } 
  cur_input.index_field = in_open; 
  line_stack[cur_input.index_field]= line; 
  cur_input.start_field = first; 
  cur_input.state_field = 1; 
  cur_input.name_field = 0; 
} 
void end_file_reading (void) 
{ 
  first = cur_input.start_field; 
  line = line_stack[cur_input.index_field]; 
  if(cur_input.name_field > 17)
    (void) a_close(input_file[cur_input.index_field]); 
  {
    decr(input_ptr); 
    cur_input = input_stack[input_ptr]; 
  } 
  decr(in_open); 
} 
/* called only form tex0.c */
void clear_for_error_prompt (void) 
{
  while((cur_input.state_field != 0)&&
     (cur_input.name_field == 0)&&
     (input_ptr > 0)&&
     (cur_input.loc_field > cur_input.limit_field)) 
  end_file_reading (); 
  print_ln (); 
} 
void check_outer_validity (void) 
{ 
  halfword p; 
  halfword q; 
  if(scanner_status != 0)
  {
    deletions_allowed = false; 
    if(cur_cs != 0)
    {
      if((cur_input.state_field == 0)||(cur_input.name_field < 1)||
      (cur_input.name_field > 17)) 
      {
/*     begin p:=get_avail; info(p):=cs_token_flag+cur_cs; */
  p = get_avail (); 
  mem[p].hh.v.LH = 4095 + cur_cs; 
  begin_token_list(p, 3); 
      } 
      cur_cmd = 10; 
      cur_chr = 32; 
    } 
    if(scanner_status > 1)
    {
      runaway (); 
      if(cur_cs == 0)
		  print_err("File ended");
      else {
    
  cur_cs = 0;
  print_err("Forbidden control sequence found");
      } 
      print(603); /*  while scanning  */
      p = get_avail (); 
      switch(scanner_status)
      {case 2 : 
  {
    print(567); /* definition */
    mem[p].hh.v.LH = 637; 
  } 
  break; 
      case 3 : 
  {
    print(609); /* use */
    mem[p].hh.v.LH = par_token; 
    long_state = 113; 
  } 
  break; 
      case 4 : 
  {
    print(569); /* preamble */
    mem[p].hh.v.LH = 637; 
    q = p; 
    p = get_avail (); 
    mem[p].hh.v.RH = q; 
/*    mem[p].hh.v.LH = (hash_size + 4610);  */
/*    mem[p].hh.v.LH = (hash_size + 4095 + 515);  */
    mem[p].hh.v.LH = (hash_size + hash_extra + 4095 + 515); /*96/Jan/10*/
    align_state = -1000000L; 
  } 
  break; 
      case 5 : 
  {
    print(570); /* text */
    mem[p].hh.v.LH = 637; 
  } 
  break; 
      } 
      begin_token_list(p, 4); 
      print(604); /*  of  */
      sprint_cs(warning_index);
	  help4("I suspect you have forgotten a `}', causing me",
		  "to read past where you wanted me to stop.",
		  "I'll try to recover; but if the error is serious,",
		  "you'd better type `E' or `X' now and fix your file.");
      error (); 
    } 
    else {
  print_err("Incomplete ");
      print_cmd_chr(105, cur_if); /* i */
      print(596); /*; all text was ignored after line  */
      print_int(skip_line);
	  help3("A forbidden control sequence occurred in skipped text.",
		  "This kind of error happens when you say `\\if...' and forget",
		  "the matching `\\fi'. I've inserted a `\\fi'; this might work.");
      if(cur_cs != 0)
      cur_cs = 0; 
      else help_line[2]= 600; /* The file ended while I was skipping conditional text. */
/*      cur_tok = (hash_size + 4613);  */
/*      cur_tok = (hash_size + 4095 + 518);  */
      cur_tok = (hash_size + hash_extra + 4095 + 518); /* 96/Jan/10 */
      ins_error (); 
  }
    deletions_allowed = true; 
  }
} 
/*****************************************************************************/
/* get_next() moved from here to end for pragma optimize reasons 96/Sep/12 */
void get_next(void);
/*****************************************************************************/
void firm_up_the_line (void) 
{ 
  integer k; 
  cur_input.limit_field = last; 
  if(eqtb[(hash_size + 3191)].cint > 0)
    if(interaction > 1) {
    ; 
      print_ln (); 
      if(cur_input.start_field < cur_input.limit_field) {
        register integer for_end; 
        k = cur_input.start_field; 
        for_end = cur_input.limit_field - 1; 
        if(k <= for_end) do print(buffer[k]); 
        while(k++ < for_end);
      } 
      first = cur_input.limit_field; 
      {
      ; 
        print(615); /* => */
        term_input(615, 0); 
      } 
      if(last > first){
        {
          register integer for_end; 
          k = first; 
          for_end = last - 1; 
          if(k <= for_end) do 
            buffer[k + cur_input.start_field - first]= buffer[k]; 
          while(k++ < for_end);
        } 
        cur_input.limit_field = cur_input.start_field + last - first; 
      } 
    } 
} 
void get_token (void) 
{ 
  no_new_control_sequence = false; 
  get_next (); 
  no_new_control_sequence = true; 
  if(cur_cs == 0)cur_tok =(cur_cmd * 256)+ cur_chr; 
  else cur_tok = 4095 + cur_cs; 
} 
void macro_call (void) 
{/* 10 22 30 31 40 */ 
  halfword r; 
  halfword p; 
  halfword q; 
  halfword s; 
  halfword t; 
  halfword u, v; 
  halfword rbraceptr; 
  small_number n; 
  halfword unbalance; 
  halfword m; 
  halfword refcount; 
  small_number savescannerstatus; 
  halfword savewarningindex; 
  ASCII_code matchchr; 

  savescannerstatus = scanner_status;  
  savewarningindex = warning_index; 
  warning_index = cur_cs; 
  refcount = cur_chr; 
  r = mem[refcount].hh.v.RH; 
  n = 0; 
  if(eqtb[(hash_size + 3193)].cint > 0)
  {
    begin_diagnostic (); 
    print_ln (); 
    print_cs(warning_index); 
    token_show(refcount); 
    end_diagnostic(false); 
  } 
  if(mem[r].hh.v.LH != 3584)
  {
    scanner_status = 3; 
    unbalance = 0; 
    long_state = eqtb[cur_cs].hh.b0; 
    if(long_state >= 113)
    long_state = long_state - 2; 
    do {
  mem[mem_top - 3].hh.v.RH = 0; /* repeat link(temp_head):=null; */
      if((mem[r].hh.v.LH > 3583)||(mem[r].hh.v.LH < 3328)) 
      s = 0; /* s:=null l.7984 */
      else {
    
  matchchr = mem[r].hh.v.LH - 3328; 
  s = mem[r].hh.v.RH; 
  r = s; 
  p = mem_top - 3; 
  m = 0; 
      } 
      lab22: get_token (); 
      if(cur_tok == mem[r].hh.v.LH)
      {
  r = mem[r].hh.v.RH; 
  if((mem[r].hh.v.LH >= 3328)&&(mem[r].hh.v.LH <= 3584 
  ))
  {
    if(cur_tok < 512)
    decr(align_state); 
    goto lab40; 
  } 
  else goto lab22; 
      } 
      if(s != r)
      if(s == 0)
      {
		  print_err("Use of ");
  sprint_cs(warning_index); 
  print(648);     /*  doesn't match its definition */
  help4("If you say, e.g., `\\def\\a1{...}', then you must always",
	  "put `1' after `\\a', since control sequence names are",
	  "made up of letters only. The macro here has not been",
	  "followed by the required stuff, so I'm ignoring it."); 
  error (); 
  goto lab10; 
      } 
      else {
    
  t = s; 
  do {
      { 
      q = get_avail (); 
      mem[p].hh.v.RH = q; 
      mem[q].hh.v.LH = mem[t].hh.v.LH; 
      p = q; 
    } 
    incr(m); 
    u = mem[t].hh.v.RH; 
    v = s; 
    while(true){
      if(u == r)
      if(cur_tok != mem[v].hh.v.LH)
      goto lab30; 
      else {
    
        r = mem[v].hh.v.RH; 
        goto lab22; 
      } 
      if(mem[u].hh.v.LH != mem[v].hh.v.LH)
      goto lab30; 
      u = mem[u].hh.v.RH; 
      v = mem[v].hh.v.RH; 
    } 
    lab30: t = mem[t].hh.v.RH; 
  } while(!(t == r)); 
  r = s; 
      } 
      if(cur_tok == par_token)
      if(long_state != 112)
      {
  if(long_state == 111)
  {
    runaway ();
	print_err("Paragraph ended before ");
    sprint_cs(warning_index); 
    print(643);   /* was complete */
	help3("I suspect you've forgotten a `}', causing me to apply this",
		"control sequence to too much text. How can we recover?",
		"My plan is to forget the whole thing and hope for the best."); 
    back_error (); 
  } 
  pstack[n]= mem[mem_top - 3].hh.v.RH; 
  align_state = align_state - unbalance; 
  {
    register integer for_end; 
    m = 0; 
    for_end = n; 
    if(m <= for_end) do 
      flush_list(pstack[m]); 
    while(m++ < for_end);
  } 
  goto lab10; 
      } 
      if(cur_tok < 768)
      if(cur_tok < 512)
      {
  unbalance = 1; 
  while(true){
    {
      {
        q = avail; 
        if(q == 0)
        q = get_avail (); 
        else {
      
    avail = mem[q].hh.v.RH; 
    mem[q].hh.v.RH = 0; 
  ;
#ifdef STAT
    incr(dyn_used); 
#endif /* STAT */
        } 
      } 
      mem[p].hh.v.RH = q; 
      mem[q].hh.v.LH = cur_tok; 
      p = q; 
    } 
    get_token (); 
    if(cur_tok == par_token)
    if(long_state != 112)
    {
      if(long_state == 111)
      {
        runaway ();
		print_err("Paragraph ended before ");
        sprint_cs(warning_index); 
        print(643); /*  was complete */
		help3("I suspect you've forgotten a `}', causing me to apply this",
			"control sequence to too much text. How can we recover?",
			"My plan is to forget the whole thing and hope for the best.");
        back_error (); 
      } 
      pstack[n]= mem[mem_top - 3].hh.v.RH; 
      align_state = align_state - unbalance; 
      {
      register integer for_end; 
      m = 0; 
      for_end = n; 
      if(m <= for_end) do 
        flush_list(pstack[m]); 
      while(m++ < for_end);
    } 
      goto lab10; 
    } 
    if(cur_tok < 768)
    if(cur_tok < 512)
    incr(unbalance); 
    else {
        
      decr(unbalance); 
      if(unbalance == 0)
      goto lab31; 
    } 
  } 
  lab31: rbraceptr = p; 
  {
    q = get_avail (); 
    mem[p].hh.v.RH = q; 
    mem[q].hh.v.LH = cur_tok; 
    p = q; 
  } 
      } 
      else {
    
  back_input ();
  print_err("Argument of ");
  sprint_cs(warning_index); 
  print(635);     /*  has an extra } */
  help6("I've run across a `}' that doesn't seem to match anything.",
	  "For example, `\\def\\a#1{...}' and `\\a}' would produce",
	  "this error. If you simply proceed now, the `\\par' that",
	  "I've just inserted will cause me to report a runaway",
	  "argument that might be the root of the problem. But if",
	  "your `}' was spurious, just type `2' and it will go away.");
  incr(align_state); 
  long_state = 111; 
  cur_tok = par_token; 
  ins_error (); 
      } 
      else {
    
  if(cur_tok == 2592)
  if(mem[r].hh.v.LH <= 3584)
  if(mem[r].hh.v.LH >= 3328)
  goto lab22; 
  {
    q = get_avail (); 
    mem[p].hh.v.RH = q;   /* p may be used without having ... */
    mem[q].hh.v.LH = cur_tok; 
    p = q; 
  } 
      } 
      incr(m);          /* m may be used without having been ... */
      if(mem[r].hh.v.LH > 3584)
      goto lab22; 
      if(mem[r].hh.v.LH < 3328)
      goto lab22; 
      lab40: if(s != 0)
      {
  if((m == 1)&&(mem[p].hh.v.LH < 768)&&(p != mem_top - 3 
  ))
  {
    mem[rbraceptr].hh.v.RH = 0; /* rbraceptr may be used without ... */
    {
      mem[p].hh.v.RH = avail; 
      avail = p; 
  ;
#ifdef STAT
      decr(dyn_used); 
#endif /* STAT */
    } 
    p = mem[mem_top - 3].hh.v.RH; 
    pstack[n]= mem[p].hh.v.RH; 
    {
      mem[p].hh.v.RH = avail; 
      avail = p; 
  ;
#ifdef STAT
      decr(dyn_used); 
#endif /* STAT */
    } 
  } 
  else pstack[n]= mem[mem_top - 3].hh.v.RH; 
  incr(n); 
  if(eqtb[(hash_size + 3193)].cint > 0)
  {
    begin_diagnostic (); 
    print_nl(matchchr); /* matchchar may be used without ... */
    print_int(n); 
    print(653); /* <- */
    show_token_list(pstack[n - 1], 0, 1000); 
    end_diagnostic(false); 
  } 
      } 
    } while(!(mem[r].hh.v.LH == 3584)); 
  } 
/* while (state=token_list)and(loc=null) do end_token_list; l.7956 */
  while((cur_input.state_field == 0)&&(cur_input.loc_field == 0)) 
  end_token_list (); 
  begin_token_list(refcount, 5); 
  cur_input.name_field = warning_index; 
  cur_input.loc_field = mem[r].hh.v.RH; 
  if(n > 0)
  {
    if(param_ptr + n > max_param_stack)
    {
      max_param_stack = param_ptr + n; 
#ifdef ALLOCATEPARAMSTACK
    if(max_param_stack > current_param_size)
      param_stack = realloc_param_stack (increment_param_size);
    if(max_param_stack > current_param_size){ /* check again after allocation */
      overflow(633, current_param_size);
      return;     // abort_flag set
    }
#else
    if(max_param_stack > param_size){
      overflow(633, param_size); /* parameter stack - not dynamic */
      return;     // abort_flag set
    }
#endif
    } 
    {
    register integer for_end; 
    m = 0; 
    for_end = n - 1; 
    if(m <= for_end) 
      do param_stack[param_ptr + m]= pstack[m]; 
    while(m++ < for_end);
  } 
    param_ptr = param_ptr + n; 
  } 
  lab10: scanner_status = savescannerstatus; 
  warning_index = savewarningindex; 
}
void insert_relax (void) 
{
/* begin cur_tok:=cs_token_flag+cur_cs; back_input; */
  cur_tok = 4095 + cur_cs; 
  back_input (); 
/* cur_tok:=cs_token_flag+frozen_relax; back_input; token_type:=inserted; */
/*  cur_tok = (hash_size + 4616);  */
/*  cur_tok = (hash_size + 4095 + 521);  */
  cur_tok = (hash_size + hash_extra + 4095 + 521);  /* 96/Jan/10 */
  back_input (); 
  cur_input.index_field = 4; 
} 
void expand (void) 
{
  halfword t; 
  halfword p, q, r; 
  integer j; 
  integer cvbackup; 
  small_number cvlbackup, radixbackup, cobackup; 
  halfword backupbackup; 
  small_number savescannerstatus; 

  cvbackup = cur_val; 
  cvlbackup = cur_val_level;  
  radixbackup = radix;  
  cobackup = cur_order;  
  backupbackup = mem[mem_top - 13].hh.v.RH; 
  if(cur_cmd < 111)
  {
    if(eqtb[(hash_size + 3199)].cint > 1)
    show_cur_cmd_chr (); 
    switch(cur_cmd)
    {case 110 : 
      {
/* begin if cur_mark[cur_chr]<>null then l.7881 */
  if(cur_mark[cur_chr]!= 0)
  begin_token_list(cur_mark[cur_chr], 14); 
      } 
      break; 
    case 102 : 
      {
  get_token (); 
  t = cur_tok; 
  get_token (); 
  if(cur_cmd > 100){
    expand ();
  }
  else back_input (); 
  cur_tok = t; 
  back_input (); 
      } 
      break; 
    case 103 : 
      {
  savescannerstatus = scanner_status;  
  scanner_status = 0; 
  get_token (); 
  scanner_status = savescannerstatus; 
  t = cur_tok; 
  back_input (); 
  if(t >= 4095)   /* if t>=cs_token_flag then */
  {
/*   begin p:=get_avail; info(p):=cs_token_flag+frozen_dont_expand; */
    p = get_avail (); 
/*    mem[p].hh.v.LH = (hash_size + 4618);  */
/*    mem[p].hh.v.LH = (hash_size + 4095 + 523); */
    mem[p].hh.v.LH = (hash_size + hash_extra + 4095 + 523); /*96/Jan/10*/
    mem[p].hh.v.RH = cur_input.loc_field; 
    cur_input.start_field = p; 
    cur_input.loc_field = p; 
  } 
      } 
      break; 
    case 107 : 
      {
  r = get_avail (); 
  p = r; 
  do {
      get_x_token (); 
    if(cur_cs == 0){
      q = get_avail (); 
      mem[p].hh.v.RH = q; 
      mem[q].hh.v.LH = cur_tok; 
      p = q; 
    } 
  } while(!(cur_cs != 0)); 
  if(cur_cmd != 67)
  {
	  print_err("Missing ");
    print_esc("endcsname");
    print(623);   /*  inserted */
	help2("The control sequence marked <to be read again> should",
		"not appear between \\csname and \\endcsname.");
    back_error (); 
  } 
  j = first; 
  p = mem[r].hh.v.RH; 
  while(p != 0){  /* while p<>null do l.7742 */
      
    if(j >= max_buf_stack)
    {
      max_buf_stack = j + 1; 
#ifdef ALLOCATEBUFFER
    if(max_buf_stack == current_buf_size)
      buffer = realloc_buffer (increment_buf_size);
    if(max_buf_stack == current_buf_size){  /* check again after allocation */
      overflow(256, current_buf_size);
      return;     // abort_flag set
    }
#else
    if(max_buf_stack == buf_size){
      overflow(256, buf_size); /* buffer size - not dynamic */
      return;     // abort_flag set
    }
#endif
    } 
    buffer[j]= mem[p].hh.v.LH % 256; 
/*    buffer[j]= mem[p].hh.v.LH & 255; */ /* last 8 bits */
    incr(j); 
    p = mem[p].hh.v.RH; 
  } 
  if(j > first + 1)
  {
    no_new_control_sequence = false; 
    cur_cs = id_lookup(first, j - first); 
    no_new_control_sequence = true; 
  } 
  else if(j == first)
  cur_cs = 513; 
/* else cur_cs:=single_base+buffer[first] {the list has length one} */
  else cur_cs = 257 + buffer[first]; 
  flush_list(r); 
  if(eqtb[cur_cs].hh.b0 == 101)
  {
    eq_define(cur_cs, 0, 256); 
  } 
  cur_tok = cur_cs + 4095; 
  back_input (); 
      } 
      break; 
    case 108 : 
      conv_toks (); 
      break; 
    case 109 : 
      ins_the_toks (); 
      break; 
    case 105 : 
      conditional (); 
      break; 
    case 106 : 
      if(cur_chr > if_limit)
      if(if_limit == 1)
      insert_relax (); 
      else {
    print_err("Extra ");
  print_cmd_chr(106, cur_chr);  /* j */
  help1("I'm ignoring this; it doesn't match any \\if.");
  error (); 
      } 
      else {
    
  while(cur_chr != 2)pass_text (); 
  {
    p = cond_ptr; 
    if_line = mem[p + 1].cint; 
    cur_if = mem[p].hh.b1; 
    if_limit = mem[p].hh.b0; 
    cond_ptr = mem[p].hh.v.RH; 
    free_node(p, 2); 
  } 
      } 
      break; 
    case 104 : 
      if(cur_chr > 0)force_eof = true; 
      else if(name_in_progress)insert_relax (); 
      else start_input (); 
      break; 
      default: 
      {
		  print_err("Undefined control sequence");
		  help5("The control sequence at the end of the top line",
			  "of your error message was never \\def'ed. If you have",
			  "misspelled it (e.g., `\\hobx'), type `I' and the correct",
			  "spelling (e.g., `I\\hbox'). Otherwise just continue,",
			  "and I'll forget about whatever was undefined."); 
  error (); 
      } 
      break; 
    } 
  } 
  else if(cur_cmd < 115){
    macro_call ();
  }
  else {
      
/*    cur_tok = (hash_size + 4615);  */
/*    cur_tok = (hash_size + 4095 + 520);  */
    cur_tok = (hash_size + hash_extra + 4095 + 520); /* 96/Jan/10 */
    back_input (); 
  } 
  cur_val = cvbackup; 
  cur_val_level = cvlbackup; 
  radix = radixbackup; 
  cur_order = cobackup; 
  mem[mem_top - 13].hh.v.RH = backupbackup; 
} 
void get_x_token (void) 
{/* 20 30 */ 

lab20:
  get_next (); 
  if(cur_cmd <= 100) goto lab30; 
  if(cur_cmd >= 111)
    if(cur_cmd < 115){
      macro_call ();
    }
    else {
/*      cur_cs = (hash_size + 520);  */
      cur_cs = (hash_size + hash_extra + 520);  /* 96/Jan/10 */
      cur_cmd = 9; 
      goto lab30; 
    } 
  else {
    expand ();
  }
  goto lab20; 
lab30: if(cur_cs == 0)
       cur_tok =(cur_cmd * 256)+ cur_chr; 
     else cur_tok = 4095 + cur_cs; 
} 
void x_token (void) 
{
  while(cur_cmd > 100){
    expand (); 
    get_next (); 
  } 
  if(cur_cs == 0)
  cur_tok =(cur_cmd * 256)+ cur_chr; 
  else cur_tok = 4095 + cur_cs; 
} 
void scan_left_brace (void) 
{ 
  do {
      get_x_token (); 
  } while(!((cur_cmd != 10)&&(cur_cmd != 0))); 
  if(cur_cmd != 1)
  {
	print_err("Missing { inserted"); 
	help4("A left brace was mandatory here, so I've put one in.",
		"You might want to delete and/or insert some corrections",
		"so that I will find a matching right brace soon.",
		"(If you're confused by all this, try typing `I}' now.)");
    back_error (); 
    cur_tok = 379; 
    cur_cmd = 1; 
    cur_chr = 123; 
    incr(align_state); 
  } 
} 
void scan_optional_equals (void) 
{
  do {
      get_x_token (); 
  } while(!(cur_cmd != 10)); 
  if(cur_tok != 3133)back_input (); 
} 
bool scan_keyword_(str_number s)
{/* 10 */ register bool Result; 
  halfword p; 
  halfword q; 
  pool_pointer k; 
  p = mem_top - 13; 
  mem[p].hh.v.RH = 0; 
  k = str_start[s]; 
  while(k < str_start[s + 1]){
    get_x_token (); 
    if((cur_cs == 0)&&((cur_chr == str_pool[k])||(cur_chr == 
    str_pool[k]- 32))) {
      {
  q = get_avail (); 
  mem[p].hh.v.RH = q; 
  mem[q].hh.v.LH = cur_tok; 
  p = q; 
      } 
      incr(k); 
    } 
    else if((cur_cmd != 10)||(p != mem_top - 13)) 
    {
      back_input (); 
      if(p != mem_top - 13)
      begin_token_list(mem[mem_top - 13].hh.v.RH, 3); 
      Result = false; 
      return(Result); 
    } 
  } 
  flush_list(mem[mem_top - 13].hh.v.RH); 
  Result = true; 
  return Result; 
} 
void mu_error (void) 
{ 
	print_err("Incompatible glue units");
	help1("I'm going to assume that 1mu=1pt when they're mixed."); 
  error (); 
} 
void scan_eight_bit_int (void) 
{ 
    scan_int (); 
  if((cur_val < 0)||(cur_val > 255)) 
  {
	  print_err("Bad register code");
	  help2("A register number must be between 0 and 255.",
		  "I changed this one to zero.");
    int_error(cur_val); 
    cur_val = 0; 
  } 
} 
void scan_char_num (void) 
{
    scan_int (); 
  if((cur_val < 0)||(cur_val > 255)) 
  {
	  print_err("Bad character code");
	  help2("A character number must be between 0 and 255.",
		  "I changed this one to zero.");
    int_error(cur_val); 
    cur_val = 0; 
  } 
} 
void scan_four_bit_int (void) 
{
    scan_int (); 
  if((cur_val < 0)||(cur_val > 15)) 
  {
	  print_err("Bad number");
	  help2("Since I expected to read a number between 0 and 15,",
		  "I changed this one to zero.");
    int_error(cur_val); 
    cur_val = 0; 
  } 
} 
void scan_fifteen_bit_int (void) 
{ 
    scan_int (); 
  if((cur_val < 0)||(cur_val > 32767)) 
  {
	  print_err("Bad mathchar");
	  help2("A mathchar number must be between 0 and 32767.",
		  "I changed this one to zero.");
    int_error(cur_val); 
    cur_val = 0; 
  } 
} 
void scan_twenty_seven_bit_int (void) 
{ 
    scan_int (); 
  if((cur_val < 0)||(cur_val > 134217727L)) /* 2^27 - 1 */
  {
	  print_err("Bad delimiter code");
	  help2("A numeric delimiter code must be between 0 and 2^{27}-1.",
		  "I changed this one to zero.");
    int_error(cur_val); 
    cur_val = 0; 
  } 
} 
void scan_font_ident (void) 
{
  internal_font_number f; 
  halfword m; 
  do {
      get_x_token (); 
  } while(!(cur_cmd != 10)); 
  if(cur_cmd == 88)
  f = eqtb[(hash_size + 1834)].hh.v.RH; 
  else if(cur_cmd == 87)
  f = cur_chr; 
  else if(cur_cmd == 86)
  {
    m = cur_chr; 
    scan_four_bit_int (); 
    f = eqtb[m + cur_val].hh.v.RH; 
  } 
  else {
      print_err("Missing font identifier");
	  help2("I was looking for a control sequence whose",
		  "current meaning has been defined by \\font.");
    back_error (); 
    f = 0; 
  }
  cur_val = f; 
} 
void find_font_dimen_(bool writing)
{ 
  internal_font_number f; 
  integer n; 
  scan_int (); 
  n = cur_val; 
  scan_font_ident (); 
  f = cur_val; 
/*  if(n <= 0)*/            /* change 98/Oct/5 */
  if(n < 0 || (n == 0 && font_dimen_zero == 0))
    cur_val = fmem_ptr; 
  else {
/* else  begin if writing and(n<=space_shrink_code)and@|
    (n>=space_code)and(font_glue[f]<>null) then
    begin delete_glue_ref(font_glue[f]); l.11225 */
    if(writing &&(n <= 4)&&(n >= 2)&&(font_glue[f]!= 0)) 
    {
    delete_glue_ref(font_glue[f]); 
    font_glue[f]= 0;  /* font_glue[f]:=null */
    } 
    if(n > font_params[f])
    if(f < font_ptr)
      cur_val = fmem_ptr; 
    else {
      do {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
 #ifdef ALLOCATEFONT
        if(fmem_ptr == current_font_mem_size) { /* 93/Nov/28 ??? */
          font_info = realloc_font_info(increment_font_mem_size);
        }
        if(fmem_ptr == current_font_mem_size){    /* 94/Jan/24 */
          overflow(818, current_font_mem_size); /* font memory */
          return;     // abort_flag set
        }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
        if(fmem_ptr == font_mem_size){
          overflow(818, font_mem_size); /* font memory */
          return;     // abort_flag set
        }
#endif
        font_info[fmem_ptr].cint = 0; 
        incr(fmem_ptr); 
        incr(font_params[f]); 
      } while(!(n == font_params[f])); 
      cur_val = fmem_ptr - 1; 
    } 
/*  else cur_val = n + param_base[f];   */      /* 98/Oct/5 */
  else if (n > 0) cur_val = n + param_base[f];    /* 98/Oct/5 */
  else cur_val = &font_check[f] - &font_info[0]; /* 98/Oct/5 */
/*  checksum =  (((font_check[f].b0) << 8 | font_check[f].b1) << 8 |
        font_check[f].b2) << 8 | font_check[f].b3; */
  } 
/* compiler error: '-' : incompatible types - from 'union fmemoryword *' to 'struct fourunsignedchars *' */
  if(cur_val == fmem_ptr){
	  print_err("Font ");
/*    print_esc(hash[(hash_size + 524) + f].v.RH); */
    //print_esc(hash[(hash_size + hash_extra + 524) + f].v.RH); /*96/Jan/10*/
	  print_esc("");print(hash[(hash_size + hash_extra + 524) + f].v.RH);
    print(814); /* has  only  */
    print_int(font_params[f]); 
    print(815); /*  fontdimen parameters */
	help2("To increase the number of font parameters, you must",
		"use \\fontdimen immediately after the \\font is loaded.");
    error (); 
  }
} 

/* NOTE: the above use of /fontdimen0 to access the checksum is a kludge */
/* In future would be better to do this by allocating one more slot for */
/* for parameters when a font is read rather than carry checksum separately */
/* The above gets the value byte order reversed ... 98/Oct/5 */

void scan_something_internal_(small_number level, bool negative)
{ 
  halfword m; 
  integer p; 
  m = cur_chr; 
  switch(cur_cmd)
  {case 85 : 
    {
      scan_char_num (); 
      if(m == (hash_size + 2907))
      {
  cur_val = eqtb[(hash_size + 2907) + cur_val].hh.v.RH; 
  cur_val_level = 0; 
      } 
      else if(m < (hash_size + 2907))
      {
  cur_val = eqtb[m + cur_val].hh.v.RH; 
  cur_val_level = 0; 
      } 
      else {
    
  cur_val = eqtb[m + cur_val].cint; 
  cur_val_level = 0; 
      } 
    } 
    break; 
  case 71 : 
  case 72 : 
  case 86 : 
  case 87 : 
  case 88 : 
    if(level != 5)
    {
		print_err("Missing number, treated as zero");
		help3("A number should have been here; I inserted `0'.",
			"(If you can't figure out why I needed to see a number,",
			"look up `weird error' in the index to The TeXbook.)"); 
      back_error (); 
      {
      cur_val = 0; 
      cur_val_level = 1; 
      } 
    } 
    else if(cur_cmd <= 72)
    {
      if(cur_cmd < 72)
      {
  scan_eight_bit_int (); 
  m = (hash_size + 1322) + cur_val; 
      } 
      {
  cur_val = eqtb[m].hh.v.RH; 
  cur_val_level = 5; 
      } 
    } 
    else {
  
      back_input (); 
      scan_font_ident (); 
      {
/*  cur_val = (hash_size + 524) + cur_val;  */
  cur_val = (hash_size + hash_extra + 524) + cur_val; /* 96/Jan/10 */
  cur_val_level = 4; 
      } 
    } 
    break; 
  case 73 : 
    {
      cur_val = eqtb[m].cint; 
      cur_val_level = 0; 
    } 
    break; 
  case 74 : 
    {
      cur_val = eqtb[m].cint; 
      cur_val_level = 1; 
    } 
    break; 
  case 75 : 
    {
      cur_val = eqtb[m].hh.v.RH; 
      cur_val_level = 2; 
    } 
    break; 
  case 76 : 
    {
      cur_val = eqtb[m].hh.v.RH; 
      cur_val_level = 3; 
    } 
    break; 
  case 79 : 
    if(abs(mode)!= m)
    {
		print_err("Improper ");
      print_cmd_chr(79, m); /* O */
	  help4("You can refer to \\spacefactor only in horizontal mode;",
		  "you can refer to \\prevdepth only in vertical mode; and",
		  "neither of these is meaningful inside \\write. So",
		  "I'm forgetting what you said and using zero instead.");
      error (); 
      if(level != 5) {
      cur_val = 0; 
      cur_val_level = 1; 
      } 
      else {
    
  cur_val = 0; 
  cur_val_level = 0; 
      } 
    } 
    else if(m == 1)
    {
      cur_val = cur_list.aux_field.cint; 
      cur_val_level = 1; 
    } 
    else {
  
      cur_val = space_factor; 
      cur_val_level = 0; 
    } 
    break; 
  case 80 : 
    if(mode == 0)
    {
      cur_val = 0; 
      cur_val_level = 0; 
    } 
    else {
  
      nest[nest_ptr]= cur_list; 
      p = nest_ptr; 
      while(abs(nest[p].mode_field)!= 1)decr(p); 
      {
  cur_val = nest[p].pg_field; 
  cur_val_level = 0; 
      } 
    } 
    break; 
  case 82 : 
    {
      if(m == 0)
      cur_val = dead_cycles; 
      else cur_val = insert_penalties; 
      cur_val_level = 0; 
    } 
    break; 
  case 81 : 
    {
      if((page_contents == 0)&&(! output_active)) 
      if(m == 0)
      cur_val = 1073741823L;  /* 2^30 - 1 */
      else cur_val = 0; 
      else cur_val = page_so_far[m]; 
      cur_val_level = 1; 
    } 
    break; 
  case 84 : 
    {
      if(eqtb[(hash_size + 1312)].hh.v.RH == 0)
      cur_val = 0; 
      else cur_val = mem[eqtb[(hash_size + 1312)].hh.v.RH].hh.v.LH; 
      cur_val_level = 0; 
    } 
    break; 
  case 83 : 
    {
      scan_eight_bit_int (); 
      if(eqtb[(hash_size + 1578) + cur_val].hh.v.RH == 0)
      cur_val = 0; 
      else cur_val = mem[eqtb[(hash_size + 1578) + cur_val].hh.v.RH + m].cint; 
      cur_val_level = 1; 
    } 
    break; 
  case 68 : 
  case 69 : 
    {
      cur_val = cur_chr; 
      cur_val_level = 0; 
    } 
    break; 
  case 77 : 
    {
      find_font_dimen(false); 
      font_info[fmem_ptr].cint = 0; 
      {
  cur_val = font_info[cur_val].cint; 
  cur_val_level = 1; 
      } 
    } 
    break; 
  case 78 : 
    {
      scan_font_ident (); 
      if(m == 0){
  cur_val = hyphen_char[cur_val]; 
  cur_val_level = 0; 
      } 
      else {
    
  cur_val = skew_char[cur_val]; 
  cur_val_level = 0; 
      } 
    } 
    break; 
  case 89 : 
    {
      scan_eight_bit_int (); 
      switch(m)
      {case 0 : 
  cur_val = eqtb[(hash_size + 3218) + cur_val].cint; 
  break; 
      case 1 : 
  cur_val = eqtb[(hash_size + 3751) + cur_val].cint; 
  break; 
      case 2 : 
  cur_val = eqtb[(hash_size + 800) + cur_val].hh.v.RH; 
  break; 
      case 3 : 
  cur_val = eqtb[(hash_size + 1056) + cur_val].hh.v.RH; 
  break; 
      } 
      cur_val_level = m; 
    } 
    break; 
  case 70 : 
    if(cur_chr > 2)
    {
      if(cur_chr == 3)
      cur_val = line; 
      else cur_val = last_badness; 
      cur_val_level = 0; 
    } 
    else {
  
      if(cur_chr == 2)
      cur_val = 0; 
      else cur_val = 0; 
      cur_val_level = cur_chr; 
      if(!(tail >= hi_mem_min)&&(mode != 0)
    )
      switch(cur_chr)
      {case 0 : 
  if(mem[tail].hh.b0 == 12)
  cur_val = mem[tail + 1].cint; 
  break; 
      case 1 : 
  if(mem[tail].hh.b0 == 11)
  cur_val = mem[tail + 1].cint; 
  break; 
      case 2 : 
  if(mem[tail].hh.b0 == 10)
  {
    cur_val = mem[tail + 1].hh.v.LH; 
    if(mem[tail].hh.b1 == 99)
    cur_val_level = 3; 
  } 
  break; 
      } 
      else if((mode == 1)&&(tail == cur_list 
     .head_field)) 
      switch(cur_chr)
      {case 0 : 
  cur_val = last_penalty; 
  break; 
      case 1 : 
  cur_val = last_kern; 
  break; 
      case 2 : 
/*  if(last_glue != 262143L) */ /* NO! */
  if(last_glue != empty_flag)
  cur_val = last_glue; 
  break; 
      } 
    } 
    break; 
    default: 
    {
		print_err("You can't use `");
      print_cmd_chr(cur_cmd, cur_chr); 
      print(683); /* ' after  */
      print_esc("the");
	  help1("I'm forgetting what you said and using zero instead."); 
      error (); 
      if(level != 5){
      cur_val = 0; 
      cur_val_level = 1; 
      } 
      else {
      cur_val = 0; 
      cur_val_level = 0; 
      } 
    } 
    break; 
  } 
  while(cur_val_level > level){
      
    if(cur_val_level == 2)
    cur_val = mem[cur_val + 1].cint; 
    else if(cur_val_level == 3){
    mu_error (); 
  }
    decr(cur_val_level); 
  } 
  if(negative)
  if(cur_val_level >= 2)
  {
    cur_val = new_spec(cur_val); 
    {
      mem[cur_val + 1].cint = - (integer) mem[cur_val + 1].cint; 
      mem[cur_val + 2].cint = - (integer) mem[cur_val + 2].cint; 
      mem[cur_val + 3].cint = - (integer) mem[cur_val + 3].cint; 
    } 
  } 
  else cur_val = - (integer) cur_val; 
  else if((cur_val_level >= 2)&&(cur_val_level <= 3)) 
  incr(mem[cur_val].hh.v.RH); 
} 

/*****************************************************************************/

/* Moved here to avoid question about pragma optimize 96/Sep/12 */

/* #pragma optimize ("a", off) */

void get_next (void) 
{/* 20 25 21 26 40 10 */ 
  integer k; 
  halfword t; 
/*  char cat; */    /* make this an int ? */
  int cat;      /* make this an int ? 95/Jan/7 */
  ASCII_code c, cc; 
  char d; 

lab20:
  cur_cs = 0; 
  if(cur_input.state_field != 0) {
    lab25: if(cur_input.loc_field <= cur_input.limit_field) {
      cur_chr = buffer[cur_input.loc_field]; 
      incr(cur_input.loc_field); 
      lab21: cur_cmd = eqtb[(hash_size + 1883) + cur_chr].hh.v.RH; 
      switch(cur_input.state_field + cur_cmd)
      {case 10 : 
      case 26 : 
      case 42 : 
      case 27 : 
      case 43 : 
  goto lab25; 
  break; 
      case 1 : 
      case 17 : 
      case 33 : 
  {
    if(cur_input.loc_field > cur_input.limit_field)
    cur_cs = 513; 
    else {
        
      lab26: k = cur_input.loc_field; 
      cur_chr = buffer[k]; 
      cat = eqtb[(hash_size + 1883) + cur_chr].hh.v.RH; 
      incr(k); 
      if(cat == 11)
      cur_input.state_field = 17; 
      else if(cat == 10)
      cur_input.state_field = 17; 
      else cur_input.state_field = 1; 
      if((cat == 11)&&(k <= cur_input.limit_field)) 
      {
        do {
      cur_chr = buffer[k]; 
    cat = eqtb[(hash_size + 1883) + cur_chr].hh.v.RH; 
    incr(k); 
        } while(!((cat != 11)||(k > cur_input.limit_field)))
     ; 
        {
    if(buffer[k]== cur_chr)
    if(cat == 7)
    if(k < cur_input.limit_field)
    {
      c = buffer[k + 1]; 
      if(c < 128)
      {
        d = 2; 
        if((((c >= 48)&&(c <= 57)) ||((c >= 97)&& 
       (c <= 102)))) 
        if(k + 2 <= cur_input.limit_field)
        {
          cc = buffer[k + 2]; 
          if((((cc >= 48)&&(cc <= 57)) ||((cc >= 97 
        )&&(cc <= 102)))) 
          incr(d); 
        } 
        if(d > 2)
        {
          if(c <= 57)
          cur_chr = c - 48; 
          else cur_chr = c - 87; 
          if(cc <= 57)
          cur_chr = 16 * cur_chr + cc - 48; 
          else cur_chr = 16 * cur_chr + cc - 87; 
          buffer[k - 1]= cur_chr; 
        } 
        else if(c < 64)
        buffer[k - 1]= c + 64; 
        else buffer[k - 1]= c - 64; 
        cur_input.limit_field = cur_input.limit_field - d; 
        first = first - d; 
        while(k <= cur_input.limit_field){
      
          buffer[k]= buffer[k + d]; 
          incr(k); 
        } 
        goto lab26; 
      } 
    } 
        } 
        if(cat != 11)
        decr(k); 
        if(k > cur_input.loc_field + 1)
        {
    cur_cs = id_lookup(cur_input.loc_field, k - cur_input.loc_field 
    ); 
    cur_input.loc_field = k; 
    goto lab40; 
        } 
      } 
      else {
    
        if(buffer[k]== cur_chr)
        if(cat == 7)
        if(k < cur_input.limit_field)
        {
    c = buffer[k + 1]; 
    if(c < 128)             /* ? */
    {
      d = 2; 
      if((((c >= 48)&&(c <= 57)) ||((c >= 97)&&(
      c <= 102)))) 
      if(k + 2 <= cur_input.limit_field)
      {
        cc = buffer[k + 2]; 
        if((((cc >= 48)&&(cc <= 57)) ||((cc >= 97)
        &&(cc <= 102)))) 
        incr(d); 
      } 
      if(d > 2)
      {
        if(c <= 57)
        cur_chr = c - 48; 
        else cur_chr = c - 87; 
        if(cc <= 57)          /* cc may be used without ... */
        cur_chr = 16 * cur_chr + cc - 48; 
        else cur_chr = 16 * cur_chr + cc - 87; 
        buffer[k - 1]= cur_chr; 
      } 
      else if(c < 64)
        buffer[k - 1]= c + 64; 
      else buffer[k - 1]= c - 64; 
      cur_input.limit_field = cur_input.limit_field - d; 
      first = first - d; 
      while(k <= cur_input.limit_field){
        buffer[k]= buffer[k + d]; 
        incr(k); 
      } 
      goto lab26; 
    } 
        } 
      } 
/*   cur_cs:=single_base+buffer[loc]; incr(loc); */
      cur_cs = 257 + buffer[cur_input.loc_field]; 
      incr(cur_input.loc_field); 
    } 
    lab40: cur_cmd = eqtb[cur_cs].hh.b0; 
    cur_chr = eqtb[cur_cs].hh.v.RH; 
    if(cur_cmd >= 113){
      check_outer_validity ();
    }
  } 
  break; 
      case 14 : 
      case 30 : 
      case 46 : 
  {
    cur_cs = cur_chr + 1; 
    cur_cmd = eqtb[cur_cs].hh.b0; 
    cur_chr = eqtb[cur_cs].hh.v.RH; 
    cur_input.state_field = 1; 
    if(cur_cmd >= 113){
      check_outer_validity ();
    }
  } 
  break; 
      case 8 : 
      case 24 : 
      case 40 : 
  {
    if(cur_chr == buffer[cur_input.loc_field])
    if(cur_input.loc_field < cur_input.limit_field)
    {
      c = buffer[cur_input.loc_field + 1]; 
      if(c < 128)
      {
        cur_input.loc_field = cur_input.loc_field + 2; 
        if((((c >= 48)&&(c <= 57)) ||((c >= 97)&&(c <= 
        102)))) 
        if(cur_input.loc_field <= cur_input.limit_field)
        {
    cc = buffer[cur_input.loc_field]; 
    if((((cc >= 48)&&(cc <= 57)) ||((cc >= 97)&&(
    cc <= 102)))) 
    {
      incr(cur_input.loc_field); 
      if(c <= 57)
      cur_chr = c - 48; 
      else cur_chr = c - 87; 
      if(cc <= 57)
      cur_chr = 16 * cur_chr + cc - 48; 
      else cur_chr = 16 * cur_chr + cc - 87; 
      goto lab21; 
    } 
        } 
        if(c < 64)
        cur_chr = c + 64; 
        else cur_chr = c - 64; 
        goto lab21; 
      } 
    } 
    cur_input.state_field = 1; 
  } 
  break; 
      case 16 : 
      case 32 : 
      case 48 : 
  {
	  print_err("Text line contains an invalid character");
	  help2("A funny symbol that I can't read has just been input.",
		  "Continue, and I'll forget that it ever happened.");
    deletions_allowed = false; 
    error (); 
    deletions_allowed = true; 
    goto lab20; 
  } 
  break; 
      case 11 : 
  {
    cur_input.state_field = 17; 
    cur_chr = 32; 
  } 
  break; 
      case 6 : 
  {
    cur_input.loc_field = cur_input.limit_field + 1; 
    cur_cmd = 10; 
    cur_chr = 32; 
  } 
  break; 
      case 22 : 
      case 15 : 
      case 31 : 
      case 47 : 
  {
    cur_input.loc_field = cur_input.limit_field + 1; 
    goto lab25; 
  } 
  break; 
      case 38 : 
  {
    cur_input.loc_field = cur_input.limit_field + 1; 
    cur_cs = par_loc; 
    cur_cmd = eqtb[cur_cs].hh.b0; 
    cur_chr = eqtb[cur_cs].hh.v.RH; 
    if(cur_cmd >= 113){
      check_outer_validity ();
    }
  } 
  break; 
      case 2 : 
  incr(align_state); 
  break; 
      case 18 : 
      case 34 : 
  {
    cur_input.state_field = 1; 
    incr(align_state); 
  } 
  break; 
      case 3 : 
  decr(align_state); 
  break; 
      case 19 : 
      case 35 : 
  {
    cur_input.state_field = 1; 
    decr(align_state); 
  } 
  break; 
      case 20 : 
      case 21 : 
      case 23 : 
      case 25 : 
      case 28 : 
      case 29 : 
      case 36 : 
      case 37 : 
      case 39 : 
      case 41 : 
      case 44 : 
      case 45 : 
  cur_input.state_field = 1; 
  break; 
  default: 
  ; 
  break; 
      } 
    } 
    else {
      cur_input.state_field = 33; 
      if(cur_input.name_field > 17) {
      incr(line); 
      first = cur_input.start_field; 
      if(! force_eof){
        if(input_ln(input_file[cur_input.index_field], true)) {
          firm_up_the_line ();
        }
        else force_eof = true; 
      } 
      if(force_eof){
        print_char(41);   /*)*/
        decr(open_parens); 
#ifndef _WINDOWS
        fflush(stdout); 
#endif
        force_eof = false; 
        end_file_reading (); 
        check_outer_validity (); 
        goto lab20; 
      } 
      if((eqtb[(hash_size + 3211)].cint < 0)||
        (eqtb[(hash_size + 3211)].cint > 255)) 
        decr(cur_input.limit_field); 
/*    long to unsigned char ... */
      else buffer[cur_input.limit_field]= eqtb[(hash_size + 3211)].cint; 
      first = cur_input.limit_field + 1; 
      cur_input.loc_field = cur_input.start_field; 
    } 
    else {
      if(!(cur_input.name_field == 0)) {
        cur_cmd = 0; 
        cur_chr = 0; 
        return; 
      } 
      if(input_ptr > 0){
        end_file_reading (); 
        goto lab20; 
      } 
      if(selector < 18) open_log_file (); 
      if(interaction > 1){
        if((eqtb[(hash_size + 3211)].cint < 0)||
          (eqtb[(hash_size + 3211)].cint > 255)
        )
          incr(cur_input.limit_field); 
        if(cur_input.limit_field == cur_input.start_field)
          print_nl("(Please type a command or say `\\end')");    /*  */
        print_ln (); 
        first = cur_input.start_field; 
        {
        ; 
          print(42);    /* * */
          term_input(42, 0); 
        } 
        cur_input.limit_field = last; 
        if((eqtb[(hash_size + 3211)].cint < 0)||
          (eqtb[(hash_size + 3211)].cint > 255)
        )
          decr(cur_input.limit_field); 
/*    long to unsigned char ... */
        else buffer[cur_input.limit_field]= eqtb[(hash_size + 3211)].cint; 
        first = cur_input.limit_field + 1; 
        cur_input.loc_field = cur_input.start_field; 
      } 
      else {
        fatal_error("*** (job aborted, no legal \\end found)"); /*  */
        return;     // abort_flag set
      }
    } 
    {
      if(interrupt != 0){
        pause_for_instructions ();
      }
      } 
      goto lab25; 
    } 
  } 
  else if(cur_input.loc_field != 0)
  {
    t = mem[cur_input.loc_field].hh.v.LH; 
    cur_input.loc_field = mem[cur_input.loc_field].hh.v.RH; 
    if(t >= 4095)
    {
      cur_cs = t - 4095; 
      cur_cmd = eqtb[cur_cs].hh.b0; 
      cur_chr = eqtb[cur_cs].hh.v.RH; 
      if(cur_cmd >= 113)
      if(cur_cmd == 116)
      {
  cur_cs = mem[cur_input.loc_field].hh.v.LH - 4095; 
  cur_input.loc_field = 0; 
  cur_cmd = eqtb[cur_cs].hh.b0; 
  cur_chr = eqtb[cur_cs].hh.v.RH; 
  if(cur_cmd > 100)
  {
    cur_cmd = 0; 
    cur_chr = 257; 
  } 
      } 
      else {
      check_outer_validity ();
    }
    } 
    else {
  
      cur_cmd = t / 256; 
/*      cur_cmd = t >> 8; */  /* top 8 bits */
      cur_chr = t % 256; 
/*      cur_chr = t & 255; */ /* last 8 bits */
      switch(cur_cmd)
      {case 1 : 
  incr(align_state); 
  break; 
      case 2 : 
  decr(align_state); 
  break; 
      case 5 : 
  {
    begin_token_list(param_stack[cur_input.limit_field + cur_chr - 1], 
    0); 
    goto lab20; 
  } 
  break; 
  default: 
  ; 
  break; 
      } 
    } 
  } 
  else {
    end_token_list (); 
    goto lab20; 
  } 
  if(cur_cmd <= 5)
  if(cur_cmd >= 4)
  if(align_state == 0)
  {
    if(scanner_status == 4){
    fatal_error("(interwoven alignment preambles are not allowed)"); /*  */
    return;     // abort_flag set
  }

    cur_cmd = mem[cur_align + 5].hh.v.LH; 
    mem[cur_align + 5].hh.v.LH = cur_chr; 
    if(cur_cmd == 63)
    begin_token_list(mem_top - 10, 2); 
    else begin_token_list(mem[cur_align + 2].cint, 2); 
    align_state = 1000000L; 
    goto lab20; 
  } 
} 

#pragma optimize ("", on)             /* 96/Sep/12 */

/*****************************************************************************/
