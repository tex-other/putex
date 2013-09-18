Static void appendchoices(void)
{
  tailappend(newchoice());
  saveptr++;
  saved(-1) = 0;
  pushmath(mathchoicegroup);
  scanleftbrace();
}
