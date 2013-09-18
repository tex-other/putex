void print_ln(void)
{
    switch (selector) {
        case term_and_log: putc('\n', termout); putc('\n', logfile); termoffset = 0; fileoffset = 0; break;
        case log_only: putc('\n', logfile); fileoffset = 0; break;
        case term_only: putc('\n', termout); termoffset = 0; break;
        case no_print: case pseudo: case new_string: break;
        default: putc('\n', writefile[selector]); break;
    }
}
