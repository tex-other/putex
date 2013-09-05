/* putexextra.h: banner etc. for PUTeX.
   This is included by PUTeX, from putexextra.c
*/

#include <putexdir/putex_version.h> /* for PUTEX_VERSION */

#define BANNER "This is PUTeX, Version 3.1415926-" PUTEX_VERSION
#define COPYRIGHT_HOLDER "Clerk Ma"
#define AUTHOR NULL
#define PROGRAM_HELP PUTEXHELP
#define BUG_ADDRESS "clerkma@gmail.com"
#define DUMP_VAR TEXformatdefault
#define DUMP_LENGTH_VAR formatdefaultlength
#define DUMP_OPTION "fmt"
#define DUMP_EXT ".fmt"
#define INPUT_FORMAT kpse_tex_format
#define INI_PROGRAM "iniputex"
#define VIR_PROGRAM "virputex"

#ifdef Xchr
#undef Xchr
#define Xchr(x) (x)
#endif /* Xchr */
