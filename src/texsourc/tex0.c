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

#ifdef IGNORED
static void winerror (char * message)
{
  (void) MessageBox(NULL, message, "YandYTeX.DLL", MB_ICONSTOP | MB_OK);
}
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void append_char (ASCII_code c)
{
  str_pool[pool_ptr] = c;
  incr(pool_ptr);
}
void print_err (const char * s)
{
  if (interaction == error_stop_mode);
    print_nl("! ");
  print_string(s);
}

void tex_help (unsigned int n, ...)
{
  unsigned int i;
  va_list help_arg;

  if (n > 6) n = 6;
  help_ptr = n;
  va_start(help_arg, n);
  for (i = n - 1; i > n - 1; --i)
    help_line[i] = va_arg(help_arg, char *);
  va_end(help_arg);
}
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// print newline
/* sec 0058 */
void print_ln (void)
{
  switch (selector) {
    case term_and_log:
      {
        show_char('\n');
        term_offset = 0;
        (void) putc ('\n', log_file);
        file_offset = 0;
      }
      break;
    case log_only:
      {
        (void) putc ('\n',  log_file);
        file_offset = 0;
      }
      break;
    case term_only:
      {
        show_char('\n');
        term_offset = 0;
      }
      break;
    case no_print:
    case pseudo:
    case new_string:
      ;
      break;
    default:
      (void) putc ('\n', write_file[selector]);
      break;
  }
}
/* sec 0059 */
void print_char_ (ASCII_code s)
{
  if(s == eqtb[(hash_size + 3212)].cint)
    if (selector < pseudo) {
      print_ln ();
      return;
    }
  switch (selector) {
    case term_and_log:
      {
        (void) show_char(Xchr(s));
        incr(term_offset);
        (void) putc(Xchr(s), log_file);
        incr(file_offset);
        if (term_offset == max_print_line) {
          show_char('\n');
          term_offset = 0;
        }
        if (file_offset == max_print_line) {
          (void) putc ('\n', log_file);
          file_offset = 0;
        }
      }
      break;
    case log_only:
      {
        (void) putc(Xchr(s), log_file);
        incr(file_offset);
        if (file_offset == max_print_line) print_ln ();
      }
      break;
    case term_only:
      {
        (void) show_char(Xchr(s));
        incr(term_offset); 
        if (term_offset == max_print_line) print_ln ();
      }
      break;
    case no_print:
      ;
      break;
    case pseudo:
      if(tally < trick_count)
        trick_buf[tally % error_line]= s;
      break;
    case new_string:
      {
#ifdef ALLOCATESTRING
        if (pool_ptr + 1 > current_pool_size) {
          str_pool = realloc_str_pool (increment_pool_size);
        }
        if (pool_ptr < current_pool_size) {
          str_pool[pool_ptr]= s;
          incr(pool_ptr);
        }
#else
        if (pool_ptr < pool_size) {
          str_pool[pool_ptr]= s;
          incr(pool_ptr);
        }
#endif
      }
      break; 
    default: 
      (void) putc(Xchr(s), write_file[selector]);
      break; 
  }
  incr(tally);
}
/* sec 0059 */
/* This could be made more efficient using fputs ? ... bkph */
void print_ (integer s)
{
  pool_pointer j;
  integer nl;
  if (s >= str_ptr) s = 259; /* ??? */
  else if (s < 256)
    if (s < 0) s = 259; /* ??? */
    else {
      if (selector > pseudo){
        print_char(s);
        return;
      }
      if ((s == eqtb[(hash_size + 3212)].cint))
        if (selector < 20) {
          print_ln ();
          return;
        }
      nl = eqtb[(hash_size + 3212)].cint; /* save eol */
      eqtb[(hash_size + 3212)].cint = -1; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*    if (!show_in_hex && s < 256)  */      /* show control chars also ?? */
      if (!show_in_hex && s < 256 && s >= 32) {      /* 94/Jan/26 */
/*        following added 1996/Jan/20 */
        if (show_in_dos && s > 127) {     /* translate ANSI to DOS 850 */
          if (wintodos[s-128] > 0) print_char (wintodos[s-128]);
          else {              /* print in hex after all */
            j = str_start[s]; 
            while(j < str_start[s + 1]){
              print_char(str_pool[j]); 
              incr(j); 
            } 
          }
        } else print_char(s);       /* don't translate to hex */
      } else {                  /* not just a character */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
        j = str_start[s]; 
        while(j < str_start[s + 1]){
          print_char(str_pool[j]); 
          incr(j); 
        } 
      }
      eqtb[(hash_size + 3212)].cint = nl; /* restore eol */
      return; 
    }
/*  we get here with s > 256 - i.e. not a single character */
  j = str_start[s]; 
  while(j < str_start[s + 1]){
    print_char(str_pool[j]); 
    incr(j); 
  }
} 
void print_string_ (unsigned char *s)
{
  while (*s > 0) print_char(*s++);
}
/* sec 0060 */
// print string number s from string pool by calling print_
void slow_print_ (integer s)
{
  pool_pointer j;
  if ((s >= str_ptr) || (s < 256)) print(s);
  else {
    j = str_start[s];
    while (j < str_start[s + 1]) {
//    if (str_pool[j]>= 128) print(str_pool[j]-128); // debugging only
//    if (str_pool[j]== 0) print(36); // debugging only
      print(str_pool[j]);
      incr(j);
    }
  }
}
/* sec 0062 */
// print newline followed by string number s (unless at start of line)
void print_nl_ (char * s)
{
  if (((term_offset > 0) && (odd(selector))) ||
      ((file_offset > 0) && (selector >= log_only)))
    print_ln ();
  print_string(s);
}
/* sec 0063 */
// print string number s preceded by escape character
void print_esc_ (char * s)
{
  integer c;
  c = eqtb[(hash_size + 3208)].cint;
  if(c >= 0)
    if(c < 256) print(c);
  //slow_print(s);
  print_string(s);
} 
/* sec 0064 */
void print_the_digs_ (eight_bits k)
{
  while (k > 0) {
    decr(k);
    if(dig[k]< 10)
      print_char('0' + dig[k]);
    else
      print_char('A' + dig[k]);
  }
}
/* sec 0065 */
void print_int_ (integer n)
{
  char k;
  integer m;
  k = 0;
  if (n < 0) {
    print_char('-');
    if(n > -100000000L)
      n = - (integer) n;
    else {
      m = -1 - n;
      n = m / 10;
      m =(m % 10)+ 1;
      k = 1;
      if(m < 10)
        dig[0]= (char) m;
      else {
        dig[0]= 0;
        incr(n);
      }
    }
  }
  do {
    dig[k]= (char) (n % 10);
    n = n / 10;
    incr(k);
  } while(!(n == 0));
  print_the_digs(k);
}
/* sec 0262 */
void print_cs_ (integer p)
{
  if (p < 514)       /* if p < hash_base then ... p.262 */
  if (p >= 257)        /* if p > single_base then ... p.262 */
  if (p == 513)        /* if p = null_cs then ... p.262 */
  {
    print_esc("csname");
    print_esc("endcsname");
    print_char(' ');
  } else {
    //print_esc(p - 257);   /* p - single_base */
    print_esc("");print(p - 257);
/*  if cat_code(p - single_base) = letter then ... p.262 */
    if(eqtb[(hash_size + 1883) + p - 257].hh.v.RH == 11)
    print_char(' ');
  } 
  else if (p < 1)
    print_esc("IMPOSSIBLE.");
  else print(p - 1); 
  else if (p >= (hash_size + 781)) /* undefined_control_sequence */
    print_esc("IMPOSSIBLE.");
  else if ((hash[p].v.RH >= str_ptr)) 
    print_esc("NONEXISTENT.");
  else {
    //print_esc(hash[p].v.RH); 
    print_esc(""); print(hash[p].v.RH);
    print_char(32);     /*    */
  }
}
/* sec 0263 */
void sprint_cs_(halfword p)
{ 
  if(p < 514)       /* if p < hash_base then ... p.263 */
  if(p < 257)       /* if p < single_base then ... p.263 */
    print(p - 1);     /* print (p - active_base); */
  else if(p < 513)      /* else if p < null_cs then ... */
    //print_esc(p - 257); /* print (p - single_base); */
  {print_esc(""); print(p-257);}
  else {
    print_esc("csname");
    print_esc("endcsname");
  } 
  else //print_esc(hash[p].v.RH); 
  {
    print_esc(""); print(hash[p].v.RH);
  }
} 
/* sec 0518 */
/* ! I can't find file `  c:/foo/  accents  .tex  '. */
void print_file_name_(integer n, integer a, integer e)
{ 
/*  sprintf(log_line, "\na %d n %d e %d\n", a, n, e); */
/*  show_line(log_line, 0); */
//  print_char(33);  // debugging only
  slow_print(a); 
//  print_char(33);  // debugging only
  slow_print(n); 
//  print_char(33);  // debugging only
  slow_print(e); 
//  print_char(33);  // debugging only
}
/* sec 0699 */
void print_size_(integer s) 
{ 
  if (s == 0)
    print_esc("textfont");
  else if (s == 16)
    print_esc("scriptfont");
  else
    print_esc("scriptscriptfont");
} 
/* sec 1355 */
void print_write_whatsit_(str_number s, halfword p)
{
  //print_esc(s); 
  print_esc(""); print(s);
  if (mem[p + 1].hh.v.LH < 16)
    print_int(mem[p + 1].hh.v.LH); 
  else if (mem[p + 1].hh.v.LH == 16)
    print_char('*');
  else print_char('-');
} 
#ifdef DEBUG
#endif /* DEBUG */
/* sec 0081 */
// called from itex.c and tex0.c only  NASTY NASTY!
// now uses uses non-local goto (longjmp) 1999/Nov/7
void jump_out (void) 
{
  close_files_and_terminate (); 
  {
    int code;
#ifndef _WINDOWS
    fflush(stdout); 
#endif
    ready_already = 0;

    if (trace_flag) show_line("EXITING at JUMPOUT\n", 0);

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
//    if (endit(history) != 0) history = 2; /* 93/Dec/26 in local.c */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

//    abort_flag++;       // TURN OFF THE POWER ???

    if ((history != 0) && (history != 1))
      code = 1;
    else
      code = 0;
    uexit(code); 
//    longjmp(jumpbuffer, code+1);
  }
}
/* sec 0082 */
// deal with error by asking for user response 0-9, D, E, H, I, X, Q, R, S
// NOTE: this may JUMPOUT either via X, or because of too many errors
void error (void)
{/* 22 10 */ 
  ASCII_code c; 
  integer s1, s2, s3, s4; 
  if (history < 2) history = 2;

  print_char('.');
  show_context();

  if (interaction == 3)
    while (true) {
lab22:          /* loop */
      clear_for_error_prompt ();
      {
        ;
        print_string("? ");
        term_input(264, help_ptr);
      } 
      if (last == first) return;    // no input
      c = buffer[first];        // analyze first letter typed
      if(c >= 97)           // uppercase letter first
        c = (unsigned char) (c - 32); 
      switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          if(deletions_allowed) {
            s1 = cur_tok; 
            s2 = cur_cmd; 
            s3 = cur_chr; 
            s4 = align_state; 
            align_state = 1000000L; 
            OK_to_interrupt = false; 
            if ((last > first + 1) && (buffer[first + 1]>= 48) && (buffer[first + 1]<= 57))
              c = (unsigned char) (c * 10 + buffer[first + 1]- 48 * 11); 
            else
              c = (unsigned char) (c - 48);
            while (c > 0) {
              get_token ();
              decr(c);
            }
            cur_tok = s1;
            cur_cmd = s2;
            cur_chr = s3;
            align_state = s4;
            OK_to_interrupt = true;
            help2("I have just deleted some text, as you asked.",
                "You can now delete more, or insert, or whatever.");
            show_context ();
            goto lab22;     /* loop again */
          } 
          break; 
          ;
#ifdef DEBUG
        case 'D':
          {
            debug_help ();
            goto lab22;       /* loop again */
          }
          break; 
#endif /* DEBUG */
        case 'E':
          if (base_ptr > 0) {
            edit_name_start = str_start[input_stack[base_ptr].name_field];
            edit_name_length = str_start[input_stack[base_ptr].name_field + 1] - 
              str_start[input_stack[base_ptr].name_field];
            edit_line = line;
            jump_out();
//            return;     // can drop through now 99/Oct/20
          }
          break;
        case 'H':
          {
            if (use_err_help)
            {
              give_err_help ();
              use_err_help = false;
            } else {
              if (help_ptr == 0)
                help2("Sorry, I don't know how to help in this situation.",
                    "Maybe you should try asking a human?");
              do {
                decr(help_ptr); 
                print_string(help_line[help_ptr]);
                print_ln ();
              } while (!(help_ptr == 0));
            }
            help4("Sorry, I already gave what help I could...",
                "Maybe you should try asking a human?",
                "An error might have occurred before I noticed any problems.",
                "``If all else fails, read the instructions.''");
            goto lab22; /* loop again */
          }
          break;
        case 'I':
          {
            begin_file_reading ();
            if (last > first + 1)
            {
              cur_input.loc_field = first + 1;
              buffer[first]= 32;
            } else {
              {
                ;
                print_string("insert>");
                term_input(276, 0);
              }
              cur_input.loc_field = first;
            }
            first = last;
            cur_input.limit_field = last - 1;
            return;
          }
          break;
        case 'Q':
        case 'R':
        case 'S':
        {
          error_count = 0; 
          interaction = 0 + c - 81; /* Q = 0, R = 1, S = 2, T = 3 */
          print_string("OK, entering ");
          switch(c){
            case 81 :       /* Q */
            {
              print_esc("batchmode"); /*  */
              decr(selector); 
            } 
            break; 
            case 82 :       /* R */
              print_esc("nonstopmode"); /*  */
              break; 
            case 83 :       /* S */
              print_esc("scrollmode"); /*   */
              break; 
          } 
          print_string("...");
          print_ln (); 
#ifndef _WINDOWS
          fflush(stdout); 
#endif
          return; 
        } 
        break; 
        case 88 :       /* X */
        {
          interaction = 2; 
          jump_out();
//          return;     // can drop through now 99/Oct/20   
        } 
        break; 
        default: 
          ; 
          break; 
      }           /* end of switch analysing response character */
      {
        print_string("Type <return> to proceed, S to scroll future error messages,");
        print_nl("R to run without stopping, Q to run quietly,");  /*  */
        print_nl("I to insert something, ");  /*  */
        if (base_ptr > 0) print_string("E to edit your file,");
        if(deletions_allowed) print_nl("1 or ... or 9 to ignore the next 1 to 9 tokens of input,"); /*  */
        print_nl("H for help, X to quit.");  /*  */
      } 
    }   /* end of while(true) loop */

  incr(error_count); 
  if(error_count == 100) {
    print_nl("(That makes 100 errors; please try again.)");
    history = 3; 
    jump_out();
//    return;     // can drop through now 99/Oct/20   
  } 
  if(interaction > 0) decr(selector); 
  if(use_err_help) {
    print_ln (); 
    give_err_help (); 
  } 
  else while(help_ptr > 0){
    decr(help_ptr); 
    print_nl(help_line[help_ptr]); 
  } 
  print_ln (); 
  if(interaction > 0)incr(selector); 
  print_ln (); 
} 

void fatal_error_(char * s)
{
  normalize_selector ();
  print_err("Emergency stop");
  help1(s); 
  {
    if(interaction == 3)interaction = 2; 
    if(log_opened){
      error ();
    }
    ;
#ifdef DEBUG
    if(interaction > 0)debug_help (); 
#endif /* DEBUG */
    history = 3; 
    jump_out();
//    return;     // can drop through now 99/Oct/20   
  } 
}
/* sec 0094 */
void overflow_(char * s, integer n)
{
  normalize_selector ();
  print_err("TeX capacity exceeded, sorry[");
  print_string(s); 
  print_char('=');
  print_int(n); 
  print_char(']');
  help2("If you really absolutely need more capacity,",
      "you can ask a wizard to enlarge me."); 
  if (! knuth_flag) {   /*  Additional comments 98/Jan/5 */
    if (!strcmp(s, "pattern memory") && n == trie_size) {
      sprintf(log_line, "\n  (Maybe use -h=... on command line in ini-TeX)\n");
      show_line(log_line, 0);
    } else if (!strcmp(s, "exception dictionary") && n == hyphen_prime) {
      sprintf(log_line, "\n  (Maybe use -e=... on command line in ini-TeX)\n");
      show_line(log_line, 0);
    }
  }
  if (interaction == 3)
    interaction = 2;
  if (log_opened) {
    error ();
  }
  ;
#ifdef DEBUG
  if (interaction > 0)
    debug_help ();
#endif
  history = 3;
  jump_out();
}
/* sec 0095 */
void confusion_(str_number s)
{
  normalize_selector (); 
  if (history < 2) {
    print_err("This can't happen(");
    print(s); 
    print_char(41); /*)*/
    help1("I'm broken. Please show this to someone who can fix can fix");
  } else {
    print_err("I can't go on meeting you like this");
    help2("One of your faux pas seems to have wounded me deeply...",
        "in fact, I'm barely conscious. Please fix it and try again.");
  }
  {
    if (interaction == 3)
      interaction = 2;
    if (log_opened) {
      error();
    }
    ;
#ifdef DEBUG
    if (interaction > 0)
      debug_help();
#endif
    history = 3;
    jump_out();
  }
}
/* sec 0037 */
bool init_terminal (void) 
{/* 10 */
  register bool Result; 
  int flag;
  t_open_in(); 

  if (last > first) {
    cur_input.loc_field = first; 
    while((cur_input.loc_field < last) && (buffer[cur_input.loc_field]== ' '))
      incr(cur_input.loc_field);    // step over initial white space
    if(cur_input.loc_field < last){
      Result = true; 
      return Result;    // there is an input file name
    } 
  } 

//  failed to find input file name
  while(true){
    ; 
#ifdef _WINDOWS
    flag = ConsoleInput("**", "Please type a file name or a control sequence\r\n(or ^z to exit)", (char *) &buffer[first]);
    last = first + strlen((char *) &buffer[first]); /* -1 ? */
//    may need to be more elaborate see input_line in texmf.c
#else
    (void) fputs("**", stdout);
    fflush(stdout); 
    flag = input_ln(stdin, true);
#endif
    if(! flag){
      show_char('\n');
      show_line("! End of file on the terminal... why?\n", 1);
      Result = false;
      return Result;
    }

    cur_input.loc_field = first;
    while ((cur_input.loc_field < last) && (buffer[cur_input.loc_field]== ' '))
      incr(cur_input.loc_field);    // step over intial white space
    if (cur_input.loc_field < last) {
      Result = true;
      return Result;    // there is an input file name
    } 
    sprintf(log_line, "%s\n", "Please type the name of your input file.");
    show_line(log_line, 1);
  }
}
/* sec 0043 */
// Make string from str_start[str_ptr]to pool_ptr
str_number make_string (void)
{
  register str_number Result; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
  if (str_ptr == current_max_strings)
    str_start = realloc_str_start(increment_max_strings);
  if (str_ptr == current_max_strings) {
//  printf("**********MAKESTRING**********");   // debugging only
    overflow("number of strings", current_max_strings - init_str_ptr); /* 97/Mar/9 */
    return 0;     // abort_flag set
  }
#else
  if (str_ptr == max_strings) {
    overflow("number of strings", max_strings - init_str_ptr); /* number of strings */
    return 0;     // abort_flag set
  }
#endif
  incr(str_ptr); 
  str_start[str_ptr] = pool_ptr;
  Result = str_ptr - 1;
  return Result;
} 
/* sec 0044 */
bool str_eq_buf_ (str_number s, integer k)
{
  register bool Result;
  pool_pointer j;
  bool result;
  j = str_start[s];
  while (j < str_start[s + 1]) {
    if (str_pool[j]!= buffer[k])
    {
      result = false;
      goto lab45;
    } 
    incr(j);
    incr(k);
  } 
  result = true;
lab45: Result = result;
  return Result; 
} 
/* sec 0045 */
bool str_eq_str_ (str_number s, str_number t)
{
  register bool Result;
  pool_pointer j, k;
  bool result;
  result = false;
  if ((str_start[s + 1] - str_start[s]) != (str_start[t + 1] - str_start[t]))
    goto lab45;
  j = str_start[s];
  k = str_start[t];
  while (j < str_start[s + 1]) {
    if (str_pool[j] != str_pool[k])
      goto lab45;
    incr(j);
    incr(k);
  }
  result = true;
  lab45: Result = result;
  return Result;
}
/* sec 0066 */
void print_two_(integer n)
{ 
  n = abs(n) % 100;
  print_char('0' + (n / 10));
  print_char('0' + (n % 10));
} 
/* sec 0067 */
void print_hex_(integer n)
{
  char k;
  k = 0;
  print_char('"');
  do {
    dig[k] = (unsigned char) (n % 16);
    n = n / 16;
    incr(k);
  } while (!(n == 0));
  print_the_digs(k);
}
/* sec 0069 */
void print_roman_int_(integer n)
{
  pool_pointer j, k;
  nonnegative_integer u, v;
  j = str_start[260]; /*  m2d5c2l5x2v5i */
  v = 1000;
  while (true) {
    while (n >= v) {
      print_char(str_pool[j]);
      n = n - v;
    } 
    if (n <= 0)
      return;
    k = j + 2;
    u = v / (str_pool[k - 1]- 48);
    if (str_pool[k - 1]== 50)
    {
      k = k + 2;
      u = u / (str_pool[k - 1]- 48);
    }
    if (n + u >= v)
    {
      print_char(str_pool[k]);
      n = n + u;
    } else {
      j = j + 2;
      v = v / (str_pool[j - 1]- 48);
    }
  }
}
/* sec 0070 */
void print_current_string (void)
{
  pool_pointer j;
  j = str_start[str_ptr];
  while (j < pool_ptr) {
    print_char(str_pool[j]);
    incr(j);
  }
}

int stringlength (int str_ptr)
{
  int nstart, nnext;
  nstart = str_start[str_ptr];
  nnext = str_start[str_ptr + 1];
  return (nnext - nstart) + 2;
}

char * add_string (char *s, int str_ptr)
{
  int nstart, nnext, n;
  nstart = str_start[str_ptr];
  nnext = str_start[str_ptr + 1];
  n = nnext - nstart;
  memcpy(s, &str_pool[nstart], n);
  s += n;
  strcpy(s, "\r\n");
  s += 2;
  return s;
}

int addextrahelp=1;

// make one long \r\n separated string out of help lines 
// str_pool is packed_ASCII_code *

char * make_up_help_string (int nhelplines)
{
  char * helpstring, *s;
  int k, nlen=0;
  
//  get length of help for this specific message
  for (k = nhelplines-1; k >= 0; k--) {
    //nlen += stringlength(help_line[k]);
    nlen += strlen(help_line[k]);
  }
  nlen += 2;    // for blank line separator
  if (addextrahelp) {
    nlen += stringlength(265);
    nlen += stringlength(266);
    nlen += stringlength(267);
    if(base_ptr > 0)nlen += stringlength(268);
    if(deletions_allowed)nlen += stringlength(269);
    nlen += stringlength(270);
  }
  helpstring = (char *) malloc(nlen+1);
  s = helpstring;
  for (k = nhelplines-1; k >= 0; k--) {
    s = add_string(s, help_line[k]);
  }
  if (addextrahelp) {
    strcpy(s, "\r\n");
    s += 2;
    s = add_string(s, 265);    /* Type <return> to proceed, S to scroll future error messages, */
    s = add_string(s, 266);    /* R to run without stopping, Q to run quietly, */
    s = add_string(s, 267);    /* I to insert something,  */
    if(base_ptr > 0)s = add_string(s, 268);  /*  E to edit your file,  */
    if(deletions_allowed)s = add_string(s,  269);  /* 1 or ... or 9 to ignore the next 1 to 9 tokens of input, */
    s = add_string(s, 270);    /* H for help, X to quit. */
  }
  return helpstring;
}

char * make_up_query_string (int promptstr)
{
  char *querystr;
  int nstart, nnext, n;
  char *s;
  nstart = str_start[ promptstr];
  nnext = str_start[ promptstr + 1];
  n = nnext - nstart;
  querystr = (char *) malloc(n + 1);
  s = querystr;
  memcpy(s, &str_pool[nstart], n);  
  s += n;
  *s = '\0';
  return querystr;
}

// abort_flag set if input_line / ConsoleInput returns non-zero
// should set interrupt instead ???
// called from tex0.c, tex2.c, tex3.c

// void term_input(void)
void term_input (int promptstr, int nhelplines)
{ 
  integer k;
  int flag;
  char * helpstring = NULL;
  char * querystring = NULL;
//  if (nhelplines != 0) {
//    helpstring = make_up_help_string (nhelplines);
//    printf(helpstring);
//    free(helpstring);
//  }
  show_line("\n", 0);    // force it to show what may be buffered up ???
  helpstring = NULL;  
#ifdef _WINDOWS
  if (promptstr != 0) querystring = make_up_query_string (promptstr);
  if (nhelplines != 0) helpstring = make_up_help_string (nhelplines);
  if (helpstring == NULL && querystring != NULL) {
    if (strcmp(querystring, ": ") == 0)
      helpstring = xstrdup("Please type another file name (or ^z to exit):");
    else if (strcmp(querystring, "=>") == 0)    // from firm_up_the_line
      helpstring = xstrdup("Please type <enter> to accept this line\r\nor type a replacement line");
    else if (strcmp(querystring, "insert>") == 0) // from error() after "I"
      helpstring = xstrdup("Please type something to insert here");
    else if (strcmp(querystring, "") == 0)      // from read_toks
      helpstring = xstrdup("Please type a control sequence");
    else if (strcmp(querystring, "= ") == 0)    // from read_toks
      helpstring = xstrdup("Please type a token");
    else if (strcmp(querystring, "*") == 0)   // get_next
      helpstring = xstrdup("Please type a control sequence\r\n(or ^z to exit)");
//    else if (strcmp(querystring, "**") == 0)  // init_terminal
//      helpstring = xstrdup("Please type a control sequence or a file name\r\n(or ^z to exit)");     
//    else if (strcmp(querystring, "? ") == 0)  // from error()
//      helpstring = xstrdup("Please type a character to select an action");
  }
  flag = ConsoleInput(querystring, helpstring, (char *) &buffer[first]);  // ???
//  flag == 0 means trouble --- EOF on terminal
  if (querystring != NULL) free(querystring);
  if (helpstring != NULL) free(helpstring);
  helpstring = querystring = NULL;

  last = first + strlen((char *) &buffer[first]); /* -1 ? */
//  flag = (last > first);
//  may need to be more elaborate see input_line in texmf.c ???
//  sprintf(log_line, "first %d last %d flag %d - %s",
//      first, last, flag, (char *) &buffer[first]);
//  winerror(log_line);
#else
  fflush(stdout); 
  flag = input_ln(stdin, true);
#endif
  if(! flag){
    fatal_error("End of file on the terminal!"); /*  */
    return;         // abort_flag set
  }
  term_offset = 0; 
#ifdef _WINDOWS
// echo what was typed into Console buffer also
  if(last != first)
    {register integer for_end; k = first; for_end = last - 1;
  if(k <=  for_end) do
    print(buffer[k]); 
  while(k++ < for_end);
    } 
  print_ln (); 
#else
  decr(selector);     // shut off echo
  if(last != first)
    {register integer for_end; k = first; for_end = last - 1;
  if(k <=  for_end) do
    print(buffer[k]); 
  while(k++ < for_end);
    } 
  print_ln (); 
  incr(selector);     // reset selector again
#endif
}
/* sec 0091 */
void int_error_ (integer n)
{
  print_string(" (");
  print_int(n);
  print_char(')');
  error();
}
/* sec 0092 */
void normalize_selector (void)
{
  if (log_opened)
    selector = 19;
  else
    selector = 17;
  if (job_name == 0)
    open_log_file();
  if (interaction == 0)
    decr(selector);
}
/* sec 0098 */
void pause_for_instructions (void)
{
   if (OK_to_interrupt) {
    interaction = 3;
    if ((selector == 18)||(selector == 16))
      incr(selector);
    print_err("Interruption");
    help3("You rang?",
        "Try to insert some instructions for me (e.g.,`I\\showlists'),",
        "unless you just want to quit by typing `X'.");
    deletions_allowed = false;
    error ();
    deletions_allowed = true;
    interrupt = 0;
  }
}
/* sec 0100 */
integer half_(integer x)
{
  register integer Result;
  if (odd(x))
  Result =(x + 1)/ 2;
  else Result = x / 2;
  return Result;
}
/* sec 0102 */
scaled round_decimals_(small_number k)
{
  register scaled Result;
  integer a;
  a = 0;
  while (k > 0) {
    decr(k);
    a = (a + dig[k]* 131072L) / 10; /* 2^17 */
  }
  Result =(a + 1)/ 2;
  return Result;
}
/* sec 0103 */
/* This has some minor speedup changes - no real advantage probably ... */
void print_scaled_(scaled s)
{ 
  scaled delta;
  if (s < 0)
  {
    print_char('-');
    s = - (integer) s;
  }
  print_int(s / 65536L);
  print_char('.');
  s = 10 * (s % 65536L) + 5;
  delta = 10;
  do {
    if (delta > 65536L)
      s = s - 17232; /* 2^15 - 50000 - rounding */
    print_char('0' + (s / 65536L));
    s = 10 * (s % 65536L);
    delta = delta * 10;
  } while (!(s <= delta));
}
/* sec 0105 */
scaled mult_and_add_(integer n, scaled x, scaled y, scaled maxanswer)
{
  register scaled Result;
  if (n < 0)
  {
    x = - (integer) x;
    n = - (integer) n;
  }
  if (n == 0)
    Result = y;
  else if(((x <= (maxanswer - y) / n) && (- (integer) x <= (maxanswer + y) / n)))
    Result = n * x + y; 
  else {
    arith_error = true;
    Result = 0;
  }
  return Result;
}
/* sec 0106 */
scaled x_over_n_(scaled x, integer n)
{
  register scaled Result;
  bool negative;
  negative = false;
  if (n == 0)
  {
    arith_error = true;
    Result = 0;
    tex_remainder = x;
  } else {
    if (n < 0)
    {
      x = - (integer) x;
      n = - (integer) n;
      negative = true;
    }
    if (x >= 0)
    {
      Result = x / n;
      tex_remainder = x % n;
    } else {
      Result = - (integer) ((- (integer) x)/ n);
      tex_remainder = - (integer) ((- (integer) x)% n);
    }
  }
  if (negative)
    tex_remainder = - (integer) tex_remainder;
  return Result;
}
/* sec 0107 */
scaled xn_over_d_(scaled x, integer n, integer d)
{
  register scaled Result;
  bool positive;
  nonnegative_integer t, u, v;
  if (x >= 0)
    positive = true; 
  else {
    x = - (integer) x;
    positive = false;
  }
/*  t =(x % 32768L)* n;  */
  t =(x & 32767L) * n;
/*  u =(x / 32768L)* n +(t / 32768L);  */
  u =(x >> 15) * n + (t >> 15); 
/*  v =(u % d)* 32768L +(t % 32768L);  */
  v =((u % d) << 15) + (t & 32767L); 
  if (u / d >= 32768L)
    arith_error = true; 
/*  else u = 32768L *(u / d)+(v / d);  */ 
  else u =((u / d) << 15) + (v / d); 
  if (positive)
  {
    Result = u;
    tex_remainder = v % d;
  } else {
    Result = - (integer) u;
    tex_remainder = - (integer)(v % d);
  }
  return Result;
}
/* sec 0108 */
halfword badness_(scaled t, scaled s)
{
  register halfword Result;
  integer r;
  if (t == 0)
    Result = 0; 
  else if (s <= 0)
    Result = 10000; 
  else {
    if (t <= 7230584L)
      r = (t * 297) / s; 
    else if (s >= 1663497L)
      r = t / (s / 297);
    else
      r = t;
    if (r > 1290)
      Result = 10000; 
/*    safe to assume that r is positive ? */
/*    else Result =(r * r * r + 131072L)/ 262144L;  */
    else Result = (r * r * r + 131072L) >> 18;  /* 2^17 */
  }
  return Result;
}

#ifdef DEBUG
void print_word_(memory_word w)
{ 
  print_int(w.cint); 
  print_char(' ');
  print_scaled(w.cint); 
  print_char(' ');
  print_scaled(round(65536L * w.gr));
  print_ln ();
  print_int(w.hh.v.LH);
  print_char('=');
  print_int(w.hh.b0);
  print_char(':');
  print_int(w.hh.b1);
  print_char(';');
  print_int(w.hh.v.RH);
  print_char(' ');
  print_int(w.qqqq.b0); 
  print_char(':');
  print_int(w.qqqq.b1); 
  print_char(':');
  print_int(w.qqqq.b2); 
  print_char(':');
  print_int(w.qqqq.b3);
} 
/* need this version only if SHORTFONTINFO defined */
void zprintfword(fmemoryword w)
{
  print_int(w.cint);
  print_char(' ');
  print_scaled(w.cint);
  print_char(' ');
  print_scaled(round(65536L * w.gr));
  print_ln ();
  print_int(w.hh.v.LH);
  print_char('=');
  print_int(w.hh.b0);
  print_char(':');
  print_int(w .hh.b1);
  print_char(';');
  print_int(w.hh.v.RH);
  print_char(' ');
  print_int(w.qqqq.b0);
  print_char(':');
  print_int(w.qqqq.b1);
  print_char(':');
  print_int(w.qqqq.b2);
  print_char(':');
  print_int(w.qqqq.b3);
}
#endif

void show_token_list_(integer p, integer q, integer l)
{
  integer m, c;
  ASCII_code matchchr;
  ASCII_code n;
  matchchr = 35;
  n = 48;
  tally = 0;
/* while (p<>null) and (tally<l) do l.6239 */
  while ((p != 0) && (tally < l)) {
    if (p == q)
    {
      first_count = tally;
      trick_count = tally + 1 + error_line - half_error_line;
      if (trick_count < error_line)
        trick_count = error_line;
    }
    if ((p < hi_mem_min) || (p > mem_end))
    {
      print_esc("CLOBBERED.");
      return;
    }
    if (mem[p].hh.v.LH >= 4095)
      print_cs(mem[p].hh.v.LH - 4095);
    else {
      m = mem[p].hh.v.LH / 256;
      c = mem[p].hh.v.LH % 256;
      if (mem[p].hh.v.LH < 0)
        print_esc("BAD.");
      else
        switch (m)
        {
          case 1:
          case 2:
          case 3:
          case 4:
          case 7:
          case 8:
          case 10:
          case 11:
          case 12:
            print(c);
            break;
          case 6:
            {
              print(c);
              print(c);
            }
            break;
          case 5:
            {
              print(matchchr);
              if (c <= 9)
                print_char(c + '0');
              else {
                print_char('!');
                return;
              }
            }
            break;
          case 13:
            {
              matchchr = (ASCII_code) c;
              print(c);
              incr(n);
              print_char(n);
              if (n > 57)
                return;
            }
            break;
          case 14:
            print_string("->");
            break;
          default:
            print_esc("BAD. ");
            break;
        }
    }
    p = mem[p].hh.v.RH; 
  } 
/* if p<>null then print_esc("ETC."); l.6244 */
  if (p != 0)
    print_esc("ETC.");
}
/* sec 0306 */
void runaway (void)
{
  halfword p;
  if (scanner_status > 1)
  {
    print_nl("Runaway ");
    switch (scanner_status)
    {
      case 2:
        {
          print_string("definition");
          p = def_ref;
        }
        break;
      case 3:
        {
          print_string("argument");
          p = temp_head;
        }
        break;
      case 4:
        {
          print_string("preamble");
          p = hold_head;
        }
        break;
      case 5:
        {
          print_string("text");
          p = def_ref;
        }
        break;
    }
    print_char('?');
    print_ln ();
/*  p may be used without being initialized -- OK */
    show_token_list(mem[p].hh.v.RH, 0, error_line - 10); 
  }
}
/* sec 0120 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* first try list of available nodes (avail != NULL)                   */
/* then see if can go upwards (mem_end < mem_max)                      */
/* then see if can go downwards (hi_mem_min > lo_mem_max)              */
/* if not, extend memory at the top and grab from there --- new        */
/* else fail ! paragraph 120                                           */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
halfword get_avail (void)
{
  register halfword Result;
  halfword p;
  p = avail;
  if (p != 0) /* while p<>null do */
    avail = link(avail);
  else if (mem_end < mem_max) /* mem_end + 1 < mem_max ? NO */
  {
    incr(mem_end);
    p = mem_end;
  } else {
    decr(hi_mem_min);
    p = hi_mem_min;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (hi_mem_min <= lo_mem_max) {   /* have we run out in middle ? */
      incr(hi_mem_min);       /* undo the change */
/*    realloc_main (0, mem_top/2); */ /* extend main memory at hi end */
      mem = realloc_main (0, mem_top / 2);  /* zzzaa = zmem = mem */
      if (mem == NULL) {
        return 0;
      }
/* presumably now mem_end < mem_max - but need test in case allocation fails */
      if (mem_end >= mem_max) {
        runaway ();
        overflow("main memory size", mem_max + 1 - mem_min); /* main memory size */
        return 0;           // abort_flag set
      }
      incr(mem_end);        /* then grab from new area */
      p = mem_end;          /* 1993/Dec/14 */
    }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  }
  mem[p].hh.v.RH = 0;       /* link(p) = null !!! */
  ;
#ifdef STAT
  incr(dyn_used); 
#endif /* STAT */
  Result = p; 
  return Result; 
} 
/* sec 0123 */
void flush_list_(halfword p)          /* paragraph 123 */
{ 
  halfword q, r; 
  if (p != 0)              /* null !!! */
  {
    r = p;
    do {
      q = r;
      r = link(r);
      ;
#ifdef STAT
      decr(dyn_used);
#endif /* STAT */
    } while (!(r == 0));     /* r != null */
    link(q) = avail;
    avail = p;
  }
}
/* sec 0125 */
halfword get_node_(integer s)
{
  register halfword Result;
  halfword p;
  halfword q;
  integer r;
  integer t;
lab20: p = rover;
  do {
      q = p + mem[p].hh.v.LH;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*    while((mem[q].hh.v.RH == 262143L)) { */ /* NO! */
      while ((mem[q].hh.v.RH == empty_flag)) {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
        if (q == 0) {
/* should never happen, since this field is reference count for zeroglue */
        }  /* debugging code 93/DEC/15 */ /* eventually remove */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
        t = mem[q + 1].hh.v.RH;
        if (q == rover)
          rover = t;
        mem[t + 1].hh.v.LH = mem[q + 1].hh.v.LH;
        mem[mem[q + 1].hh.v.LH + 1].hh.v.RH = t;
        q = q + mem[q].hh.v.LH;
      }
      r = q - s;
      if (r > toint(p + 1)) 
      {
        mem[p].hh.v.LH = r - p;
        rover = p;
        goto lab40;
      }
      if (r == p)
        if (mem[p + 1].hh.v.RH != p)
        {
          rover = mem[p + 1].hh.v.RH;
          t = mem[p + 1].hh.v.LH;
          mem[rover + 1].hh.v.LH = t;
          mem[t + 1].hh.v.RH = rover;
          goto lab40;
        }
      mem[p].hh.v.LH = q - p;
      p = mem[p + 1].hh.v.RH;
  } while (!(p == rover));
  if (s == 1073741824L)    /* 2^30 - special case - merge adjacent */
  {
    Result = empty_flag;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (trace_flag)
      show_line("Merged adjacent multi-word nodes\n", 0);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    return Result;
  }
/*  maybe try downward epxansion first instead ? */
  if (lo_mem_max + 2 < hi_mem_min)
/*  if(lo_mem_max + 2 <= 262143L) */  /* NO! */
    if(lo_mem_max + 2 <= mem_bot + max_halfword)  /* silly ? flush 93/Dec/16 */
    {
/*    if(hi_mem_min - lo_mem_max >= 1998) */
      if (hi_mem_min - lo_mem_max >= (block_size + block_size - 2))
/*    t = lo_mem_max + 1000;  */
        t = lo_mem_max + block_size;
      else
        t = lo_mem_max + 1 +(hi_mem_min - lo_mem_max) / 2;
      p = mem[rover + 1].hh.v.LH;
      q = lo_mem_max;
      mem[p + 1].hh.v.RH = q;
      mem[rover + 1].hh.v.LH = q;
/*    if(t > 262143L)   t = 262143L;  */ /* NO! */
      if (t > mem_bot + max_halfword)
        t = mem_bot + max_halfword;     /* silly ? flush 93/Dec/16 */
      mem[q + 1].hh.v.RH = rover;
      mem[q + 1].hh.v.LH = p;
      mem[q].hh.v.RH = empty_flag;
      mem[q].hh.v.LH = t - lo_mem_max; /* block size */
      lo_mem_max = t;
      mem[lo_mem_max].hh.v.RH = 0;
      mem[lo_mem_max].hh.v.LH = 0;
      rover = q;
      goto lab20;
    }
/*  overflow("main memory size", mem_max + 1 - mem_min);  */ /* what used to happen! */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* we've run out of space in the middle for variable length blocks */
/* try and add new block from below mem_bot *//* first check if space ! */
  if (mem_min - (block_size + 1) <= mem_start) {/* extend lower memory downwards */
/*    realloc_main (mem_top/2, 0); */
    mem = realloc_main (mem_top/2 + block_size, 0);  /* zzzaa = zmem = mem */
    if (mem == NULL) {
      return 0;
    }
  }
/*  if (trace_flag) show_line("Extending downwards by %d\n", block_size, 0); */
  if (mem_min - (block_size + 1) <= mem_start) { /* check again */
    if (trace_flag) {
      sprintf(log_line, "mem_min %d, mem_start %d, block_size %d\n", mem_min, mem_start, block_size);
      show_line(log_line, 0);
    }
    overflow("main memory size", mem_max + 1 - mem_min); /* darn: allocation failed ! */
    return 0;     // abort_flag set
  }
/* avoid function call in following ? */
  add_variable_space (block_size); /* now to be found in itex.c */
  goto lab20;         /* go try get_node again */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

  lab40: mem[r].hh.v.RH = 0; 
  ;
#ifdef STAT
  var_used = var_used + s; 
#endif /* STAT */

/*  if (trace_flag) {
    if (r == 0) show_line("r IS ZERO in ZGETNODE!\n", 0);
  } */      /* debugging code 93/dec/15 */

  Result = r; 
  return Result; 
} 

void free_node_(halfword p, halfword s)
{ 
  halfword q;
  mem[p].hh.v.LH = s;
  mem[p].hh.v.RH = empty_flag;
  q = mem[rover + 1].hh.v.LH;
  mem[p + 1].hh.v.LH = q;
  mem[p + 1].hh.v.RH = rover;
  mem[rover + 1].hh.v.LH = p;
  mem[q + 1].hh.v.RH = p;
  ;
#ifdef STAT
  var_used = var_used - s; 
#endif /* STAT */
} 

halfword new_null_box (void) 
{
  register halfword Result;
  halfword p; 
  p = get_node(7);
  mem[p].hh.b0 = 0;
  mem[p].hh.b1 = 0;
  mem[p + 1].cint = 0;
  mem[p + 2].cint = 0;
  mem[p + 3].cint = 0;
  mem[p + 4].cint = 0;
  mem[p + 5].hh.v.RH = 0;
  mem[p + 5].hh.b0 = 0;
  mem[p + 5].hh.b1 = 0;
  mem[p + 6].gr = 0.0;
  Result = p;
  return Result;
} 

// @ A new rule node is delivered by the |new_rule| function. It
//   makes all the dimensions ``running,'' so you have to change the
//   ones that are not allowed to run.

halfword new_rule (void) 
{
  register halfword Result;
  halfword p; 
  p = get_node(4);          /* rule_node_size */
  mem[p].hh.b0 = 2;       /* rule_node type */
  mem[p].hh.b1 = 0;       /* sub_type zero */
  mem[p + 1].cint = -1073741824L; /* -2^30 null_flag width */
  mem[p + 2].cint = -1073741824L; /* -2^30 null_flag depth */
  mem[p + 3].cint = -1073741824L; /* -2^30 null_flag height */
  Result = p; 
  return Result; 
} 

// @ The |new_ligature| function creates a ligature node having given
//   contents of the |font|, |character|, and |lig_ptr| fields.

halfword new_ligature_(quarterword f, quarterword c, halfword q)
{
  register halfword Result; 
  halfword p; 
  p = get_node(2);      /* small_node_size */
  mem[p].hh.b0 = 6;   /* ligature_node type */
  mem[p + 1].hh.b0 = f; /* font */
  mem[p + 1].hh.b1 = c; /* character */
  mem[p + 1].hh.v.RH = q; /* pointer */
  mem[p].hh.b1 = 0;   /* subtype zero */
  Result = p; 
  return Result; 
} 

//  We also have a |new_lig_item| function, which returns a two-word
//  node having a given |character| field. Such nodes are used for
//  temporary processing as ligatures are being created.

halfword new_lig_item_(quarterword c)
{
  register halfword Result;
  halfword p; 
  p = get_node(2);      /* small_node_size */
  mem[p].hh.b1 = c;   /* character */ 
  mem[p + 1].hh.v.RH = 0; /* lig_ptr(p):=null; */
  Result = p; 
  return Result; 
} 

halfword new_disc (void) 
{
  register halfword Result; 
  halfword p; 
  p = get_node(2); 
  mem[p].hh.b0 = 7; 
  mem[p].hh.b1 = 0; 
  mem[p + 1].hh.v.LH = 0; /* pre_break(p):=null; */
  mem[p + 1].hh.v.RH = 0; /* post_break(p):=null; */
  Result = p; 
  return Result; 
} 

halfword new_math_(scaled w, small_number s)
{
  register halfword Result;
  halfword p; 
  p = get_node(2); 
  mem[p].hh.b0 = 9; 
  mem[p].hh.b1 = s; 
  mem[p + 1].cint = w; 
  Result = p; 
  return Result; 
} 

halfword new_spec_(halfword p)
{
  register halfword Result;
  halfword q; 
  q = get_node(4); 
  mem[q]= mem[p]; 
  mem[q].hh.v.RH = 0; 
  mem[q + 1].cint = mem[p + 1].cint; 
  mem[q + 2].cint = mem[p + 2].cint; 
  mem[q + 3].cint = mem[p + 3].cint; 
  Result = q; 
  return Result; 
} 

halfword new_param_glue_(small_number n)
{
  register halfword Result; 
  halfword p; 
  halfword q; 
  p = get_node(2); 
  mem[p].hh.b0 = 10; 
  mem[p].hh.b1 = n + 1; /* conversion int to unsigned short */
  mem[p + 1].hh.v.RH = 0; 
  q = eqtb[(hash_size + 782) + n].hh.v.RH; /* gluebase + n */
  mem[p + 1].hh.v.LH = q; 
  incr(mem[q].hh.v.RH); 
  Result = p; 
  return Result; 
} 

halfword new_glue_(halfword q)
{
  register halfword Result; 
  halfword p; 
  p = get_node(2); 
  mem[p].hh.b0 = 10; 
  mem[p].hh.b1 = 0; 
  mem[p + 1].hh.v.RH = 0; 
  mem[p + 1].hh.v.LH = q; 
  incr(mem[q].hh.v.RH); 
  Result = p; 
  return Result; 
} 

halfword new_skip_param_(small_number n)
{
  register halfword Result; 
  halfword p; 
  temp_ptr = new_spec(eqtb[(hash_size + 782) + n].hh.v.RH); /* gluebase + n */
  p = new_glue(temp_ptr); 
  mem[temp_ptr].hh.v.RH = 0; 
  mem[p].hh.b1 = n + 1;   /* conversion int to unsigned short */
  Result = p; 
  return Result; 
} 

halfword new_kern_(scaled w)
{
  register halfword Result;
  halfword p; 
  p = get_node(2); 
  mem[p].hh.b0 = 11; 
  mem[p].hh.b1 = 0; 
  mem[p + 1].cint = w; 
  Result = p; 
  return Result; 
} 

halfword new_penalty_(integer m)
{
  register halfword Result; 
  halfword p; 
  p = get_node(2); 
  mem[p].hh.b0 = 12; 
  mem[p].hh.b1 = 0; 
  mem[p + 1].cint = m; 
  Result = p; 
  return Result; 
} 

#ifdef DEBUG
void check_mem_(bool printlocs)
{/* 31 32 */  
  halfword p, q; 
  bool clobbered; 
  {
    register integer for_end;
    p = mem_min;
    for_end = lo_mem_max;
    if(p <= for_end) do 
      freearr[p]= false;
    while (p++ < for_end);
  }
  {
    register integer for_end;
    p = hi_mem_min;
    for_end = mem_end;
    if(p <= for_end) do
      freearr[p]= false;
    while (p++ < for_end);
  }
  p = avail;
  q = 0;
  clobbered = false;
  while (p != 0) {    /* while p<>null do */
    if ((p > mem_end) || (p < hi_mem_min))
      clobbered = true;
    else if (freearr[p])
      clobbered = true;
    if (clobbered)
    {
      print_nl("AVAIL list clobbered at ");
      print_int(q);
      goto lab31;
    }
    freearr[p] = true;
    q = p;
    p = mem[q].hh.v.RH;
  }
lab31:;
  p = rover;
  q = 0;        /* q:=null */
  clobbered = false;
  do {
      if ((p >= lo_mem_max) || (p < mem_min))
        clobbered = true;
      else if ((mem[p + 1].hh.v.RH >= lo_mem_max) || (mem[p + 1].hh.v.RH < mem_min))
        clobbered = true;
/*    else if(!((mem[p].hh.v.RH == 262143L)) ||(mem[p].hh *//*NO!*/
    else if(!((mem[p].hh.v.RH == empty_flag)) ||
        (mem[p].hh .v.LH < 2) || 
        (p + mem[p].hh.v.LH > lo_mem_max) ||
        (mem[mem[p + 1].hh.v.RH + 1].hh.v.LH != p))
      clobbered = true;
    if(clobbered)
    {
      print_nl("Double-AVAIL list clobbered at ");
      print_int(q);
      goto lab32;
    } 
    {
      register integer for_end;
      q = p;
      for_end = p + mem[p].hh.v.LH - 1;
      if (q <= for_end) do 
      {
        if (freearr[q])
        {
          print_nl("Doubly free location at ");
          print_int(q);
          goto lab32;
        }
        freearr[q]= true;
      } while (q++ < for_end);
    }
    q = p;
    p = mem[p + 1].hh.v.RH;
  } while (!(p == rover));
lab32:;
  p = mem_min;
  while (p <= lo_mem_max) {
    if((mem[p].hh.v.RH == empty_flag)) 
    {
      print_nl("Bad flag at ");
      print_int(p);
    }
    while ((p <= lo_mem_max) && !freearr[p]) incr(p);
    while ((p <= lo_mem_max) && freearr[p]) incr(p);
  }
  if (printlocs)
  {
    print_nl("New busy locs:");
    {
      register integer for_end;
      p = mem_min;
      for_end = lo_mem_max;
      if(p <= for_end) do
        if (!freearr[p] && ((p > was_lo_max) || wasfree[p]))
        {
          print_char(' ');
          print_int(p);
        } while (p++ < for_end);
    }
    {
      register integer for_end;
      p = hi_mem_min;
      for_end = mem_end;
      if (p <= for_end) do
        if (!freearr[p] && ((p < was_hi_min) || (p > was_mem_end) || wasfree[p]))
        {
          print_char(' ');
          print_int(p);
        } while (p++ < for_end);
    }
  } 
  {
    register integer for_end;
    p = mem_min;
    for_end = lo_mem_max;
    if(p <= for_end) do
      wasfree[p]= freearr[p];
    while (p++ < for_end);
  }
  {
    register integer for_end;
    p = hi_mem_min;
    for_end = mem_end;
    if(p <= for_end) do
      wasfree[p] = freearr[p];
    while (p++ < for_end);
  }
  was_mem_end = mem_end;
  was_lo_max = lo_mem_max;
  was_hi_min = hi_mem_min;
}
#endif /* DEBUG */

#ifdef DEBUG
void search_mem_(halfword p)
{
  integer q; 
  {
    register integer for_end;
    q = mem_min;
    for_end = lo_mem_max;
    if(q <= for_end) do
    {
      if (link(q) == p)
      {
        print_nl("LINK(");
        print_int(q);
        print_char(')');
      }
      if (info(q) == p)
      {
        print_nl("INFO(");
        print_int(q);
        print_char(')');
      }
    } while (q++ < for_end);
  }
  {
    register integer for_end;
    q = hi_mem_min;
    for_end = mem_end;
    if (q <= for_end) do
    {
      if(link(q) == p)
      {
        print_nl("LINK(");
        print_int(q);
        print_char(')');
      }
      if (info(q) == p)
      {
        print_nl("INFO(");
        print_int(q);
        print_char(')');
      }
    } while (q++ < for_end);
  }
  {
    register integer for_end;
    q = 1;
    for_end = (hash_size + 1833);
    if (q <= for_end) do
    {
      if(eqtb[q].hh.v.RH == p)
      {
        print_nl("EQUIV(");
        print_int(q);
        print_char(')');
      }
    } while(q++ < for_end);
  }
  if (save_ptr > 0)
  {
    register integer for_end;
    q = 0;
    for_end = save_ptr - 1;
    if(q <= for_end) do 
    {
      if (save_stack[q].hh.v.RH == p)
      {
        print_nl("SAVE(");
        print_int(q);
        print_char(41);
      }
    } while (q++ < for_end);
  }
/*  {register integer for_end; q = 0; for_end = 607; if(q <= for_end) do */
  {
    register integer for_end;
    q = 0;
    for_end = hyphen_prime;
    if (q <= for_end) do 
    {
      if (hyph_list[q]== p)
      {
        print_nl("HYPH(");
        print_int(q);
        print_char(')');
      }
    } while(q++ < for_end);
  }
}
#endif /* DEBUG */

void short_display_(integer p)
{
  integer n; 
/*  while(p > mem_min){ */
  while (p != 0) {      /* want p != null here bkph 93/Dec/15 !!! */
                /* NOTE: still not fixed in 3.14159 ! */
     if ((p >= hi_mem_min))  /* is_char_node(p) */
     {
       if (p <= mem_end)
       {
         if (mem[p].hh.b0 != font_in_short_display) /* font(p) */
         {
           if ((mem[p].hh.b0 > font_max)) 
             print_char('*');
/*    else print_esc(hash[(hash_size + 524) + mem[p].hh.b0].v.RH); */
           else //print_esc(hash[(hash_size + hash_extra + 524) + mem[p].hh.b0].v.RH);
           {print_esc("");print(hash[(hash_size + hash_extra + 524) + mem[p].hh.b0].v.RH);}
/* 96/Jan/10 */
           print_char(' ');
           font_in_short_display = mem[p].hh.b0;
         }
         print(mem[p].hh.b1);          /* character(p) */
       }
    } else switch (mem[p].hh.b0)
    {
      case 0:
      case 1:
      case 3:
      case 8:
      case 4:
      case 5:
      case 13:
        print_string("[]");
        break;
      case 2:
        print_char('|');
        break;
      case 10:
        if (mem[p + 1].hh.v.LH != 0)
          print_char(' ');
        break;
      case 9:
        print_char('$');
        break;
      case 6:
        short_display(mem[p + 1].hh.v.RH);
        break;
      case 7:
        {
          short_display(mem[p + 1].hh.v.LH);
          short_display(mem[p + 1].hh.v.RH);
          n = mem[p].hh.b1;
          while (n > 0) {
            if(link(p) != 0) /* if link(p)<>null then */
              p = mem[p].hh.v.RH;
            decr(n);
          }
        }
        break;
      default:
        ;
        break;
    }
    p = mem[p].hh.v.RH;
  }
}

void print_font_and_char_ (integer p)
{
  if (p > mem_end)
    print_esc("CLOBBERED.");
  else {
    if((mem[p].hh.b0 > font_max)) /* font(p) */
      print_char(42);   /* * */
/*    else print_esc(hash[(hash_size + 524) + mem[p].hh.b0].v.RH); */
    else
      //print_esc(hash[(hash_size + hash_extra + 524) + mem[p].hh.b0].v.RH); /* 96/Jan/10 */
      {print_esc("");print(hash[(hash_size + hash_extra + 524) + mem[p].hh.b0].v.RH);}
    print_char(' ');
    print(mem[p].hh.b1);      /* character(p) */
  } 
} 

void print_mark_ (integer p)
{ 
  print_char('{');
  if ((p < hi_mem_min)||(p > mem_end))
    print_esc("CLOBBERED.");
  else
    show_token_list(mem[p].hh.v.RH, 0, max_print_line - 10);
  print_char('}');
} 

void print_rule_dimen_ (scaled d)
{
  if ((d == -1073741824L)) /* - 2^30 */
    print_char('*');
  else
    print_scaled(d);
}

void print_glue_(scaled d, integer order, str_number s)
{
  print_scaled(d); 
  if ((order < 0)||(order > 3))
    print_string("foul");
  else if (order > 0)
  {
    print_string("fil");
    while (order > 1) {
      print_char('l');
      decr(order);
    }
  } else if(s != 0)
    print(s);
}

void print_spec_(integer p, str_number s)
{
  if ((p < mem_min)||(p >= lo_mem_max)) 
    print_char('*');
  else {
    print_scaled(mem[p + 1].cint);
    if (s != 0)
      print(s);
    if (mem[p + 2].cint != 0)
    {
      print_string("plus");
      print_glue(mem[p + 2].cint, mem[p].hh.b0, s);
    }
    if (mem[p + 3].cint != 0)
    {
      print_string("minus");
      print_glue(mem[p + 3].cint, mem[p].hh.b1, s);
    }
  }
}

void print_fam_and_char_(halfword p)
{
  print_esc("fam");
  print_int(mem[p].hh.b0);
  print_char(' ');
  print(mem[p].hh.b1);
}

void print_delimiter_(halfword p)
{
  integer a;
  a = mem[p].qqqq.b0 * 256 + mem[p].qqqq.b1;
  a = a * 4096 + mem[p].qqqq.b2 * 256 + mem[p].qqqq.b3;
  if (a < 0)
    print_int(a);
  else
    print_hex(a);
} 

void print_subsidiary_data_(halfword p, ASCII_code c)
{
  if ((pool_ptr - str_start[str_ptr]) >= depth_threshold)
  {
    if (mem[p].hh.v.RH != 0)
      print_string("[]");
  } else {
    {
      str_pool[pool_ptr]= c;
      incr(pool_ptr);
    }
    temp_ptr = p;
    switch (mem[p].hh.v.RH)
    {
      case 1:
        {
          print_ln ();
          print_current_string ();
          print_fam_and_char(p);
        }
        break;
      case 2:
        show_info ();
        break;
      case 3:
        if (mem[p].hh.v.LH == 0)
        {
          print_ln ();
          print_current_string ();
          print_string("{}");
        } else show_info ();
        break;
      default:
        ;
        break;
    }
    decr(pool_ptr);
  }
}
/* sec 0694 */
void print_style_(integer c)
{
  switch (c / 2)
  {
    case 0:
      print_esc("displaystyle");
      break;
    case 1:
      print_esc("textstyle");
      break;
    case 2:
      print_esc("scriptstyle");
      break;
    case 3:
      print_esc("scriptscriptstyle");
      break;
    default:
      print_string("Unknown style!");
      break;
  }
}
/* sec 0225 */
void print_skip_param_(integer n)
{
  switch(n)
  {
    case line_skip_code:
      print_esc("lineskip");
      break;
    case baseline_skip_code:
      print_esc("baselineskip");
      break; 
    case par_skip_code:
      print_esc("parskip");
      break;
    case above_display_skip_code:
      print_esc("abovedisplayskip");
      break;
    case below_display_skip_code:
      print_esc("belowdisplayskip");
      break;
    case above_display_short_skip_code:
      print_esc("abovedisplayshortskip");
      break;
    case below_display_short_skip_code:
      print_esc("belowdisplayshortskip");
      break;
    case left_skip_code:
      print_esc("leftskip");
      break;
    case right_skip_code:
      print_esc("rightskip");
      break;
    case top_skip_code:
      print_esc("topskip");
      break;
    case split_top_skip_code:
      print_esc("splittopskip");
      break;
    case tab_skip_code:
      print_esc("tabskip");
      break;
    case space_skip_code:
      print_esc("spaceskip");
      break;
    case xspace_skip_code:
      print_esc("xspaceskip");
      break;
    case par_fill_skip_code:
      print_esc("parfillskip");
      break;
    case thin_mu_skip_code:
      print_esc("thinmuskip");
      break;
    case med_mu_skip_code:
      print_esc("medmuskip");
      break; 
    case thick_mu_skip_code:
      print_esc("thickmuskip");
      break;
    default:
      print_string("[unknown glue parameter!]");
      break; 
  } 
} 

void show_node_list_(integer p)
{/* 10 */ 
  integer n; 
  real g; 
/* begin if cur_length>depth_threshold then */
  if ((pool_ptr - str_start[str_ptr]) > depth_threshold)
  {
/*    if(p > 0) */  /* was p>null !!! line 3662 in tex.web */
    if(p != 0)    /* fixed 94/Mar/23 BUG FIX */
            /* NOTE: still not fixed in 3.14159 ! */
    print_string("[]");
    return; 
  } 
  n = 0; 
/*  while(p > mem_min){ */  /* was p>mem_min !!! line 3667 in tex.web */
  while(p != 0){      /* want p != null - bkph 93/Dec/15 */
                /* NOTE: still not fixed in 3.14159 ! */
    print_ln (); 
    print_current_string (); 
    if (p > mem_end)
    {
      print_string("Bad link, display aborted.");
      return;
    }
    incr(n);
    if (n > breadth_max)
    {
      print_string("etc.");
      return;
    }
    if ((p >= hi_mem_min))
      print_font_and_char(p);
    else switch (mem[p].hh.b0)
    {
      case 0:
      case 1:
      case 13:
        {
          if (mem[p].hh.b0 == 0)
            print_esc("h");
          else if (mem[p].hh.b0 == 1)
            print_esc("v");
          else print_esc("unset");
          print_string("box(");
          print_scaled(mem[p + 3].cint);
          print_char('+');
          print_scaled(mem[p + 2].cint);
          print_string(", shifted ");
          print_scaled(mem[p + 1].cint);
          if (mem[p].hh.b0 == 13)
          {
            if (mem[p].hh.b1 != 0)
            {
              print_string(" (");
              print_int(mem[p].hh.b1 + 1);
              print_string(" columns)");
            }
            if (mem[p + 6].cint != 0)
            {
              print_string(", stretch");
              print_glue(mem[p + 6].cint, mem[p + 5].hh.b1, 0);
            }
            if (mem[p + 4].cint != 0)
            {
              print_string(", shrink");
              print_glue(mem[p + 4].cint, mem[p + 5].hh.b0, 0);
            }
          } else {
            g = mem[p + 6].gr;
            if ((g != 0.0)&&(mem[p + 5].hh.b0 != 0))
            {
              print_string(", glue set");
              if (mem[p + 5].hh.b0 == 2)
                print_string("- ");
              if (fabs(g)> 20000.0)
              {
                if (g > 0.0)
                  print_char('>');
                else
                  print_string("< -");
                print_glue(20000 * 65536L, mem[p + 5].hh.b1, 0);
              } else print_glue(round(65536L * g), mem[p + 5].hh.b1, 0);
            }
            if (mem[p + 4].cint != 0)
            {
              print_string("shifted");
              print_scaled(mem[p + 4].cint);
            }
          }
          {
            {
              str_pool[pool_ptr]= 46;
              incr(pool_ptr);
            }
            show_node_list(mem[p + 5].hh.v.RH);
            decr(pool_ptr);
          }
        }
        break;
      case 2:
        {
          print_esc("rule(");
          print_rule_dimen(mem[p + 3].cint);
          print_char('+');
          print_rule_dimen(mem[p + 2].cint);
          print_string(")x");
          print_rule_dimen(mem[p + 1].cint);
        }
        break;
      case 3:
        {
          print_esc("insert");
          print_int(mem[p].hh.b1);
          print_string(",natural size ");
          print_scaled(mem[p + 3].cint);
          print_string("; split(");
          print_spec(mem[p + 4].hh.v.RH, 0);
          print_char(',');
          print_scaled(mem[p + 2].cint);
          print_string("(; float cost");
          print_int(mem[p + 1].cint);
          {
            {
              str_pool[pool_ptr]= 46;
              incr(pool_ptr);
            }
            show_node_list(mem[p + 4].hh.v.LH);
            decr(pool_ptr);
          }
        }
        break;
      case 8:
        switch (mem[p].hh.b1)
        {
          case 0:
            {
              print_write_whatsit(1279, p);   /* debug # (-1 to exit): */
              print_char('=');
              print_file_name(mem[p + 1].hh.v.RH, mem[p + 2].hh.v.LH, mem[p + 2].hh.v.RH);
            }
            break;
          case 1:
            {
              print_write_whatsit(591, p);  /* write */
              print_mark(mem[p + 1].hh.v.RH);
            }
            break;
          case 2:
            print_write_whatsit(1280, p); /* closeout */
            break;
          case 3:
            {
              print_esc("special");
              print_mark(mem[p + 1].hh.v.RH);
            }
            break;
          case 4:
            {
              print_esc("setlanguage");
              print_int(mem[p + 1].hh.v.RH);
              print_string(" (hyphenmin");
              print_int(mem[p + 1].hh.b0);
              print_char(',');
              print_int(mem[p + 1].hh.b1);
              print_char(')');
            }
            break;
          default:
            print_string("whatsit");
            break;
        }
        break;
      case 10:
        if (mem[p].hh.b1 >= 100)
        {
          print_esc("");
          if (mem[p].hh.b1 == 101)
            print_char('c');
          else if (mem[p].hh.b1 == 102)
            print_char('x');
          print_string("leaders ");
          print_spec(mem[p + 1].hh.v.LH, 0);
          {
            {
              str_pool[pool_ptr]= 46;
              incr(pool_ptr);
            }
            show_node_list(mem[p + 1].hh.v.RH);
            decr(pool_ptr);
          }
        } else {
          print_esc("glue");
          if (mem[p].hh.b1 != 0)
          {
            print_char('(');
            if (mem[p].hh.b1 < 98)
              print_skip_param(mem[p].hh.b1 - 1);
            else if (mem[p].hh.b1 == 98)
              print_esc("nonscript");
            else print_esc("mskip");
            print_char(')');
          }
          if (mem[p].hh.b1 != 98)
          {
            print_char(' ');
            if (mem[p].hh.b1 < 98)
              print_spec(mem[p + 1].hh.v.LH, 0);
            else
              print_spec(mem[p + 1].hh.v.LH, 334); /* mu */
          }
        }
        break;
      case 11:
        if (mem[p].hh.b1 != 99)
        {
          print_esc("kern");
          if (mem[p].hh.b1 != 0)
            print_char(' ');
          print_scaled(mem[p + 1].cint);
          if (mem[p].hh.b1 == 2)
            print_string(" (for accent)");
        } else {
          print_esc("mkern");
          print_scaled(mem[p + 1].cint);
          print_string("mu");
        }
        break;
      case 9:
        {
          print_esc("math");
          if (mem[p].hh.b1 == 0)
            print_string("on");
          else print_string("off");
          if (mem[p + 1].cint != 0)
          {
            print_string(", surrounded");
            print_scaled(mem[p + 1].cint);
          }
        }
        break;
      case 6:
        {
          print_font_and_char(p + 1);
          print_string("(ligature");
          if (mem[p].hh.b1 > 1)
            print_char('|');
          font_in_short_display = mem[p + 1].hh.b0; 
          short_display(mem[p + 1].hh.v.RH);
          if (odd(mem[p].hh.b1))
            print_char('|');
          print_char(')');
        }
        break;
      case 12:
        {
          print_esc("penalty ");
          print_int(mem[p + 1].cint);
        }
        break;
      case 7:
        {
          print_esc("discretionary");
          if (mem[p].hh.b1 > 0)
          {
            print_string(" replacing ");
            print_int(mem[p].hh.b1);
          }
          {
            {
              str_pool[pool_ptr]= 46;
              incr(pool_ptr);
            }
            show_node_list(mem[p + 1].hh.v.LH);
            decr(pool_ptr);
          }
          {
            str_pool[pool_ptr]= 124;
            incr(pool_ptr);
          }
          show_node_list(mem[p + 1].hh.v.RH);
          decr(pool_ptr);
        }
        break;
      case 4:
        {
          print_esc("mark");
          print_mark(mem[p + 1].cint);
        }
        break;
      case 5:
        {
          print_esc("vadjust");
          {
            {
              str_pool[pool_ptr]= 46;
              incr(pool_ptr);
            }
            show_node_list(mem[p + 1].cint);
            decr(pool_ptr);
          }
        }
        break;
      case 14:
        print_style(mem[p].hh.b1);
        break;
      case 15:
        {
          print_esc("mathchoice");
          {
            str_pool[pool_ptr]= 68;
            incr(pool_ptr);
          }
          show_node_list(mem[p + 1].hh.v.LH);
          decr(pool_ptr);
          {
            str_pool[pool_ptr]= 84;
            incr(pool_ptr);
          }
          show_node_list(mem[p + 1].hh.v.RH);
          decr(pool_ptr);
          {
            str_pool[pool_ptr]= 83;
            incr(pool_ptr);
          }
          show_node_list(mem[p + 2].hh.v.LH);
          decr(pool_ptr);
          {
            str_pool[pool_ptr]= 115;
            incr(pool_ptr);
          } 
          show_node_list(mem[p + 2].hh.v.RH); 
          decr(pool_ptr); 
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
      case 30:
      case 31:
        {
          switch (mem[p].hh.b0)
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
            case 27:
              print_esc("overline");
              break;
            case 26:
              print_esc("underline");
              break;
            case 29:
              print_esc("vcenter");
              break;
            case 24:
              {
                print_esc("radical");
                print_delimiter(p + 4);
              }
              break;
            case 28:
              {
                print_esc("accent");
                print_fam_and_char(p + 4);
              }
              break;
            case 30:
              {
                print_esc("left");
                print_delimiter(p + 1);
              }
              break;
            case 31:
              {
                print_esc("right");
                print_delimiter(p + 1);
              }
              break;
          } 
          if (mem[p].hh.b1 != 0)
            if (mem[p].hh.b1 == 1)
              print_esc("limits");
            else print_esc("nolimits");
          if (mem[p].hh.b0 < 30)
            print_subsidiary_data(p + 1, 46);
          print_subsidiary_data(p + 2, 94);
          print_subsidiary_data(p + 3, 95);
        }
        break;
      case 25:
        {
          print_esc("fraction");
          if (mem[p + 1].cint == 1073741824L)  /* 2^30 */
            print_string("= default");
          else print_scaled(mem[p + 1].cint); 
          if ((mem[p + 4].qqqq.b0 != 0) || (mem[p + 4].qqqq.b1 != 0) ||
              (mem[p + 4].qqqq.b2 != 0) || (mem[p + 4].qqqq.b3 != 0))
          {
            print_string(", left");
            print_delimiter(p + 4);
          }
          if ((mem[p + 5].qqqq.b0 != 0) || (mem[p + 5].qqqq.b1 != 0) ||
              (mem[p + 5].qqqq.b2 != 0)||(mem[p + 5].qqqq.b3 != 0))
          {
            print_string(", right");
            print_delimiter(p + 5);
          }
          print_subsidiary_data(p + 2, 92);
          print_subsidiary_data(p + 3, 47);
        }
        break;
      default:
        print_string("Unknown node type!");
        break;
    }
    p = mem[p].hh.v.RH;
  } 
} 
/* NOTE: 262143L should be empty_flag */
