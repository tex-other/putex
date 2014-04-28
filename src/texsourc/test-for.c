#include <stdio.h>

int main (void)
{
  unsigned char i;
  //for (i = 0; i <= 255; i++)
  for (i = 0; i++ < 255;)
    printf("At: %d.", i);
  return 0;
}
