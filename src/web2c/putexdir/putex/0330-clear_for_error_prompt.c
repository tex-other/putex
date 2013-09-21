Static void clearforerrorprompt(void)
{
  while (state != tokenlist && terminalinput && inputptr > 0 && loc > limit) {
    endfilereading();
  }
  println();
}

