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

/* #pragma optimize("a", off) */          /* 98/Dec/10 experiment */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

void show_box_(halfword p)
{
  depth_threshold = eqtb[(hash_size + 3188)].cint; 
  breadth_max = eqtb[(hash_size + 3187)].cint; 
  if(breadth_max <= 0)
  breadth_max = 5; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
  if(pool_ptr + depth_threshold >= current_pool_size) /* ??? 93/Nov/28 */
    str_pool = realloc_str_pool (increment_pool_size);    /* ??? 94/Jan/24 */
  if(pool_ptr + depth_threshold >= current_pool_size) /* in case it failed */
    depth_threshold = current_pool_size - pool_ptr - 1; 
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if(pool_ptr + depth_threshold >= pool_size)
    depth_threshold = pool_size - pool_ptr - 1; 
#endif
  show_node_list(p); 
  print_ln (); 
} 
void delete_token_ref_(halfword p) 
{
  if(mem[p].hh.v.LH == 0)
  flush_list(p); 
  else decr(mem[p].hh.v.LH); 
} 
void delete_glue_ref_(halfword p)
{ 
  if(mem[p].hh.v.RH == 0)
  free_node(p, 4); 
  else decr(mem[p].hh.v.RH); 
} 
void flush_node_list_(halfword p) 
{/* 30 */ 
  halfword q; 
  while(p != 0){      /* while p<>null */
      
    q = mem[p].hh.v.RH; 
    if((p >= hi_mem_min)) 
    {
      mem[p].hh.v.RH = avail; 
      avail = p; 
  ;
#ifdef STAT
      decr(dyn_used); 
#endif /* STAT */
    } 
    else {
  
      switch(mem[p].hh.b0)
      {case 0 : 
      case 1 : 
      case 13 : 
  {
    flush_node_list(mem[p + 5].hh.v.RH); 
    free_node(p, 7); 
    goto lab30; 
  } 
  break; 
      case 2 : 
  {
    free_node(p, 4); 
    goto lab30; 
  } 
  break; 
      case 3 : 
  {
    flush_node_list(mem[p + 4].hh.v.LH); 
    delete_glue_ref(mem[p + 4].hh.v.RH); 
    free_node(p, 5); 
    goto lab30; 
  } 
  break; 
      case 8 : 
  {
    switch(mem[p].hh.b1)
    {case 0 : 
      free_node(p, 3); 
      break; 
    case 1 : 
    case 3 : 
      {
        delete_token_ref(mem[p + 1].hh.v.RH); 
        free_node(p, 2); 
        goto lab30; 
      } 
      break; 
    case 2 : 
    case 4 : 
      free_node(p, 2); 
      break; 
    default: 
      {
        confusion(1289);    /* ext3 */
        return;         // abort_flag set
      }
      break; 
    } 
    goto lab30; 
  } 
  break; 
      case 10 : 
  {
    {
      if(mem[mem[p + 1].hh.v.LH].hh.v.RH == 0)
      free_node(mem[p + 1].hh.v.LH, 4); 
      else decr(mem[mem[p + 1].hh.v.LH].hh.v.RH); 
    } 
/*     if leader_ptr(p)<>null then flush_node_list(leader_ptr(p)); */
    if(mem[p + 1].hh.v.RH != 0)
    flush_node_list(mem[p + 1].hh.v.RH); 
  } 
  break; 
      case 11 : 
      case 9 : 
      case 12 : 
  ; 
  break; 
      case 6 : 
  flush_node_list(mem[p + 1].hh.v.RH); 
  break; 
      case 4 : 
  delete_token_ref(mem[p + 1].cint); 
  break; 
      case 7 : 
  {
    flush_node_list(mem[p + 1].hh.v.LH); 
    flush_node_list(mem[p + 1].hh.v.RH); 
  } 
  break; 
      case 5 : 
  flush_node_list(mem[p + 1].cint); 
  break; 
      case 14 : 
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
      case 16 : 
      case 17 : 
      case 18 : 
      case 19 : 
      case 20 : 
      case 21 : 
      case 22 : 
      case 23 : 
      case 24 : 
      case 27 : 
      case 26 : 
      case 29 : 
      case 28 : 
  {
    if(mem[p + 1].hh.v.RH >= 2)
    flush_node_list(mem[p + 1].hh.v.LH); 
    if(mem[p + 2].hh.v.RH >= 2)
    flush_node_list(mem[p + 2].hh.v.LH); 
    if(mem[p + 3].hh.v.RH >= 2)
    flush_node_list(mem[p + 3].hh.v.LH); 
    if(mem[p].hh.b0 == 24)
    free_node(p, 5); 
    else if(mem[p].hh.b0 == 28)
    free_node(p, 5); 
    else free_node(p, 4); 
    goto lab30; 
  } 
  break; 
      case 30 : 
      case 31 : 
  {
    free_node(p, 4); 
    goto lab30; 
  } 
  break; 
      case 25 : 
  {
    flush_node_list(mem[p + 2].hh.v.LH); 
    flush_node_list(mem[p + 3].hh.v.LH); 
    free_node(p, 6); 
    goto lab30; 
  } 
  break; 
    default: 
      {
        confusion(350);   /* flushing */
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
{register halfword Result; 
  halfword h; 
  halfword q; 
  halfword r; 
  char words; 
  h = get_avail (); 
  q = h; 
  while(p != 0){      /* while p<>null do l.3969 */
      
    words = 1; 
    if((p >= hi_mem_min)) 
    r = get_avail (); 
    else switch(mem[p].hh.b0)
    {case 0 : 
    case 1 : 
    case 13 : 
      {
  r = get_node(7); 
  mem[r + 6]= mem[p + 6]; 
  mem[r + 5]= mem[p + 5]; 
  mem[r + 5].hh.v.RH = copy_node_list(mem[p + 5].hh.v.RH); 
  words = 5; 
      } 
      break; 
    case 2 : 
      {
  r = get_node(4); 
  words = 4; 
      } 
      break; 
    case 3 : 
      {
  r = get_node(5); 
  mem[r + 4]= mem[p + 4]; 
  incr(mem[mem[p + 4].hh.v.RH].hh.v.RH); 
  mem[r + 4].hh.v.LH = copy_node_list(mem[p + 4].hh.v.LH); 
  words = 4; 
      } 
      break; 
    case 8 : 
      switch(mem[p].hh.b1)
      {case 0 : 
  {
    r = get_node(3); 
    words = 3; 
  } 
  break; 
      case 1 : 
      case 3 : 
  {
    r = get_node(2); 
    incr(mem[mem[p + 1].hh.v.RH].hh.v.LH); 
    words = 2; 
  } 
  break; 
      case 2 : 
      case 4 : 
  {
    r = get_node(2); 
    words = 2; 
  } 
  break; 
    default: 
      {
        confusion(1288);    /* ext2 */
        return 0;         // abort_flag set
      }
      break; 
      } 
      break; 
    case 10 : 
      {
  r = get_node(2); 
  incr(mem[mem[p + 1].hh.v.LH].hh.v.RH); 
  mem[r + 1].hh.v.LH = mem[p + 1].hh.v.LH; 
  mem[r + 1].hh.v.RH = copy_node_list(mem[p + 1].hh.v.RH); 
      } 
      break; 
    case 11 : 
    case 9 : 
    case 12 : 
      {
  r = get_node(2); 
  words = 2; 
      } 
      break; 
    case 6 : 
      {
  r = get_node(2); 
  mem[r + 1]= mem[p + 1]; 
  mem[r + 1].hh.v.RH = copy_node_list(mem[p + 1].hh.v.RH); 
      } 
      break; 
    case 7 : 
      {
  r = get_node(2); 
  mem[r + 1].hh.v.LH = copy_node_list(mem[p + 1].hh.v.LH); 
  mem[r + 1].hh.v.RH = copy_node_list(mem[p + 1].hh.v.RH); 
      } 
      break; 
    case 4 : 
      {
  r = get_node(2); 
  incr(mem[mem[p + 1].cint].hh.v.LH); 
  words = 2; 
      } 
      break; 
    case 5 : 
      {
  r = get_node(2); 
  mem[r + 1].cint = copy_node_list(mem[p + 1].cint); 
      } 
      break; 
  default: 
    {
      confusion(351);   /* copying */
      return 0;       // abort_flag set
    }
    break; 
    } 
    while(words > 0){
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
  if(m > 0)
  switch(m /(101)) 
  {case 0 : 
    print(352);   /* vertical */
    break; 
  case 1 : 
    print(353);   /* horizontal */
    break; 
  case 2 : 
    print(354);   /* display math */
    break; 
  } 
  else if(m == 0)
  print(355);   /* no */
  else switch((- (integer) m)/(101)) 
  {case 0 : 
    print(356);   /* internal vertical */
    break; 
  case 1 : 
    print(357);   /* restricted horizontal */
    break; 
  case 2 : 
    print(340);   /* math */
    break; 
  } 
  print(358);   /*  mode */
} 
void push_nest (void) 
{ 
  if(nest_ptr > max_nest_stack)
  {
    max_nest_stack = nest_ptr; 
#ifdef ALLOCATEINPUTSTACK
  if(nest_ptr == current_nest_size)
    nest = realloc_nest_stack (increment_nest_size);
  if(nest_ptr == current_nest_size){  /* check again after allocation */
    overflow(359, current_nest_size);
    return;     // abort_flag set
  }
#else
  if(nest_ptr == nest_size){
    overflow(359, nest_size); /* semantic next size - not dynamic */
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
  print_nl("  ");    /*  */
  print_ln (); 
  {
    register integer for_end; 
    p = nest_ptr; 
    for_end = 0; 
    if(p >= for_end) do 
      {
        m = nest[p].mode_field; 
        a = nest[p].aux_field; 
		print_nl("### ");    /* ###  */
        print_mode(m);
        print(361);   /*  entered at line  */
        print_int(abs(nest[p].ml_field)); 
        if(m == 102)
/* ************************************************************************ */
/* major change from 3.141 -> 3.14159 in following */
/*.pg_field instead of .lhmfield and .rhmfield */
/* WAS if((nest[p].lhmfield != 2)||(nest[p].rhmfield != 3)) */
      if(nest[p].pg_field != 8585216L)  /* 830000 hex ??? */
      {
  print(362);   /*  (language */
  print_int(nest[p].pg_field % 65536L);   /* last 16 bits */
/*  print_int(nest[p].pg_field & 65535L);  */
  print(363);   /* :hyphenmin */
  print_int(nest[p].pg_field / 4194304L);   /* 400000 hex ??? */
/*  print_int(nest[p].pg_field >> 22); */ /* top 10 bits */
  print_char(44); /*, */
  print_int((nest[p].pg_field / 65536L)% 64); 
/*  print_int((nest[p].pg_field >> 16)& 63); */ /* next 8 bits */
/*  this used to refer to .lhmfield and .rhmfield ... */
/* ********************************************************************* */
  print_char(41); /*)*/
      } 
      if(nest[p].ml_field < 0)
      print(364);     /*  (\output routine) */
      if(p == 0)
      {
  if(mem_top - 2 != page_tail)
  {
    print_nl("### current page:");      /*  */
    if(output_active)
      print(975);   /*  (held over for next output) */
    show_box(mem[mem_top - 2].hh.v.RH); 
    if(page_contents > 0)
    {
      print_nl("total height ");    /*  */
      print_totals (); 
      print_nl("goal height ");    /*   */
      print_scaled(page_so_far[0]); 
      r = mem[mem_top].hh.v.RH; 
      while(r != mem_top){
    
        print_ln (); 
        print_esc(327); /* insert */
        t = mem[r].hh.b1; 
        print_int(t); 
        print(978);   /*  adds  */
        t = x_over_n(mem[r + 3].cint, 1000)*
        eqtb[(hash_size + 3218) + t].cint; 
        print_scaled(t); 
        if(mem[r].hh.b0 == 1)
        {
    q = mem_top - 2; 
    t = 0; 
    do {
        q = mem[q].hh.v.RH; 
      if((mem[q].hh.b0 == 3)&&(mem[q].hh.b1 == mem 
     [r].hh.b1)) 
      incr(t); 
    } while(!(q == mem[r + 1].hh.v.LH)); 
    print(979);     /*, # */
    print_int(t); 
    print(980);     /*  might split */
        } 
        r = mem[r].hh.v.RH; 
      } 
    } 
  } 
/*  if link(contrib_head)<>null then l.4393 */
  if(mem[mem_top - 1].hh.v.RH != 0)
  print_nl(" (\\output routine)");    /*  */
      } 
      show_box(mem[nest[p].head_field].hh.v.RH); 
      switch(abs(m)/(101)) 
      {case 0 : 
  {
    print_nl("### recent contributions:");      /*  */
    if(a.cint <= ignore_depth)
    print(367);     /* ignored */
    else print_scaled(a.cint); 
    if(nest[p].pg_field != 0)
    {
      print(368);     /*, prevgraf  */
      print_int(nest[p].pg_field); 
      print(369);     /*  line */
      if(nest[p].pg_field != 1)
      print_char(115);    /* s */
    } 
  } 
  break; 
      case 1 : 
  {
    print_nl("spacefactor ");      /*  */
    print_int(a.hh.v.LH); 
    if(m > 0)
    if(a.hh.v.RH > 0)
    {
      print(371);     /*, current language  */
      print_int(a.hh.v.RH); 
    } 
  } 
  break; 
      case 2 : 
  if(a.cint != 0)
  {
    print(372);     /* this will be denominator of: */
    show_box(a.cint); 
  } 
  break; 
      } 
    } 
  while(p-- > for_end); } 
} 
void print_param_(integer n)
{
  switch(n)
  {case 0 : 
    print_esc(417);   /* pretolerance */
    break; 
  case 1 : 
    print_esc(418);   /* tolerance */
    break; 
  case 2 : 
    print_esc(419);   /* linepenalty */
    break; 
  case 3 : 
    print_esc(420);   /* hyphenpenalty */
    break; 
  case 4 : 
    print_esc(421);   /* exhyphenpenalty */
    break; 
  case 5 : 
    print_esc(422);   /* clubpenalty */
    break; 
  case 6 : 
    print_esc(423);   /* widowpenalty */
    break; 
  case 7 : 
    print_esc(424);   /* displaywidowpenalty */
    break; 
  case 8 : 
    print_esc(425);   /* brokenpenalty */
    break; 
  case 9 : 
    print_esc(426);   /* binoppenalty */
    break; 
  case 10 : 
    print_esc(427);   /* relpenalty */
    break; 
  case 11 : 
    print_esc(428);   /* predisplaypenalty */
    break; 
  case 12 : 
    print_esc(429);   /* postdisplaypenalty */
    break; 
  case 13 : 
    print_esc(430);   /* interlinepenalty */
    break; 
  case 14 : 
    print_esc(431);   /* doublehyphendemerits */
    break; 
  case 15 : 
    print_esc(432);   /* finalhyphendemerits */
    break; 
  case 16 : 
    print_esc(433);   /* adjdemerits */
    break; 
  case 17 : 
    print_esc(434);   /* mag */
    break; 
  case 18 : 
    print_esc(435);   /* delimiterfactor */
    break; 
  case 19 : 
    print_esc(436);   /* looseness */
    break; 
  case 20 : 
    print_esc(437);   /* time */
    break; 
  case 21 : 
    print_esc(438);   /* day */
    break; 
  case 22 : 
    print_esc(439);   /* month */
    break; 
  case 23 : 
    print_esc(440);   /* year */
    break; 
  case 24 : 
    print_esc(441);   /* showboxbreadth */
    break; 
  case 25 : 
    print_esc(442);   /* showboxdepth */
    break; 
  case 26 : 
    print_esc(443);   /* hbadness */
    break; 
  case 27 : 
    print_esc(444);   /* vbadness */
    break; 
  case 28 : 
    print_esc(445);   /* pausing */
    break; 
  case 29 : 
    print_esc(446);   /* tracingonline */
    break; 
  case 30 : 
    print_esc(447);   /* tracingmacros */
    break; 
  case 31 : 
    print_esc(448);   /* tracingstats */
    break; 
  case 32 : 
    print_esc(449);   /* tracingparagraphs */
    break; 
  case 33 : 
    print_esc(450);   /* tracingpages */
    break; 
  case 34 : 
    print_esc(451);   /* tracingoutput */
    break; 
  case 35 : 
    print_esc(452);   /* tracinglostchars */
    break; 
  case 36 : 
    print_esc(453);   /* tracingcommands */
    break; 
  case 37 : 
    print_esc(454);   /* tracingrestores */
    break; 
  case 38 : 
    print_esc(455);   /* uchyph */
    break; 
  case 39 : 
    print_esc(456);   /* outputpenalty */
    break; 
  case 40 : 
    print_esc(457);   /* maxdeadcycles */
    break; 
  case 41 : 
    print_esc(458);   /* hangafter */
    break; 
  case 42 : 
    print_esc(459);   /* floatingpenalty */
    break; 
  case 43 : 
    print_esc(460);   /* globaldefs */
    break; 
  case 44 : 
    print_esc(461);   /* fam */
    break; 
  case 45 : 
    print_esc(462);   /* escapechar */
    break; 
  case 46 : 
    print_esc(463);   /* defaulthyphenchar */
    break; 
  case 47 : 
    print_esc(464);   /* defaultskewchar */
    break; 
  case 48 : 
    print_esc(465);   /* endlinechar */
    break; 
  case 49 : 
    print_esc(466);   /* newlinechar */
    break; 
  case 50 : 
    print_esc(467);   /* language */
    break; 
  case 51 : 
    print_esc(468);   /* lefthyphenmin */
    break; 
  case 52 : 
    print_esc(469);   /* righthyphenmin */
    break; 
  case 53 : 
    print_esc(470);   /* holdinginserts */
    break; 
  case 54 : 
    print_esc(471);   /* errorcontextlines */
    break; 
  default: 
    print(472);     /* [unknown integer parameter!] */
    break; 
  } 
} 
void begin_diagnostic (void) 
{ 
    old_setting = selector; 
  if((eqtb[(hash_size + 3192)].cint <= 0)&&(selector == 19)) {
    decr(selector); 
    if(history == 0)history = 1; 
  } 
} 
void end_diagnostic_(bool blankline)
{ 
    print_nl("  ");    /*  */
  if(blankline)print_ln (); 
  selector = old_setting; 
} 
void print_length_param_ (integer n)
{ 
  switch(n)
  {case 0 : 
    print_esc(475);   /* parindent */
    break; 
  case 1 : 
    print_esc(476);   /* mathsurround */
    break; 
  case 2 : 
    print_esc(477);   /* lineskiplimit */
    break; 
  case 3 : 
    print_esc(478);   /* hsize */
    break; 
  case 4 : 
    print_esc(479);   /* vsize */
    break; 
  case 5 : 
    print_esc(480);   /* maxdepth */
    break; 
  case 6 : 
    print_esc(481);   /* splitmaxdepth */
    break; 
  case 7 : 
    print_esc(482);   /* boxmaxdepth */
    break; 
  case 8 : 
    print_esc(483);   /* hfuzz */
    break; 
  case 9 : 
    print_esc(484);   /* vfuzz */
    break; 
  case 10 : 
    print_esc(485);   /* delimitershortfall */
    break; 
  case 11 : 
    print_esc(486);   /* nulldelimiterspace */
    break; 
  case 12 : 
    print_esc(487);   /* scriptspace */
    break; 
  case 13 : 
    print_esc(488);   /* predisplaysize */
    break; 
  case 14 : 
    print_esc(489);   /* displaywidth */
    break; 
  case 15 : 
    print_esc(490);   /* displayindent */
    break; 
  case 16 : 
    print_esc(491);   /* overfullrule */
    break; 
  case 17 : 
    print_esc(492);   /* hangindent */
    break; 
  case 18 : 
    print_esc(493);   /* hoffset */
    break; 
  case 19 : 
    print_esc(494);   /* voffset */
    break; 
  case 20 : 
    print_esc(495);   /* emergencystretch */
    break; 
  default: 
    print(496);     /* [unknown dimen parameter!] */
    break; 
  } 
} 
void print_cmd_chr_ (quarterword cmd, halfword chrcode)
{
  switch(cmd)
  {case 1 : 
    {
      print(554);   /* begin-group character  */
      print(chrcode); 
    } 
    break; 
  case 2 : 
    {
      print(555);   /* end-group character  */
      print(chrcode); 
    } 
    break; 
  case 3 : 
    {
      print(556);   /* math shift character  */
      print(chrcode); 
    } 
    break; 
  case 6 : 
    {
      print(557);   /* macro parameter character  */
      print(chrcode); 
    } 
    break; 
  case 7 : 
    {
      print(558);   /* superscript character  */
      print(chrcode); 
    } 
    break; 
  case 8 : 
    {
      print(559);   /* subscript character  */
      print(chrcode); 
    } 
    break; 
  case 9 : 
    print(560);     /* end of alignment template */
    break; 
  case 10 : 
    {
      print(561);   /* blank space  */
      print(chrcode); 
    } 
    break; 
  case 11 : 
    {
      print(562);   /* the letter  */
      print(chrcode); 
    } 
    break; 
  case 12 : 
    {
      print(563);   /* the character  */
      print(chrcode); 
    } 
    break; 
  case 75 : 
  case 76 : 
/* if chr_code<skip_base then print_skip_param(chr_code-glue_base) */
    if(chrcode < (hash_size + 800))
    print_skip_param(chrcode - (hash_size + 782));  /* lineskip */
/* else if chr_code<mu_skip_base then
    begin print_esc("skip"); print_int(chr_code-skip_base); */
    else if(chrcode < (hash_size + 1056))
    {
      print_esc(392);   /* skip */
      print_int(chrcode - (hash_size + 800)); 
    } 
    else {
/*   else  begin print_esc("muskip"); print_int(chr_code-mu_skip_base); */  
      print_esc(393);   /* muskip */
      print_int(chrcode - (hash_size + 1056)); 
    } 
    break; 
  case 72 : 
    if(chrcode >= (hash_size + 1322))
    {
      print_esc(404);   /* toks */
      print_int(chrcode - (hash_size + 1322)); 
    } 
    else switch(chrcode)
    {
  case (hash_size + 1313) : 
      print_esc(395);   /* output */
      break; 
    case (hash_size + 1314) : 
      print_esc(396);   /* everypar */
      break; 
    case (hash_size + 1315) : 
      print_esc(397);   /* everymath */
      break; 
    case (hash_size + 1316) : 
      print_esc(398);   /* everydisplay */
      break; 
    case (hash_size + 1317) : 
      print_esc(399);   /* everyhbox */
      break; 
    case (hash_size + 1318) : 
      print_esc(400);   /* everyvbox */
      break; 
    case (hash_size + 1319) : 
      print_esc(401);   /* everyjob */
      break; 
    case (hash_size + 1320) : 
      print_esc(402);   /* everycr */
      break; 
  default: 
      print_esc(403);   /* errhelp */
      break; 
    } 
    break; 
  case 73 : 
    if(chrcode < (hash_size + 3218))
    print_param(chrcode - (hash_size + 3163)); 
    else {
      print_esc(473);   /* count */
      print_int(chrcode - (hash_size + 3218)); 
    } 
    break; 
  case 74 : 
    if(chrcode < (hash_size + 3751))
    print_length_param(chrcode - (hash_size + 3730)); 
    else {
  
      print_esc(497);   /* dimen */
      print_int(chrcode - (hash_size + 3751)); 
    } 
    break; 
  case 45 : 
    print_esc(505);   /* accent */
    break; 
  case 90 : 
    print_esc(506);   /* advance */
    break; 
  case 40 : 
    print_esc(507);   /* afterassignment */
    break; 
  case 41 : 
    print_esc(508);   /* aftergroup */
    break; 
  case 77 : 
    print_esc(516);   /* fontdimen */
    break; 
  case 61 : 
    print_esc(509);   /* begingroup */
    break; 
  case 42 : 
    print_esc(528);   /* penalty */
    break; 
  case 16 : 
    print_esc(510);   /* char */
    break; 
  case 107 : 
    print_esc(501);   /* csname */
    break; 
  case 88 : 
    print_esc(515);   /* font */
    break; 
  case 15 : 
    print_esc(511);   /* delimiter */
    break; 
  case 92 : 
    print_esc(512);   /* divide */
    break; 
  case 67 : 
    print_esc(502);   /* endcsname */
    break; 
  case 62 : 
    print_esc(513);   /* endgroup */
    break; 
  case 64 : 
    print_esc(32);    /*   */
    break; 
  case 102 : 
    print_esc(514);   /* expandafter */
    break; 
  case 32 : 
    print_esc(517);   /* halign */
    break; 
  case 36 : 
    print_esc(518);   /* hrule */
    break; 
  case 39 : 
    print_esc(519);   /* ignorespaces */
    break; 
  case 37 : 
    print_esc(327);   /* insert */
    break; 
  case 44 : 
    print_esc(47);    /* / */
    break; 
  case 18 : 
    print_esc(348);   /* mark */
    break; 
  case 46 : 
    print_esc(520);   /* mathaccent */
    break; 
  case 17 : 
    print_esc(521);   /* mathchar */
    break; 
  case 54 : 
    print_esc(522);   /* mathchoice */
    break; 
  case 91 : 
    print_esc(523);   /* multiply */
    break; 
  case 34 : 
    print_esc(524);   /* noalign */
    break; 
  case 65 : 
    print_esc(525);   /* noboundary */
    break; 
  case 103 : 
    print_esc(526);   /* noexpand */
    break; 
  case 55 : 
    print_esc(332);   /* nonscript */
    break; 
  case 63 : 
    print_esc(527);   /* omit */
    break; 
  case 66 : 
    print_esc(530);   /* radical */
    break; 
  case 96 : 
    print_esc(531);   /* read */
    break; 
  case 0 : 
    print_esc(532);   /* relax */
    break; 
  case 98 : 
    print_esc(533);   /* setbox */
    break; 
  case 80 : 
    print_esc(529);   /* prevgraf */
    break; 
  case 84 : 
    print_esc(405);   /* parshape */
    break; 
  case 109 : 
    print_esc(534);   /* the */
    break; 
  case 71 : 
    print_esc(404);   /* toks */
    break; 
  case 38 : 
   print_esc(349);    /* vadjust */
    break; 
  case 33 : 
    print_esc(535);   /* valign */
    break; 
  case 56 : 
    print_esc(536);   /* vcenter */
    break; 
  case 35 : 
    print_esc(537);   /* vrule */
    break; 
  case 13 : 
    print_esc(594);   /* par */
    break; 
  case 104 : 
    if(chrcode == 0)
    print_esc(626);   /* input */
    else print_esc(627);    /* endinput */
    break; 
  case 110 : 
    switch(chrcode)
    {case 1 : 
      print_esc(629);   /* firstmark */
      break; 
    case 2 : 
      print_esc(630);   /* botmark */
      break; 
    case 3 : 
      print_esc(631);   /* splitfirstmark */
      break; 
    case 4 : 
      print_esc(632);   /* splitbotmark */
      break; 
  default: 
      print_esc(628);   /* topmark */
      break; 
    } 
    break; 
  case 89 : 
    if(chrcode == 0)
    print_esc(473);   /* count */
    else if(chrcode == 1)
    print_esc(497);   /* dimen */
    else if(chrcode == 2)
    print_esc(392);   /* skip */
    else print_esc(393);    /* muskip */
    break; 
  case 79 : 
    if(chrcode == 1)
    print_esc(666);   /* prevdepth */
    else print_esc(665);    /* spacefactor */
    break; 
  case 82 : 
    if(chrcode == 0)
    print_esc(667);   /* dead_cycles */
    else print_esc(668);    /* insert_penalties */
    break; 
  case 83 : 
    if(chrcode == 1)
    print_esc(669);   /* wd */
    else if(chrcode == 3)
    print_esc(670);   /* ht */
    else print_esc(671);    /* dp */
    break; 
  case 70 : 
    switch(chrcode)
    {case 0 : 
      print_esc(672);   /* last_penalty */
      break; 
    case 1 : 
      print_esc(673);   /* last_kern */
      break; 
    case 2 : 
      print_esc(674);   /* lastskip */
      break; 
    case 3 : 
      print_esc(675);   /* inputlineno */
      break; 
  default: 
      print_esc(676);   /* badness */
      break; 
    } 
    break; 
  case 108 : 
    switch(chrcode)
    {case 0 : 
      print_esc(732);   /* number */
      break; 
    case 1 : 
      print_esc(733);   /* romannumeral */
      break; 
    case 2 : 
      print_esc(734);   /* string */
      break; 
    case 3 : 
      print_esc(735);   /* meaning */
      break; 
    case 4 : 
      print_esc(736);   /* font_name */
      break; 
    default: 
      print_esc(737);   /* job_name */
      break; 
    } 
    break; 
  case 105 : 
    switch(chrcode)
    {case 1 : 
      print_esc(754);   /* ifcat */
      break; 
    case 2 : 
      print_esc(755);   /* ifnum */
      break; 
    case 3 : 
      print_esc(756);   /* ifdim */
      break; 
    case 4 : 
      print_esc(757);   /* ifodd */
      break; 
    case 5 : 
      print_esc(758);   /* ifvmode */
      break; 
    case 6 : 
      print_esc(759);   /* ifhmode */
      break; 
    case 7 : 
      print_esc(760);   /* ifmmode */
      break; 
    case 8 : 
      print_esc(761);   /* ifinner */
      break; 
    case 9 : 
      print_esc(762);   /* ifvoid */
      break; 
    case 10 : 
      print_esc(763);   /* ifhbox */
      break; 
    case 11 : 
      print_esc(764);   /* ifvbox */
      break; 
    case 12 : 
      print_esc(765);   /* ifx */
      break; 
    case 13 : 
      print_esc(766);   /* ifeof */
      break; 
    case 14 : 
      print_esc(767);   /* iftrue */
      break; 
    case 15 : 
      print_esc(768);   /* iffalse */
      break; 
    case 16 : 
      print_esc(769);   /* ifcase */
      break; 
  default: 
      print_esc(753);   /* if */
      break; 
    } 
    break; 
  case 106 : 
    if(chrcode == 2)
    print_esc(770);   /* fi */
    else if(chrcode == 4)
    print_esc(771);   /* or */
    else print_esc(772);    /* else */
    break; 
  case 4 : 
    if(chrcode == 256)  /* pool size */ /* max_quarterword + 1 ? */
    print_esc(892);   /* span */
    else {
      print(896);   /* alignment tab character  */
      print(chrcode); 
    } 
    break; 
  case 5 : 
    if(chrcode == 257)    /* cr_code */
    print_esc(893);   /* cr */
    else print_esc(894);    /* crcr */
    break; 
  case 81 : 
    switch(chrcode)
    {case 0 : 
      print_esc(964);   /* pagegoal */
      break; 
    case 1 : 
      print_esc(965);   /* pagetotal */
      break; 
    case 2 : 
      print_esc(966);   /* pagestretch */
      break; 
    case 3 : 
      print_esc(967);   /* pagefilstretch */
      break; 
    case 4 : 
      print_esc(968);   /* pagefillstretch */
      break; 
    case 5 : 
      print_esc(969);   /* pagefilllstretch */
      break; 
    case 6 : 
      print_esc(970);   /* pageshrink */
      break; 
  default: 
      print_esc(971);   /* pagedepth */
      break; 
    } 
    break; 
  case 14 : 
    if(chrcode == 1)
    print_esc(1020);      /* dump */
    else print_esc(1019);   /* end */
    break; 
  case 26 : 
    switch(chrcode)
    {case 4 : 
      print_esc(1021);    /* hskip */
      break; 
    case 0 : 
      print_esc(1022);    /* hfil */
      break; 
    case 1 : 
      print_esc(1023);    /* hfill */
      break; 
    case 2 : 
      print_esc(1024);    /* hss */
      break; 
  default: 
      print_esc(1025);    /* hfilneg */
      break; 
    } 
    break; 
  case 27 : 
    switch(chrcode)
    {case 4 : 
      print_esc(1026);    /* vskip */
      break; 
    case 0 : 
      print_esc(1027);    /* vfil */
      break; 
    case 1 : 
      print_esc(1028);    /* vfill */
      break; 
    case 2 : 
      print_esc(1029);    /* vss */
      break; 
  default: 
      print_esc(1030);    /* vfilneg */
      break; 
    } 
    break; 
  case 28 : 
    print_esc(333);     /* mskip */
    break; 
  case 29 : 
    print_esc(337);     /* kern */
    break; 
  case 30 : 
    print_esc(339);     /* mkern */
    break; 
  case 21 : 
    if(chrcode == 1)
    print_esc(1048);      /* moveleft */
    else print_esc(1049);   /* moveright */
    break; 
  case 22 : 
    if(chrcode == 1)
    print_esc(1050);      /* raise */
    else print_esc(1051);   /* lower */
    break; 
  case 20 : 
    switch(chrcode)
    {case 0 : 
      print_esc(406);   /* box */
      break; 
    case 1 : 
      print_esc(1052);    /* copy */
      break; 
    case 2 : 
      print_esc(1053);    /* lastbox */
      break; 
    case 3 : 
      print_esc(959);   /* vsplit */
      break; 
    case 4 : 
      print_esc(1054);    /* vtop */
      break; 
    case 5 : 
      print_esc(961);   /* vbox */
      break; 
  default: 
      print_esc(1055);    /* hbox */
      break; 
    } 
    break; 
  case 31 : 
    if(chrcode == 100)
    print_esc(1057);      /* leaders */
    else if(chrcode == 101)
    print_esc(1058);      /* cleaders */
    else if(chrcode == 102)
    print_esc(1059);      /* xleaders */
    else print_esc(1056);   /* ship_out */
    break; 
  case 43 : 
    if(chrcode == 0)
    print_esc(1075);      /* noindent */
    else print_esc(1074);   /* indent */
    break; 
  case 25 : 
    if(chrcode == 10)
    print_esc(1086);      /* unskip */
    else if(chrcode == 11)
    print_esc(1085);      /* unkern */
    else print_esc(1084);   /* unpenalty */
    break; 
  case 23 : 
    if(chrcode == 1)
    print_esc(1088);      /* unhcopy */
    else print_esc(1087);   /* unhbox */
    break; 
  case 24 : 
    if(chrcode == 1)
    print_esc(1090);      /* unvcopy */
    else print_esc(1089);   /* unvbox */
    break; 
  case 47 : 
    if(chrcode == 1)
    print_esc(45);      /* - */
    else print_esc(346);      /* discretionary */
    break; 
  case 48 : 
    if(chrcode == 1)
    print_esc(1122);      /* leqno */
    else print_esc(1121);   /* eqno */
    break; 
  case 50 : 
    switch(chrcode)
    {case 16 : 
      print_esc(860);   /* mathord */
      break; 
    case 17 : 
      print_esc(861);   /* mathop */
      break; 
    case 18 : 
      print_esc(862);   /* mathbin */
      break; 
    case 19 : 
      print_esc(863);   /* mathrel */
      break; 
    case 20 : 
      print_esc(864);   /* mathopen */
      break; 
    case 21 : 
      print_esc(865);   /* mathclose */
      break; 
    case 22 : 
      print_esc(866);   /* mathpunct */
      break; 
    case 23 : 
      print_esc(867);   /* mathinner */
      break; 
    case 26 : 
      print_esc(869);   /* underline */
      break; 
  default: 
      print_esc(868);   /* overline */
      break; 
    } 
    break; 
  case 51 : 
    if(chrcode == 1)
    print_esc(872);   /* limits */
    else if(chrcode == 2)
    print_esc(873);   /* nolimits */
    else print_esc(1123); /* displaylimits */
    break; 
  case 53 : 
    print_style(chrcode); 
    break; 
  case 52 : 
    switch(chrcode)
    {case 1 : 
      print_esc(1142);    /* over */
      break; 
    case 2 : 
      print_esc(1143);    /* atop */
      break; 
    case 3 : 
      print_esc(1144);    /* abovewithdelims */
      break; 
    case 4 : 
      print_esc(1145);    /* overwithdelims */
      break; 
    case 5 : 
      print_esc(1146);    /* atopwithdelims */
      break; 
  default: 
      print_esc(1141);    /* above */
      break; 
    } 
    break; 
  case 49 : 
    if(chrcode == 30)
    print_esc(870);   /* left */
    else print_esc(871);    /* right */
    break; 
  case 93 : 
    if(chrcode == 1)
    print_esc(1165);      /* long */
  else if(chrcode == 2)
    print_esc(1166);      /* outer */
    else print_esc(1167);   /* global */
    break; 
  case 97 : 
    if(chrcode == 0)
    print_esc(1168);      /* def */
    else if(chrcode == 1)
    print_esc(1169);      /* gdef */
    else if(chrcode == 2)
    print_esc(1170);      /* edef */
    else print_esc(1171);   /* xdef */
    break; 
  case 94 : 
    if(chrcode != 0)
    print_esc(1186);      /* futurelet */
    else print_esc(1185);   /* let */
    break; 
  case 95 : 
    switch(chrcode)
    {case 0 : 
      print_esc(1187);    /* chardef */
      break; 
    case 1 : 
      print_esc(1188);    /* mathchardef */
      break; 
    case 2 : 
      print_esc(1189);    /* countdef */
      break; 
    case 3 : 
      print_esc(1190);    /* dimendef */
      break; 
    case 4 : 
      print_esc(1191);    /* skipdef */
      break; 
    case 5 : 
      print_esc(1192);    /* muskipdef */
      break; 
  default: 
      print_esc(1193);    /* toksdef */
      break; 
    } 
    break; 
  case 68 : 
    {
      print_esc(510);   /* char */
      print_hex(chrcode); 
    } 
    break; 
  case 69 : 
    {
      print_esc(521);   /* mathchar */
      print_hex(chrcode); 
    } 
    break; 
  case 85 : 
    if(chrcode == (hash_size + 1883))
    print_esc(412);   /* catcode */
    else if(chrcode == (hash_size + 2907))
    print_esc(416);   /* mathcode */
    else if(chrcode == (hash_size + 2139))
    print_esc(413);   /* lccode */
    else if(chrcode == (hash_size + 2395))
    print_esc(414);   /* uccode */
    else if(chrcode == (hash_size + 2651))
    print_esc(415);   /* sfcode */
    else print_esc(474);    /* delcode */
    break; 
  case 86 : 
    print_size(chrcode - (hash_size + 1835)); /* chr - math_font_base */
    break; 
  case 99 : 
    if(chrcode == 1)
    print_esc(947);   /* patterns */
    else print_esc(935);    /* hyphenation */
    break; 
  case 78 : 
    if(chrcode == 0)
    print_esc(1211);    /* hyphen_char */
    else print_esc(1212); /* skew_char */
    break; 
  case 87 : 
    {
      print(1220);    /* select font  */
      slow_print(font_name[chrcode]); 
      if(font_size[chrcode]!= font_dsize[chrcode])
      {
      print(738);     /*  at  */
      print_scaled(font_size[chrcode]); 
      print(394);     /* pt */
      } 
    } 
    break; 
  case 100 : 
    switch(chrcode)
    {case 0 : 
      print_esc(272); /* batchmode */
      break; 
    case 1 : 
      print_esc(273); /* nonstopmode */
      break; 
    case 2 : 
      print_esc(274); /* scrollmode */
      break; 
  default: 
      print_esc(1221);  /* errorstopmode  */
      break; 
    } 
    break; 
  case 60 : 
    if(chrcode == 0)
    print_esc(1223);      /* closein */
    else print_esc(1222);   /* openin */
    break; 
  case 58 : 
    if(chrcode == 0)
    print_esc(1224);      /* message */
    else print_esc(1225);   /* errmessage */
    break; 
  case 57 : 
    if(chrcode == (hash_size + 2139))
    print_esc(1231);      /* lowercase */
    else print_esc(1232);   /* uppercase */
    break; 
  case 19 : 
    switch(chrcode)
    {case 1 : 
      print_esc(1234);    /* show_box */
      break; 
    case 2 : 
      print_esc(1235);    /* showthe */
      break; 
    case 3 : 
      print_esc(1236);    /* showlists */
      break; 
  default: 
      print_esc(1233);    /* show */
      break; 
    } 
    break; 
  case 101 : 
    print(1243);      /* undefined */
  break;
  case 111 : 
    print(1244);      /* macro */
    break; 
  case 112 : 
    print_esc(1245);      /* long macro */
    break; 
  case 113 : 
    print_esc(1246);      /* outer macro */
    break; 
  case 114 : 
    {
      print_esc(1165);    /* long */
      print_esc(1246);    /* outer macro */
    } 
    break; 
  case 115 : 
    print_esc(1247);      /* outer endtemplate */
    break; 
  case 59 : 
    switch(chrcode)
    {case 0 : 
      print_esc(1279);    /* openout */
      break; 
    case 1 : 
      print_esc(591);   /* write */
      break; 
    case 2 : 
      print_esc(1280);    /* closeout */
      break; 
    case 3 : 
      print_esc(1281);    /* special */
      break; 
    case 4 : 
      print_esc(1282);    /* immediate */
      break; 
    case 5 : 
      print_esc(1283);    /* setlanguage */
      break; 
    default: 
      print(1284);      /* [unknown extension!] */
      break; 
    } 
    break; 
  default: 
    print(564);     /* [unknown command code!] */
    break; 
  } 
} 

#ifdef STAT
void show_eqtb_(halfword n)
{ 
  if(n < 1)
  print_char(63);       /* ? */
  else if(n < (hash_size + 782))    /* lineskip */
  {
    sprint_cs(n); 
    print_char(61);       /* = */
    print_cmd_chr(eqtb[n].hh.b0, eqtb[n].hh.v.RH); 
    if(eqtb[n].hh.b0 >= 111)
    {
      print_char(58);     /* : */
      show_token_list(mem[eqtb[n].hh.v.RH].hh.v.RH, 0, 32); 
    } 
  } 
  else if(n < (hash_size + 1312))
  if(n < (hash_size + 800))
  {
    print_skip_param(n - (hash_size + 782));  /* lineskip */
    print_char(61);       /* = */
    if(n < (hash_size + 797))
    print_spec(eqtb[n].hh.v.RH, 394); /* pt */
    else print_spec(eqtb[n].hh.v.RH, 334);  /* mu */
  } 
  else if(n < (hash_size + 1056))
  {
    print_esc(392);   /* skip */
    print_int(n - (hash_size + 800)); 
    print_char(61);   /* = */
    print_spec(eqtb[n].hh.v.RH, 394);   /* pt */
  } 
  else {
    print_esc(393);   /* muskip */
    print_int(n - (hash_size + 1056)); 
    print_char(61);   /* = */
    print_spec(eqtb[n].hh.v.RH, 334);   /* mu */
  } 
  else if(n < (hash_size + 3163))
  if(n == (hash_size + 1312))
  {
    print_esc(405);   /* parshape */
    print_char(61);   /* = */
    if(eqtb[(hash_size + 1312)].hh.v.RH == 0)
    print_char(48);     /* 0 */
    else print_int(mem[eqtb[(hash_size + 1312)].hh.v.RH].hh.v.LH); 
  } 
  else if(n < (hash_size + 1322))
  {
    print_cmd_chr(72, n); /* H */
    print_char(61);    /* = */
    if(eqtb[n].hh.v.RH != 0)
    show_token_list(mem[eqtb[n].hh.v.RH].hh.v.RH, 0, 32); 
  } 
  else if(n < (hash_size + 1578))
  {
    print_esc(404);   /* toks */
    print_int(n - (hash_size + 1322)); 
    print_char(61);     /* = */
    if(eqtb[n].hh.v.RH != 0)
    show_token_list(mem[eqtb[n].hh.v.RH].hh.v.RH, 0, 32); 
  } 
  else if(n < (hash_size + 1834))
  {
    print_esc(406);   /* box */
    print_int(n - (hash_size + 1578)); 
    print_char(61);   /* = */
    if(eqtb[n].hh.v.RH == 0)
    print(407);     /* void */
    else {
  
      depth_threshold = 0; 
      breadth_max = 1; 
      show_node_list(eqtb[n].hh.v.RH); 
    } 
  } 
  else if(n < (hash_size + 1883))   /* cat_code_base ... */
  {
    if(n == (hash_size + 1834))
    print(408);     /* current font */
    else if(n < (hash_size + 1851))
    {
      print_esc(409);   /* textfont */
      print_int(n - (hash_size + 1835)); 
    } 
    else if(n < (hash_size + 1867))
    {
      print_esc(410); /* scriptfont */
      print_int(n - (hash_size + 1851)); 
    } 
    else {
      print_esc(411); /* scriptscriptfont */
      print_int(n - (hash_size + 1867)); 
    } 
    print_char(61);   /* = */
/*    print_esc(hash[(hash_size + 524) + eqtb[n].hh.v.RH].v.RH); */
    print_esc(hash[(hash_size + hash_extra + 524) + eqtb[n].hh.v.RH].v.RH); 
                /* 96/Jan/10 */
  } 
  else if(n < (hash_size + 2907))
  {
    if(n < (hash_size + 2139))
    {
      print_esc(412);   /* catcode */
      print_int(n - (hash_size + 1883)); 
    } 
    else if(n < (hash_size + 2395))
    {
      print_esc(413);   /* lccode */
      print_int(n - (hash_size + 2139)); 
    } 
    else if(n < (hash_size + 2651))
    {
      print_esc(414);   /* uccode */
      print_int(n - (hash_size + 2395)); 
    } 
    else {
  
      print_esc(415);   /* sfcode */
      print_int(n - (hash_size + 2651)); 
    } 
    print_char(61);     /* = */
    print_int(eqtb[n].hh.v.RH); 
  } 
  else {
      
    print_esc(416);   /* mathcode */
    print_int(n - (hash_size + 2907)); 
    print_char(61);     /* = */
    print_int(eqtb[n].hh.v.RH); 
  } 
  else if(n < (hash_size + 3730))
  {
    if(n < (hash_size + 3218))
    print_param(n - (hash_size + 3163)); 
    else if(n < (hash_size + 3474))
    {
      print_esc(473);   /* count */
      print_int(n - (hash_size + 3218)); 
    } 
    else {
  
      print_esc(474);   /* delcode */
      print_int(n - (hash_size + 3474)); 
    } 
    print_char(61);     /* = */
    print_int(eqtb[n].cint); 
  } 
  else if(n <= (hash_size + 4006))
  {
    if(n < (hash_size + 3751))
    print_length_param(n - (hash_size + 3730)); 
    else {
  
      print_esc(497);   /* dimen */
      print_int(n - (hash_size + 3751)); 
    } 
    print_char(61);     /* = */
    print_scaled(eqtb[n].cint); 
    print(394);       /* pt */
  } 
  else print_char(63);    /* = */
} 
#endif /* STAT */

halfword id_lookup_(integer j, integer l)
{/* 40 */ register halfword Result; 
    integer h; 
  integer d; 
  halfword p; 
  halfword k; 
  h = buffer[j]; 
  {
    register integer for_end; 
    k = j + 1; 
    for_end = j + l - 1; 
    if(k <= for_end) do {
      h = h + h + buffer[k]; 
      while(h >= hash_prime)h = h - hash_prime; /* buffer size hash prime */
    } 
    while(k++ < for_end);
  } 
  p = h + 514;            /* h + hash_base */
  while(true){
    if(hash[p].v.RH > 0)
      if((str_start[hash[p].v.RH + 1]- str_start[hash[p].v.RH])
         == l)
        if(str_eq_buf(hash[p].v.RH, j)) 
          goto lab40; 
    if(hash[p].v.LH == 0){
      if(no_new_control_sequence)
        p = (hash_size + 781);    /* undefine_control_sequence */
      else {
        if(hash[p].v.RH > 0){
          do {
            if((hash_used == 514)) {  /* if hash_used = hashbase ... */ 
/*        we can't expand the hash table ... */
/*        overflow(500, hash_size); */ /* hash size - NOT DYNAMIC */
              overflow(500, hash_size + hash_extra); /* 96/Jan/10 */
              return 0;     // abort_flag set
            }
            decr(hash_used); 
          } while(!(hash[hash_used].v.RH == 0)); 
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
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
    if(pool_ptr + l > current_pool_size)
/*        str_pool = realloc_str_pool (increment_pool_size); */
      str_pool = realloc_str_pool (increment_pool_size + 1);
    if(pool_ptr + l > current_pool_size){ /* in case it failed 97/Mar/7 */
      overflow(257, current_pool_size - init_pool_ptr); /* pool size */
      return 0;     // abort_flag set
    }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if(pool_ptr + l > pool_size){
      overflow(257, pool_size - init_pool_ptr); /* pool size - not dynamic */
      return;     // abort_flag set
    }
#endif
        } 
        d =(pool_ptr - str_start[str_ptr]); 
        while(pool_ptr > str_start[str_ptr]){
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
          if(k <= for_end) do {
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
   if(save_ptr > max_save_stack)        /* check_full_save_stack; p.274 */
  {
    max_save_stack = save_ptr; 
#ifdef ALLOCATESAVESTACK
  if(max_save_stack > current_save_size - 6)
    save_stack = realloc_save_stack (increment_save_size);
  if(max_save_stack > current_save_size - 6){ /* check again after allocation */
    overflow(538, current_save_size);
    return;     // abort_flag set
  }
#else
  if(max_save_stack > save_size - 6){ /* save size - not dynamic */
    overflow(538, save_size);
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
/*  if(cur_level == 255)*/      /* 94/Apr/4 */
  if(cur_level == max_quarterword)  {
/* { quit if (cur_level + 1) is too large to store in eqtb } */
/*  overflow("grouping levels", max_quarterword - min_quarterword); */
/*  overflow(539, 255); */      /* grouping levels - not dynamic */
    overflow(539, max_quarterword - min_quarterword); /* 96/Oct/12 ??? */
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
  switch(w .hh.b0)
  {case 111 : 
  case 112 : 
  case 113 : 
  case 114 : 
    delete_token_ref(w.hh.v.RH); 
    break; 
  case 117 : 
    delete_glue_ref(w.hh.v.RH); 
    break; 
  case 118 : 
    {
      q = w.hh.v.RH; 
      if(q != 0)    /* if q<>null then free_node(... l.5937 */
      free_node(q, mem[q].hh.v.LH + mem[q].hh.v.LH + 1); 
    } 
    break; 
  case 119 : 
    flush_node_list(w.hh.v.RH); 
    break; 
  default: 
 ; 
    break; 
  } 
} 
void eq_save_(halfword p, quarterword l)
{ 
  if(save_ptr > max_save_stack)
  {
    max_save_stack = save_ptr; 
#ifdef ALLOCATESAVESTACK
  if(max_save_stack > current_save_size - 6)
    save_stack = realloc_save_stack (increment_save_size);
  if(max_save_stack > current_save_size - 6){ /* check again after allocation */
    overflow(538, current_save_size);
    return;     // abort_flag set
  }
#else
  if(max_save_stack > save_size - 6){ /* save size not dynamic */
    overflow(538, save_size);
    return;     // abort_flag set
  }
#endif
  } 
  if(l == 0)
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
  if(eqtb[p].hh.b1 == cur_level)
  eq_destroy(eqtb[p]); 
  else if(cur_level > 1)
  eq_save(p, eqtb[p].hh.b1); 
/*  eqtb[p].hh.b1 = cur_level;  */
  eqtb[p].hh.b1 = (quarterword) cur_level; /* because cur_level padded out */
  eqtb[p].hh.b0 = t; 
  eqtb[p].hh.v.RH = e; 
} 
void eq_word_define_(halfword p, integer w)
{
  if(xeq_level[p]!= cur_level)
  {
    eq_save(p, xeq_level[p]); 
/*    xeq_level[p]= cur_level;  */
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
  if(cur_level > 1)
  {
    if(save_ptr > max_save_stack)
    {
      max_save_stack = save_ptr; 
#ifdef ALLOCATESAVESTACK
    if(max_save_stack > current_save_size - 6)
      save_stack = realloc_save_stack (increment_save_size);
    if(max_save_stack > current_save_size - 6){ /* check again after allocation */
      overflow(538, current_save_size);
      return;     // abort_flag set
    }
#else
    if(max_save_stack > save_size - 6){   /* save satck - not dynamic */
      overflow(538, save_size);
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
