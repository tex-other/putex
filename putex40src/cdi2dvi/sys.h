#ifndef SYS_H_
#define SYS_H_

#include <string>
using namespace std;

extern bool get_cfg_file (const char *fName, char *path);
extern void prepare_path (string&, string&, string&, string&, string&);
extern void setup_cdifont_dir ();

#endif




