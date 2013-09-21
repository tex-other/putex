Static void normalizeselector(void)
{
  if (logopened)
    selector = termandlog;
  else
    selector = termonly;
  if (jobname == 0)
    openlogfile();
  if (interaction == batchmode)
    selector--;
}
