Static void printmode(long m)
{
  if (m > 0) {
    switch (m / (maxcommand + 1)) {

    case 0:
      print(S(431));
      break;

    case 1:
      print(S(432));
      break;

    case 2:
      print(S(433));
      break;
    }
  } else if (m == 0)
    print(S(434));
  else {
    switch (-m / (maxcommand + 1)) {

    case 0:
      print(S(435));
      break;

    case 1:
      print(S(436));
      break;

    case 2:
      print(S(394));
      break;
    }
  }
  print(S(437));
}
