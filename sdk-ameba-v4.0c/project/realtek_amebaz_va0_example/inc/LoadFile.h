#ifndef LOAD_FILE_H
#define LOAD_FILE_H

enum {
	FIRMWARE_TYPE,	
	ASRP_TYPE,
};
int load_file(int load_type,int skip_bytes);
int load_amodel_file();
int Load_A_Model();
int Load_ASRP_paramD4();
int read_checksum(char *checksum);
#endif