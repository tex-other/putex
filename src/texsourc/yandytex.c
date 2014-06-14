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

#define EXTERN

#include "texd.h"

#define dump_ext_length     4
#define edit_value          tex_edit_value

extern char * replacement[];
static char * program_name = NULL;
int    gargc;   /* number of args - set to zero after initialization */
char **gargv;

/* The entry point: set up for reading the command line, which will
   happen in `t_open_in', then call the main body.  */

int jump_used = 0;

jmp_buf jumpbuffer;

int main (int ac, char *av[])
{
  int flag = 0, ret = 0;

  gargc = ac;
  gargv = av;

  if (main_init(gargc, gargv))
    return -1;

  TEX_format_default = " plain.fmt";
  format_default_length = strlen(TEX_format_default + 1);

  jump_used = 0;

  ret = setjmp(jumpbuffer);

  if (ret == 0)
  {
    flag = main_program();

    if (trace_flag)
      printf("EXITING at %s: flag = %d, ret = %d, jump_used = %d\n", "main", flag, ret, jump_used);
  }
  else
  {
    if (trace_flag)
      printf("EXITING at %s: flag = %d, ret = %d, jump_used =  %d\n", "jump_out", flag, ret, jump_used);
  }

  if (endit(flag) != 0)
    flag = 1; /* do final clean up in local.c */

  if (flag == 0)
    return 0;
  else
    exit (flag);
}

/* texk/web2c/lib/texmfmp.c */
void t_open_in (void)
{
  int i;

  buffer[first] = 0;  /* In case there are no arguments.  */

#ifdef MSDOS
  if (gargc > optind && optind > 0)
  {
    for (i = optind; i < gargc; i++)
#else
  if (gargc > 1)
  {
    for (i = 1; i < gargc; i++)
#endif
    {
      if (allow_quoted_names && strchr(gargv[i], ' ') != NULL)
      {
        (void) strcat ((char *) &buffer[first], "\"");
        (void) strcat ((char *) &buffer[first], gargv[i]);
        (void) strcat ((char *) &buffer[first], "\"");
      }
      else
        (void) strcat ((char *) &buffer[first], gargv[i]);

      (void) strcat ((char *) &buffer[first], " ");
    }
    
    gargc = 0;
  }

  /* Find the end of the buffer.  */
  for (last = first; buffer[last]; ++last)
    ;

  for (--last; last >= first && ISBLANK (buffer[last]) && buffer[last] != '\r'; --last)
    ;

  last++;

/* One more time, this time converting to TeX's internal character
   representation.  */ /* for command line input in this case */
  if (non_ascii)
  {
    for (i = first; i < last; i++)
      buffer[i] = xord[buffer[i]];
  }
}

static void catch_interrupt(int err)
{
  (void) err;
  (void) signal(SIGINT, SIG_IGN);

  if (interrupt++ >= 3)
    exit(1);

  (void) signal(SIGINT, catch_interrupt);
}

void fix_date_and_time (void)
{
  time_t clock;
  struct tm *tmptr;

  (void) time(&clock);

  if (trace_flag)
    printf("The time is %u\n", clock);

  if (clock < 0)
    puts("Time not available!");

  tmptr = localtime (&clock);

  if (tmptr == NULL)
  {
    printf("Cannot convert time (%0ld)!\n", clock);
    year     = 2038;
    month    = 1;
    day      = 18;
    tex_time = 22 * 60 + 14;
  }
  else
  {
    tex_time = tmptr->tm_hour * 60 + tmptr->tm_min;
    day      = tmptr->tm_mday;
    month    = tmptr->tm_mon + 1;
    year     = tmptr->tm_year + 1900;

    if (trace_flag)
      printf("%d-%d-%d %d:%d\n",
        tmptr->tm_year + 1900,
        tmptr->tm_mon + 1,
        tmptr->tm_mday,
        tmptr->tm_hour,
        tmptr->tm_min);
  }

  {
#ifdef MSDOS
    if (!no_interrupts)
    {
      if (signal(SIGINT, catch_interrupt) == SIG_ERR)
      {
        puts(" CTRL-C handler not installed\n");
#ifndef _WINDOWS
        uexit(EXIT_FAILURE);  /* do we care when run as DLL ? */
#endif
      }
    }
#else
    void (*old_handler)();

    if ((old_handler = signal (SIGINT, catch_interrupt)) != SIG_DFL)
      (void) signal (SIGINT, old_handler);
#endif
  }
}

/* I/O for TeX and Metafont.  */
void complain_line (FILE *output)
{
  show_line("\n", 0);

#ifdef ALLOCATEBUFFER
  sprintf(log_line, "! Unable to read an entire line---buf_size=%d.\n", current_buf_size);
#else
  sprintf(log_line, "! Unable to read an entire line---buf_size=%d.\n", buf_size);
#endif

  fputs(log_line, output);
  puts("  (File may have a line termination problem.)");
}

void show_bad_line (FILE *output, int first, int last)
{
  int i, c, d, ch;
  char *s = log_line;

  for (i = first; i <= last; i++)
  {
    ch = buffer[i];

    if ((show_in_hex && ch > 127))
    {
      c = ch >> 4;
      d = ch & 15; 
      if (c > 9)
        c = c + 'a' - 10;
      else
        c = c + '0';
      if (d > 9)
        d = d + 'a' - 10;
      else
        d = d + '0';
/* putc('^', output); putc('^', output); */
      *s++ = '^';
      *s++ = '^';
/* putc (c, output); putc (d, output); */
      *s++ = (char) c;
      *s++ = (char) d;
    }
    else
      if (ch < 32)
      {
/* putc('^', output); putc('^', output); */
        *s++ = '^';
        *s++ = '^';
/* putc (ch + 64, output); */
        *s++ = (char) (ch + 64);
      }
      else
        if (ch == 127)
        {
/* putc('^', output); putc('^', output); */
          *s++ = '^';
          *s++ = '^';
/* putc (ch - 64, output); */
          *s++ = (char) (ch - 64);
        }
        else
        {
/* putc(ch, output); */
          *s++ = (char) ch;
        }
  }
//  putc(' ', output);    /*  putc('\n', output); */
  *s++ = ' ';
  *s++ = '\0';

  fputs(log_line, output);   // log_file
}

// split off for convenience and use in ConsoleInput
boolean input_line_finish (void)
{
  int i = '\0';
  int ch, flag;
  
/*  if last line in file does not end with \n - never happens ? */
/*  if (i == EOF && buffer[last] != '\n') buffer[last++] = '\n'; */

  buffer[last] = ' ';           /* space terminate */

  if (last >= max_buf_stack)
    max_buf_stack = last; /* remember longest line */

/* Trim trailing whitespace.  */ 
/* #define isblank(c) ((c) == ' ' || (c) == '\t') */
/* What about \n ?  Can't get in here ?- bkph */
/* What about control-Z that gets read in binary mode ? - bkph */
// #ifdef MYDEBUG
/*  while (last > first && buffer[last - 1] <= ' ')  --last; */
  while (last > first)
  {
    i = buffer[last - 1];

    if (i == ' ' || i == '\t')
      --last;
    else
      break;
  }

/* following added to check source file integrity ASCII 32 - 127 */
/* allow space, tab, new-page - also allow return, newline ? */
  if (restrict_to_ascii)
  {
    flag = 0;
    for (i = first; i <= last; i++)
    {
      ch = buffer[i];
/*      if (ch > 127 || (ch < ' ' && ch != '\t' && ch != '\f')) */
/*      1 -- 8, 11, 14 -- 31 are not good ASCII characters */
      if (ch > 126 ||  (ch < ' ' && ch != '\t' && ch != '\f' && ch != '\r' && ch != '\n'))
      {
        sprintf(log_line, "\n! non ASCII char (%d) in line: ", ch);
        show_line(log_line, 1);

        if (log_opened)
          fprintf(log_file, "\n! non ASCII char (%d) in line: ", ch);

/*        buffer[i]= 127; */ /* not defined - invalid char */
        flag = 1;
        break;
      }
    }
    if (flag)
    {
      show_bad_line(errout, first, last);
      if (log_opened)
        show_bad_line(log_file, first, last);
    }
  }
/* Don't bother using xord if we don't need to. */ /* for input line */
/* #ifdef NONASCII */ /* has been turned into command line flag - bkph */
  if (non_ascii)
  {
    for (i = first; i <= last; i++)
      buffer[i] = xord[buffer[i]];
  }
/* #endif */
  return true;
}

/* Read a line of input into buffer as efficiently as possible (ha ha)
   while still looking like Pascal.
   We set `last' to `first' and return `false' if we get to eof.
   Otherwise, we return `true' and set last = first +
   length(line except trailing whitespace).  */
/* texmfmp.c */

boolean input_line (FILE *f)
{
//  int ch, flag;         /* for restrict_to_ascii case 94/Jan/21 */
  char *u;            /* 1994/July/3 for key_replace */
  int i = '\0';

/*  and here is the long way of doing this */
  last = first;
/*  following is new version with tab expansion and key replacement */
/*  may want to expand out separately for speed 1994/July/3 */
/*  different versions depending on return_flag / tabexpand / key_replace */
/*  while (last < buf_size && (i = getc (f)) != EOF)  */
#ifdef ALLOCATEBUFFER
  for ( ; ; ) 
#else
  while (last < buf_size) 
#endif
  {
    i = getc(f);

    if (i < ' ')
    {
      if (i == EOF || i == '\n' || (i == '\r' && return_flag))
        break;
      else if (i == '\t' && tab_step != 0)
      {
        buffer[last++] = (ASCII_code) ' ';

#ifdef ALLOCATEBUFFER
        if (last >= current_buf_size)
        {
          buffer = realloc_buffer(increment_buf_size);  

          if (last >= current_buf_size)
            break;
        }
#endif

#ifdef ALLOCATEBUFFER
        while ((last - first) % tab_step != 0) 
#else
        while (last < buf_size && (last - first) % tab_step != 0)
#endif
        {

          buffer[last++] = (ASCII_code) ' ';

#ifdef ALLOCATEBUFFER
          if (last >= current_buf_size)
          {
            buffer = realloc_buffer(increment_buf_size);

            if (last >= current_buf_size)
              break;
          }
#endif
        }

        continue;
      }
    }

    if (key_replace && (u = replacement[i]) != NULL)
    {
#ifdef ALLOCATEBUFFER
      while (*u != '\0') 
#else
      while (last < buf_size && *u != '\0')  
#endif
      {
        buffer[last++] = (ASCII_code) *u++;
#ifdef ALLOCATEBUFFER
        if (last >= current_buf_size)
        {
          buffer = realloc_buffer(increment_buf_size);

          if (last >= current_buf_size)
            break;
        }
#endif
      }
    }
    else       /* normal case */
    {
      buffer[last++] = (ASCII_code) i;

#ifdef ALLOCATEBUFFER
      if (last >= current_buf_size)
      {
        buffer = realloc_buffer(increment_buf_size);

        if (last >= current_buf_size)
          break;
      }
#endif
    }
  }

  if (return_flag)    /* let return terminate line as well as newline */
  {
    if (i == '\r')      /* see whether return followed by newline */
    {
      i = getc (f);       /* in which case throw away the newline */

      if (i != '\n')
      {
        ungetc (i, f);
        i = '\r';
      }
/*      else  buffer[last-1] = (ASCII_code) i; */
    }
  }

  //  Turn Ctrl-Z at end of file into newline 2000 June 22
  if (i == EOF && trimeof && buffer[last - 1] == 26)
  {
    last--;
  }

  if (i == EOF && last == first)
    return false;

/*  Didn't get the whole line because buffer was too small?  */
/*  This shouldn't happen anymore 99/Jan/23 */
  if (i != EOF && i != '\n' && i != '\r')
  {
    complain_line(errout);

    if (log_opened)
      complain_line(log_file);

    /* This may no longer be needed ... now that we grow it */
    if (truncate_long_lines)
    {
      while (i != EOF && i != '\n' && i != '\r')
      {
        i = getc (f);     // discard rest of line
      }

      last--;       /* just in case */
    }
    else
      uexit(EXIT_FAILURE);      /* line too long */
  }

  return input_line_finish();
}

/* This string specifies what the `e' option does in response to an
   error message.  */

static char *edit_value = "c:\\yandy\\WinEdt\\WinEdt.exe [Open('%s');SelLine(%d,7)]";

/* This procedure is due to sjc@s1-c.  TeX (or Metafont) calls it when
   the user types `e' in response to an error, invoking a text editor on
   the erroneous source file.  FNSTART is how far into STRINGPOOL the
   actual filename starts; FNLENGTH is how long the filename is.
   
   See ../site.h for how to set the default, and how to override it.  */

/* called from close_files_and_terminate in  tex9.c */

void call_edit (ASCII_code *stringpool, pool_pointer fnstart, integer fnlength, integer linenumber)
{
  char *command, *s, *t, *u;
  char c;
  int sdone, ddone, ldone;
  int i, n;
  unsigned int commandlen;
  ASCII_code *texfilename;
  ASCII_code *log_file_name;
  pool_pointer lgstart;
  integer lglength;

  if (log_opened)
  {
    lgstart = str_start[texmf_log_name];
    lglength = length(texmf_log_name);
    log_file_name = stringpool + lgstart;
  }
  else
  {
    lglength = 0;
    log_file_name = (unsigned char *) "";
  }

  sdone = ddone = ldone = 0;
  texfilename = stringpool + fnstart;

  for (i = 1; i <= in_open; i++)
    (void) fclose (input_file[i]);

  n = fcloseall();

  if (n > 0 && verbose_flag)
  {
    sprintf(log_line, "Closed %d streams\n", n);
    show_line(log_line, 0);
  }

  s = kpse_var_value(edit_value);  

  if (s != NULL)
    edit_value = s;

  commandlen = strlen (edit_value) + fnlength + lglength + 10 + 1 + 2;
  command = (string) xmalloc (commandlen); 

  s = command;

  u = edit_value;

  while ((c = *u++) != 0)
  {
    if (c == '%')
    {
      switch (c = *u++)
      {
        case 'd':
          if (ddone)
          {
#ifdef _WIN32
            sprintf(log_line, "! bad command syntax (%c).\n", 'd');
            show_line(log_line, 1);
#else
            sprintf(log_line, "! `%%d' cannot appear twice in editor command.\n");
            show_line(log_line, 1);
#endif
            uexit(EXIT_FAILURE); 
          }

          (void) sprintf (s, "%d", linenumber);

          while (*s != '\0')
            s++;

          ddone = 1;
          break;

        case 's':
          if (sdone)
          {
#ifdef __WIN32
            sprintf(log_line, "! bad command syntax (%c).\n", 's'); 
            show_line(log_line, 1);
#else
            sprintf(log_line, "! `%%s' cannot appear twice in editor command.\n");
            show_line(log_line, 1);
#endif
            uexit(EXIT_FAILURE); 
          }

          t = (char *) texfilename;
          n = fnlength;

          if (non_ascii)
            for (i = 0; i < n; i++)
              *s++ = xchr [*t++];
          else
            for (i = 0; i < n; i++)
              *s++ = (char) *t++;

          sdone = 1;
          break;

        case 'l':
          if (ldone)
          {
#ifdef __WIN32
            sprintf(log_line, "! bad command syntax (%c).\n", 'l'); 
            show_line(log_line, 1);
#else
            sprintf(log_line, "! `%%l' cannot appear twice in editor command.\n");
            show_line(log_line, 1);
#endif
            uexit(EXIT_FAILURE); 
          }

          t = (char *) log_file_name;
          n = lglength;

          if (non_ascii)
            for (i = 0; i < n; i++)
              *s++ = xchr [*t++];
          else
            for (i = 0; i < n; i++)
              *s++ = (char) *t++;

          ldone = 1;
          break;

        case '\0':
          *s++ = '%'; 
          u--;
          break;

        default:
          *s++ = '%';
          *s++ = c;
          break;
      }
    }
    else
      *s++ = c;
  }

  *s = 0; /* terminate the command string */

  if (strlen(command) + 1 >= commandlen)
  {
    sprintf(log_line, "Command too long (%d > %d)\n", strlen(command) + 1, commandlen);
    show_line(log_line, 1);
    uexit(EXIT_FAILURE);
  }

  //flushall();
  fflush(NULL);

  if (system (command) != 0)
  {
    show_line("\n", 0);
    sprintf(log_line, "! Error in call: %s\n", command);
    show_line(log_line, 1);

#ifdef __WIN32
    if (errno != 0)
      perrormod("! DOS says");
#endif

    sprintf(log_line, "  (TEXEDIT=%s)\n", edit_value);
    show_line(log_line, 0);
    puts("  (Editor specified may be missing or path may be wrong)\n");
    puts("  (or there may be missing -- or extraneous -- quotation signs)\n");
  }

  uexit(EXIT_FAILURE);
}


#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)

/* We don't REALLY care what `endian' the machine is after all ! */
/* But we do care about speed - so check exe file for following - bkph */

// #ifdef MYDEBUG
// char swapmarkerstring="ERROR: SWAPPING - NOT BigEndian AND NOT NoFmtBaseSwap";
// #endif

/* This macro is always invoked as a statement.  It assumes a variable
   `temp'.  */
   
#define SWAP(x, y) temp = (x); (x) = (y); (y) = temp;


/* Make the NITEMS items pointed at by P, each of size SIZE, be the
   opposite-endianness of whatever they are now.  */

static int swap_items (char *p, int nitems, int size)
{
  char temp;

  /* Since `size' does not change, we can write a while loop for each
     case, and avoid testing `size' for each time.  */
  switch (size)
  {
    case 8:
      while (nitems--)
      {
        SWAP (p[0], p[7]);
        SWAP (p[1], p[6]);
        SWAP (p[2], p[5]);
        SWAP (p[3], p[4]);
        p += size;
      }
      break;

    case 4:
      while (nitems--)
      {
        SWAP (p[0], p[3]);
        SWAP (p[1], p[2]);
        p += size;
      }
      break;

    case 2:
      while (nitems--)
      {
        SWAP (p[0], p[1]);
        p += size;
      }
      break;

    case 1:
    /* Nothing to do.  */
      break;

    default:
      show_line("\n", 0);
      sprintf(log_line, "! I can't (un)dump a %d byte item.\n", size);
      show_line(log_line, 1);
      uexit(EXIT_FAILURE);
  }
  return 0;
}
#endif /* not WORDS_BIGENDIAN and not NO_FMTBASE_SWAP */

#ifdef COMPACTFORMAT
int do_dump(char *p, int item_size, int nitems, gzFile out_file)
#else
int do_dump(char *p, int item_size, int nitems, FILE *out_file)
#endif
{
#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)
  swap_items (p, nitems, item_size);
#endif

#ifdef COMPACTFORMAT
  if (gzwrite(out_file, p, (item_size * nitems)) != (item_size * nitems))
#else
  if ((int) fwrite(p, item_size, nitems, out_file) != nitems)
#endif
  {
    show_line("\n", 0);
    sprintf(log_line, "! Could not write %d %d-byte item%s.\n",
               nitems, item_size, (nitems > 1) ? "s" : "");
    show_line(log_line, 1);
    uexit(EXIT_FAILURE);
  }

/* Have to restore the old contents of memory, since some of it might get used again.  */
#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)
  swap_items (p, nitems, item_size);
#endif

  return 0;
}

#ifdef COMPACTFORMAT
int do_undump(char *p, int item_size, int nitems, gzFile in_file)
#else
int do_undump(char *p, int item_size, int nitems, FILE *in_file)
#endif
{
#ifdef COMPACTFORMAT
  if (gzread(in_file, (void *) p, (unsigned int) (item_size * nitems)) <= 0)
#else
  if ((int) fread((void *) p, item_size, nitems, in_file) != nitems)
#endif
  {
    show_line("\n", 0);
    sprintf(log_line, "! Could not read %d %d-byte item%s.\n",
               nitems, item_size, (nitems > 1) ? "s" : "");
    show_line(log_line, 1);
    uexit(EXIT_FAILURE);
  }

#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)
  swap_items (p, nitems, item_size);
#endif

  return 0;
}

#ifdef FUNNY_CORE_DUMP

void funny_core_dump (void)
{
  int pid, w;
  union wait status;

  switch (pid = vfork ())
  {
    case -1:
      perrormod ("vfork");
      exit (-1);

    case 0:
       (void) signal (SIGQUIT, SIG_DFL);
       (void) kill (getpid (), SIGQUIT);
       (void) write (2, "how did we get here?\n", 21);
       exit (1);

    default:
      while ((w = wait (&status)) != pid && w != -1)
        ;

      if (status.w_coredump)
        exit (0);

      (void) write (2, "attempt to dump core failed\n", 28);
      exit (1);
  }
}
#endif /* FUNNY_CORE_DUMP */