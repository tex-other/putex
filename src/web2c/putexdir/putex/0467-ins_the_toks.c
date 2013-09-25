/* Here's part of the |expand| subroutine that we are now ready to complete: */
void ins_the_toks(void)
{
  link(garbage)=the_toks(); ins_list(link(temp_head));
}
