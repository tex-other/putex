Static void newinteraction(void)
{
  println();
  interaction = curchr;   /*75:*/
  if (interaction == batchmode)
    selector = noprint;
  else {
    selector = termonly;
    /*
:75*/
  }
  if (logopened)
    selector += 2;
}
