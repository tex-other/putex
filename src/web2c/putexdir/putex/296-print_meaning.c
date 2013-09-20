Static void printmeaning(int cur_chr, int cur_cmd)
{
  printcmdchr(cur_cmd, cur_chr);
  if (cur_cmd >= call) {
    printchar(':');
    println();
    tokenshow(cur_chr);
    return;
  }
  if (cur_cmd != topbotmark)
    return;
  printchar(':');
  println();
  tokenshow(curmark[cur_chr - topmarkcode]);
}

