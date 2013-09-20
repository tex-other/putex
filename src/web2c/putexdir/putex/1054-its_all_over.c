Static boolean itsallover(void)
{
  boolean Result;

  if (privileged()) {
    if (pagehead == pagetail && head == tail && deadcycles == 0) {
      Result = true;
      goto _Lexit;
    }
    backinput();
    tailappend(newnullbox());
    width(tail) = hsize;
    tailappend(newglue(fillglue));
    tailappend(newpenalty(-1073741824L));
    buildpage();
  }
  Result = false;
_Lexit:
  return Result;
}
