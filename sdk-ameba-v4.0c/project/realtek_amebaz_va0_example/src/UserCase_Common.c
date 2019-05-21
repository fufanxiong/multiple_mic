#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"


#include "device.h"
#include "spi_api.h"
#include "sys_api.h"
#include "osdep_service.h"
#include "flash_api.h"
#include "device_lock.h"


#include "UserCase_Commom.h"
#include "LoadFile.h"
#include "dbmdx_melon_def.h"
#include "Dbmd_rtl_Device.h"
#include "Dbmd_init.h"

#define HW_CHIP_D4P
#define MAX_PULL_BUFF	2048
/*****var*******/
int fw_debug_mode = 0;
int d4_asrp_rec=1;
int d4_fw_rec = 0;
long m_clk = 32768;

int asrp_delay = 0x019F;

int rec_points_5_d4 = 0;
int rec_points_6_d4 = 0;
int rec_points_7_d4 = 0;

extern int send_recv_info(char *buf,int len);
extern int send_head_to_baidu();
extern int send_data_to_baidu(char *buf_voice,int voice_len);
extern int get_result_baidu();

#if 0
char voice_data[] = {
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x2d ,0x00 ,0x32 ,0x00 ,0x38 ,0x00 ,0x47 ,0x00 ,0x2c ,0x00 ,0x19,		\
0x00 ,0x2e ,0x00 ,0x22 ,0x00 ,0x26 ,0x00 ,0x43 ,0x00 ,0x3d ,0x00 ,0x35 ,0x00 ,0x16 ,0x00 ,0x0b,		
0x00 ,0x15 ,0x00 ,0x11 ,0x00 ,0x1e ,0x00 ,0x29 ,0x00 ,0x17 ,0x00 ,0x1f ,0x00 ,0x18 ,0x00 ,0x0e,		\
0x00 ,0x15 ,0x00 ,0xfa ,0xff ,0x10 ,0x00 ,0x08 ,0x00 ,0x0e ,0x00 ,0x12 ,0x00 ,0x0d ,0x00 ,0x20,		\
0x00 ,0x1a ,0x00 ,0x27 ,0x00 ,0x0c ,0x00 ,0x2b ,0x00 ,0x34 ,0x00 ,0x34 ,0x00 ,0x37 ,0x00 ,0x24,		\
0x00 ,0x33 ,0x00 ,0x1c ,0x00 ,0x16 ,0x00 ,0x17 ,0x00 ,0x1d ,0x00 ,0x03 ,0x00 ,0x07 ,0x00 ,0x04,		\
0x00 ,0xfd ,0xff ,0x0b ,0x00 ,0x05 ,0x00 ,0x0e ,0x00 ,0x19 ,0x00 ,0x21 ,0x00 ,0x13 ,0x00 ,0x10,		\
0x00 ,0x11 ,0x00 ,0x13 ,0x00 ,0x20 ,0x00 ,0x1d ,0x00 ,0x26 ,0x00 ,0x25 ,0x00 ,0x26 ,0x00 ,0x27,		\
0x00 ,0x26 ,0x00 ,0x22 ,0x00 ,0x26 ,0x00 ,0x21 ,0x00 ,0x23 ,0x00 ,0x20 ,0x00 ,0x16 ,0x00 ,0x0c,		\
0x00 ,0x19 ,0x00 ,0x25 ,0x00 ,0x16 ,0x00 ,0x1c ,0x00 ,0x0d ,0x00 ,0x0c ,0x00 ,0x11 ,0x00 ,0x12,		\
0x00 ,0xfb ,0xff ,0x01 ,0x00 ,0x16 ,0x00 ,0x09 ,0x00 ,0x16 ,0x00 ,0x10 ,0x00 ,0x0a ,0x00 ,0x06,		\
0x00 ,0xf8 ,0xff ,0xf0 ,0xff ,0x13 ,0x00 ,0x0a ,0x00 ,0xed ,0xff ,0x11 ,0x00 ,0xff ,0xff ,0xde,		\
0xff ,0xd8 ,0xff ,0xef ,0xff ,0x08 ,0x00 ,0xf8 ,0xff ,0xee ,0xff ,0xf9 ,0xff ,0xf1 ,0xff ,0xe6,		\
0xff ,0xf2 ,0xff ,0xf6 ,0xff ,0x09 ,0x00 ,0x06 ,0x00 ,0x03 ,0x00 ,0x04 ,0x00 ,0x0d ,0x00 ,0x10,		\
0x00 ,0x18 ,0x00 ,0x21 ,0x00 ,0x14 ,0x00 ,0x20 ,0x00 ,0x12 ,0x00 ,0x01 ,0x00 ,0x05 ,0x00 ,0x19,		\
0x00 ,0x08 ,0x00 ,0x06 ,0x00 ,0x11 ,0x00 ,0x04 ,0x00 ,0x05 ,0x00 ,0x09 ,0x00 ,0x0b ,0x00 ,0x09,		\
0x00 ,0x04 ,0x00 ,0x03 ,0x00 ,0x13 ,0x00 ,0x06 ,0x00 ,0x13 ,0x00 ,0x19 ,0x00 ,0x13 ,0x00 ,0x1d,		\
0x00 ,0x14 ,0x00 ,0x18 ,0x00 ,0x16 ,0x00 ,0x1c ,0x00 ,0x1b ,0x00 ,0x1d ,0x00 ,0x27 ,0x00 ,0x1f,		\
0x00 ,0x1a ,0x00 ,0x2b ,0x00 ,0x25 ,0x00 ,0x25 ,0x00 ,0x2b ,0x00 ,0x29 ,0x00 ,0x21 ,0x00 ,0x0b,		\
0x00 ,0x17 ,0x00 ,0x0e ,0x00 ,0x1d ,0x00 ,0x16 ,0x00 ,0x10 ,0x00 ,0x15 ,0x00 ,0x16 ,0x00 ,0x16,		\
0x00 ,0x0a ,0x00 ,0x1a ,0x00 ,0x16 ,0x00 ,0x14 ,0x00 ,0x13 ,0x00 ,0x1a ,0x00 ,0x0c ,0x00 ,0x10,		\
0x00 ,0x0c ,0x00 ,0x04 ,0x00 ,0x1a ,0x00 ,0x0a ,0x00 ,0x0b ,0x00 ,0x10 ,0x00 ,0x1e ,0x00 ,0x28,		\
0x00 ,0x22 ,0x00 ,0x10 ,0x00 ,0x02 ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x12 ,0x00 ,0x1b ,0x00 ,0x10,		\
0x00 ,0x02 ,0x00 ,0x10 ,0x00 ,0x0b ,0x00 ,0xff ,0xff ,0x11 ,0x00 ,0x0d ,0x00 ,0x0e ,0x00 ,0x05,		\
0x00 ,0xf3 ,0xff ,0xf3 ,0xff ,0xfa ,0xff ,0x06 ,0x00 ,0x00 ,0x00 ,0xf5 ,0xff ,0xf7 ,0xff ,0xf1,		\
0xff ,0xff ,0xff ,0xf6 ,0xff ,0xf3 ,0xff ,0x01 ,0x00 ,0x03 ,0x00 ,0xf7 ,0xff ,0xf1 ,0xff ,0xfa,		\
0xff ,0xf6 ,0xff ,0x00 ,0x00 ,0xfa ,0xff ,0xf6 ,0xff ,0xf9 ,0xff ,0xf2 ,0xff ,0xf9 ,0xff ,0xfe,		\
0xff ,0xfb ,0xff ,0xff ,0xff ,0x03 ,0x00 ,0x0a ,0x00 ,0x07 ,0x00 ,0x09 ,0x00 ,0x0d ,0x00 ,0x02,		\
0x00 ,0x03 ,0x00 ,0xfa ,0xff ,0xf6 ,0xff ,0xf7 ,0xff ,0xf3 ,0xff ,0xfc ,0xff ,0xfd ,0xff ,0xfc,		\
0xff ,0xf9 ,0xff ,0xf7 ,0xff ,0xfd ,0xff ,0xfd ,0xff ,0xff ,0xff ,0xf6 ,0xff ,0x00 ,0x00 ,0xfd,		\
0xff ,0xf6 ,0xff ,0xfc ,0xff ,0xfd ,0xff ,0xfb ,0xff ,0xf9 ,0xff ,0xf6 ,0xff ,0xf4 ,0xff ,0xf9,		\
0xff ,0xfc ,0xff ,0xf3 ,0xff ,0xfc ,0xff ,0xf8 ,0xff ,0xf1 ,0xff ,0xdf ,0xff ,0xde ,0xff ,0xf3,		\
0xff ,0xed ,0xff ,0xef ,0xff ,0xe3 ,0xff ,0xf0 ,0xff ,0xea ,0xff ,0xd9 ,0xff ,0xe9 ,0xff ,0xea,		\
0xff ,0xe4 ,0xff ,0xdd ,0xff ,0xda ,0xff ,0xce ,0xff ,0xe6 ,0xff ,0xcf ,0xff ,0xc0 ,0xff ,0xcf,		\
0xff ,0xda ,0xff ,0xd2 ,0xff ,0xda ,0xff ,0x03 ,0x00 ,0xf9 ,0xff ,0xe5 ,0xff ,0xe0 ,0xff ,0xf0,		\
0xff ,0xed ,0xff ,0xe7 ,0xff ,0xd4 ,0xff ,0xe4 ,0xff ,0xdc ,0xff ,0xc9 ,0xff ,0xe1 ,0xff ,0xd6,		\
0xff ,0xe4 ,0xff ,0xe5 ,0xff ,0xfd ,0xff ,0xf1 ,0xff ,0xdf ,0xff ,0xe5 ,0xff ,0xd9 ,0xff ,0xef,		\
0xff ,0xec ,0xff ,0xf1 ,0xff ,0xfc ,0xff ,0x08 ,0x00 ,0x06 ,0x00 ,0x0b ,0x00 ,0x05 ,0x00 ,0x05,		\
0x00 ,0xfe ,0xff ,0xee ,0xff ,0xf3 ,0xff ,0x00 ,0x00 ,0x04 ,0x00 ,0xf9 ,0xff ,0x03 ,0x00 ,0xf8,		\
0xff ,0xf5 ,0xff ,0xe8 ,0xff ,0xc9 ,0xff ,0xcc ,0xff ,0xd7 ,0xff ,0xcb ,0xff ,0xde ,0xff ,0xe0,		\
0xff ,0xee ,0xff ,0xe8 ,0xff ,0xed ,0xff ,0x05 ,0x00 ,0x04 ,0x00 ,0x0b ,0x00 ,0x0c ,0x00 ,0x0b,		\
0x00 ,0xf6 ,0xff ,0xf9 ,0xff ,0x02 ,0x00 ,0xf1 ,0xff ,0xd7 ,0xff ,0xe5 ,0xff ,0xe5 ,0xff ,0xdb,		\
0xff ,0xbb ,0xff ,0x9e ,0xff ,0xaf ,0xff ,0xa8 ,0xff ,0xb5 ,0xff ,0xa9 ,0xff ,0x9b ,0xff ,0xb0,		\
0xff ,0xad ,0xff ,0xb6 ,0xff ,0xbd ,0xff ,0xce ,0xff ,0xcd ,0xff ,0xbb ,0xff ,0xc7 ,0xff ,0xc0,		\
0xff ,0xc7 ,0xff ,0xca ,0xff ,0xce ,0xff ,0xec ,0xff ,0xca ,0xff ,0xbb ,0xff ,0xd0 ,0xff ,0xb7,		\
0xff ,0xab ,0xff ,0xb8 ,0xff ,0xa9 ,0xff ,0xc5 ,0xff ,0xcc ,0xff ,0xc2 ,0xff ,0xcd ,0xff ,0xc0,		\
0xff ,0xbd ,0xff ,0xb0 ,0xff ,0xc1 ,0xff ,0xc9 ,0xff ,0xb8 ,0xff ,0xc6 ,0xff ,0xca ,0xff ,0xd3,		\
0xff ,0xce ,0xff ,0xd2 ,0xff ,0xe9 ,0xff ,0xec ,0xff ,0xfe ,0xff ,0xf0 ,0xff ,0xde ,0xff ,0xef,		\
0xff ,0xf3 ,0xff ,0xe0 ,0xff ,0xfc ,0xff ,0x09 ,0x00 ,0xeb ,0xff ,0xed ,0xff ,0xeb ,0xff ,0xfd,		\
0xff ,0x25 ,0x00 ,0x29 ,0x00, 0x09 ,0x00 ,0x21 ,0x00 ,0xfe ,0xff ,0xe4 ,0xff ,0x03 ,0x00 ,0xec,		\
0xff ,0xf7 ,0xff ,0xfb ,0xff ,0x02 ,0x00 ,0xd5 ,0xff ,0xc1 ,0xff ,0xdf ,0xff ,0xd6 ,0xff ,0xfd,		\
0xff ,0x0c ,0x00 ,0xfc ,0xff ,0x02 ,0x00 ,0x18 ,0x00 ,0x2a ,0x00 ,0x28 ,0x00 ,0x2f ,0x00 ,0x3a,		\
0x00 ,0x2b ,0x00 ,0x1a ,0x00 ,0x25 ,0x00 ,0x42 ,0x00 ,0x28 ,0x00 ,0x23 ,0x00 ,0x1c ,0x00 ,0x28,		\
0x00 ,0x28 ,0x00 ,0x30 ,0x00 ,0x27 ,0x00 ,0x0b ,0x00 ,0x27 ,0x00 ,0x1f ,0x00 ,0x16 ,0x00 ,0x04,		\
0x00 ,0x12 ,0x00 ,0x22 ,0x00 ,0x39 ,0x00 ,0x3b ,0x00 ,0x19 ,0x00 ,0x0d ,0x00 ,0x1b ,0x00 ,0x18,		\
0x00 ,0x18 ,0x00 ,0x0b ,0x00 ,0xee ,0xff ,0x04 ,0x00 ,0x0f ,0x00 ,0x16 ,0x00 ,0x12 ,0x00 ,0x13,		\
0x00 ,0x17 ,0x00 ,0x1e ,0x00 ,0x36 ,0x00 ,0x40 ,0x00 ,0x23 ,0x00 ,0x21 ,0x00 ,0x1e ,0x00 ,0x06,		\
0x00 ,0x36 ,0x00 ,0x2c ,0x00 ,0x0d ,0x00 ,0x16 ,0x00 ,0x0e ,0x00 ,0x09 ,0x00 ,0x1d ,0x00 ,0x1e,		\
0x00 ,0x09 ,0x00 ,0x07 ,0x00 ,0xfd ,0xff ,0x08 ,0x00 ,0x08 ,0x00 ,0xfd ,0xff ,0x01 ,0x00 ,0x15,		\
0x00 ,0xf7 ,0xff ,0xec ,0xff ,0xfd ,0xff ,0xfb ,0xff ,0x05 ,0x00 ,0xf6 ,0xff ,0x01 ,0x00 ,0xff,		\
0xff ,0x10 ,0x00 ,0xf8 ,0xff ,0xf1 ,0xff ,0xfc ,0xff ,0xeb ,0xff ,0xf2 ,0xff ,0xe2 ,0xff ,0xf2,		\
0xff ,0xda ,0xff ,0xfa ,0xff ,0xe1 ,0xff ,0xce ,0xff ,0xd4 ,0xff ,0x00 ,0x00 ,0x1c ,0x00 ,0xe8,		\
0xff ,0x14 ,0x00 ,0x15 ,0x00 ,0x08 ,0x00 ,0xec ,0xff ,0x04 ,0x00 ,0x0e ,0x00 ,0x02 ,0x00 ,0x04,		\
0x00 ,0x0a ,0x00 ,0x17 ,0x00 ,0x04 ,0x00 ,0x0b ,0x00 ,0x15 ,0x00 ,0x21 ,0x00 ,0x14 ,0x00 ,0x22		
};
#endif


int Init_D4(void)
{
	int fail_cnt = 0;
	printf("[%s]: Start.\n", __func__);

	while(fail_cnt < 5) {
		/* release D2 from reset */
		/* Load Firmware */
		if(load_file(FIRMWARE_TYPE,4) < 0) 
		{
			fail_cnt++;

			if(fail_cnt >= 5) {
				printf("[%s]: FAiled uploading FirmWare to D4.\n", __func__);
				goto err;
			}
		}
		else
			break;
	}

	printf ("[%s]: Finish!\n", __func__);
	return 0;
err:
	return -1;
}

/***********************************************************************
* FUNCTION NAME: Run_D4()
*
* DESCRIPTION:
*	Rum DBMD4
*
* PARAMETERS:
*
* RETURNS:
* 	0 on sucsses elese device_abort
***********************************************************************/
int Run_D4()
{
	int ret;
	unsigned char buf_run[] = {0x5A, 0x0B};
	
	ret = device_write_spi(&spi_master,buf_run,2);
	printf("[%s]: write ret = %d\n", __func__, ret);

	wait_ms(10);
	return 0;
}


//#ifdef MELON_V_3_222
		/***********************************************************************
		* FUNCTION NAME: Config_D4()
		*
		* DESCRIPTION:
		*	config D4
		* PARAMETERS:
		*	char *filename
		* RETURNS:
		*	0 on sucsses elese error
		***********************************************************************/
//file is name V_3_222
int Config_D4()
{
	printf("[%s]: Start.\n", __func__);
		
#ifdef D4_USE_HOST_INTERFACE_UART
	if (fw_debug_mode || d4_asrp_rec || d4_fw_rec)
	{

		write_register_host_interface(&spi_master,HOST_INTERFACE_SUPPORT,UART0_ENABLE_FOR_HOST_D2_TDM2_D4_D6_GPIO_17_18 | UART_DEBUG_MODE_0);						
		if (fw_debug_mode==0)
			write_register_host_interface(&spi_master,FW_DEBUG_REGISTER,TOGGLE_UART_DEBUG_PRINTS); 							
	}
	else
		write_register_host_interface(&spi_master,HOST_INTERFACE_SUPPORT, UART0_ENABLE_FOR_HOST_D2_TDM2_D4_D6_GPIO_17_18);											

	read_register_host_interface(&spi_master,FW_VERSION_NUMBER); 						     	          							
#else
	if (fw_debug_mode || d4_asrp_rec || d4_fw_rec)
	{	write_register_host_interface(&spi_master,HOST_INTERFACE_SUPPORT,SPI_D2_TDM1_D4_GPIO_4_5_6_7 | UART_DEBUG_MODE_0);						
		if (fw_debug_mode==0)
			write_register_host_interface(&spi_master,FW_DEBUG_REGISTER,TOGGLE_UART_DEBUG_PRINTS); 							
	}
	else
		write_register_host_interface(&spi_master,HOST_INTERFACE_SUPPORT, SPI_D2_TDM1_D4_GPIO_4_5_6_7);											

	read_register_host_interface(&spi_master,FW_VERSION_NUMBER); 						     	          							

#endif
	
	if (m_clk == 32768)
		write_register_host_interface(&spi_master,MASTER_CLOCK_FREQUENCY, MCLK_32768Hz); 										
	else
		write_register_host_interface(&spi_master,MASTER_CLOCK_FREQUENCY, MCLK_24576000Hz); 									

//16k/16bit 
//set 3s buffer need AHP size > 93k
//set 4s buffer need AHP size > 125k
//so when we set 3s,we can config memory 112kw

#ifdef HW_CHIP_D4P
		write_register_host_interface(&spi_master,AUDIO_BUFFER_SIZE, AUDIO_BUFFER_3_SEC); 									
#else
		write_register_host_interface(&spi_master,AUDIO_BUFFER_SIZE, AUDIO_BUFFER_1_SEC); 									
#endif
#if 1		//split will get 64*n remain 
		write_register_host_interface(&spi_master,GENERAL_CONFIG_1,	SPLIT_MIC_BUFFER	|
														DSP_CLK_GEN_PLL		| 
														MAX_NUMBER_OF_MIC_IS_4); 						    	
#else
		write_register_host_interface(&spi_master,GENERAL_CONFIG_1,	DSP_CLK_GEN_PLL		| 
														MAX_NUMBER_OF_MIC_IS_4);
#endif
#ifdef HW_CHIP_D4P
#if 1
#if 1
		write_register_host_interface(&spi_master,MEMORY_CONFIG, AMODEL_0_LOC_DTCM						|
														AMODEL_1_LOC_DTCM						|
														AMODEL_4_LOC_AHB						|
														AHB_ON_SIZE_112KW_32KW_CACHE			|
														AUDIO_BUF_LOC_AHB_USING_MEM_ALLOCATION	|
														DTCM_SIZE_D4_D6_96_KW_D8_192KW);		//			# If no need for VC this can be reduced to 112KW on D8 for
#else
	write_register_host_interface(&spi_master,MEMORY_CONFIG, AMODEL_0_LOC_DTCM						|
														AMODEL_1_LOC_DTCM						|
														AMODEL_4_LOC_AHB						|
														AHB_ON_SIZE_192KW_32KW_CACHE_D8			|
														AUDIO_BUF_LOC_AHB_USING_MEM_ALLOCATION	|
														DTCM_SIZE_D4_D6_96_KW_D8_192KW);		//
#endif
#else
		write_register_host_interface(instance_D4, MEMORY_CONFIG, AMODEL_0_LOC_DTCM						|
														AMODEL_4_LOC_AHB						|
														AHB_ON_SIZE_128KW_16KW_CACHE			|
														AUDIO_BUF_LOC_AHB_USING_MEM_ALLOCATION	|
														DTCM_SIZE_D4_D6_96_KW_D8_128KW);		//			# If no need for VC this can be reduced to 112KW on D8 for

#endif
#else

#if 0
		write_register_host_interface(instance_D4, MEMORY_CONFIG, AMODEL_0_LOC_DTCM						|
														AMODEL_4_LOC_AHB						|
														AHB_ON_SIZE_128KW_16KW_CACHE			|
														AUDIO_BUF_LOC_AHB_USING_MEM_ALLOCATION	|
														DTCM_SIZE_D4_D6_96_KW_D8_128KW);
#else
		write_register_host_interface(&spi_master,MEMORY_CONFIG, AMODEL_0_LOC_AHB						|
														AHB_ON_SIZE_128KW_16KW_CACHE			|
														DTCM_SIZE_D4_D6_96_KW_D8_128KW);

		//write_register_host_interface(instance_D4, MEMORY_CONFIG, 0x175);
#endif
#endif
		

	//vad	voice activity detection
	write_register_host_interface(&spi_master,GENERAL_CONFIG_2,	FW_VAD_TYPE_NO_VAD		|
														DDF_SAMPLE_WIDTH_16_BIT | 
														MIC_SAMPLE_RATE_16K); 							     	

	
	if (m_clk == 32768)
	{

#if 1
		write_register_host_interface(&spi_master,DSP_CLOCK_CONFIG,	PLL_STEP_7 		|
															TL3_DIV_1		|
															APB_DIV_1		|
															AHB_DIV_1);			

#else
		write_register_host_interface(instance_D4, DSP_CLOCK_CONFIG,	PLL_STEP_4 		|
															TL3_DIV_1		|
															APB_DIV_1		|
															AHB_DIV_1);			

	write_register_host_interface(instance_D4, DSP_CLOCK_CONFIG_EXTENSION, SRC_SET_CONFIG_CLK_ACCORDING_TO_CLK_SEL   |
																				PLL_OSC_SEL_USE_OSC	|
																				CLK_EXT_PLL_STEP_4
																);

#endif
		wait_ms(50);																													
																													

	}
	else
	{
		write_register_host_interface(&spi_master,DSP_CLOCK_CONFIG,	PLL_STEP_3		|
																	TL3_DIV_1		|
																	APB_DIV_1		|
																	AHB_DIV_1	);		

		wait_ms(100);																													
	}																												

	//we need not uart
	//write_register_host_interface(UART_BAUD_RATE, NORMAL_MODE_UART_BAUD_RATE);		
		

	//write_register_host_interface(&spi_master,AUDIO_BUFFER_READOUT_CONFIG, /*EXTENDED_HISTORY_TIME_500_MS					| */
	//																	HISTORY_POINT_IS_PASS_PHRASE_START_POINT);
	write_register_host_interface(&spi_master,AUDIO_BUFFER_READOUT_CONFIG, HISTORY_POINT_IS_PASS_PHRASE_END_POINT);		//fu modify it for also detect trigger word

	write_register_host_interface(&spi_master,HPF_ENABLE_DISABLE,	0x0010 					|
															IIR_HPF_EN				|
															DC_REMOVE_COARSE_EN		);		
#if 0			// fu test for int
	write_register_host_interface(PHRASE_DETECTION_GPIO_CONFIG,	SET_SEC_GPIO					|
																			SEC_MODEL_DET_GPIO_NUM_20		|
																			SET_PRI_GPIO					|
																			PRI_MODEL_DET_GPIO_NUM_20);

#else

#if 0
	write_register_host_interface(&spi_master,PHRASE_DETECTION_GPIO_CONFIG,	SET_SEC_GPIO					|
																			PRI_MODEL_DET_GPIO_NUM_20		|
																			SET_PRI_GPIO					|
																			SEC_MODEL_DET_GPIO_NUM_20);
#else
	write_register_host_interface(&spi_master,PHRASE_DETECTION_GPIO_CONFIG,	SET_SEC_GPIO			|
																			MODE_GPIO_NUM_12		|
																			SET_PRI_GPIO			|
																			DET_GPIO_NUM_12);
#endif

#endif
	return 0;

}


/***********************************************************************
* FUNCTION NAME: dump_audio_data()
*
* DESCRIPTION:
*	read SPI data from DBMD4 chip
* PARAMETERS:
*
* RETURNS:
*	bytes_to_read
***********************************************************************/
int dump_audio_data(char *buf, int max_size)
{
	uint16_t remained_samples_in_d4;
	int bytes_to_read;
	int retries = 0;
	uint16_t value;
	int n_total;
	int n_read;
	int tmp_size;
	int read_size;
//	int set_buf_size;
//	static int k = 1;
#if 1
	do {
		remained_samples_in_d4 = read_register_host_interface(&spi_master,NUMBER_OF_SAMPLES_IN_BUFFER);
		//printf("remained in d4=%d\n",remained_samples_in_d4);
		//set_buf_size = read_register_host_interface(&spi_master,AUDIO_BUFFER_SIZE);
		/* current_timestamp(); */ /* debug */
		if(remained_samples_in_d4 == 0xffff) {
				printf("audio buffer is not in streaming.\n");
				return 0; /* no more data. */
		}
		//printf("remain&&\n");
		if(remained_samples_in_d4 == 0x0000) {
			/* printf("."); */ /* debug */
			printf("^^^^^^^&&&&&&&  get remain is 0\n");
			retries++;
			wait_ms(10);
			if(retries < 100)
				continue;
			else {
				printf("remained samples in d4 0.\n");
				value = read_register_host_interface(&spi_master,FW_VERSION_NUMBER);
				if (value >= 0) {
					printf ("D4 FirmWare version = 0x%X\n", value);
				} else {
					printf ("FAIL read_register(FW_VERSION_NUMBER)\n");
				}
				return 0;
			}
		} else {
			printf("remained_samples_in_d4 =%d\n",remained_samples_in_d4);
			//printf("buf size len=%d\n",set_buf_size);
			break;
		}
	} while(1);
	//printf("start calc bytes&&\n");
//	if((*sared_stop_streaming_flag) == 1) {
//		printf("*sared_stop_streaming_flag ==1\n");
//
//		return 0;
//	}
	bytes_to_read = remained_samples_in_d4 * 16 ;
	//bytes_to_read = remained_samples_in_d4 * 8 ;
	if(bytes_to_read > max_size)
	{
		bytes_to_read = max_size;
	}
	else
	{
	#if 0
		if(remained_samples_in_d4 >= 128)
			bytes_to_read = max_size;
		else
			bytes_to_read = 32*remained_samples_in_d4;
	#else
		//if(remained_samples_in_d4 < 128)
		//	return 0;
			
	#endif
	}
#endif
	
	
#if 1
	
//	bytes_to_read /= 2;
//	tmp_size = bytes_to_read;
	write_register_host_interface(&spi_master,READ_AUDIO_BUFFER, (bytes_to_read/16));
	//write_register_host_interface(&spi_master,READ_AUDIO_BUFFER, (bytes_to_read/8));
	//wait_ms(1);
	
	n_total = 0;
	do 
	{
		//n_read = device_read(&buf[n_total], bytes_to_read - n_total);
		//n_read = device_write_spi(&spi_master,&buf[n_total],bytes_to_read - n_total);
		n_read = device_read_spi(&spi_master,&buf[n_total],bytes_to_read - n_total);
		//if(tmp_size > max_size)
		//	read_size = max_size;
		//else
		//	read_size = tmp_size;
		//n_read = device_read_spi(&spi_master,buf,read_size);	// i want to test once read over
		if(n_read > 0)
		{
			n_total += n_read;
			//tmp_size -= n_read;
			//k++;
			//if(n_total > k)
			//printf("ntotal-k =%d,get buff[ntotal]=%d,nread=%d\n",n_total,buf[n_total-k],n_read);
			printf("ntotal=%d,nread=%d\n",n_total,n_read);
		}
		else
		{
			printf("read timeout\n");
			return 0;
		}
		
	} while (n_total < bytes_to_read);
#if 0
	k++;
	if(k > 5)
	{
		k = 0;
		printf("set to detection mode\n");
		write_register_host_interface(&spi_master,OPERATION_MODE, DETECTION_MODE);	//i modify it to detection mode
		wait_ms(10);
	}
#endif
	/* current_timestamp(); */ /*debug */
	//wait_ms(80);
	return n_total;
#else
	wait_ms(10);
	printf("test for not read\n");
	
	return max_size;
#endif
}


/***********************************************************************
* FUNCTION NAME: pull_buffer()
*
* DESCRIPTION:
*	read the streamimg from DBMD4 chip and collect them into file
* PARAMETERS:
*
* RETURNS:
*	0 on sucsses else -1.
***********************************************************************/
//char buf[0x100*16 + 4] = {0};
//char file_buf[320000]; /* 10sec*16000*16/8 */
//strategy we define 5s max size,if transfer data is large this ,we will read the max data,or we read assign size data

int pull_buffer(void)
{
#if 1
	int buffer_time_in_sec = 5;
	//int buffer_time_in_sec = 10;
	//int data_chunk_size = 0x90;
	//int data_chunk_size = 0x100;	//fu modify to it
	int data_chunk_size = 0x80;	//fu modify to it 2048
	//int data_chunk_size = 4;	//fu modify to it
	//int data_chunk_size = 0x7D0;
	//read length should set suitable large,if we set large ,we read remain is small ,so read will reduce
	//if we set too small, when http has large delay,audio buf may full,this will cause lose much data
	//we need ref sample_rate , data depth
	int Fifo_size = buffer_time_in_sec*16000*16/8; /*(time * sample_rate * bits_per_sample / convert_from_bits_to_bytes) */
	int current_data_count = 0 ;
	int read_len;
	int total_bytes;
	uint16_t val;
	int retries = 0;
	//char buf[2560];
	char *buf = NULL;
	int k,ret = 0;

//we throw data to  cloud,not a tmp sound file
#if 0
#ifdef QED_MULTIPLE_QUERIES
	mno
	char filename[32];
	snprintf(filename, 32, "audio_file_QED%d.raw", multi_qed_counter);
	audio_file = fopen (filename, "wb");
#else

#ifdef TEST_UC2S_48K32B
	ghi
	audio_file = fopen ("audio_file_48k32b.raw", "wb");
#elif defined(TEST_UC2S_48K16B)
	abc
	audio_file = fopen ("audio_file_48k16b.raw", "wb");
#elif defined(TEST_UC2S_16K32B)
	def
	audio_file = fopen ("audio_file_16k132.raw", "wb");
#else
	audio_file = fopen ("audio_file_16k16b.raw", "wb");
#endif


#endif

#endif

#if 1
	printf("\n[%s]: ~~~~~~~~~~ pull_buffer  ~~~~~~~~~~~\n", __func__);
	//buf = (uint8_t *)malloc(MAX_PULL_BUFF);
	buf = (char *)malloc(MAX_PULL_BUFF);
	if(buf == NULL)
	{
		printf("no more mem \n");
		return -1;
	}
	memset(buf,0,MAX_PULL_BUFF);
#endif

	//we not need invoke fread /fwrite
//	if(setvbuf(audio_file, buf_file, _IONBF, sizeof(buf_file)) != 0)
//		printf("Incorrect type or size of buffer.\n");

	printf ("Start downloading %d seconds of audio buffer, size = %d.\n", buffer_time_in_sec, Fifo_size);
	printf ("Please wait ...\n\n"); 
	do {
		val = read_register_host_interface(&spi_master,OPERATION_MODE);
		printf("read operation=%d\n",val);
		if(val == 0) {
			retries++;
			//TickDelay(DEL_10);
			wait_ms(10);
			if(retries < 100)
				continue;
			else {
				printf("OPERATION_MODE 0, exit.\n");
				val = read_register_host_interface(&spi_master,FW_VERSION_NUMBER);
				if(val >= 0) {
					printf ("D4 FirmWare version = 0x%X\n", val);
				} else {
					printf ("FAIL read_register(FW_VERSION_NUMBER)\n");
				}
				//how can i exit it
				//exit(1);
				if(buf != NULL)
					free(buf);
				return -1;
			}
		} else {
			break;
		}
	} while(1);

	//total_bytes = Fifo_size;
	//we do not has this more memroy,so we test for 2048 byte
	//we set fix size data,read_len is actual read data size
	send_head_to_baidu();		//send header http
	total_bytes = Fifo_size;
	while(total_bytes >= (data_chunk_size * 16)) {
		//wait_ms(1);
		//read_len = dump_audio_data(buf+current_data_count, data_chunk_size * 16);
		//memset(buf,0,sizeof(buf));		//i only want to kown what i see
		read_len = dump_audio_data(buf, data_chunk_size * 16);		//i only want to see what i recv
		if(read_len == 0) {
			printf("read end^^^^^^\n");
			//for(k=0; k<(data_chunk_size * 16); k++)
			//{
			//	printf("buf[%d]=%x\n",k,buf[k]);
			//}
			//if(total_bytes > 0)
			//{
				//default send 160000byte ,so full in zero
				int tmp_total_byte;
				memset(buf,0,sizeof(buf));
				while(total_bytes > 0)
				{
					if(total_bytes >= MAX_PULL_BUFF)
						tmp_total_byte = MAX_PULL_BUFF;
					else
						tmp_total_byte = total_bytes;
					
					send_data_to_baidu(buf,tmp_total_byte);
					total_bytes -= tmp_total_byte;
				}
			//}
			goto out;
		}
		//memcpy(p_file_buf, buf, read_len);
		//p_file_buf += read_len;
		current_data_count += read_len;
		total_bytes -= read_len;
#if 0
		if(y == 20) {
			if(x > 3) x = 0;
			printf("%c\b", barRotate[x++]);
			y = 0;
			fflush(stdout);
		}
		TickDelay(80);
#endif
		//wait_ms(20);
		//vTaskDelay(20 / portTICK_PERIOD_MS);
		//printf("current_data=%d\n",current_data_count);
		//for(k=0; k< read_len; k++)
		//{
		//	printf("buf[%d]=%x\n",k,buf[k]);
		//}
		
//		send_recv_info(buf,read_len);		//send to fd socket
		send_data_to_baidu(buf,read_len);
#if 0
		if(current_data_count > 80000)
		{
			if(ret == 0)
			{
				ret = 1;
				wait_ms(400);
			}
		}
#endif
	}

	printf("current read data = %d,left count=%d\n",current_data_count,total_bytes);
	//for(k=0; k<(data_chunk_size * 16); k++)
	//{
	//	printf("buf[%d]=%x\n",k,buf[k]);
	//}
	//last read at most less data_chunk_count*16
	while(total_bytes > 0)
	{
		//read_len = dump_audio_data(buf+current_data_count, total_bytes);
		printf("read for last byte\n");
		read_len = dump_audio_data(buf, total_bytes);		//i only want to see what i recv
		if(read_len == 0)
		{
			goto out;
		}
		current_data_count += read_len;
		total_bytes -= read_len;
		
		//for(k=0; k < read_len; k++)
		//{
		//	printf("buf[%d]=%x\n",k,buf[k]);
		//}
		//send_recv_info(buf,read_len);		//send to fd socket
		printf("send last buf len=%d\n",read_len);
		send_data_to_baidu(buf,read_len);
	}
	//read respone
	//get_result_baidu();
	
#if 0
	for(k=0; k<current_data_count; k++)
	{
		printf("buf[%d]=%x\n",k,buf[k]);
	}
#endif
	printf("[END]current_data_count:%d,Fifo_size(%d),total_bytes(%d) \n",current_data_count,Fifo_size,total_bytes);

out:
	get_result_baidu();
	val = read_register_host_interface(&spi_master,NUMBER_OF_SAMPLES_IN_BUFFER);
	printf ("End of pulling audio data!\n");

#if 1
	printf("\n[%s]: eeeeeeeeeeee  pull_buffer eeeeeeeeeeeee \n", __func__);
#endif

	if(buf != NULL)
		free(buf);
	return 0;
#else
#if 0
	char *send_buffer = NULL;
	int read_len = 1024;

	send_buffer = (char *)malloc(MAX_PULL_BUFF);
	if(send_buffer == NULL)
	{
		printf("no more mem \n");
		return -1;
	}
	memset(send_buffer,0,MAX_PULL_BUFF);
	memcpy(send_buffer,voice_data,1024);
	send_head_to_baidu();
	send_data_to_baidu(send_buffer,read_len);
	get_result_baidu();

	if(send_buffer != NULL)
		free(send_buffer);
	return 0;
#endif
#endif
}



