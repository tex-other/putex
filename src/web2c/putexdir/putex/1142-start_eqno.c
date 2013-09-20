Static void starteqno(void)
{
  saved(0) = curchr;
  saveptr++;   /*1139:*/
  pushmath(mathshiftgroup);
  eqworddefine(intbase + curfamcode, -1);
  if (everymath != 0)   /*:1139*/
    begintokenlist(everymath, everymathtext);
}

