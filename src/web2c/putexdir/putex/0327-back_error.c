/* The |back_error| routine is used when we want to replace an offending token
just before issuing an error message. This routine, like |back_input|,
requires that |cur_tok| has been set. We disable interrupts during the
call of |back_input| so that the help message won't be lost. */
void back_error(void) /* back up one token and call |error| */
{
  OK_to_interrupt=false; back_input(); OK_to_interrupt=true; error();
}

