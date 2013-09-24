/* \TeX\ is occasionally supposed to print diagnostic information that
goes only into the transcript file, unless |tracing_online| is positive.
Here are two routines that adjust the destination of print commands: */
void begin_diagnostic(void) /* prepare to do some tracing */
{
  old_setting=selector;
  if ((tracing_online>0) || (selector!=term_and_log))
    return;
  decr(selector);
  if (history==spotless) history=warning_issued;
}
