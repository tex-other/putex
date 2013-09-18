void printstyle(long c)
{
 switch (c / 2) {
 case 0: printesc(S(336)); break;
 case 1: printesc(S(337)); break;
 case 2: printesc(S(338)); break;
 case 3: printesc(S(339)); break;
 default: print(S(340)); break;
 }
}
