#include <iostream>
#include <direct.h>
#include "miktex.h"

#include "config.h"
#include "exception.h"
#include "cdifile.h"
#include "dvifile.h"
#include "cfontdef.h"
#include "fontused.h"
#include "cmap.h"
#include "sys.h"
#include "cfntmap.h"
#include "CDirectory.h"
#include "dpi.h"
#include "VCFontDrv.h"
#include "vcencoding.h"


#include "cttf2bmp_w32.h"

using namespace std;

extern void convert(CdiFile&, DviFile&);

static char banner[] = "This is cdi2dvi Version 3.0 by C. W. Tsay, Y. L. Hsieh, and H. P. Liu";

// capacity parameters
int max_cfont = 1024;
int max_tex_font = 1500;

const char default_mode[] = "ljfour";
const int default_dpi_x = 600;
const int default_dpi_y = 600;

CJKfontMap cfontmap;
string cfontmap_fName = "cfonts.map";
CdiFile cdi;
DviFile dvi;
FontUsedTbl font_used;
CJKfontDefTable cfontTable;
CJKCharMapping cjkCharMapper;
string job_name;
string tfm_dir, pk_root_dir;
dev_info dev;
VCFontDrv *font_drv = 0;


static string page_rng;

static void parse_cmd_line (int argc,char* argv[], string& cdi_name, string& dvi_name)
{
	int i = 1, fc = 0;

	if (argc == 1) {
		cout << banner << endl;
		cout << "Usage: cdi2dvi [flags] cdifile[.cdi] [dvifile[.dvi]]\n";
		cout << "Supported flags:\n";
		cout << "\t-m cfont_map_file: use cfont_map_file for CJK font mapping\n";
		cout << "\t-p name x_res y_res: set device mode name and resolutions\n";
		cout << "\t-x page_range: extract output pages\n";
		exit(0);
	}

	while (i < argc) {
		if (argv[i][0] == '-')
			switch (argv[i][1]) {
				case 'm':
					if (i+1 >= argc)
						throw(EFatal("Missing cfontmap file"));
					cfontmap_fName = argv[i+1];
					i += 2;
					break;
				case 'p': 
					if (i+3 >= argc)
						throw(EFatal("Missing printer mode parameters"));
					dev.mode = argv[i+1];
					dev.dpi_x = atoi(argv[i+2]);
					dev.dpi_y = atoi(argv[i+3]);
					if (dev.dpi_x == 0 || dev.dpi_y == 0)
						throw(EFatal("Illegal device resolution"));
					i += 3;
					break;
				case 'x':
					if (i+1 >= argc)
						throw(EFatal("Missing page output range string"));
					page_rng = argv[i+1];
					i += 2;
					break;
				default:
					cerr << "Skip unknown flag: " <<  argv[i];
					i++;
			}
		else {
			if (fc == 0)
				cdi_name = argv[i];
			else if (fc == 1)
				dvi_name = argv[i];
			else {
				cerr << "Redundent argument: (" << argv[i] << ")\n";
				fc++;
			}
		}
		i++;
	}
	if (cdi_name.empty())
		throw(EFatal("Missing cdi file"));
}

static void init ()
{
	init_charset_table ();
	cfontTable.init(max_cfont);
	font_used.init(max_tex_font);
	cfontmap.init(max_cfont);
	init_cmd_type();
	dpiTbl = new CDpiTable(30);
}

static void setup ()
{
	char path[_MAX_PATH];

	setup_cdifont_dir();
	get_cfg_file(cfontmap_fName.c_str(), path);

	cfontmap.parse(path);
	tfm_dir = ".\\cdifont\\tfm\\";
	pk_root_dir = ".\\cdifont\\";
	pk_root_dir += dev.mode;
	_mkdir(pk_root_dir.c_str());
	pk_root_dir += "\\";
	
	// tc = 0404  sc =0804
	// char langbuf[6];
	// GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, langbuf, 6);

	font_drv = new TTF2Bmp_W32;
}

int main (int argc, char *argv[])
{
	string cdi_name, dvi_name;
	string cdi_path, dvi_path;
	char path[256]="None";

	try {
		miktex_initialize();	// declared in ../lib/miktex.h

		if (!parse_cfg_file(dev))
			cerr << "Can't open config file" << endl;
		parse_cmd_line (argc, argv, cdi_name, dvi_name);
		cout << banner << endl;
		init();
		prepare_path (cdi_name, dvi_name, cdi_path, dvi_path, job_name);
		cout << "Convert " << cdi_path << " to " << dvi_path << endl;
		setup();
		cdi.open(cdi_path.c_str(), page_rng);

		miktex_uninitialize ();

		cjkCharMapper.init(cfontTable);

		dvi.open(dvi_path.c_str());
		convert(cdi, dvi);
		cjkCharMapper.clean_up();
		cdi.close();
		dvi.close();
	}
	catch (EUnknown err) {
		cerr << err.what() << endl;
	}
	catch (exception err) {
		cerr << err.what() << endl;
	}
	catch (...) {
		cerr << "Unknown Error" << endl;
	}

	cout << endl;
	return 0;

}