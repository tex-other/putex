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

#define	EXTERN			/* Instantiate data in `texd.h' or `mfd.h' here ! */

#include "texd.h"

/* Hand-coded routines for TeX or Metafont in C.  This code was (mostly)
   written by Tim Morgan, drawing from other Unix ports of TeX.  */

/* Either `texd.h' or `mfd.h' will include `../common/texmf.h'.  */

/* Note: INITEX definition in makefile only affects included *.h files */

#ifdef MSDOS
//  int __cdecl read (int, void *, unsigned int);
#endif

/* Instantiate data in `texd.h' or `mfd.h' here.  */

#ifdef TeX
  #define dump_default_var    TEX_format_default
  #define dump_default        " plain.fmt"
  #define dump_format         " %s.fmt"
  #define dump_ext_length     4
  #define dump_default_length format_default_length
  #define virgin_program      "virtex"
  #define main_program        texbody
  #define edit_value          tex_edit_value
  #define edit_var            "UFYFEJU" /* shrouded 93/Nov/20 */
#endif

#include <ctype.h>				// needed for isascii and isalpha

#define ISSPACE(c) (isascii (c) && isspace(c))

// #include "c-ctype.h"
// #include "c-pathch.h"

/* For `struct tm'.  */

#include <time.h>		// needed for time, struct tm etc.

extern struct tm *localtime ();

/* Catch interrupts.  */
#include <signal.h>		// needed for signal, SIGINT, SIG_IGN, SIG_ERR etc.

/* following may be found in local.c --- used for key replacement */

extern char *replacement[];		/* pointers to replacement strings */

/* extern char *buffercopy; */	/* pointer to allocated copy of buffer */

// extern char *grabenv(char *);				/* in local.c - bkph */

/* extern void try_and_open(char *); */	/* inb local.c */

#ifdef FUNNY_CORE_DUMP
  void funny_core_dump (void);
#endif

/* ridderbusch.pad@nixdorf.com says this is necessary.  */
#ifdef ATARI_ST
  int _stksize = -1L;
#endif

/* The main program, etc.  */

/* What we were invoked as and with.  */
static char *program_name = NULL;
int gargc;			/* number of args - set to zero after initialization */
char **gargv;		/* char *gargv[] -- bkph ? */

/* The entry point: set up for reading the command line, which will
   happen in `t_open_in', then call the main body.  */

#ifdef MSDOS
  int init(int, char **);			/* in local.c */
#endif /* INIVIR */ 

char *set_program_name(char *comm)
{			/* bkph */
	char *s;
	if ((s = strrchr (comm, '\\')) != NULL) s++;
	else if ((s = strrchr (comm, '/')) != NULL) s++;
	else if ((s = strrchr (comm, ':')) != NULL) s++;
	else s = comm;
/*	program_name = s; */
	return s;
}

int jump_used=0;

jmp_buf jumpbuffer;		// for non-local jumps

int main (int ac, char *av[])
{
	int flag=0, ret=0;
#ifndef INI
	char custom_default[PATH_MAX];
#endif
	gargc = ac;					/* make available globally */
	gargv = av;					/* make available globally */
#ifdef MSDOS
	program_name = set_program_name(av[0]);		/* rewritten 1994/Mar/1 - bkph */
#else
	program_name = strrchr (av[0], PATH_SEP); 
	if (program_name == NULL) program_name = av[0];
	else program_name++; 
#endif

#ifdef MSDOS
	if (init(gargc, gargv))		/* in local.c */
		return -1;				// failure
#endif /* INIVIR */

	dump_default_var = dump_default;
/*  dump_default_length = strlen (dump_default + 1); */
	dump_default_length = strlen (dump_default_var + 1);	/* 93/Nov/20 */

/* The following doesn't make sense on DOS since we can't core dump */

#ifndef INI
	if (ready_already != 314159) {
#ifdef MSDOS
		program_name = set_program_name(av[0]);	/* rewritten 1994/Mar/1 - bkph */
#else
		program_name = strrchr (av[0], PATH_SEP);
		if (program_name == NULL)	program_name = av[0];
		else program_name++; 
#endif
		if (strcmp (program_name, virgin_program) != 0)	{
/*		  TeX or Metafont adds the space at the end of the name.  */
			(void) sprintf (custom_default, dump_format, program_name);
			dump_default_var = custom_default;
			dump_default_length = strlen (program_name) + dump_ext_length;
		}
	}
#endif /* not INI */

//	call main_program = texbody in itex.c
//	now creates jump buffer for non-local goto's  99/Nov/7

	jump_used = 0;
	ret = setjmp(jumpbuffer);
	if (ret == 0) {	// get here when setting up jumpbuffer
		flag = main_program ();		// texbody in itex.c
		if (trace_flag) {
			sprintf(log_line, "EXITING at %s %d %d %d\n", "MAIN", flag, ret, jump_used);
			show_line(log_line, 0);
		}
	} else {	// get here from non-local jump via jumpbuffer - if any
		if (trace_flag) {
			sprintf(log_line, "EXITING at %s %d %d %d\n", "JUMPOUT", flag, ret, jump_used);
			show_line(log_line, 0);
		}
	}

//	ABORTCHECKZERO;				// after storefmtfiles e.g.

	if (endit(flag) != 0) flag = 1;	/* do final clean up in local.c */
	if (flag == 0) return 0;
#ifdef _WINDOWS
	return flag;
#else
	else exit (flag);		// avoid circularity!
#endif
}	/* end of main */

/* This is supposed to ``open the terminal for input'', but what we
   really do is copy command line arguments into TeX's or Metafont's
   buffer, so they can handle them.  If nothing is available, or we've
   been called already (and hence, gargc==0), we return with
   `last=first'.  */

void t_open_in (void)
{
/* int ch, flag; */
	int i;

/*	if (trace_flag) show_line("Entering t_open_in (texmf)\n", 0); */

	buffer[first] = 0;	/* So the first `strcat' will work.  */

/*	We just string the command line arguments together with spaces */
#ifdef MSDOS
/*  if (gargc > optind) { 	*/ /* command line stuff after arguments ? */
	if (gargc > optind && optind > 0) { 	/* command line arguments? 94/Apr/10 */
/*      for (i = 1; i < gargc; i++)	*/
		for (i = optind; i < gargc; i++) 			/* 94/Apr/10 */
#else
			if (gargc > 1) {			/* We do have command line arguments.  */
				for (i = 1; i < gargc; i++)
#endif
				{
/*	the following won't happen if pseudo_space is set ... */
					if (allow_quoted_names && strchr(gargv[i], ' ') != NULL) {
						(void) strcat ((char *) &buffer[first], "\"");
						(void) strcat ((char *) &buffer[first], gargv[i]);
						(void) strcat ((char *) &buffer[first], "\"");
					}
					else (void) strcat ((char *) &buffer[first], gargv[i]);
					(void) strcat ((char *) &buffer[first], " ");
				}
				gargc = 0;	/* Don't do this again.  */
			}

  /* Find the end of the buffer.  */
		for (last = first; buffer[last]; ++last) ;

  /* Make `last' be one past the last non-blank non-formfeed character
     in `buffer'.  */
		for (--last; last >= first
			 && ISSPACE (buffer[last]) && buffer[last] != '\f'; --last)  ;
		last++;

/* do we want to check line for non-ASCII at this point ? */

  /* One more time, this time converting to TeX's internal character
     representation.  */ /* for command line input in this case */
/* #ifdef NONASCII */
		if (non_ascii) {
			for (i = first; i < last; i++)
				buffer[i] = xord[buffer[i]];
		}
/* #endif */
}

/* All our interrupt handler has to do is set TeX's or Metafont's global
   variable `interrupt'; then they will do everything needed.  */

static RETSIGTYPE
/* catch_interrupt () */
catch_interrupt (int err)
{			/* NOTE: err is unreferenced - bkph */
  (void) signal (SIGINT, SIG_IGN);	/* turn off interrupts for now */
/*  interrupt = 1; */				/* make sure interrupt declared volatile */
//  if (interrupt++ >= 3) uexit(1);	/* emergency exit -- bkph */
  if (interrupt++ >= 3) exit(1);	/* emergency exit -- bkph */
  (void) signal (SIGINT, catch_interrupt);  /* turn them back on again */
}

/* Besides getting the date and time here, we also set up the interrupt
   handler, for no particularly good reason.  It's just that since the
   `fix_date_and_time' routine is called early on (section 1337 in TeX,
   ``Get the first line of input and prepare to start''), this is as
   good a place as any.  */

void get_date_and_time (integer *minutes, integer *pday, integer *pmonth, integer *pyear)
{
/*	int sig=0; */
	time_t clock;
	struct tm *tmptr;

/*	time_t clock = time ((long *) 0); */
/*	clock = time (NULL); */
	(void)  time (&clock);	/* - seconds since 1970 */ 
	if (trace_flag) {
		sprintf(log_line, "The time is %u\n", clock); 	/* debugging */
		show_line(log_line, 0);		
	}
	if (clock < 0) {
		show_line("Time not available!\n", 1);
/*		clock = 0; *//* 901621283 1998 July 28 06:21:00 */
	}
	tmptr = localtime (&clock);
/*	MS C runtime library has trouble for clock >= 2^31 !!! */
	if (tmptr == NULL) {						/* debugging 95/Dec/30*/
		sprintf(log_line, "Cannot convert time (%0ld)!\n", clock);
		show_line(log_line, 1);
		*pyear=2038; *pmonth=1; *pday=18; *minutes=22 * 60 + 14;
	}
	else {
		*minutes = tmptr->tm_hour * 60 + tmptr->tm_min;
		*pday    = tmptr->tm_mday;
		*pmonth  = tmptr->tm_mon + 1;
		*pyear   = tmptr->tm_year + 1900;
		if (trace_flag) {
			sprintf(log_line, "%d-%d-%d %d:%d\n",
			tmptr->tm_year + 1900, tmptr->tm_mon + 1, tmptr->tm_mday,
				tmptr->tm_hour, tmptr->tm_min);
			show_line(log_line, 0);
		}
	}

  {
#ifdef MSDOS
	if (!no_interrupts) {
		if (signal(SIGINT, catch_interrupt)== SIG_ERR) {
			show_line(" CTRL-C handler not installed\n", 0);
#ifndef _WINDOWS
			uexit(1);  /* do we care when run as DLL ? */
#endif
		}
	}
#else
    RETSIGTYPE (*old_handler) ();

    if ((old_handler = signal (SIGINT, catch_interrupt)) != SIG_DFL)
      (void) signal (SIGINT, old_handler);
#endif
  }
}

/* I/O for TeX and Metafont.  */ /* give file name ? */

void complain_line (FILE *output)
{
	show_line("\n", 0);
#ifdef ALLOCATEBUFFER
	sprintf(log_line, "! Unable to read an entire line---buf_size=%d.\n", current_buf_size);
#else
	sprintf(log_line, "! Unable to read an entire line---buf_size=%d.\n",	buf_size);
#endif
	if (output == stderr) show_line(log_line, 1);
	else if (output == stdout) show_line(log_line, 0);
	else fputs(log_line, output);			// never
	show_line("  (File may have a line termination problem.)", 0);
}

void show_bad_line (FILE *output, int first, int last)
{	/* 1994/Jan/21 */
	int i, c, d, ch;
	char *s=log_line;
	for (i = first; i <= last; i++) {
		ch = buffer[i];
	    if ((show_in_hex && ch > 127)) {
			c = ch >> 4; d = ch & 15; 
			if (c > 9) c = c + 'a' - 10;
			else c = c + '0';
			if (d > 9) d = d + 'a' - 10;
			else d = d + '0';
//			putc('^', output); putc('^', output);
			*s++ = '^'; *s++ = '^';
//			putc (c, output); putc (d, output);
			*s++ = (char) c; *s++ = (char) d;
		}
		else if (ch < 32) {
//			putc('^', output); putc('^', output);
			*s++ = '^'; *s++ = '^';
//			putc (ch + 64, output); 
			*s++ = (char) (ch+64);
		}
		else if (ch == 127) {
//			putc('^', output); putc('^', output);
			*s++ = '^'; *s++ = '^';
//			putc (ch - 64, output); 
			*s++ = (char) (ch-64);
		}
		else {
//			putc(ch, output);
			*s++ = (char) ch;
		}
	}
//	putc(' ', output);		/*	putc('\n', output); */
	*s++ = ' ';
	*s++ = '\0';
	if (output == stderr) show_line(log_line, 1);
	else if (output == stdout) show_line(log_line, 0);
	else fputs(log_line, output);		// log_file
}

// split off for convenience and use in ConsoleInput

bool input_line_finish (void) {
	int i = '\0';
	int ch, flag;
	
/*	if last line in file does not end with \n - never happens ? */
/*	if (i == EOF && buffer[last] != '\n') buffer[last++] = '\n'; */

	buffer[last] = ' ';						/* space terminate */
	if (last >= max_buf_stack)  max_buf_stack = last;	/* remember longest line */

/* Trim trailing whitespace.  */ 
/* #define isblank(c) ((c) == ' ' || (c) == '\t') */
/* What about \n ?  Can't get in here ?- bkph */
/* What about control-Z that gets read in binary mode ? - bkph */
// #ifdef MYDEBUG
/*  while (last > first && buffer[last - 1] <= ' ')  --last; */
	while (last > first) {
		i = buffer[last - 1];
		if (i == ' ' || i == '\t') --last;
/*	  else if (trimeof && i == 26) --last;	 */		/* 93/Nov/24 */
		else break;
	}
/*  if (trimeof != 0 && i == EOF && last == first)	
      return false; */							/* EOF and line empty */
// #else
//   while (last > first
//         && isblank (buffer[last - 1]) && buffer[last - 1] != '\r')
//    --last;
// #endif

/* following added to check source file integrity ASCII 32 - 127 */
/* allow space, tab, new-page - also allow return, newline ? */
	if (restrict_to_ascii) {
		flag = 0;
		for (i = first; i <= last; i++) {
			ch = buffer[i];
/*		  if (ch > 127 || (ch < ' ' && ch != '\t' && ch != '\f')) */
/*		  1 -- 8, 11, 14 -- 31 are not good ASCII characters */
			if (ch > 126 ||  (ch < ' ' && ch != '\t' && ch != '\f'
							  && ch != '\r' && ch != '\n')) {
				sprintf(log_line, "\n! non ASCII char (%d) in line: ", ch);
				show_line(log_line, 1);
				if (log_opened) 
					fprintf(log_file, "\n! non ASCII char (%d) in line: ", ch);
/*			  buffer[i]= 127; */ /* not defined - invalid char */
				flag = 1;
				break;
			}
		}
		if (flag) {
			show_bad_line(errout, first, last);
			if (log_opened)  show_bad_line(log_file, first, last);
		}
	}
/* Don't bother using xord if we don't need to. */ /* for input line */
/* #ifdef NONASCII */ /* has been turned into command line flag - bkph */
	if (non_ascii) {
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

bool input_line (FILE *f)
{
//	int ch, flag;					/* for restrict_to_ascii case 94/Jan/21 */
	char *u;						/* 1994/July/3 for key_replace */
	int i='\0';

/*	and here is the long way of doing this */
	last = first;
/*  following is new version with tab expansion and key replacement */
/*	may want to expand out separately for speed 1994/July/3 */
/*	different versions depending on return_flag / tabexpand / key_replace */
/*	while (last < buf_size && (i = getc (f)) != EOF)  */
#ifdef ALLOCATEBUFFER
	for (;;) 
#else
	while (last < buf_size) 
#endif
	{
		i = getc (f);
		if (i < ' ') {	  /* isolate the more expensive tests */
			if (i == EOF || i == '\n' || (i == '\r' && return_flag)) break;
			else if (i == '\t' && tab_step != 0) {	// deal with tab
/*				i = ' '; */
				buffer[last++] = (ASCII_code) ' ';
#ifdef ALLOCATEBUFFER
				if (last >= current_buf_size) {
					buffer = realloc_buffer(increment_buf_size);	
					if (last >= current_buf_size) break;
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
					if (last >= current_buf_size) {
						buffer = realloc_buffer(increment_buf_size);	
						if (last >= current_buf_size) break;
					}
#endif
				}
				continue;
			}
		}
		if (key_replace && (u = replacement[i]) != NULL) {
#ifdef ALLOCATEBUFFER
			while (*u != '\0') 
#else
			while (last < buf_size && *u != '\0')  
#endif
			{
				buffer[last++] = (ASCII_code) *u++;
#ifdef ALLOCATEBUFFER
				if (last >= current_buf_size) {
					buffer = realloc_buffer(increment_buf_size);
					if (last >= current_buf_size) break;
				}
#endif
			}
		}
		else {				/* normal case */
			buffer[last++] = (ASCII_code) i;
#ifdef ALLOCATEBUFFER
			if (last >= current_buf_size) {
				buffer = realloc_buffer(increment_buf_size);
				if (last >= current_buf_size) break;
			}
#endif
		}
	}		// end of for(;;) or while loop

//	can break out of above on EOF '\n' or '\r
//	sprintf(log_line, "BREAK on %d at %ld\n", i, ftell(f));
//	show_line(log_line, 0);	// debugging only

	if (return_flag) {		/* let return terminate line as well as newline */
	  if (i == '\r') {			/* see whether return followed by newline */
		  i = getc (f);				/* in which case throw away the newline */
		  if (i != '\n')  {
			  ungetc (i, f);
			  i = '\r';
		  }
/*		  else  buffer[last-1] = (ASCII_code) i; */
	  }
	}

//	sprintf(log_line, "first %d last %d\n", first, last);
//	show_line(log_line, 0);		// debugging only
//	strncpy(log_line, &buffer[first], last - first + 1);
//	log_line[last-first] = '\n';
//	log_line[last-first+1] = '\0';
//	show_line(log_line, 0);		// debugging only

//	Turn Ctrl-Z at end of file into newline 2000 June 22
//	if (i == EOF && trimeof != 0 && buffer[last-1] == 26) last--;	/* ^Z */
	if (i == EOF && trimeof && buffer[last-1] == 26) {
//		buffer[last-1] = 10;	/* ^J */
//		buffer[last] = '\0';
		last--;
//		sprintf(log_line, "CTRL-Z first %d last %d\n", first, last);
//		show_line(log_line, 0);	// debugging only
	}
	if (i == EOF && last == first)
		return false;		/* EOF and line empty - true end of file */

/*	Didn't get the whole line because buffer was too small?  */
/*	This shouldn't happen anymore 99/Jan/23 */
	if (i != EOF && i != '\n' && i != '\r')  {
		complain_line(errout);
		if (log_opened) complain_line(log_file);	/* ? 93/Nov/20 */
/*		This may no longer be needed ... now that we grow it */
		if (truncate_long_lines) {				/* 98/Feb/3 */
			while (i != EOF && i != '\n' && i != '\r')  {
				i = getc (f);			// discard rest of line
			}
			last--;				/* just in case */
		}
		else uexit(1);			/* line too long */
	}
	return input_line_finish();
}	/* end of input_line */


/* This string specifies what the `e' option does in response to an
   error message.  */ 

static char *edit_value = EDITOR;

void unshroud_string (char *real_var, char *var, int n) {
	int c;
	char *s=real_var;
	char *t=var;
	
/*	while ((c = *t++) != '\0' && n-- > 0) *s++ = (char) (c - 1); */
	while ((c = *t++) != '\0' && --n > 0) *s++ = (char) (c - 1);
	if (n >= 0)	*s = (char) c;
	else *s = '\0';				/* terminate it anyway */
} /* 93/Nov/20 */

char *get_env_shroud (char *var) {
	char real_var[32];
	char *real_value;

	unshroud_string (real_var, var, sizeof(real_var));
/*	real_value = getenv(real_var); */			/* 1994/Mar/1 */
	real_value = grabenv(real_var);				/* 1994/Mar/1 */
	if (trace_flag) {
		sprintf(log_line, "\nset %s=", real_var);
		show_line(log_line, 0);
		if (real_value != NULL) {
			show_line(real_value, 0);
		}
		show_line("\n", 0);
	}
/*	return get_env_shroud (real_var); */	/* serious bug ! since 93/Nov/20 */
/*	return getenv (real_var);	*/		/* fixed 93/Dec/28 */
	return real_value;					/* 94/Mar/1 */
} 	/* 93/Nov/20 */

/* This procedure is due to sjc@s1-c.  TeX (or Metafont) calls it when
   the user types `e' in response to an error, invoking a text editor on
   the erroneous source file.  FNSTART is how far into STRINGPOOL the
   actual filename starts; FNLENGTH is how long the filename is.
   
   See ../site.h for how to set the default, and how to override it.  */

/* called from close_files_and_terminate in  tex9.c */

void call_edit (ASCII_code *stringpool, pool_pointer fnstart,
			  integer fnlength, integer linenumber) {
	char *command, *s, *t, *u;
	char c;
	int sdone, ddone, ldone;
	int i, n;
	unsigned int commandlen;
	ASCII_code *texfilename;
	ASCII_code *log_file_name;
	pool_pointer lgstart;					/* 1994/Jan/94 */
	integer lglength;						/* 1994/Jan/94 */

	if (log_opened) {						/* 1994/Aug/10 */
		lgstart = str_start[texmf_log_name];
		lglength = str_start[texmf_log_name + 1]- str_start[texmf_log_name];
		log_file_name = stringpool + lgstart;
	}
	else {								/* 1994/Aug/10 */
		lglength = 0;
		log_file_name = (unsigned char *) "";
	}

	sdone = ddone = ldone = 0;
/*  filename += fnstart; */
	texfilename = stringpool + fnstart;

/*	Close any open input files, since we're going to kill the job.  */
/*	and since the editor will need access to them... */
	for (i = 1; i <= in_open; i++) (void) fclose (input_file[i]);

	n = fcloseall();						/* paranoia 1994/Aug/10 */
	if (n > 0 && verbose_flag) {
		sprintf(log_line, "Closed %d streams\n", n);
		show_line(log_line, 0);
	}

/*	Replace the default with the value of the appropriate environment
    variable, if it's set.  */
/*  s = getenv (edit_var);   */		/* 93/Nov/20 */
	s = get_env_shroud (edit_var);	
	if (s != NULL) edit_value = s;	/* OK, replace wired in default */

/*	Construct the command string.  */
/*	The `11' is the maximum length a 32 bit integer might be, plus one for null.  */
/*	Plus 2 for quotes if needed 99/May/31 */
/*  command = (string) xmalloc (strlen (edit_value) + fnlength + 11); */
	commandlen = strlen (edit_value) + fnlength + lglength + 10 + 1 + 2;
    command = (string) xmalloc (commandlen); 
/*  make more space for log_file_name 1994/Jan/26 */
/*	So we can construct it as we go.  */
	s = command;

/*	should we manipulate edit_value first ? Add quotes if space in exe name ? */
/*	remove quotes around [...] string for WinEdt ? */

	u = edit_value;
	while ((c = *u++) != 0) {
		if (c == '%') {					/* handle special codes */
			switch (c = *u++)  {
				case 'd':
					if (ddone) {
#ifdef MSDOS
						sprintf(log_line,
								"! bad command syntax (%c).\n", 'd');
						show_line(log_line, 1);
#else
						sprintf(log_line,
								"! `%%d' cannot appear twice in editor command.\n");
						show_line(log_line, 1);
#endif
						uexit(1); 
					}
					(void) sprintf (s, "%d", linenumber);
					while (*s != '\0') s++;
					ddone = 1;			/* indicate already used %d */
					break;

				case 's':
					if (sdone) {
#ifdef MSDOS
						sprintf(log_line,
								"! bad command syntax (%c).\n", 's'); 
						show_line(log_line, 1);
#else
						sprintf(log_line,
								"! `%%s' cannot appear twice in editor command.\n");
						show_line(log_line, 1);
#endif
						uexit(1); 
					}
					t = (char *) texfilename;
					n = fnlength;

/* following modified to allow non ASCII - bkph */ /* for file names */
					if (non_ascii)
/*				for (i = 0; i < fnlength; i++)  *s++ = xchr [filename[i]]; */
						for (i = 0; i < n; i++)  *s++ = xchr [*t++];
					else
/*				for (i = 0; i < fnlength; i++)  *s++ = (char) filename[i]; */
						for (i = 0; i < n; i++)  *s++ = (char) *t++;
					sdone = 1;			/* indicate already used %s */
					break;

				case 'l':						/* 1994/Jan/28 */
					if (ldone) {
#ifdef MSDOS
						sprintf(log_line, 
								 "! bad command syntax (%c).\n", 'l'); 
						show_line(log_line, 1);
#else
						sprintf(log_line,
								"! `%%l' cannot appear twice in editor command.\n");
						show_line(log_line, 1);
#endif
						uexit(1); 
					}
					t = (char *) log_file_name;
					n = lglength;				/* 1994/Jan/28 */

/* following modified to allow non ASCII - bkph */ /* for file names */
					if (non_ascii)
/*			for (i = 0; i < fnlength; i++)  *s++ = xchr [filename[i]]; */
						for (i = 0; i < n; i++)  *s++ = xchr [*t++];
					else
/*			for (i = 0; i < fnlength; i++)  *s++ = (char) filename[i]; */
						for (i = 0; i < n; i++)  *s++ = (char) *t++;
					ldone = 1;			/* indicate already used %l */
					break;

				case '\0':			/*  '%'  at end of line */
					*s++ = '%';	
					u--;	/* Back up to the null to force termination.  */
					break;

				default:			/* something other than 's', 'd', 'l' follows */
					*s++ = '%';
					*s++ = c;
					break;
			}
		}
		else *s++ = c;			/* ordinary character pass it through */
	}

	*s = 0;					/* terminate the command string */
	if (strlen(command) + 1 >= commandlen) {	/* should not happen! */
		sprintf(log_line,
				"Command too long (%d > %d)\n", strlen(command) + 1, commandlen);
		show_line(log_line, 1);
		uexit(1); 
	}

/*	You must explicitly flush (using fflush or _flushall) or close any stream before calling system. */
	_flushall();
/*	Try and execute the command.  */
/*	There may be problem here with long names and spaces ??? */
/*	Use _exec or _spawn instead ??? */

	if (system (command) != 0) {
//		fprintf (errout, "\n");
		show_line("\n", 0);
//		fprintf (errout,
		sprintf(log_line,
				 "! Error in call: %s\n", command); /* shroud ? */
		show_line(log_line, 1);
/*		errno seems to be 0 typically, so perror says "no error" */
#ifdef MSDOS
		if (errno != 0) perrormod("! DOS says");			/* 94/Aug/10 - bkph */
#endif
		sprintf(log_line, "  (TEXEDIT=%s)\n", edit_value);
		show_line(log_line, 0);
		show_line("  (Editor specified may be missing or path may be wrong)\n", 0);
		show_line("  (or there may be missing -- or extraneous -- quotation signs)\n", 0);
	}
	uexit(1);				/*	Quit, since we found an error.  */
}


/* Read and write format (for TeX) or base (for Metafont) files.  In
   tex.web, these files are architecture dependent; specifically,
   BigEndian and LittleEndian architectures produce different files.
   These routines always output BigEndian files.  This still does not
   make the dump files architecture-independent, because it is possible
   to make a format file that dumps a glue ratio, i.e., a floating-point
   number.  Fortunately, none of the standard formats do that.  */

#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP) /* this fn */

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

static int swap_items (char *p, int nitems, int size) {
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
			uexit(1);
	}
	return 0;
}
#endif /* not WORDS_BIGENDIAN and not NO_FMTBASE_SWAP */

/* Hmm, this could benefit from some on the fly compression - bkph */
/* and complementary decompression on input - bkph */

/* Here we write NITEMS items, each item being ITEM_SIZE bytes long.
   The pointer to the stuff to write is P, and we write to the file
   OUT_FILE.  */

int do_dump (char *p, int item_size, int nitems, FILE *out_file)
{
#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)
	swap_items (p, nitems, item_size);
#endif
/*  if (fwrite (p, item_size, nitems, out_file) != nitems) */ /* bkph */
	if ((int) fwrite (p, item_size, nitems, out_file) != nitems){
		show_line("\n", 0);
		sprintf(log_line, "! Could not write %d %d-byte item%s.\n",
               nitems, item_size, (nitems > 1) ? "s" : "");
		show_line(log_line, 1);
		uexit(1);
    }

  /* Have to restore the old contents of memory, since some of it might
     get used again.  */
#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)
	swap_items (p, nitems, item_size);
#endif
	return 0;
}

/* Hmm, this could benefit from some on the fly decompression - bkph */

/* Here is the dual of the writing routine.  */
int do_undump (char *p, int item_size, int nitems, FILE *in_file)
{
/*  if (fread(p, item_size, nitems, in_file) != nitems) */ /* bkph */
/*	try and speed this up a bit using read() ? bkph 93/Nov/26 */
/*	doubt whether it makes any real difference ... so forget it ! */
#ifdef MSDOS_HACK
	unsigned int nbytes = item_size * nitems;
    if ((unsigned int) read(fileno (in_file), p, nbytes) != nbytes) {
		show_line("\n", 0);
		sprintf(log_line, "! Could not read %d %d-byte item%s.\n",
               nitems, item_size, (nitems > 1) ? "s" : "");
		show_line(log_line, 1);
		uexit(1);
    }
#else
    if ((int) fread((void *) p, item_size, nitems, in_file) != nitems) {
		show_line("\n", 0);
		sprintf(log_line, "! Could not read %d %d-byte item%s.\n",
               nitems, item_size, (nitems > 1) ? "s" : "");
		show_line(log_line, 1);
		uexit(1);
    }
#endif

#if !defined (WORDS_BIGENDIAN) && !defined (NO_FMTBASE_SWAP)
	swap_items (p, nitems, item_size);
#endif
	return 0;
}


#ifdef FUNNY_CORE_DUMP
/* This procedure is due to chris@mimsy.umd.edu.  It makes a core dump
   without any sort of error status (abort(2) does return an error status,
   so we don't want to use that).  It is used only when making a preloaded
   TeX from virtex, and is triggered by a magic file name requested as
   input (see `open_input', above).  */

void funny_core_dump () {
  int pid, w;
  union wait status;

  switch (pid = vfork ())
    {
    case -1:		/* failed */
      perrormod ("vfork");
      exit (-1);      /* NOTREACHED */

    case 0:             /* child */
       (void) signal (SIGQUIT, SIG_DFL);
       (void) kill (getpid (), SIGQUIT);
       (void) write (2, "how did we get here?\n", 21);
       exit (1);       /* NOTREACHED */

    default:		/* parent */
      while ((w = wait (&status)) != pid && w != -1)
	;
      if (status.w_coredump)
	exit (0);
      (void) write (2, "attempt to dump core failed\n", 28);
      exit (1);
    }
}
#endif /* FUNNY_CORE_DUMP */

#ifndef TeX
/* On-line display routines for Metafont.  Here we use a dispatch table
   indexed by the MFTERM or TERM environment variable to select the
   graphics routines appropriate to the user's terminal.  stdout must be
   connected to a terminal for us to do any graphics.  */

/* We don't want any other window routines screwing us up if we're
   trying to do the trap test.  We could have written another device for
   the trap test, but the terminal type conditionals in initscreen argue
   against that.  */

#if defined (TRAP) || defined (INI)
#undef HP2627WIN
#undef SUNWIN
#undef XVIEWWIN
#undef TEKTRONIXWIN
#undef UNITERMWIN
#undef X10WIN
#undef X11WIN
#endif

#ifdef MSDOS
extern mf_pm_initscreen (), mf_pm_updatescreen ();
extern mf_pm_blankrectangle (), mf_pm_paintrow ();
#endif


#ifdef HP2627WIN
extern mf_hp2627_initscreen (), mf_hp2627_updatescreen ();
extern mf_hp2627_blankrectangle (), mf_hp2627_paintrow ();
#endif

#ifdef SUNWIN
extern mf_sun_initscreen (), mf_sun_updatescreen ();
extern mf_sun_blankrectangle (), mf_sun_paintrow ();
#endif

#ifdef TEKTRONIXWIN
extern mf_tektronix_initscreen (), mf_tektronix_updatescreen ();
extern mf_tektronix_blankrectangle (), mf_tektronix_paintrow ();
#endif

#ifdef UNITERMWIN
extern mf_uniterm_initscreen (), mf_uniterm_updatescreen();
extern mf_uniterm_blankrectangle(), mf_uniterm_paintrow();
#endif

#ifdef X10WIN
extern mf_x10_initscreen (), mf_x10_updatescreen ();
extern mf_x10_blankrectangle (), mf_x10_paintrow ();
#endif

#ifdef X11WIN
extern mf_x11_initscreen (), mf_x11_updatescreen ();
extern mf_x11_blankrectangle (), mf_x11_paintrow ();
#endif


/* `mfwsw' contains the dispatch tables for each terminal.  We map the
   Pascal calls to the routines `init_screen', `update_screen',
   `blank_rectangle', and `paint_row' into the appropriate entry point
   for the specific terminal that MF is being run on.  */

struct mfwin_sw
{
  char *mfwsw_type;		/* Name of terminal a la TERMCAP.  */
  int (*mfwsw_initscreen) ();
  int (*mfwsw_updatescrn) ();
  int (*mfwsw_blankrect) ();
  int (*mfwsw_paintrow) ();
} mfwsw[] =

/* Now we have the initializer for all the devices we support.  */

{
#ifdef HP2627WIN
  { "hp2627", mf_hp2627_initscreen, mf_hp2627_updatescreen,
    mf_hp2627_blankrectangle, mf_hp2627_paintrow },
#endif

#ifdef SUNWIN
  { "sun", mf_sun_initscreen, mf_sun_updatescreen,
    mf_sun_blankrectangle, mf_sun_paintrow },
#endif

#ifdef TEKTRONIXWIN
  { "tek", mf_tektronix_initscreen, mf_tektronix_updatescreen,
    mf_tektronix_blankrectangle, mf_tektronix_paintrow },
#endif

#ifdef UNITERMWIN
   { "uniterm", mf_uniterm_initscreen, mf_uniterm_updatescreen,
     mf_uniterm_blankrectangle, mf_uniterm_paintrow },
#endif

#ifdef X10WIN
  { "xterm", mf_x10_initscreen, mf_x10_updatescreen,
    mf_x10_blankrectangle, mf_x10_paintrow },
#endif

#ifdef X11WIN
  { "xterm", mf_x11_initscreen, mf_x11_updatescreen, 
    mf_x11_blankrectangle, mf_x11_paintrow },
#endif

#ifdef MSDOS
  { "PM", mf_pm_initscreen, mf_pm_updatescreen, 
    mf_pm_blankrectangle, mf_pm_paintrow },
#endif

/* Finally, we must have an entry with a terminal type of NULL.  */
  { NULL, NULL, NULL, NULL, NULL }

}; /* End of the array initialization.  */


/* This is a pointer to the mfwsw[] entry that we find.  */
static struct mfwin_sw *mfwp;

/* The following are routines that just jump to the correct
   terminal-specific graphics code. If none of the routines in the
   dispatch table exist, or they fail, we produce trap-compatible
   output, i.e., the same words and punctuation that the unchanged
   mf.web would produce.  */


/* This returns true if we can do window operations, else false.  */

bool
initscreen ()
{
#ifndef TRAP
  /* If MFTERM is set, use it.  */
  char *ttytype = getenv ("MFTERM");	/* not for TeX */
  
  if (ttytype == NULL)
    { /* If DISPLAY is set, we are X11; otherwise, who knows.  */
      bool have_display = getenv ("DISPLAY") != NULL;
      ttytype = have_display ? "xterm" : getenv ("TERM");
    }

  /* If we don't know kind of terminal this is, or if Metafont isn't
      being run interactively, don't do any online output.  */
  if (ttytype == NULL || !isatty (fileno (stdout)))
    return 0;

  /* Test each of the terminals given in `mfwsw' against the terminal
     type, and take the first one that matches, or if the user is running
     under Emacs, the first one.  */
  for (mfwp = mfwsw; mfwp->mfwsw_type != NULL; mfwp++)
    if (!strncmp (mfwp->mfwsw_type, ttytype, strlen (mfwp->mfwsw_type))
	|| !strcmp (ttytype, "emacs"))
      if (mfwp->mfwsw_initscreen)
	return ((*mfwp->mfwsw_initscreen) ());
      else
	{
		show_line("\n", 0);
		sprintf(log_line,
                   "! Couldn't initialize the online display for a `%s'.\n",
                   ttytype);
		show_line(log_line, 1);
		return 1;
	}
  
  /* The current terminal type wasn't found in any of the entries, so
     silently give up, assuming that the user isn't on a terminal that
     supports graphic output.  */
  return 0;
#else /* TRAP */
  return 1;
#endif /* TRAP */
}


/* Make sure everything is visible.  */

void
updatescreen ()
{
#ifndef TRAP
  if (mfwp->mfwsw_updatescrn)
    ((*mfwp->mfwsw_updatescrn) ());
  else
    {
      show_line("Updatescreen called\n", 0);
    }
#else /* TRAP */
  fprintf (log_file, "Calling UPDATESCREEN\n");
#endif /* TRAP */
}


/* This sets the rectangle bounded by ([left,right], [top,bottom]) to
   the background color.  */

void
blankrectangle (left, right, top, bottom)
     screencol left, right;
     screenrow top, bottom;
{
#ifndef TRAP
  if (mfwp->mfwsw_blankrect)
    ((*mfwp->mfwsw_blankrect) (left, right, top, bottom));
  else
    {
      sprintf(log_line, "Blankrectangle l=%d  r=%d  t=%d  b=%d\n",
	      left, right, top, bottom);
	  show_line(log_line, 0);
    }
#else /* TRAP */
  fprintf (log_file, "\nCalling BLANKRECTANGLE(%d,%d,%d,%d)\n", left,
	   right, top, bottom);
#endif /* TRAP */
}

/* This paints ROW, starting with the color INIT_COLOR. 
   TRANSITION_VECTOR then specifies the length of the run; then we
   switch colors.  This goes on for VECTOR_SIZE transitions.  */

void
paintrow (row, init_color, transition_vector, vector_size)
     screenrow row;
     pixelcolor init_color;
     transspec transition_vector;
     screencol vector_size;
{
#ifndef TRAP
  if (mfwp->mfwsw_paintrow)
    ((*mfwp->mfwsw_paintrow) (row, init_color,
			      transition_vector, vector_size));
  else
    {
      sprintf(log_line, "Paintrow r=%d  c=%d  v=");
	  show_line(log_line, 0);
      while (vector_size-- > 0) {
//		  printf ("%d  ", transition_vector++);
		  sprintf(log_line, "%d  ", transition_vector++);
		  show_line(log_line, 0);
	  }
      show_line("\n", 0);
    }
#else /* TRAP */
  unsigned k;

  fprintf(log_file, "Calling PAINTROW(%d,%d;", row, init_color);
  for (k = 0; k <= vector_size; k++)
    {
      fprintf(log_file, "%d", transition_vector[k]);
      if (k != vector_size)
	fprintf(log_file, ",");
    }
  fprintf(log_file, ")\n");
#endif /* TRAP */
}
#endif /* not TeX */

/* int tab_step;  tab_step = 8;  `-H=8' */

