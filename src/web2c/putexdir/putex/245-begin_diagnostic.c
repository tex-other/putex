Static void begindiagnostic(void)
{
  diag_oldsetting = selector;
  if (tracingonline > 0 || selector != termandlog)
    return;
  selector--;
  if (history == spotless)
    history = warningissued;
}
