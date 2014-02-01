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

/* #pragma optimize("a", off) */        /* 98/Dec/10 experiment */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

void char_warning_(internal_font_number f, eight_bits c)
{ 
  if(eqtb[(hash_size + 3198)].cint > 0)
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (show_missing == 0)            /* show on screen 94/June/10 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      begin_diagnostic (); 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (show_missing) {     /* add ! before 94/June/10 */
    print_nl("! ");    /* !  */
    print(819);     /* Missing character: there is no */
  }
  else 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  print_nl("Missing character: there is no");      /*  */
    print(c); 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (show_numeric) {             /* bkph 93/Dec/21 */
    print_char(32);   /*   */
    print_char(40);   /*(*/
    if (c / 100 > 0) {
      print_char(48 + c / 100);
/*      c = c % 100; */
      c = c - (c / 100) * 100;      /* ? */
      print_char(48 + c / 10);
    }
    else {
/*      c = c % 100; */
      c = c - (c / 100) * 100;      /* ? */
      if (c / 10 > 0) print_char(48 + c / 10);
    }
    print_char(48 + c % 10);
    print_char(41);   /*)*/
/*    print_char(32); */
  }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    print(820);               /* in font */
    slow_print(font_name[f]); 
    print_char(33);     /* ! */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  if (show_missing) show_context(); */    /* in tex2.c 94/June/10 */
  if (show_missing) {
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
halfword new_character_(internal_font_number f, eight_bits c)
{/* 10 */ register halfword Result; 
  halfword p; 
  if(font_bc[f]<= c)
  if(font_ec[f]>= c)
  if((font_info[char_base[f]+ c].qqqq.b0 > 0)) 
  {
    p = get_avail (); 
/*  long to unsigned short ... */
    mem[p].hh.b0 = f; 
    mem[p].hh.b1 = c; 
    Result = p; 
    return(Result); 
  } 
  char_warning(f, c); /* char_warning(f,c); l.11283 */
  Result = 0;       /* new_character:=null */
  return Result; 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void dvi_swap (void) 
{ 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (trace_flag) {
  show_char('\n');
  sprintf(log_line, "dvi_swap %d", dvi_gone);
  show_line(log_line, 0);
}
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(dvi_limit == dvi_buf_size)   {
    writedvi(0, half_buf - 1); 
    dvi_limit = half_buf; 
    dvi_offset = dvi_offset + dvi_buf_size; 
    dvi_ptr = 0; 
  } 
  else {
    writedvi(half_buf, dvi_buf_size - 1); 
    dvi_limit = dvi_buf_size; 
  } 
  dvi_gone = dvi_gone + half_buf; 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void dvi_four_(integer x)    /* attempt at speeding up bkph - is compiler smart ? */
{ 
  if(x >= 0)
  {
/*    dvi_buf[dvi_ptr]= x / 16777216L;  */ /* 2^24 */
/*    dvi_buf[dvi_ptr]=(x >> 24);  */
    dvi_buf[dvi_ptr]= (unsigned char)(x >> 24); 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  else {
    x = x + 1073741824L;    /* 2^30 40000000 hex */ 
    x = x + 1073741824L; 
    {
/*      dvi_buf[dvi_ptr]=(x / 16777216L)+ 128;   */  /* 2^24 */
      dvi_buf[dvi_ptr]=(x >> 24)+ 128;      /* set sign bit */
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
  } 
/*  x = x % 16777216L;  */  /* % 2^24 */
  x = x & 16777215L; 
  {
/*    dvi_buf[dvi_ptr]= x / 65536L;  */
/*    dvi_buf[dvi_ptr]=(x >> 16);  */
    dvi_buf[dvi_ptr]= (unsigned char)(x >> 16); 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
/*  x = x % 65536L;  */ /* % 2^16 */
  x = x & 65535L;
  {
/*    dvi_buf[dvi_ptr]= x / 256;  */
/*    dvi_buf[dvi_ptr]=(x >> 8);  */
    dvi_buf[dvi_ptr]= (unsigned char)(x >> 8); 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
/*    dvi_buf[dvi_ptr]= x % 256; */ /* % 2^8 */
    dvi_buf[dvi_ptr]= x & 255; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void zdvipop(integer l)
{ 
  if((l == dvi_offset + dvi_ptr)&&(dvi_ptr > 0)) 
  decr(dvi_ptr); 
  else {
      
    dvi_buf[dvi_ptr]= 142; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void dvi_font_def_(internal_font_number f)
{ 
  pool_pointer k; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
  if (f <= 256) {
  {
    dvi_buf[dvi_ptr]= 243; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= f - 1; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  }
  else {
  {
    dvi_buf[dvi_ptr]= 244; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= (f - 1) >> 8; /* top byte */
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= (f - 1) & 255; /* bottom byte */
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  }
#else
  {
    dvi_buf[dvi_ptr]= 243; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= f - 1; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* spit out the font checksum now */
  {
    dvi_buf[dvi_ptr]= font_check[f].b0; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= font_check[f].b1; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= font_check[f].b2; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    dvi_buf[dvi_ptr]= font_check[f].b3; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  dvi_four(font_size[f]); 
  dvi_four(font_dsize[f]); 
  {
/*  long to unsigned char ... */
    dvi_buf[dvi_ptr]=
      (str_start[font_area[f]+ 1]- str_start[font_area[f]]); 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
/*  long to unsigned char ... */
    dvi_buf[dvi_ptr]=
      (str_start[font_name[f]+ 1]- str_start[font_name[f]]); 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    register integer for_end; 
    k = str_start[font_area[f]]; 
    for_end = str_start[font_area[f]+ 1]- 1; 
    if(k <= for_end) do 
    {
      dvi_buf[dvi_ptr]= str_pool[k]; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    while(k++ < for_end);
  } 
  {
    register integer for_end; 
    k = str_start[font_name[f]]; 
    for_end =   str_start[font_name[f]+ 1]- 1;
    if(k <= for_end) do 
    {
      dvi_buf[dvi_ptr]= str_pool[k]; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    while(k++ < for_end);
  } 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void zmovement(scaled w, eight_bits o) 
{/* 10 40 45 2 1 */ 
  small_number mstate; 
  halfword p, q; 
  integer k; 
  q = get_node(3); 
  mem[q + 1].cint = w; 
  mem[q + 2].cint = dvi_offset + dvi_ptr; 
  if(o == 157)          /* 157 == down1 */
  {
    mem[q].hh.v.RH = down_ptr; 
    down_ptr = q; 
  } 
  else {              /* 143 == right1 */
      
    mem[q].hh.v.RH = right_ptr; 
    right_ptr = q; 
  } 
  p = mem[q].hh.v.RH; 
  mstate = 0; 
  while(p != 0){  /* while p<>null do l.12153 */
      
    if(mem[p + 1].cint == w)
    switch(mstate + mem[p].hh.v.LH)
    {case 3 : 
    case 4 : 
    case 15 : 
    case 16 : 
      if(mem[p + 2].cint < dvi_gone)
      goto lab45; 
      else {
    
  k = mem[p + 2].cint - dvi_offset; 
  if(k < 0)
  k = k + dvi_buf_size; 
  dvi_buf[k]= dvi_buf[k]+ 5; 
  mem[p].hh.v.LH = 1; 
  goto lab40; 
      } 
      break; 
    case 5 : 
    case 9 : 
    case 11 : 
      if(mem[p + 2].cint < dvi_gone)
      goto lab45; 
      else {
    
  k = mem[p + 2].cint - dvi_offset; 
  if(k < 0)
  k = k + dvi_buf_size; 
  dvi_buf[k]= dvi_buf[k]+ 10; 
  mem[p].hh.v.LH = 2; 
  goto lab40; 
      } 
      break; 
    case 1 : 
    case 2 : 
    case 8 : 
    case 13 : 
      goto lab40; 
      break; 
      default: 
   ; 
      break; 
    } 
    else switch(mstate + mem[p].hh.v.LH)
    {case 1 : 
      mstate = 6; 
      break; 
    case 2 : 
      mstate = 12; 
      break; 
    case 8 : 
    case 13 : 
      goto lab45; 
      break; 
      default: 
   ; 
      break; 
    } 
    p = mem[p].hh.v.RH; 
  } 
  lab45:; 
  mem[q].hh.v.LH = 3; 
  if(abs(w)>= 8388608L) /* 2^23 */
  {
    {
      dvi_buf[dvi_ptr]= o + 3; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    dvi_four(w); 
    return; 
  } 
  if(abs(w)>= 32768L)
  {
    {
      dvi_buf[dvi_ptr]= o + 2; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    if(w < 0)
    w = w + 16777216L;  /* 2^24 */
    {
/*      dvi_buf[dvi_ptr]= w / 65536L;  */
/*      dvi_buf[dvi_ptr]=(w >> 16);  */
      dvi_buf[dvi_ptr]= (unsigned char)(w >> 16); 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
/*    w = w % 65536L; */
  w = w & 65535L; 
    goto lab2; 
  } 
  if(abs(w)>= 128)
  {
    {
      dvi_buf[dvi_ptr]= o + 1; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    if(w < 0)
    w = w + 65536L; 
    goto lab2; 
  } 
  {
    dvi_buf[dvi_ptr]= o; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  if(w < 0)
  w = w + 256; 
  goto lab1; 
  lab2: {
      
/*    dvi_buf[dvi_ptr]= w / 256;  */
/*    dvi_buf[dvi_ptr]=(w >> 8);  */
    dvi_buf[dvi_ptr]= (unsigned char)(w >> 8); 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  lab1: {
      
/*    dvi_buf[dvi_ptr]= w % 256;  */
    dvi_buf[dvi_ptr]= w & 255; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  return; 
  lab40: mem[q].hh.v.LH = mem[p].hh.v.LH; 
  if(mem[q].hh.v.LH == 1)
  {
    {
      dvi_buf[dvi_ptr]= o + 4; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    while(mem[q].hh.v.RH != p){
  
      q = mem[q].hh.v.RH; 
      switch(mem[q].hh.v.LH)
      {case 3 : 
  mem[q].hh.v.LH = 5; 
  break; 
      case 4 : 
  mem[q].hh.v.LH = 6; 
  break; 
  default: 
  ; 
  break; 
      } 
    } 
  } 
  else {
      
    {
      dvi_buf[dvi_ptr]= o + 9; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    while(mem[q].hh.v.RH != p){
  
      q = mem[q].hh.v.RH; 
      switch(mem[q].hh.v.LH)
      {case 3 : 
  mem[q].hh.v.LH = 4; 
  break; 
      case 5 : 
  mem[q].hh.v.LH = 6; 
  break; 
  default: 
  ; 
  break; 
      } 
    } 
  } 
} 
void prune_movements_(integer l) 
{/* 30 10 */ 
  halfword p; 
  while(down_ptr != 0){ /* while down_ptr<>null do l.12206 */
      
    if(mem[down_ptr + 2].cint < l)
    goto lab30; 
    p = down_ptr; 
    down_ptr = mem[p].hh.v.RH; 
    free_node(p, 3); 
  } 
  lab30: while(right_ptr != 0){ /* done: while right_ptr<>null do */
      
    if(mem[right_ptr + 2].cint < l)
    return; 
    p = right_ptr; 
    right_ptr = mem[p].hh.v.RH; 
    free_node(p, 3); 
  } 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void special_out_(halfword p)
{
  char old_setting; 
  pool_pointer k; 
  if(cur_h != dvi_h)
  {
    movement(cur_h - dvi_h, 143); /* 143 == right1 */
    dvi_h = cur_h; 
  } 
  if(cur_v != dvi_v)
  {
    movement(cur_v - dvi_v, 157); /* 157 == down1 */
    dvi_v = cur_v; 
  } 
  old_setting = selector; 
  selector = 21; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* About output \special{...} make some space in string pool 97/Mar/9 */
#ifdef ALLOCATESTRING
  if(pool_ptr + 32000 > current_pool_size)
    str_pool = realloc_str_pool (increment_pool_size);
/* We don't bother to check whether this worked - will catch later */
#endif
/* Potential problem here if current_pool_size is not much more than pool_ptr! */
/* Fixed 97/Mar/9 in version 2.0.3 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
/*  show_token_list(mem[mem[p + 1].hh.v.RH].hh.v.RH, 0,
    current_pool_size - pool_ptr); */
  show_token_list(mem[mem[p + 1].hh.v.RH].hh.v.RH, 0,
    10000000L);   
/*  Above is safe, since print/print_char/print_esc will extend string space */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  show_token_list(mem[mem[p + 1].hh.v.RH].hh.v.RH, 0,
      pool_size - pool_ptr); 
#endif

  selector = old_setting; 
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* str_room(1) - is there space for one more character in string pool ? */
#ifdef ALLOCATESTRING
  if(pool_ptr + 1 > current_pool_size)
    str_pool = realloc_str_pool (increment_pool_size);
  if(pool_ptr + 1 > current_pool_size)  { /* in case it failed 94/Jan/24 */
    overflow(257, current_pool_size - init_pool_ptr); /* 97/Mar/7 */
    return;     // abort_flag set
  }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if(pool_ptr + 1 > pool_size){
    overflow(257, pool_size - init_pool_ptr); /* pool size */
    return;     // abort_flag set
  }
#endif
  } 
  if((pool_ptr - str_start[str_ptr])< 256)  /* can use xxx1 ? */
  {
    {
      dvi_buf[dvi_ptr]= 239;    /* xxx1 */
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    {
/*    long to unsigned char ... */
      dvi_buf[dvi_ptr]=(pool_ptr - str_start[str_ptr]); 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
  } 
  else {              /* use xxx4 instead */
      
    {
      dvi_buf[dvi_ptr]= 242;    /* xxx4 */
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    dvi_four((pool_ptr - str_start[str_ptr])); 
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
          print_char(40);       /*(*/
          print_int(line);      /* line number */
          print_char(41);       /*)*/
          print_char(32);       /*   */
          print_char(58);       /* : */
        }
#ifndef _WINDOWS
        fflush(stdout);
#endif
      }
    }
  }
#endif

  {
    register integer for_end; 
    k = str_start[str_ptr]; 
    for_end = pool_ptr - 1; 
    if(k <= for_end) do 
    {
      dvi_buf[dvi_ptr]= str_pool[k]; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    while(k++ < for_end);
  } 
  pool_ptr = str_start[str_ptr]; 
} 
/* noew returns -1 if it fails */
void write_out_(halfword p)
{
  char old_setting; 
/*  integer oldmode;  */
  int oldmode;          /* 1995/Jan/7 */
/*  small_number j;  */
  int j;              /* 1995/Jan/7 */
  halfword q, r; 
/* q:=get_avail; info(q):=right_brace_token+"}";@/ */
  q = get_avail (); 
  mem[q].hh.v.LH = 637; 
/* r:=get_avail; link(q):=r; info(r):=end_write_token; ins_list(q);@/ */
/* @d end_write_token==cs_token_flag+end_write */
  r = get_avail (); 
  mem[q].hh.v.RH = r; 
/*  mem[r].hh.v.LH = (hash_size + 4617);  */
/*  mem[r].hh.v.LH = (hash_size + 4095 + 522); */
  mem[r].hh.v.LH = (hash_size + hash_extra + 4095 + 522); 
  begin_token_list(q, 4); 
  begin_token_list(mem[p + 1].hh.v.RH, 15); 
  q = get_avail (); 
  mem[q].hh.v.LH = 379; 
  begin_token_list(q, 4); 
  oldmode = mode; 
  mode = 0; 
  cur_cs = write_loc; 
  q = scan_toks(false, true); 
  get_token (); 
/*  if(cur_tok != (hash_size + 4617)) */
/*  if(cur_tok != (hash_size + 4095 + 522)) */
  if(cur_tok != (hash_size + hash_extra + 4095 + 522))
  {
	  print_err("Unbalanced write command");
	  help2("On this page there's a \\write with fewer real {'s than }'s.",
		  "I can't handle that very well; good luck.");
    error (); 
    do {
    get_token (); 
/*    } while(!(cur_tok == (hash_size + 4617)));  */
/*    } while(!(cur_tok == (hash_size + 4095 + 522)));  */
    } while(!(cur_tok == (hash_size + hash_extra + 4095 + 522))); /*1996/Jan/10*/
  } 
  mode = oldmode; 
  end_token_list (); 
  old_setting = selector; 
  j = mem[p + 1].hh.v.LH; 
  if(write_open[j])selector = j; 
  else {
    if((j == 17)&&(selector == 19)) 
    selector = 18; 
    print_nl("");  /*  */
  } 
  token_show(def_ref); 
  print_ln (); 
  flush_list(def_ref); 
  selector = old_setting; 
} 
void out_what_(halfword p)
{
/*  small_number j;  */
  int j;            /* 1995/Jan/7 */
  switch(mem[p].hh.b1)
  {case 0 : 
  case 1 : 
  case 2 : 
    if(! doing_leaders){
      j = mem[p + 1].hh.v.LH; 
      if(mem[p].hh.b1 == 1){
      write_out(p);
    }
      else {
      if(write_open[j])
        (void) a_close(write_file[j]); 
      if(mem[p].hh.b1 == 2)
        write_open[j]= false; 
      else if(j < 16){
        cur_name = mem[p + 1].hh.v.RH; 
        cur_area = mem[p + 2].hh.v.LH; 
        cur_ext = mem[p + 2].hh.v.RH; 
        if(cur_ext == 335)  /* "" */
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
      confusion(1293); /* ext4 */
      return;       // abort_flag set
    }
    break; 
  } 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void hlist_out (void) 
{/* 21 13 14 15 */ 
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
/* ********************************************************************* */
  real gluetemp;  /* new in 3.14159 */
/* ********************************************************************* */
  thisbox = temp_ptr; 
  gorder = mem[thisbox + 5].hh.b1; 
  gsign = mem[thisbox + 5].hh.b0; 
  p = mem[thisbox + 5].hh.v.RH; 
  incr(cur_s); 
  if(cur_s > 0)
  {
    dvi_buf[dvi_ptr]= 141; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  if(cur_s > max_push)
  max_push = cur_s; 
  saveloc = dvi_offset + dvi_ptr; 
  baseline = cur_v; 
  leftedge = cur_h; 
/* while p<>null do l.12314 */
  while(p != 0)lab21: if((p >= hi_mem_min)) 
  {
    if(cur_h != dvi_h)
    {
      movement(cur_h - dvi_h, 143);   /* 143 == right1 */
      dvi_h = cur_h; 
    } 
    if(cur_v != dvi_v)
    {
      movement(cur_v - dvi_v, 157);   /* 157 == down1 */
      dvi_v = cur_v; 
    } 
    do {
  f = mem[p].hh.b0; 
      c = mem[p].hh.b1; 
      if(f != dvi_f)
      {
  if(! font_used[f])
  {
    dvi_font_def(f); 
    font_used[f]= true; 
  } 
  if(f <= 64)
  {               /* fnt_num_0 --- fnt_num_63 */
    dvi_buf[dvi_ptr]= f + 170; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
  else if(f <= 256){      /* if we allow greater than 256 fonts */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  else {              /* normal TeX 82 case */
#endif
    {               /* fnt1 followed by f */
      dvi_buf[dvi_ptr]= 235; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    {
      dvi_buf[dvi_ptr]= f - 1; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
  } 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
  else {              /* fnt2 followed by f / 256,  f % 256 */
    {
      dvi_buf[dvi_ptr]= 236;
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    {
      dvi_buf[dvi_ptr]= (f - 1) >> 8;   /* top byte */
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    }
    {
      dvi_buf[dvi_ptr]= (f - 1) & 255;    /* bottom byte */
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    }     
  }
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    dvi_f = f; 
      } 
      if(c >= 128)
      {
  dvi_buf[dvi_ptr]= 128; 
  incr(dvi_ptr); 
  if(dvi_ptr == dvi_limit)dvi_swap (); 
      } 
      {
  dvi_buf[dvi_ptr]= c; 
  incr(dvi_ptr); 
  if(dvi_ptr == dvi_limit)dvi_swap (); 
      } 
      cur_h = cur_h + font_info[width_base[f]+ font_info[char_base[f]+ c 
    ].qqqq.b0].cint; 
      p = mem[p].hh.v.RH; 
    } while(!(!(p >= hi_mem_min))); 
    dvi_h = cur_h; 
  } 
  else {
      
    switch(mem[p].hh.b0)
    {case 0 : 
    case 1 : 
/* if list_ptr(p)=null then cur_h:=cur_h+width(p) l.12371 */
      if(mem[p + 5].hh.v.RH == 0)
      cur_h = cur_h + mem[p + 1].cint; 
      else {
    
  saveh = dvi_h; 
  savev = dvi_v; 
  cur_v = baseline + mem[p + 4].cint; 
  temp_ptr = p; 
  edge = cur_h; 
  if(mem[p].hh.b0 == 1)vlist_out (); 
  else hlist_out ();
  dvi_h = saveh; 
  dvi_v = savev; 
  cur_h = edge + mem[p + 1].cint; 
  cur_v = baseline; 
      } 
      break; 
    case 2 : 
      {
  rule_ht = mem[p + 3].cint; 
  rule_dp = mem[p + 2].cint; 
  rule_wd = mem[p + 1].cint; 
  goto lab14; 
      } 
      break; 
    case 8 : 
      out_what(p);
      break; 
    case 10 : 
      {
  g = mem[p + 1].hh.v.LH; 
  rule_wd = mem[g + 1].cint; 
  if(gsign != 0)
  {
    if(gsign == 1)
    {
      if(mem[g].hh.b0 == gorder)
/* *********************************************************************** */
/*      rule_wd = rule_wd + round(mem[thisbox + 6].gr * mem[g + 2].cint); */
/* ******************** 3.14159 ******************************************* */
      {
        gluetemp = mem[thisbox + 6].gr * mem[g + 2].cint; 
        if(gluetemp > 1000000000.0)
        gluetemp = 1000000000.0; 
        else if(gluetemp < -1000000000.0)
        gluetemp = -1000000000.0; 
        rule_wd = rule_wd + round(gluetemp); 
      } 
/* ************************************************************************* */
    } 
/*    else { */
/*      if(mem[g].hh.b1 == gorder)*/
/*      rule_wd = rule_wd - round(mem[thisbox + 6].gr * mem[g + 3].cint); */
/*    }  */
/* ******************** 3.14159 ******************************************* */
    else if(mem[g].hh.b1 == gorder)
    {
      gluetemp = mem[thisbox + 6].gr * mem[g + 3].cint; 
      if(gluetemp > 1000000000.0)
      gluetemp = 1000000000.0; 
      else if(gluetemp < -1000000000.0)
      gluetemp = -1000000000.0; 
      rule_wd = rule_wd - round(gluetemp); 
    } 
/* ************************************************************************* */
  } 
  if(mem[p].hh.b1 >= 100)
  {
    leaderbox = mem[p + 1].hh.v.RH; 
    if(mem[leaderbox].hh.b0 == 2)
    {
      rule_ht = mem[leaderbox + 3].cint; 
      rule_dp = mem[leaderbox + 2].cint; 
      goto lab14; 
    } 
    leaderwd = mem[leaderbox + 1].cint; 
    if((leaderwd > 0)&&(rule_wd > 0)) 
    {
      rule_wd = rule_wd + 10; 
      edge = cur_h + rule_wd; 
      lx = 0; 
      if(mem[p].hh.b1 == 100)
      {
        saveh = cur_h; 
        cur_h = leftedge + leaderwd *((cur_h - leftedge)/ leaderwd)
     ; 
        if(cur_h < saveh)
        cur_h = cur_h + leaderwd; 
      } 
      else {
    
        lq = rule_wd / leaderwd; 
        lr = rule_wd % leaderwd; 
        if(mem[p].hh.b1 == 101)
        cur_h = cur_h +(lr / 2); 
        else {
      
    lx =(2 * lr + lq + 1)/(2 * lq + 2); 
    cur_h = cur_h +((lr -(lq - 1)* lx)/ 2); 
        } 
      } 
      while(cur_h + leaderwd <= edge){
    
        cur_v = baseline + mem[leaderbox + 4].cint; 
        if(cur_v != dvi_v)
        {
    movement(cur_v - dvi_v, 157); /* 157 == down1 */
    dvi_v = cur_v; 
        } 
        savev = dvi_v; 
        if(cur_h != dvi_h)
        {
    movement(cur_h - dvi_h, 143); /* 143 == right1 */
    dvi_h = cur_h; 
        } 
        saveh = dvi_h; 
        temp_ptr = leaderbox; 
        outerdoingleaders = doing_leaders; 
        doing_leaders = true; 
        if(mem[leaderbox].hh.b0 == 1)vlist_out (); 
        else hlist_out ();
        doing_leaders = outerdoingleaders; 
        dvi_v = savev; 
        dvi_h = saveh; 
/* ****************************************************************** */
/*        cur_v = saveh;  */  
        cur_v = baseline;   /* changed in 3.1415 */
/* ****************************************************************** */
        cur_h = saveh + leaderwd + lx; 
      } 
      cur_h = edge - 10; 
      goto lab15; 
    } 
  } 
  goto lab13; 
      } 
      break; 
    case 11 : 
    case 9 : 
      cur_h = cur_h + mem[p + 1].cint; 
      break; 
    case 6 : 
      {
  mem[mem_top - 12]= mem[p + 1]; 
  mem[mem_top - 12].hh.v.RH = mem[p].hh.v.RH; 
  p = mem_top - 12; 
  goto lab21; 
      } 
      break; 
      default: 
   ; 
      break; 
    } 
    goto lab15; 
    lab14: if((rule_ht == -1073741824L))  /* - 2^30 */
    rule_ht = mem[thisbox + 3].cint; 
    if((rule_dp == -1073741824L))     /* - 2^30 */
    rule_dp = mem[thisbox + 2].cint; 
    rule_ht = rule_ht + rule_dp; 
    if((rule_ht > 0)&&(rule_wd > 0)) 
    {
      if(cur_h != dvi_h)
      {
  movement(cur_h - dvi_h, 143);   /* 143 == right1 */
  dvi_h = cur_h; 
      } 
      cur_v = baseline + rule_dp; 
      if(cur_v != dvi_v)
      {
  movement(cur_v - dvi_v, 157);   /* 157 == down1 */
  dvi_v = cur_v; 
      } 
      {
  dvi_buf[dvi_ptr]= 132; 
  incr(dvi_ptr); 
  if(dvi_ptr == dvi_limit)dvi_swap (); 
      } 
      dvi_four(rule_ht); 
      dvi_four(rule_wd); 
      cur_v = baseline; 
      dvi_h = dvi_h + rule_wd; 
    } 
    lab13: cur_h = cur_h + rule_wd; 
    lab15: p = mem[p].hh.v.RH; 
  } 
  prune_movements(saveloc); 
  if(cur_s > 0)
  dvi_pop(saveloc); 
  decr(cur_s); 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void vlist_out (void) 
{/* 13 14 15 */ 
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
/* ************************************************************************ */
  real gluetemp;  /* new in 3.14159 */
/* ************************************************************************ */
  thisbox = temp_ptr; 
  gorder = mem[thisbox + 5].hh.b1; 
  gsign = mem[thisbox + 5].hh.b0; 
  p = mem[thisbox + 5].hh.v.RH; 
  incr(cur_s); 
  if(cur_s > 0)
  {
    dvi_buf[dvi_ptr]= 141; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  if(cur_s > max_push)
  max_push = cur_s; 
  saveloc = dvi_offset + dvi_ptr; 
  leftedge = cur_h; 
  cur_v = cur_v - mem[thisbox + 3].cint; 
  topedge = cur_v; 
  while(p != 0){  /* while p<>null do l.12494 OK */
    if((p >= hi_mem_min)) {
    confusion(822);   /* vlist_out */
    return;       // abort_flag set
  }
    else {
      switch(mem[p].hh.b0)
      {case 0 : 
      case 1 : 
/* if list_ptr(p)=null then cur_v:=cur_v+height(p)+depth(p) l.12529 */
  if(mem[p + 5].hh.v.RH == 0)
  cur_v = cur_v + mem[p + 3].cint + mem[p + 2].cint; 
  else {
      
    cur_v = cur_v + mem[p + 3].cint; 
    if(cur_v != dvi_v)
    {
      movement(cur_v - dvi_v, 157);   /* 157 == down1 */
      dvi_v = cur_v; 
    } 
    saveh = dvi_h; 
    savev = dvi_v; 
    cur_h = leftedge + mem[p + 4].cint; 
    temp_ptr = p; 
    if(mem[p].hh.b0 == 1)vlist_out (); 
    else hlist_out ();
    dvi_h = saveh; 
    dvi_v = savev; 
    cur_v = savev + mem[p + 2].cint; 
    cur_h = leftedge; 
  } 
  break; 
      case 2 : 
  {
    rule_ht = mem[p + 3].cint; 
    rule_dp = mem[p + 2].cint; 
    rule_wd = mem[p + 1].cint; 
    goto lab14; 
  } 
  break; 
      case 8 : 
      out_what(p);
      break; 
      case 10 : 
  {
    g = mem[p + 1].hh.v.LH; 
    rule_ht = mem[g + 1].cint; 
    if(gsign != 0)
    {
      if(gsign == 1)
      {
        if(mem[g].hh.b0 == gorder)
/* ************************************************************************ */
/*        rule_ht = rule_ht + round(mem[thisbox + 6].gr * mem[g + 2].cint); */
/* ************************ 3.14159 **************************************** */
        {
    gluetemp = mem[thisbox + 6].gr * mem[g + 2].cint; 
    if(gluetemp > 1000000000.0)
    gluetemp = 1000000000.0; 
    else if(gluetemp < -1000000000.0)
    gluetemp = -1000000000.0; 
    rule_ht = rule_ht + round(gluetemp); 
        } 
/* ************************************************************************* */
      } 
/* ************************************************************************* */
/*      else { */
/*        if(mem[g].hh.b1 == gorder) */
/*        rule_ht = rule_ht - round(mem[thisbox + 6].gr * mem[g + 3].cint); */
/*      } */
      else if(mem[g].hh.b1 == gorder)   /* BUG FIX !!! */
        {
        gluetemp = mem[thisbox + 6].gr * mem[g + 3].cint; 
        if(gluetemp > 1000000000.0)
        gluetemp = 1000000000.0; 
        else if(gluetemp < -1000000000.0)
        gluetemp = -1000000000.0; 
        rule_ht = rule_ht - round(gluetemp); 
      } 
/* ************************************************************************* */
    } 
    if(mem[p].hh.b1 >= 100)
    {
      leaderbox = mem[p + 1].hh.v.RH; 
      if(mem[leaderbox].hh.b0 == 2)
      {
        rule_wd = mem[leaderbox + 1].cint; 
        rule_dp = 0; 
        goto lab14; 
      } 
      leaderht = mem[leaderbox + 3].cint + mem[leaderbox + 2]
     .cint; 
      if((leaderht > 0)&&(rule_ht > 0)) 
      {
        rule_ht = rule_ht + 10; 
        edge = cur_v + rule_ht; 
        lx = 0; 
        if(mem[p].hh.b1 == 100)
        {
    savev = cur_v; 
    cur_v = topedge + leaderht *((cur_v - topedge)/ leaderht)
    ; 
    if(cur_v < savev)
    cur_v = cur_v + leaderht; 
        } 
        else {
      
    lq = rule_ht / leaderht; 
    lr = rule_ht % leaderht; 
    if(mem[p].hh.b1 == 101)
    cur_v = cur_v +(lr / 2); 
    else {
        
      lx =(2 * lr + lq + 1)/(2 * lq + 2); 
      cur_v = cur_v +((lr -(lq - 1)* lx)/ 2); 
    } 
        } 
        while(cur_v + leaderht <= edge){
      
    cur_h = leftedge + mem[leaderbox + 4].cint; 
    if(cur_h != dvi_h)
    {
      movement(cur_h - dvi_h, 143);   /* 143 == right1 */
      dvi_h = cur_h; 
    } 
    saveh = dvi_h; 
    cur_v = cur_v + mem[leaderbox + 3].cint; 
    if(cur_v != dvi_v)
    {
      movement(cur_v - dvi_v, 157);   /* 157 == down1 */
      dvi_v = cur_v; 
    } 
    savev = dvi_v; 
    temp_ptr = leaderbox; 
    outerdoingleaders = doing_leaders; 
    doing_leaders = true; 
    if(mem[leaderbox].hh.b0 == 1)vlist_out (); 
    else hlist_out (); 
    doing_leaders = outerdoingleaders; 
    dvi_v = savev; 
    dvi_h = saveh; 
/* ************************************************************************ */
/*    cur_h = saveh;  */
    cur_h = leftedge;  /* 3.1415 */
/* ************************************************************************ */
    cur_v = savev - mem[leaderbox + 3].cint + leaderht + lx; 
        } 
        cur_v = edge - 10; 
        goto lab15; 
      } 
    } 
    goto lab13; 
  } 
  break; 
      case 11 : 
  cur_v = cur_v + mem[p + 1].cint; 
  break; 
  default: 
  ; 
  break; 
      } 
      goto lab15; 
      lab14: if((rule_wd == -1073741824L))    /* -2^30 */
      rule_wd = mem[thisbox + 1].cint; 
      rule_ht = rule_ht + rule_dp; 
      cur_v = cur_v + rule_ht; 
      if((rule_ht > 0)&&(rule_wd > 0)) 
      {
  if(cur_h != dvi_h)
  {
    movement(cur_h - dvi_h, 143);   /* 143 == right1 */
    dvi_h = cur_h; 
  } 
  if(cur_v != dvi_v)
  {
    movement(cur_v - dvi_v, 157);   /* 157 == down1 */
    dvi_v = cur_v; 
  } 
  {
    dvi_buf[dvi_ptr]= 137; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  dvi_four(rule_ht); 
  dvi_four(rule_wd); 
      } 
      goto lab15; 
      lab13: cur_v = cur_v + rule_ht; 
    } 
    lab15: p = mem[p].hh.v.RH; 
  } 
  prune_movements(saveloc); 
  if(cur_s > 0)
  dvi_pop(saveloc); 
  decr(cur_s); 
} 
/* following needs access to dvi_buf=zdvibuf see coerce.h */
void ship_out_(halfword p)
{/* 30 */ 
  integer pageloc; 
  char j, k; 
  pool_pointer s; 
  char old_setting; 
  if(eqtb[(hash_size + 3197)].cint > 0)
  {
    print_nl("");    /*  */
    print_ln (); 
    print(823);   /* Completed box being shipped out */
  } 
  if(term_offset > max_print_line - 9)
  print_ln (); 
  else if((term_offset > 0)||(file_offset > 0)) 
  print_char(32);   /*   */
  print_char(91);   /*[*/
  j = 9; 
  while((eqtb[(hash_size + 3218) + j].cint == 0)&&(j > 0)) decr(j); 
  {
    register integer for_end; 
    k = 0; 
    for_end = j; 
    if(k <= for_end) do 
    {
      print_int(eqtb[(hash_size + 3218) + k].cint); 
      if(k < j)
        print_char(46);   /* . */
    } 
    while(k++ < for_end);
  } 
#ifndef _WINDOWS
  fflush(stdout); 
#endif
  if(eqtb[(hash_size + 3197)].cint > 0)
  {
    print_char(93);   /*]*/
    begin_diagnostic (); 
    show_box(p); 
    end_diagnostic(true); 
  } 
  if((mem[p + 3].cint > 1073741823L)|| /* 2^30 - 1 */
    (mem[p + 2].cint > 1073741823L)||
    (mem[p + 3].cint + mem[p + 2].cint +
     eqtb[(hash_size + 3749)].cint > 1073741823L)||
    (mem[p + 1].cint + eqtb[(hash_size + 3748)].cint > 1073741823L)) 
  {
	  print_err("Huge page cannot be shipped out");
	  help2("The page just created is more than 18 feet tall or",
		  "more than 18 feet wide, so I suspect something went wrong.");
    error (); 
    if(eqtb[(hash_size + 3197)].cint <= 0)
    {
      begin_diagnostic (); 
      print_nl("The following box has been deleted:");    /*  */
      show_box(p); 
      end_diagnostic(true); 
    } 
    goto lab30; 
  } 
  if(mem[p + 3].cint + mem[p + 2].cint + eqtb[(hash_size + 3749)].cint > max_v 
)
  max_v = mem[p + 3].cint + mem[p + 2].cint + eqtb[(hash_size + 3749)].cint; 
  if(mem[p + 1].cint + eqtb[(hash_size + 3748)].cint > max_h)
  max_h = mem[p + 1].cint + eqtb[(hash_size + 3748)].cint; 
  dvi_h = 0; 
  dvi_v = 0; 
  cur_h = eqtb[(hash_size + 3748)].cint; 
  dvi_f = 0; 
  if(output_file_name == 0)
  {
    if(job_name == 0)open_log_file (); 
    pack_job_name(788);   /* .dvi */
    while(! b_open_out(dvi_file)) {
    prompt_file_name(789, 788); /* file name for output  .dvi */
  }
    output_file_name = b_make_name_string(dvi_file); 
  } 

  if(total_pages == 0)
  {
    {
      dvi_buf[dvi_ptr]= 247; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    {
      dvi_buf[dvi_ptr]= 2; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    dvi_four(25400000L);  /* magic DVI scale factor */
    dvi_four(473628672L); /* 7227 * 65536 */
    prepare_mag (); 
    dvi_four(eqtb[(hash_size + 3180)].cint); 
    old_setting = selector; 
    selector = 21; 
    print(821);   /* TeX output  */
    print_int(eqtb[(hash_size + 3186)].cint); 
    print_char(46); /* . */
    print_two(eqtb[(hash_size + 3185)].cint); 
    print_char(46); /* . */
    print_two(eqtb[(hash_size + 3184)].cint); 
    print_char(58); /* : */
    print_two(eqtb[(hash_size + 3183)].cint / 60); 
    print_two(eqtb[(hash_size + 3183)].cint % 60); 
    selector = old_setting; 
    {
/*    long to unsigned char ... */
      dvi_buf[dvi_ptr]=(pool_ptr - str_start[str_ptr]); 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    {
    register integer for_end; 
    s = str_start[str_ptr]; 
    for_end = pool_ptr - 1; 
    if(s <= for_end) do 
    {
      dvi_buf[dvi_ptr]= str_pool[s]; 
      incr(dvi_ptr); 
      if(dvi_ptr == dvi_limit)dvi_swap (); 
    } 
    while(s++ < for_end);
  } 
    pool_ptr = str_start[str_ptr]; 
  } // end of if total_pages == 0

  pageloc = dvi_offset + dvi_ptr; 
  {
    dvi_buf[dvi_ptr]= 139; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  {
    register integer for_end; 
    k = 0; 
    for_end = 9; 
    if(k <= for_end) do 
      dvi_four(eqtb[(hash_size + 3218) + k].cint); 
      while(k++ < for_end);
  } 
  dvi_four(last_bop); 
  last_bop = pageloc; 
  cur_v = mem[p + 3].cint + eqtb[(hash_size + 3749)].cint; 
  temp_ptr = p; 
  if(mem[p].hh.b0 == 1)vlist_out (); 
  else hlist_out (); 
  {
    dvi_buf[dvi_ptr]= 140; 
    incr(dvi_ptr); 
    if(dvi_ptr == dvi_limit)dvi_swap (); 
  } 
  incr(total_pages); 
  cur_s = -1; 
  lab30:; 
  if(eqtb[(hash_size + 3197)].cint <= 0)
  print_char(93);   /*]*/
  dead_cycles = 0; 
#ifndef _WINDOWS
  fflush(stdout); 
#endif
  ;
#ifdef STAT
  if(eqtb[(hash_size + 3194)].cint > 1)
  {
    print_nl("Memory usage before: ");        /*  */
    print_int(var_used); 
    print_char(38);       /* & */
    print_int(dyn_used); 
    print_char(59);       /*; */
  } 
#endif /* STAT */
  flush_node_list(p); 
  ;
#ifdef STAT
  if(eqtb[(hash_size + 3194)].cint > 1)
  {
    print(825);         /* after */
    print_int(var_used); 
    print_char(38);         /* & */
    print_int(dyn_used); 
    print(826);         /* still utouched */
    print_int(hi_mem_min - lo_mem_max - 1); /* somewhat inaccurate free mem */
    print_ln (); 
  } 
#endif /* STAT */
} 
void scan_spec_(group_code c, bool threecodes)
{/* 40 */ 
  integer s; 
  char speccode; 
  if(threecodes)
    s = save_stack[save_ptr + 0].cint; 
  if(scan_keyword(836))   /* to */
    speccode = 0; 
  else if(scan_keyword(837))  /* spread */
    speccode = 1; 
  else {
    speccode = 1; 
    cur_val = 0; 
    goto lab40; 
  } 
  scan_dimen(false, false, false); 
  lab40: if(threecodes)
  {
    save_stack[save_ptr + 0].cint = s;  /* s may be used without ... */
    incr(save_ptr); 
  } 
  save_stack[save_ptr + 0].cint = speccode; 
  save_stack[save_ptr + 1].cint = cur_val; 
  save_ptr = save_ptr + 2; 
  new_save_level(c); 
  scan_left_brace (); 
} 
halfword hpack_(halfword p, scaled w, small_number m)
{/* 21 50 10 */ register halfword Result;
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
  r = get_node(7); 
  mem[r].hh.b0 = 0; 
  mem[r].hh.b1 = 0; 
  mem[r + 4].cint = 0; 
  q = r + 5; 
  mem[q].hh.v.RH = p; 
  h = 0; 
  d = 0; 
  x = 0; 
  totalstretch[0]= 0; 
  totalshrink[0]= 0; 
  totalstretch[1]= 0; 
  totalshrink[1]= 0; 
  totalstretch[2]= 0; 
  totalshrink[2]= 0; 
  totalstretch[3]= 0; 
  totalshrink[3]= 0; 
  while(p != 0){ /* while p<>null do l.12862 */
      
    lab21: while((p >= hi_mem_min)) {
  
      f = mem[p].hh.b0; 
      i = font_info[char_base[f]+ mem[p].hh.b1].qqqq; 
      hd = i.b1; 
      x = x + font_info[width_base[f]+ i.b0].cint; 
      s = font_info[height_base[f]+(hd)/ 16].cint; 
      if(s > h)
      h = s; 
      s = font_info[depth_base[f]+(hd)% 16].cint; 
      if(s > d)
      d = s; 
      p = mem[p].hh.v.RH; 
    } 
    if(p != 0)  /* if p<>null then l.12886 */
    {
      switch(mem[p].hh.b0)
      {case 0 : 
      case 1 : 
      case 2 : 
      case 13 : 
  {
    x = x + mem[p + 1].cint; 
    if(mem[p].hh.b0 >= 2)
    s = 0; 
    else s = mem[p + 4].cint; 
    if(mem[p + 3].cint - s > h)
    h = mem[p + 3].cint - s; 
    if(mem[p + 2].cint + s > d)
    d = mem[p + 2].cint + s; 
  } 
  break; 
      case 3 : 
      case 4 : 
      case 5 : 
/* if adjust_tail<>null then link(adjust_tail):=null; l.12865 / 12891 */
  if(adjust_tail != 0)
  {
    while(mem[q].hh.v.RH != p)q = mem[q].hh.v.RH; 
    if(mem[p].hh.b0 == 5)
    {
      mem[adjust_tail].hh.v.RH = mem[p + 1].cint; 
      while(mem[adjust_tail].hh.v.RH != 0)adjust_tail = mem[
      adjust_tail].hh.v.RH; 
      p = mem[p].hh.v.RH; 
      free_node(mem[q].hh.v.RH, 2); 
    } 
    else {
        
      mem[adjust_tail].hh.v.RH = p; 
      adjust_tail = p; 
      p = mem[p].hh.v.RH; 
    } 
    mem[q].hh.v.RH = p; 
    p = q; 
  } 
  break; 
      case 8 : 
  ; 
  break; 
      case 10 : 
  {
    g = mem[p + 1].hh.v.LH; 
    x = x + mem[g + 1].cint; 
    o = mem[g].hh.b0; 
    totalstretch[o]= totalstretch[o]+ mem[g + 2].cint; 
    o = mem[g].hh.b1; 
    totalshrink[o]= totalshrink[o]+ mem[g + 3].cint; 
    if(mem[p].hh.b1 >= 100)
    {
      g = mem[p + 1].hh.v.RH; 
      if(mem[g + 3].cint > h)
      h = mem[g + 3].cint; 
      if(mem[g + 2].cint > d)
      d = mem[g + 2].cint; 
    } 
  } 
  break; 
      case 11 : 
      case 9 : 
  x = x + mem[p + 1].cint; 
  break; 
      case 6 : 
  {
    mem[mem_top - 12]= mem[p + 1]; 
    mem[mem_top - 12].hh.v.RH = mem[p].hh.v.RH; 
    p = mem_top - 12; 
    goto lab21; 
  } 
  break; 
  default: 
  ; 
  break; 
      } 
      p = mem[p].hh.v.RH; 
    } 
  } 
  if(adjust_tail != 0)
  mem[adjust_tail].hh.v.RH = 0; 
  mem[r + 3].cint = h; 
  mem[r + 2].cint = d; 
  if(m == 1)
  w = x + w; 
  mem[r + 1].cint = w; 
  x = w - x; 
  if(x == 0)
  {
    mem[r + 5].hh.b0 = 0; 
    mem[r + 5].hh.b1 = 0; 
    mem[r + 6].gr = 0.0; 
    goto lab10; 
  } 
  else if(x > 0)
  {
    if(totalstretch[3]!= 0)
    o = 3; 
    else if(totalstretch[2]!= 0)
    o = 2; 
    else if(totalstretch[1]!= 0)
    o = 1; 
    else o = 0; 
    mem[r + 5].hh.b1 = o; 
    mem[r + 5].hh.b0 = 1; 
/* if total_stretch[o]<>0 then glue_set(r):=... l.12982 */
    if(totalstretch[o]!= 0)
    mem[r + 6].gr = x / ((double) totalstretch[o]); 
    else {
  
      mem[r + 5].hh.b0 = 0; 
      mem[r + 6].gr = 0.0; 
    } 
/* if o=normal then if list_ptr(r)<>null then l.12987 */
    if(o == 0)
    if(mem[r + 5].hh.v.RH != 0)
    {
      last_badness = badness(x, totalstretch[0]); 
      if(last_badness > eqtb[(hash_size + 3189)].cint)
      {
  print_ln (); 
  if(last_badness > 100)
    print_nl("Underfull");    /*  */
  else print_nl("Loose");   /*  */
  print(840);       /* \hbox (badness */
  print_int(last_badness); 
  if(last_badness > 100)underfull_hbox++;   /* 1996/Feb/9 */
  goto lab50; 
      } 
    } 
    goto lab10; 
  } 
  else {
      
    if(totalshrink[3]!= 0)
    o = 3; 
    else if(totalshrink[2]!= 0)
    o = 2; 
    else if(totalshrink[1]!= 0)
    o = 1; 
    else o = 0; 
    mem[r + 5].hh.b1 = o; 
    mem[r + 5].hh.b0 = 2; 
    if(totalshrink[o]!= 0)
    mem[r + 6].gr =(- (integer) x)/ ((double) totalshrink[o]); 
    else {
  
      mem[r + 5].hh.b0 = 0; 
      mem[r + 6].gr = 0.0; 
    } 
/* if (total_shrink[o]<-x)and(o=normal)and(list_ptr(r)<>null) then l.13946 */
    if((totalshrink[o]< - (integer) x)&&(o == 0)&&(mem[r + 5 
  ].hh.v.RH != 0)) 
    {
      last_badness = 1000000L; 
/*   set_glue_ratio_one(glue_set(r)); {use the maximum shrinkage} */
      mem[r + 6].gr = 1.0; 
/* if (-x-total_shrink[normal]>hfuzz)or(hbadness<100) then */
      if((- (integer) x - totalshrink[0]> eqtb[(hash_size + 3738)].cint)||(
      eqtb[(hash_size + 3189)].cint < 100)) 
      {
  if((eqtb[(hash_size + 3746)].cint > 0)&&(- (integer) x - totalshrink[0 
  ] > eqtb[(hash_size + 3738)].cint)) 
  {
    while(mem[q].hh.v.RH != 0)q = mem[q].hh.v.RH; 
    mem[q].hh.v.RH = new_rule (); 
    mem[mem[q].hh.v.RH + 1].cint = eqtb[(hash_size + 3746)].cint; 
  } 
  print_ln (); 
  print_nl("Overfull \\hbox(");    /* */
  print_scaled(- (integer) x - totalshrink[0]); 
  print(847);     /* pt too wide */
  overfull_hbox++;      /* 1996/Feb/9 */
  goto lab50; 
      } 
    } 
    else if(o == 0)
    if(mem[r + 5].hh.v.RH != 0)
    {
      last_badness = badness(- (integer) x, totalshrink[0]); 
      if(last_badness > eqtb[(hash_size + 3189)].cint)
      {
  print_ln (); 
  print_nl("Tight \\hbox (badness ");  /*  */
  print_int(last_badness); 
  goto lab50; 
      } 
    } 
    goto lab10; 
  } 
  lab50: if(output_active)
  print(841); /*)has occurred while \output is active */
  else {
      
    if(pack_begin_line != 0)
    {
      if(pack_begin_line > 0)
      print(842); /*)in paragraph at lines  */
      else print(843);  /*)in alignment at lines  */
      print_int(abs(pack_begin_line)); 
      print(844); /* -- */
    } 
    else print(845);  /*)detected at line  */
    print_int(line); 
  } 
  print_ln (); 
  font_in_short_display = 0; 
  short_display(mem[r + 5].hh.v.RH); 
  print_ln (); 
  begin_diagnostic (); 
  show_box(r); 
  end_diagnostic(true); 
  lab10: Result = r; 
  return Result; 
} 
halfword vpackage_(halfword p, scaled h, small_number m, scaled l)
{/* 50 10 */ register halfword Result; 
  halfword r; 
  scaled w, d, x; 
  scaled s; 
  halfword g; 
/*  glue_ord o;  */
  int o;              /* 95/Jan/7 */
  last_badness = 0; 
  r = get_node(7); 
  mem[r].hh.b0 = 1; 
  mem[r].hh.b1 = 0; 
  mem[r + 4].cint = 0; 
  mem[r + 5].hh.v.RH = p; 
  w = 0; 
  d = 0; 
  x = 0; 
  totalstretch[0]= 0; 
  totalshrink[0]= 0; 
  totalstretch[1]= 0; 
  totalshrink[1]= 0; 
  totalstretch[2]= 0; 
  totalshrink[2]= 0; 
  totalstretch[3]= 0; 
  totalshrink[3]= 0; 
  while(p != 0){
      
    if((p >= hi_mem_min)) {
    confusion(849);   /* vpack */
    return 0;       // abort_flag set
  }
    else switch(mem[p].hh.b0)
    {case 0 : 
    case 1 : 
    case 2 : 
    case 13 : 
      {
  x = x + d + mem[p + 3].cint; 
  d = mem[p + 2].cint; 
  if(mem[p].hh.b0 >= 2)
  s = 0; 
  else s = mem[p + 4].cint; 
  if(mem[p + 1].cint + s > w)
  w = mem[p + 1].cint + s; 
      } 
      break; 
    case 8 : 
   ; 
      break; 
    case 10 : 
      {
  x = x + d; 
  d = 0; 
  g = mem[p + 1].hh.v.LH; 
  x = x + mem[g + 1].cint; 
  o = mem[g].hh.b0; 
  totalstretch[o]= totalstretch[o]+ mem[g + 2].cint; 
  o = mem[g].hh.b1; 
  totalshrink[o]= totalshrink[o]+ mem[g + 3].cint; 
  if(mem[p].hh.b1 >= 100)
  {
    g = mem[p + 1].hh.v.RH; 
    if(mem[g + 1].cint > w)
    w = mem[g + 1].cint; 
  } 
      } 
      break; 
    case 11 : 
      {
  x = x + d + mem[p + 1].cint; 
  d = 0; 
      } 
      break; 
      default: 
   ; 
      break; 
    } 
    p = mem[p].hh.v.RH; 
  } 
  mem[r + 1].cint = w; 
  if(d > l)
  {
    x = x + d - l; 
    mem[r + 2].cint = l; 
  } 
  else mem[r + 2].cint = d; 
  if(m == 1)
  h = x + h; 
  mem[r + 3].cint = h; 
  x = h - x; 
  if(x == 0)
  {
    mem[r + 5].hh.b0 = 0; 
    mem[r + 5].hh.b1 = 0; 
    mem[r + 6].gr = 0.0; 
    goto lab10; 
  } 
  else if(x > 0)
  {
    if(totalstretch[3]!= 0)
    o = 3; 
    else if(totalstretch[2]!= 0)
    o = 2; 
    else if(totalstretch[1]!= 0)
    o = 1; 
    else o = 0; 
    mem[r + 5].hh.b1 = o; 
    mem[r + 5].hh.b0 = 1; 
    if(totalstretch[o]!= 0)
    mem[r + 6].gr = x / ((double) totalstretch[o]); 
    else {
  
      mem[r + 5].hh.b0 = 0; 
      mem[r + 6].gr = 0.0; 
    } 
    if(o == 0)
    if(mem[r + 5].hh.v.RH != 0)
    {
      last_badness = badness(x, totalstretch[0]); 
      if(last_badness > eqtb[(hash_size + 3190)].cint)
      {
  print_ln (); 
  if(last_badness > 100)
    print_nl("Underfull");      /*  */
  else print_nl("Loose");     /*  */
  print(850);         /* \vbox (badness */
  print_int(last_badness); 
  if(last_badness > 100)underfull_vbox++; /* 1996/Feb/9 */
  goto lab50; 
      } 
    } 
    goto lab10; 
  } 
  else {
      
    if(totalshrink[3]!= 0)
    o = 3; 
    else if(totalshrink[2]!= 0)
    o = 2; 
    else if(totalshrink[1]!= 0)
    o = 1; 
    else o = 0; 
    mem[r + 5].hh.b1 = o; 
    mem[r + 5].hh.b0 = 2; 
    if(totalshrink[o]!= 0)
    mem[r + 6].gr =(- (integer) x)/ ((double) totalshrink[o]); 
    else {
  
      mem[r + 5].hh.b0 = 0; 
      mem[r + 6].gr = 0.0; 
    } 
/* if (total_shrink[o]<-x)and(o=normal)and(list_ptr(r)<>null) then l.13046 */
    if((totalshrink[o]< - (integer) x)&&(o == 0)&&(mem[r + 5 
  ].hh.v.RH != 0)) 
    {
      last_badness = 1000000L; 
      mem[r + 6].gr = 1.0; 
      if((- (integer) x - totalshrink[0]> eqtb[(hash_size + 3739)].cint)||(
      eqtb[(hash_size + 3190)].cint < 100)) 
      {
  print_ln (); 
  print_nl("Overfull \\vbox(");  /* */
  print_scaled(- (integer) x - totalshrink[0]); 
  print(852);   /* pt too high */
  overfull_vbox++;    /* 1996/Feb/9 */
  goto lab50; 
      } 
    } 
/* else if o=normal then if list_ptr(r)<>null then l.13052  */
    else if(o == 0)
    if(mem[r + 5].hh.v.RH != 0)
    {
      last_badness = badness(- (integer) x, totalshrink[0]); 
      if(last_badness > eqtb[(hash_size + 3190)].cint)
      {
  print_ln (); 
  print_nl("Tight \\vbox (badness ");  /*  */
  print_int(last_badness); 
  goto lab50; 
      } 
    } 
    goto lab10; 
  } 
  lab50: if(output_active)
  print(841); /*)has occurred while \output is active */
  else {
      
    if(pack_begin_line != 0)
    {
      print(843); /*)in alignment at lines  */
      print_int(abs(pack_begin_line)); 
      print(844); /* -- */
    } 
    else print(845);  /*)detected at line  */
    print_int(line); 
    print_ln (); 
  } 
  begin_diagnostic (); 
  show_box(r); 
  end_diagnostic(true); 
  lab10: Result = r; 
  return Result; 
} 
void append_to_vlist_(halfword b)
{ 
  scaled d; 
  halfword p; 
  if(cur_list.aux_field.cint > ignore_depth)
  {
    d = mem[eqtb[(hash_size + 783)].hh.v.RH + 1].cint - cur_list.aux_field.cint - 
    mem[b + 3].cint; 
    if(d < eqtb[(hash_size + 3732)].cint)
    p = new_param_glue(0); 
    else {
  
      p = new_skip_param(1); 
      mem[temp_ptr + 1].cint = d; 
    } 
    mem[tail].hh.v.RH = p; 
    tail = p; 
  } 
  mem[tail].hh.v.RH = b; 
  tail = b; 
  cur_list.aux_field.cint = mem[b + 2].cint; 
} 
halfword new_noad (void) 
{register halfword Result; 
  halfword p; 
  p = get_node(4); 
  mem[p].hh.b0 = 16; 
  mem[p].hh.b1 = 0; 
  mem[p + 1].hh = empty_field; 
  mem[p + 3].hh = empty_field; 
  mem[p + 2].hh = empty_field; 
  Result = p; 
  return Result; 
} 
halfword new_style_(small_number s)
{register halfword Result; 
  halfword p; 
  p = get_node(3); 
  mem[p].hh.b0 = 14; 
  mem[p].hh.b1 = s; 
  mem[p + 1].cint = 0; 
  mem[p + 2].cint = 0; 
  Result = p; 
  return Result; 
} 
halfword new_choice (void) 
{register halfword Result; 
  halfword p; 
  p = get_node(3); 
  mem[p].hh.b0 = 15; 
  mem[p].hh.b1 = 0; 
  mem[p + 1].hh.v.LH = 0; 
  mem[p + 1].hh.v.RH = 0; 
  mem[p + 2].hh.v.LH = 0; 
  mem[p + 2].hh.v.RH = 0; 
  Result = p; 
  return Result; 
} 
void show_info (void) 
{ 
  show_node_list(mem[temp_ptr].hh.v.LH); 
} 
halfword fraction_rule_(scaled t)
{register halfword Result;
  halfword p; 
  p = new_rule (); 
  mem[p + 3].cint = t; 
  mem[p + 2].cint = 0; 
  Result = p; 
  return Result; 
} 
halfword overbar_(halfword b, scaled k, scaled t)
{register halfword Result;
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
{register halfword Result;
  ffourquarters q; 
  eight_bits hd; 
  halfword b, p; 
  q = font_info[char_base[f]+ c].qqqq; 
  hd = q.b1; 
  b = new_null_box (); 
  mem[b + 1].cint = font_info[width_base[f]+ q.b0].cint + font_info 
 [italic_base[f]+(q.b2)/ 4].cint; 
  mem[b + 3].cint = font_info[height_base[f]+(hd)/ 16].cint; 
  mem[b + 2].cint = font_info[depth_base[f]+(hd)% 16].cint; 
/*  long to unsigned short ... */
  p = get_avail (); 
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
{register scaled Result; 
  ffourquarters q; 
  eight_bits hd; 
  q = font_info[char_base[f]+ c].qqqq; 
  hd = q.b1; 
  Result = font_info[height_base[f]+(hd)/ 16].cint + font_info[
  depth_base[f]+(hd)% 16].cint; 
  return Result; 
} 
halfword var_delimiter_(halfword d, small_number s, scaled v)
{/* 40 22 */ register halfword Result; 
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
  while(true){
    if((z != 0)||(x != 0)) 
    {
      z = z + s + 16; 
      do {
    z = z - 16; 
  g = eqtb[(hash_size + 1835) + z].hh.v.RH; 
  if(g != 0)
  {
    y = x; 
    if((y >= font_bc[g])&&(y <= font_ec[g])) 
    {
      lab22: q = font_info[char_base[g]+ y].qqqq; 
      if((q.b0 > 0)) 
      {
        if(((q.b2)% 4)== 3)
        {
    f = g; 
    c = y; 
    goto lab40; 
        } 
        hd = q.b1; 
        u = font_info[height_base[g]+(hd)/ 16].cint + font_info 
       [depth_base[g]+(hd)% 16].cint; 
        if(u > w)
        {
    f = g; 
    c = y; 
    w = u; 
    if(u >= v)
    goto lab40; 
        } 
        if(((q.b2)% 4)== 2)
        {
    y = q.b3; 
    goto lab22; 
        } 
      } 
    } 
  } 
      } while(!(z < 16)); 
    } 
    if(largeattempt)
    goto lab40; 
    largeattempt = true; 
    z = mem[d].qqqq.b2; 
    x = mem[d].qqqq.b3; 
  } 
  lab40: if(f != 0)
  if(((q.b2)% 4)== 3)   /* q may be used without ... */
  {
    b = new_null_box (); 
    mem[b].hh.b0 = 1; 
    r = font_info[exten_base[f]+ q.b3].qqqq; 
    c = r.b3; 
    u = height_plus_depth(f, c); 
    w = 0; 
    q = font_info[char_base[f]+ c].qqqq; 
    mem[b + 1].cint = font_info[width_base[f]+ q.b0].cint + 
    font_info[italic_base[f]+(q.b2)/ 4].cint; 
    c = r.b2; 
    if(c != 0)
    w = w + height_plus_depth(f, c); 
    c = r.b1; 
    if(c != 0)
    w = w + height_plus_depth(f, c); 
    c = r.b0; 
    if(c != 0)
    w = w + height_plus_depth(f, c); 
    n = 0; 
    if(u > 0)
    while(w < v){
  
      w = w + u; 
      incr(n); 
      if(r.b1 != 0)
      w = w + u; 
    } 
    c = r.b2; 
    if(c != 0)
    stack_into_box(b, f, c); 
    c = r.b3; 
    {
    register integer for_end; 
    m = 1; 
    for_end = n; 
    if(m <= for_end) do 
      stack_into_box(b, f, c); 
    while(m++ < for_end);
  } 
    c = r.b1; 
    if(c != 0)
    {
      stack_into_box(b, f, c); 
      c = r.b3; 
      {
      register integer for_end; 
      m = 1; 
      for_end = n; 
      if(m <= for_end) do 
        stack_into_box(b, f, c); 
      while(m++ < for_end);
    } 
    } 
    c = r.b0; 
    if(c != 0)
    stack_into_box(b, f, c); 
    mem[b + 2].cint = w - mem[b + 3].cint; 
  } 
  else b = char_box(f, c); 
  else {    /* c may be used without ... */
    b = new_null_box (); 
    mem[b + 1].cint = eqtb[(hash_size + 3741)].cint; 
  } 
  mem[b + 4].cint = half(mem[b + 3].cint - mem[b + 2].cint)- 
  font_info[22 + param_base[eqtb[(hash_size + 1837) + s].hh.v.RH]].cint; 
  Result = b; 
  return Result; 
}
/* rebox_ etc used to follow here in tex4.c */
