void print_size(integer s)
{
  if (s==text_size) print_esc("textfont");
  else {
    if (s==script_size) print_esc("scriptfont");
    else print_esc("scriptscriptfont");
  }
} 
