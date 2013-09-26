/* Conversely, here is a routine that takes three strings and prints a file
name that might have produced them. (The routine is system dependent, because
some operating systems put the file area last instead of first.) */
void printfilename(integer n, integer a, integer e)
{
  slow_print(a); slow_print(n); slow_print(e);
}
