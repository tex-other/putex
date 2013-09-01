#ifndef CDIRECTORY_H_
#define CDIRECTORY_H_

class CDirectory {
public:
	void set_tfm_dir (const char *dir) { tfm_dir = dir; }
	void set_pk_root_dir (const char *dir) { pk_root_dir = dir; }
	const string& get_tfm_dir () { return tfm_dir; }
	const string& get_pk_root_dir () { return pk_root_dir; }
private:
	string tfm_dir;
	string pk_root_dir;
};

#endif