void print_length_param(long n)
{
 switch (n) {
 case par_indent_code: print_esc(S(1089)); break;
 case math_surround_code: print_esc(S(1090)); break;
 case line_skip_limit_code: print_esc(S(1091)); break;
 case hsize_code: print_esc(S(1092)); break;
 case vsize_code: print_esc(S(1093)); break;
 case max_depth_code: print_esc(S(1094)); break;
 case split_max_depth_code: print_esc(S(1095)); break;
 case box_max_depth_code: print_esc(S(1096)); break;
 case hfuzz_code: print_esc(S(1097)); break;
 case vfuzz_code: print_esc(S(1098)); break;
 case delimiter_shortfall_code: print_esc(S(1099)); break;
 case null_delimiter_space_code: print_esc(S(1100)); break;
 case script_space_code: print_esc(S(1101)); break;
 case pre_display_size_code: print_esc(S(1102)); break;
 case display_width_code: print_esc(S(1103)); break;
 case display_indent_code: print_esc(S(1104)); break;
 case overfull_rule_code: print_esc(S(1105)); break;
 case hang_indent_code: print_esc(S(1106)); break;
 case h_offset_code: print_esc(S(1107)); break;
 case v_offset_code: print_esc(S(1108)); break;
 case emergency_stretch_code: print_esc(S(1109)); break;
 default: print(S(1254)); break;
 }
}
