void printskipparam(long n)
{
  switch (n) {

  case lineskipcode:
    printesc(S(341));
    break;

  case baselineskipcode:
    printesc(S(342));
    break;

  case parskipcode:
    printesc(S(343));
    break;

  case abovedisplayskipcode:
    printesc(S(344));
    break;

  case belowdisplayskipcode:
    printesc(S(345));
    break;

  case abovedisplayshortskipcode:
    printesc(S(346));
    break;

  case belowdisplayshortskipcode:
    printesc(S(347));
    break;

  case leftskipcode:
    printesc(S(348));
    break;

  case rightskipcode:
    printesc(S(349));
    break;

  case topskipcode:
    printesc(S(350));
    break;

  case splittopskipcode:
    printesc(S(351));
    break;

  case tabskipcode:
    printesc(S(352));
    break;

  case spaceskipcode:
    printesc(S(353));
    break;

  case xspaceskipcode:
    printesc(S(354));
    break;

  case parfillskipcode:
    printesc(S(355));
    break;

  case thinmuskipcode:
    printesc(S(356));
    break;

  case medmuskipcode:
    printesc(S(357));
    break;

  case thickmuskipcode:
    printesc(S(358));
    break;

  default:
    print(S(359));
    break;
  }
}
