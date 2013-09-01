%% pdfparse_c.ch
@x
char *parse_ident (char **start, char *end)
@y
/* add -- cdipdfmx */
char *parse_dbcs_name (char **start, char *end)
{
  save = *start;
  while (*start < end && !isspace(**start))
    (*start)++;
  return parsed_string(*start);
}
/* end -- cdipdfmx */
char *parse_ident (char **start, char *end)
@z