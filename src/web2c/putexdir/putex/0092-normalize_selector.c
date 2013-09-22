/* In anomalous cases, the print selector might be in an unknown state;
the following subroutine is called to fix things just enough to keep
running a bit longer. */
static void normalize_selector(void)
{
  if (log_opened) selector=term_and_log;
  else selector=term_only;
  if (job_name==0)
    open_log_file();
  if (interaction==batch_mode)
    selector--;
}
