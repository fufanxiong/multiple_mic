#ifndef USERCASE_COMMON_H
#define USERCASE_COMMON_H

#include "Dbmd_rtl_Device.h"
#include "device.h"
#include "spi_api.h"
#include "spi_ex_api.h"

int Init_D4(void);
int Run_D4();
int Config_D4();
int dump_audio_data(char *buf, int max_size);
int pull_buffer(void);






extern int fw_debug_mode;
extern int d4_asrp_rec;
extern int d4_fw_rec;
extern long m_clk;

extern int asrp_delay;

extern int rec_points_5_d4;
extern int rec_points_6_d4;
extern int rec_points_7_d4;

#define read_register_host_interface	read_register_spi
#define write_register_host_interface	write_register_spi
#endif
