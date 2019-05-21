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


#include "UserCase_1Mic.h"
#include "dbmdx_melon_def.h"
#include "LoadFile.h"
#include "Dbmd_rtl_Device.h"
#include "Dbmd_init.h"
#include "UserCase_Commom.h"

int aec_ref_bit_depth = 16;			
int sample_rate  = 16000;


/***********************
*****use case load and init
************************/
int use_case_AEC_1mic_mono(int d4_i2s, int use_analog_mic)
{
	int value;
	int	clk_delay;
	
	printf("[%s]: Start.\n", __func__);


	//we close change clock src
	//change_clock_src(sample_rate,aec_ref_bit_depth);
	wait_ms(100);
	//write_register_host_interface(instance_D4, HOST_INTERFACE_SUPPORT, HOST_INTERFACE_UART | 0x1000);

	Load_ASRP_paramD4();

//	change_asrp_start_delay(0);
	//$$commit : asrp_register is above 0x100,so need invoke ASRP_read_register()
	//$$commit : when i use write_register_host_interface,i also can read this register correct
	if (asrp_delay > 0)
		write_register_host_interface(&spi_master,ASRP_USER_DEFINED_DELAY, asrp_delay);

	//write_register_host_interface(instance_D4, GENERAL_CONFIG_2, 0x0000);
#if 1	//even through i config with VAD,but it doesnt have any effect
	write_register_host_interface(&spi_master,GENERAL_CONFIG_2,	FW_VAD_TYPE_NO_VAD			|
																DDF_SAMPLE_WIDTH_16_BIT		| 
																MIC_SAMPLE_RATE_16K); 									
#else
		write_register_host_interface(&spi_master,GENERAL_CONFIG_2,	FW_VAD_TYPE_HW_VAD0			|
																DDF_SAMPLE_WIDTH_16_BIT		| 
																MIC_SAMPLE_RATE_16K); 
#endif
	
	/* Audio  Config */
	if (sample_rate == 48000) 
	{
		if (d4_i2s)
		{


			write_register_host_interface(&spi_master,AUDIO_PROCESSING_CONFIG, 	EN_48_EC_REF_DECIM		|
																				EN_COPY_HIST                            |
	                                                              											POST_DETECTION_MODE_SWITCH_TO_STREAMING |
	                                                              											VT_CP0                                  |
	                                                              											VT_PROC_EN                              |
	                                                              											ALGO2_EN_FW_MODE_1_ONLY         		|
	                                                              											ALGO1_EN_FW_MODE_1_AND_MODE_2);
			wait_ms(5);
		}
		else
		{
			write_register_host_interface(&spi_master,AUDIO_PROCESSING_CONFIG,	EN_48_EC_REF_DECIM						|
																				POST_DETECTION_MODE_SWITCH_TO_STREAMING	|
																				VT_CP0									|
																				VT_PROC_EN								|
																				ALGO1_EN_FW_MODE_1_AND_MODE_2);
			wait_ms(5);
		}
		
	}
	else	if (sample_rate == 16000) 
	{

		if (d4_i2s)
		{

			write_register_host_interface(&spi_master,AUDIO_PROCESSING_CONFIG, EN_COPY_HIST                            |
	                                                              											POST_DETECTION_MODE_SWITCH_TO_STREAMING |
	                                                              											VT_CP0                                  |
	                                                              											VT_PROC_EN                              |
	                                                              											ALGO2_EN_FW_MODE_1_ONLY         		|
	                                                              											ALGO1_EN_FW_MODE_1_AND_MODE_2);
			wait_ms(5);
		}
		else
		{
#if 1		//i dont find any affect when i shield it
			write_register_host_interface(&spi_master,AUDIO_PROCESSING_CONFIG,	POST_DETECTION_MODE_SWITCH_TO_STREAMING		|
	                                                              											VT_CP0                                  |
	                                                              											VT_PROC_EN                              |
                                                              											ALGO1_EN_FW_MODE_1_AND_MODE_2);
#endif	
			wait_ms(5);
		}

	}



#if 1	// i dont find any affect when i shield it
	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_0			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_CP_0);

	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_1 			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_NO_CP);

	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_2 			|
																	IO_3N_2_CP_2		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_NO_CP);

	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_3 			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_NO_CP);													
	
	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_4 			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_NO_CP);			
																											
	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_5 			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_CP_0);													
																	
	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_6 			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_NO_CP);																		
	
	write_register_host_interface(&spi_master,AUDIO_PROCESSING_ROUTING,		IO_SET_7 			|
																	IO_3N_2_NO_CP		|
																	IO_3N_1_NO_CP		|
																	IO_3N_0_NO_CP);																																			
#endif
	//we not use TDM
#if 0	
	/* TDM HW Regs config */
	/* D4 TDM 0 Recive */
	write_register_host_interface(TDM_ACTIVATION_CONTROL, CONFIG_TDM_0);


	if (sample_rate == 48000)
	{
		write_register_host_interface(TDM_RX_CONFIG,	DEMUX_MUX_DISABLE					|
																	SAMPLE_WIDTH_16_BIT 				|
																	INPUT_OUTPUT_SAMPLE_RATE_48_KHz		|
																	RX_TX_I2S_CH_USE_HIGH_WORD_ONLY		|
																	RESAMPLE_TYPE_DECIMATION			|
																	RESAMPLE_RATIO_NO_RESAMPLING		|
																	HW_BLOCK_EN							|
																	RX_TX_CP2);
	}
	else if (sample_rate == 16000)
	{
		write_register_host_interface(TDM_RX_CONFIG,	DEMUX_MUX_DISABLE					|
																	SAMPLE_WIDTH_16_BIT 				|
																	INPUT_OUTPUT_SAMPLE_RATE_16_KHz		|
																	RX_TX_I2S_CH_USE_HIGH_WORD_ONLY		|
																	RESAMPLE_TYPE_DECIMATION			|
																	RESAMPLE_RATIO_NO_RESAMPLING		|
																	HW_BLOCK_EN							|
																	RX_TX_CP2);
	}
	
	if (aec_ref_bit_depth == 16)
	{
		set_tdm_hw(instance_D4 , D4_TDM_0_RX_ADDR, 0x00800015, 0x0000007, 0x100f001f);
	}
	else	
	{	
		set_tdm_hw(instance_D4 , D4_TDM_0_RX_ADDR, 0x0080405d, 0x00002064, 0x103f003f);
		set_tdm_hw_mcee(instance_D4 , D4_TDM_0_RX_ADDR, 0x5);

	}
#endif
	write_register_host_interface(&spi_master,AUDIO_STREAMING_SOURCE_SELECT,	NO_STREAM_CH_4		|
																				NO_STREAM_CH_3		|
																				NO_STREAM_CH_2		|
																				STREAM_CH_1_CP_0);											
//	write_register_host_interface(&spi_master,AUDIO_STREAMING_SOURCE_SELECT,	STREAM_CH_4_CP_0		|
//																				STREAM_CH_3_CP_0		|
//																				STREAM_CH_2_CP_0		|
//																				STREAM_CH_1_CP_0);	

	/* Mics Config */

	if (use_analog_mic)
	{
		write_register_host_interface(&spi_master,FIRST_MICROPHONE_CONFIG,	DDF_AUDIO_ATTENUATION_6dB_GAIN		|
																		SAR_128Khz							|
																		DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC);
		wait_ms(1);
		write_register_host_interface(&spi_master,MICROPHONE_ANALOG_GAIN,SD_14_DB_SAR_32_DB);
		write_register_host_interface(&spi_master,HPF_ENABLE_DISABLE,	0x0010 					|
															IIR_HPF_EN				|
															DC_REMOVE_COARSE_EN		);


		//write_register_host_interface(instance_D4, HPF_ENABLE_DISABLE,	0x280					|
		//													DC_REMOVE_FINE_EN		|
		//													DC_REMOVE_COARSE_EN		|
		//													IIR_HPF_EN);												
	}
	else
	{
		write_register_host_interface(&spi_master,FIRST_MICROPHONE_CONFIG,	DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12					|
																		DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11					|
																		DDF_AUDIO_ATTENUATION_0dB							|
																		DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz		| 
																		CLOCK_POLARITY_FALLING_EDGE							|
																		DDF_AND_DM_CONFIG_SD_DDF_DM1);	
		wait_ms(1);	
#ifdef NOT_BYPASS    
		write_register_host_interface(&spi_master,HPF_ENABLE_DISABLE,	0x0010 					|
															IIR_HPF_EN				|
															DC_REMOVE_COARSE_EN		);
#endif

	}
	wait_ms(1);	

	if (d4_i2s)
	{
		write_register_host_interface(&spi_master,TDM_ACTIVATION_CONTROL, CONFIG_TDM_0);
	
		if (sample_rate == 48000)
		{
			write_register_host_interface(&spi_master,TDM_TX_CONFIG, DEMUX_MUX_DISABLE							   |
																SAMPLE_WIDTH_16_BIT 						  |
																NUM_OF_CHANNELS_1_CH						  |
																INPUT_OUTPUT_SAMPLE_RATE_48_KHz 			  |
																RX_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
																RESAMPLE_RATIO_3_1							  |
																RESAMPLE_TYPE_INTERPOLATION 				  |
																HW_BLOCK_EN 								  |
																RX_TX_CP3);
		}
		else if (sample_rate == 16000)
		{
			write_register_host_interface(&spi_master,TDM_TX_CONFIG, DEMUX_MUX_DISABLE							   |
																SAMPLE_WIDTH_16_BIT 						  |
																NUM_OF_CHANNELS_1_CH						  |
																INPUT_OUTPUT_SAMPLE_RATE_16_KHz 			  |
																RX_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
																RESAMPLE_RATIO_NO_RESAMPLING				  |
																RESAMPLE_TYPE_DECIMATION					  |
																HW_BLOCK_EN 								  |
																RX_TX_CP3);
				
		}
	
	}
	//not use TDM
#if 0
			if (aec_ref_bit_depth == 16)
			{
			set_tdm_hw(instance_D4 , D4_TDM_0_TX_ADDR, 0x804053, 0x241024, 0x100f001f);
			set_tdm_hw_mcee(instance_D4 , D4_TDM_0_TX_ADDR, 0xf);
			}
			else
			{
	
			set_tdm_hw(instance_D4 , D4_TDM_0_TX_ADDR, 0x804053, 0x64, 0x101f003f);
			set_tdm_hw_mcee(instance_D4 , D4_TDM_0_TX_ADDR, 0x5);
			}
	
#endif
	if (d4_asrp_rec || d4_fw_rec)
	{
			if (d4_asrp_rec)
			{
				write_register_host_interface(&spi_master,ASRP_RECORDS_CHANNELS,0x0019);											
				write_register_host_interface(&spi_master,IO_PORT_ADDRESS_LOW,0x0000);
				write_register_host_interface(&spi_master,IO_PORT_ADDRESS_HIGH,0x0000);
				write_register_host_interface(&spi_master,IO_PORT_VALUE_LOW,0x0000);
			}
			else
			{
				write_register_host_interface(&spi_master,IO_PORT_ADDRESS_LOW,rec_points_5_d4);
				write_register_host_interface(&spi_master,IO_PORT_ADDRESS_HIGH,rec_points_6_d4);
				write_register_host_interface(&spi_master,IO_PORT_VALUE_LOW,rec_points_7_d4);
			}
	//we dont use uart debug
#if 0
#ifdef MELON_V_3_111
			write_register_host_interface(instance_D4, UART_DEBUG_RECORDING,	ENABLE_RECORDING			|
																				ENLARGE_BUFFER_BY_1_5		|
																				ENLARGE_BUFFER_BY_2 		|
																				ALLOCATE_BUFFER_IN_AHB_MEMORY); 				
	
#else
			write_register_host_interface(instance_D4,	UART_DEBUG_RECORDING,ENABLE_RECORDING); 						
#endif
	
#endif
		//	Driver.StartDigitalRecording(dbmd4_uart)
							
	}
		//if MIPS_METER
		//	write_register_host_interface(instance_D4, FW_DEBUG_REGISTER,PRINT_MIPS_METER)											
	
	
	if (d4_i2s)
	{
	
		if (sample_rate== 48000 && aec_ref_bit_depth == 32)
			clk_delay = TDM_SYNC_DELAY_5_CLKS_CYCLES;
		else
			clk_delay = TDM_SYNC_DELAY_4_CLKS_CYCLES;
	
		write_register_host_interface(&spi_master,AUDIO_ROUTING_CONFIG, TDM_SYNC_RIGHT_CH	  |
																								   USE_TDM_MUSIC_TO_SYNC |
																								   MUSIC_IN_TDM0		 |
																								   clk_delay);
	
		if (fw_debug_mode)
					write_register_host_interface(&spi_master,0x17, 0x1000);
			
	}
	
	write_register_host_interface(&spi_master,OPERATION_MODE,DETECTION_MODE);
	//write_register_host_interface(&spi_master,TDM_ACTIVATION_CONTROL,PROC_EN_SWITCH_FW_TO_DETECTION_MODE);
	//write_register_host_interface(&spi_master,TDM_ACTIVATION_CONTROL,PROC_EN_SWITCH_FW_TO_BUFFERING_MODE);
	
#if 0		//we dont use TDM
#if 1
			write_register_host_interface(TDM_ACTIVATION_CONTROL,	TDM0_RX_EN		|
																				TDM0_TX_EN		|
																				PROC_EN_SWITCH_FW_TO_DETECTION_MODE);	
	
#else
		write_register_host_interface(instance_D4, TDM_ACTIVATION_CONTROL,	TDM0_RX_EN		|														
																			PROC_EN_SWITCH_FW_TO_DETECTION_MODE);		
#endif
#ifdef NON_VT_TEST
		if (non_vt)
		{
			//TickDelay(DEL_10);
			write_register_host_interface(TDM_ACTIVATION_CONTROL,	TDM0_RX_EN		|
																				TDM0_TX_EN		|
																				PROC_EN_SWITCH_FW_TO_BUFFERING_MODE);	
		}
#endif
#endif
#if 0
		//int val = read_register_host_interface(&spi_master,ASRP_USER_DEFINED_DELAY);		// fu add for test how write asrp register 
		int16_t val_tmp;
		//val_tmp = ASRP_read_register(&spi_master,ASRP_USER_DEFINED_DELAY);
		val_tmp = read_register_host_interface(&spi_master,ASRP_USER_DEFINED_DELAY);
		printf("get val=%d\n",val_tmp);
#endif
		printf("[%s]: Finish.\n", __func__);
		return 0;
}

