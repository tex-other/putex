/* When an interrupt has been detected, the program goes into its
highest interaction level and lets the user have nearly the full flexibility of
the |error| routine.  \TeX\ checks for interrupts only at times when it is
safe to do this. */
void pause_for_instructions(void)
{
  if (!OK_to_interrupt)
    return;
  interaction=error_stop_mode;
  if (selector==log_only || selector==no_print)
    incr(selector);
  print_err("Interruption");
  help3("You rang?",
    "Try to insert some instructions for me (e.g.,`I\showlists'),",
    "unless you just want to quit by typing `X'.");
  deletions_allowed=false; error(); deletions_allowed=true;
  interrupt=0;
}
