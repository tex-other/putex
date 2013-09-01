#include <windows.h>
#include <sys/stat.h>
#include <direct.h>
#include <cstring>
#include "../lib/miktex.h"
#include "sys.h"

using namespace std;

bool get_cfg_file (const char *fName, char *path)
{
	char buf[2048];
	char *path_list;

	path_list = MiKTeX::GetConfigValue ("PUTeX", "ConfigPath", buf, sizeof (buf), 0);
	if (path_list == 0) {
		return false;
	}
	if (! MiKTeX::FindFile (fName, path_list, path)) {
		return false;
	}
//	strcpy(path, "c:\\localtexmf\\putex\\cdi2dvi.cfg");
	return true;
}

const char cdi_ext[] = "cdi";
const char dvi_ext[] = "dvi";

void prepare_path (string& cdi_name, string& dvi_name, string& cdi_path, string& dvi_path, string& job_name)
{
	char drive[10], dir[255], fName[256], ext[10], path[_MAX_PATH];

	// setup the cdi file path
	_splitpath(cdi_name.c_str(), drive, dir, fName, ext);
	job_name = fName;  // set job's name

	if (ext[0] == '\0')	strcpy(ext, cdi_ext);
	_makepath(path, drive, dir, fName, ext);
	cdi_path = path;

	// setup the dvi file path
	if (!dvi_name.empty()) {
		_splitpath(dvi_name.c_str(), drive, dir, fName, ext);
		if (ext[0] == '\0')
			strcpy(ext, dvi_ext);
	}
	else
		strcpy(ext, dvi_ext);

	_makepath(path, drive, dir, fName, ext);
	dvi_path = path;
}

void setup_cdifont_dir ()
{
    struct _stat buf;
	char cmd[256];
	OSVERSIONINFO osinfo;

    if (_stat("cdifont", &buf) ==0) {
		osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osinfo);
		if (osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
			strcpy(cmd, "rd/s/q ");
		else
			strcpy(cmd, "deltree/y ");
		strcat(cmd, "cdifont");
		system(cmd);
	}
	_mkdir(".\\cdifont");
	_mkdir(".\\cdifont\\tfm");
}
