void restore_trace(pointer p, str_number s)
/* |eqtb[p]| has just been restored or retained */
{
  begin_diagnostic(); print_char('{'); print(s); print_char(' ');
  show_eqtb(p); print_char('}');
  end_diagnostic(false);
}

