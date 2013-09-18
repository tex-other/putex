void sprintcs(halfword p)
{
  if (p >= hashbase) {
    printesc(text(p));
    return;
  }
  if (p < singlebase) {
    print(p - activebase);
    return;
  }
  if (p < nullcs)
    printesc(p - singlebase);
  else {
    printesc(S(262));
    printesc(S(263));
  }
}
