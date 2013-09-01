#ifndef VCDIRECTORY_H_
#define VCDIRECTORY_H_

class VCDirectory {
	virtual void set_tfm_dir (char *dir) { tfm_dir = dir; }
	virtual void set_pk_dir (char *dir) { pk_root_dir = dir; }
	const string& get_tfm_dir () { return tfm_dir; }
	const string& get_pk_root_dir () { return pk_root_dir; }
private:
	string tfm_dir;
	string pk_root_dir;
};

#endif