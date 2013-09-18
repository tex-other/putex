Static void flushmath(void)
{
  flushnodelist(link(head));
  flushnodelist(incompleatnoad);
  link(head) = 0;
  tail = head;
  incompleatnoad = 0;
}
