
/* Command Registers on DBMDX */

//////////////////////////////////////////////////////////////

 
// Registers on DBMDx chip
#define FW_VERSION_NUMBER  0x00
#define OPERATION_MODE  0x01
#define DIGITAL_GAIN  0x04
#define 		KEEP_CURRENT_GAIN  0x0800
#define 		GAIN_AFFECTS_MIC1  0x1000
#define 		GAIN_AFFECTS_MIC2  0x2000
#define 		GAIN_AFFECTS_MIC3  0x3000
#define 		GAIN_AFFECTS_MIC4  0x4000
#define 		GAIN_AFFECTS_ALL_MICROPHONES  0xF000


#define PRIMARY_ACOUSTIC_MODEL_SIZE 	0x02
#define SECONDARY_ACOUSTIC_MODEL_SIZE 	0x03


#define IO_PORT_ADDRESS_LOW  0x05
#define IO_PORT_ADDRESS_HIGH  0x06
#define IO_PORT_VALUE_LOW  0x07
#define IO_PORT_VALUE_HIGH  0x08

#define AUDIO_BUFFER_SIZE  0x09
#define 		AUDIO_BUFFER_1_SEC  0x0800
#define 		AUDIO_BUFFER_1_5_SEC  0x0C00
#define 		AUDIO_BUFFER_2_SEC  0x1000
#define 		AUDIO_BUFFER_2_5_SEC  0x1400
#define 		AUDIO_BUFFER_3_SEC  0x1800

#define			AUDIO_BUFFER_4_SEC	0x2000		//fu @@

#define			AUDIO_BUFFER_5_SEC	0x3000		//fu @@


#define NUMBER_OF_SAMPLES_IN_BUFFER  0x0A
#define LAST_MAXIMUM_SAMPLE_VALUE  0x0B
#define UART_BAUD_RATE  0x0C
#define 		UART_BAUD_RATE_115_200_hz   0x0480
#define 		UART_BAUD_RATE_230_400_hz   0x0900
#define 		UART_BAUD_RATE_460_800_hz   0x1200
#define 		UART_BAUD_RATE_921_600_hz   0x2400
#define 		UART_BAUD_RATE_1_Mhz   0x2710
#define 		UART_BAUD_RATE_2_Mhz   0x4e20
#define 		UART_BAUD_RATE_3_Mhz   0x7530
#define 		UART_BAUD_RATE_4_Mhz   0x9c40
#define 		UART_BAUD_RATE_5_Mhz   0xc350

#define FW_STATUS_INDEX  0x0D
#define 		READ_FW_ERROR_COUNTER  0x0000
#define 		GET_AMOUNT_OF_FREE_DATA_MEMORY  0x0001
#define 		GET_AMOUNT_OF_FREE_AHB_MEMORY  0x0002

#define MICROPHONE_AUTO_SET  0x0E
#define 		AUTO_EN_DIS_DDF_BLOCK_D2_AFE_DDF_D4_SD_DDF  0x0001
#define 		AUTO_EN_DIS_DDF_BLOCK_D2_PDM0L_D4_DDF0_D6_DDF0  0x0002
#define 		AUTO_EN_DIS_DDF_BLOCK_D2_PDM0R_D4_DDF1_D6_DDF1  0x0004
#define 		AUTO_EN_DIS_DDF_BLOCK_D2_PDM1L_D4_SAR_DDF_D6_SAR_DDF  0x0008
#define 		AUTO_EN_DIS_DDF_BLOCK_D2_PDM1R  0x0010
#define 		AUTO_EN_DIS_DDF_BLOCK_D4_SD_ADC  0x0020
#define 		AUTO_EN_DIS_DDF_BLOCK_D4_SAR_ADC_D6_SAR_ADC  0x0040
#define 		AUTO_EN_DIS_DDF_BLOCK_D4_DM1_D6_DM1  0x0080
#define 		AUTO_EN_DIS_DDF_BLOCK_D4_DM0_D6_DM0  0x0100
#define 		AUTOSET_EVENT_DISABLE_AUTO_SET  0x0000
#define 		AUTOSET_EVENT_ENABLE_ON_VAD  0x0200
#define 		AUTOSET_EVENT_ENABLE_ON_VT_DETECTION  0x0400

#define LOAD_NEW_ACOUSTIC_MODEL  0x0F
#define 		LOAD_PRIMARY_VT_MODEL  0x0000
#define 		LOAD_SECONDARY_VT_MODEL  0x0001
#define 		LOAD_GOOGLE_VT_MODEL  0x0002
#define 		LOAD_ASRP_PARAM_FILE  0x0004

#define DSP_CLOCK_CONFIG  0x10


#define           AHB_DIV_1  0x0000
#define           AHB_DIV_2  0x0001
#define           AHB_DIV_3  0x0002
#define           AHB_DIV_4  0x0003
#define           AHB_DIV_5  0x0004
#define           AHB_DIV_6  0x0005
#define           AHB_DIV_7  0x0006
#define           AHB_DIV_8  0x0007
#define           AHB_DIV_9  0x0008
#define           AHB_DIV_10  0x0009
#define           AHB_DIV_11  0x000A
#define           AHB_DIV_12  0x000B
#define           AHB_DIV_13  0x000C
#define           AHB_DIV_14  0x000D
#define           AHB_DIV_15  0x000E
#define           AHB_DIV_16  0x000F

#define           APB_DIV_1  0x0000
#define           APB_DIV_2  0x0010
#define           APB_DIV_3  0x0020
#define           APB_DIV_4  0x0030
#define           APB_DIV_5  0x0040
#define           APB_DIV_6  0x0050
#define           APB_DIV_7  0x0060
#define           APB_DIV_8  0x0070
#define           APB_DIV_9  0x0080
#define           APB_DIV_10  0x0090
#define           APB_DIV_11  0x00A0
#define           APB_DIV_12  0x00B0
#define           APB_DIV_13  0x00C0
#define           APB_DIV_14  0x00D0
#define           APB_DIV_15  0x00E0
#define           APB_DIV_16  0x00F0

#define           TL3_DIV_1  0x0000
#define           TL3_DIV_2  0x0100
#define           TL3_DIV_3  0x0200
#define           TL3_DIV_4  0x0300
#define           TL3_DIV_5  0x0400
#define           TL3_DIV_6  0x0500
#define           TL3_DIV_7  0x0600
#define           TL3_DIV_8  0x0700
#define           TL3_DIV_9  0x0800
#define           TL3_DIV_10  0x0900
#define           TL3_DIV_11  0x0A00
#define           TL3_DIV_12  0x0B00
#define           TL3_DIV_13  0x0C00
#define           TL3_DIV_14  0x0D00
#define           TL3_DIV_15  0x0E00
#define           TL3_DIV_16  0x0F00

#define           PLL_STEP_1  0x1000
#define           PLL_STEP_2  0x2000
#define           PLL_STEP_3  0x3000
#define           PLL_STEP_4  0x4000
#define           PLL_STEP_5  0x5000
#define           PLL_STEP_6  0x6000
#define           PLL_STEP_7  0x7000
#define           PLL_STEP_8  0x8000
#define           PLL_STEP_9  0x9000
#define           PLL_STEP_10  0xA000
#define           PLL_STEP_11  0xB000
#define           PLL_STEP_12  0xC000
#define           PLL_STEP_13  0xD000
#define           PLL_STEP_14  0xE000

#define           USE_PLL_STEP_FROM_REG_0x1E  0xF000

#define AUDIO_PROCESSING_ROUTING  0x11


#define           IO_SET_0 0x0000
#define           IO_SET_1 0x1000
#define           IO_SET_2 0x2000
#define           IO_SET_3 0x3000
#define           IO_SET_4 0x4000
#define           IO_SET_5 0x5000
#define           IO_SET_6 0x6000
#define           IO_SET_7 0x7000
#define           IO_SET_8 0x8000
#define           IO_3N_0_CP_0 0x0000
#define           IO_3N_0_CP_1 0x0001
#define           IO_3N_0_CP_2 0x0002
#define           IO_3N_0_CP_3 0x0003
#define           IO_3N_0_CP_4 0x0004
#define           IO_3N_0_CP_5 0x0005
#define           IO_3N_0_CP_6 0x0006
#define           IO_3N_0_CP_7 0x0007
#define           IO_3N_0_NO_CP 0x000E
#define           IO_3N_1_CP_0 0x0000
#define           IO_3N_1_CP_1 0x0010
#define           IO_3N_1_CP_2 0x0020
#define           IO_3N_1_CP_3 0x0030
#define           IO_3N_1_CP_4 0x0040
#define           IO_3N_1_CP_5 0x0050
#define           IO_3N_1_CP_6 0x0060
#define           IO_3N_1_CP_7 0x0070
#define           IO_3N_1_NO_CP 0x00E0
#define           IO_3N_2_CP_0 0x0000
#define           IO_3N_2_CP_1 0x0100
#define           IO_3N_2_CP_2 0x0200
#define           IO_3N_2_CP_3 0x0300
#define           IO_3N_2_CP_4 0x0400
#define           IO_3N_2_CP_5 0x0500
#define           IO_3N_2_CP_6 0x0600
#define           IO_3N_2_CP_7 0x0700
#define           IO_3N_2_NO_CP 0x0E00




#define AUDIO_BUFFER_READOUT_CONFIG  0x12
#define            HISTORY_POINT_IS_TIME_OF_SWITCHING_TO_BUFFERING  0x0000
#define            HISTORY_POINT_IS_PASS_PHRASE_END_POINT  0x0001
#define            HISTORY_POINT_IS_PASS_PHRASE_START_POINT  0x0002
#define            EXTENDED_HISTORY_TIME_500_MS  0x1f4
#define            START_BUF_READ  0x1000

#define AUDIO_STREAMING_SOURCE_SELECT  0x13
//#define NO_STREAMING  0xFFFF

#define            NO_STREAM_CH_1  0x000F
#define            STREAM_CH_1_CP_0  0x0000
#define            STREAM_CH_1_CP_1  0x0001
#define            STREAM_CH_1_CP_2  0x0002
#define            STREAM_CH_1_CP_3  0x0003
#define            STREAM_CH_1_CP_4  0x0004
#define            STREAM_CH_1_CP_5  0x0005
#define            STREAM_CH_1_CP_6  0x0006
#define            STREAM_CH_1_CP_7  0x0007
#define            NO_STREAM_CH_2  0x00F0
#define            STREAM_CH_2_CP_0  0x0000
#define            STREAM_CH_2_CP_1  0x0010
#define            STREAM_CH_2_CP_2  0x0020
#define            STREAM_CH_2_CP_3  0x0030
#define            STREAM_CH_2_CP_4  0x0040
#define            STREAM_CH_2_CP_5  0x0050
#define            STREAM_CH_2_CP_6  0x0060
#define            STREAM_CH_2_CP_7  0x0070
#define            NO_STREAM_CH_3  0x0F00
#define            STREAM_CH_3_CP_0  0x0000
#define            STREAM_CH_3_CP_1  0x0100
#define            STREAM_CH_3_CP_2  0x0200
#define            STREAM_CH_3_CP_3  0x0300
#define            STREAM_CH_3_CP_4  0x0400
#define            STREAM_CH_3_CP_5  0x0500
#define            STREAM_CH_3_CP_6  0x0600
#define            STREAM_CH_3_CP_7  0x0700
#define            NO_STREAM_CH_4  0xF000
#define            STREAM_CH_4_CP_0  0x0000
#define            STREAM_CH_4_CP_1  0x1000
#define            STREAM_CH_4_CP_2  0x2000
#define            STREAM_CH_4_CP_3  0x3000
#define            STREAM_CH_4_CP_4  0x4000
#define            STREAM_CH_4_CP_5  0x5000
#define            STREAM_CH_4_CP_6  0x6000
#define            STREAM_CH_4_CP_7  0x7000



#define GOOGLE_ALGORITHM  0x14
#define            GOOGLE_ENABLE  0x0001
#define            GOOGLE_INIT  0x0002
#define            GOOGLE_DETECTED  0x0010

#define PHRASE_DETECTION_GPIO_CONFIG  0x15
#define            PRI_MODEL_DET_GPIO_NUM_16  0x0010
#define            PRI_MODEL_DET_GPIO_NUM_15 0x000f
#define            PRI_MODEL_DET_GPIO_NUM_20 0x0014

#define SET_PRI_GPIO  0x0080
#define            SEC_MODEL_DET_GPIO_NUM_16 0x1000
#define            SEC_MODEL_DET_GPIO_NUM_15 0x0f00
#define            SEC_MODEL_DET_GPIO_NUM_20 0x1400

#define            SET_SEC_GPIO  0x8000
                               
                               
#define            PRI_MODEL_DET_GPIO_NUM_16  0x0010
#define            PRI_MODEL_DET_GPIO_NUM_15 0x000f
#define            PRI_MODEL_DET_GPIO_NUM_20 0x0014
                               
#define            SEC_MODEL_DET_GPIO_NUM_16 0x1000
#define            SEC_MODEL_DET_GPIO_NUM_15 0x0f00
#define            SEC_MODEL_DET_GPIO_NUM_20 0x1400
                               

#define MICROPHONE_ANALOG_GAIN  0x16
#define            SD_0_DB_SAR_0_DB  0x0000
#define            SAR_6_DB  0x0001
#define            SAR_12_DB  0x0002
#define            SAR_14_DB  0x0003
#define            SD_4_DB_SAR_16_DB  0x0004
#define            SAR_18_DB  0x0005
#define            SD_6_DB_SAR_20_DB  0x0006
#define            SAR_21_DB  0x0007
#define            SD_8_DB_SAR_22  0x0008
#define            SAR_23_DB  0x0009
#define            SD_10_DB_SAR_24  0x000A
#define            SAR_25_DB  0x000B
#define            SD_12_DB_SAR_26_DB  0x000C
#define            SAR_28_DB  0x000D
#define            SD_14_DB_SAR_32_DB  0x000E
#define            SAR_36_DB  0x000F
#define            SD_16_DB  0x0010
#define            SD_18_DB  0x0012
#define            SD_20_DB  0x0014
#define            SD_22_DB  0x0016
#define            SD_24_DB  0x0018
#define            SD_26_DB  0x001A
#define            SD_28_DB  0x001C
#define            SD_30_DB  0x001E
#define            SD_32_DB  0x0020
#define            SD_34_DB  0x0022
#define            SD_36_DB  0x0024


#define FW_DEBUG_REGISTER  0x18
#define            DUMP_APB_REGS  0x0001
#define            DUMP_AHB_REGS  0x0002
#define            FW_PRINT_CLOCK_SETTING  0x0004
#define            TOGGLE_UART_DEBUG_PRINTS  0x0005
#define            PRINT_MEM_INFO  0x000a
#define            PRINT_MIPS_METER  0x0014
 
#define CHIP_ID_NUMBER  0x19
#define HPF_ENABLE_DISABLE  0x1A

#define            IIR_HPF_EN  0x0001
#define            DC_REMOVE_COARSE_EN  0x0002
#define            DC_REMOVE_FINE_EN  0x0004
#define            IIR_HPF_CUTOFF_FREQ_60HZ  0x0000
#define            IIR_HPF_CUTOFF_FREQ_30HZ  0x01000

#define MASTER_CLOCK_FREQUENCY  0x1B
#define            MCLK_32768Hz  0x0020
#define            MCLK_12288000Hz  0x3000
#define            MCLK_24576000Hz  0x6000

#define I2C_SERVICE  0x1C

#define TDM_SCLK_CLOCK_FREQUENCY  0x1D
#define DSP_CLOCK_CONFIG_EXTENSION  0x1E


#define            CLK_EXT_PLL_STEP_1  0x0001
#define            CLK_EXT_PLL_STEP_2  0x0002
#define            CLK_EXT_PLL_STEP_3  0x0003
#define            CLK_EXT_PLL_STEP_4  0x0004
#define            CLK_EXT_PLL_STEP_5  0x0005
#define            CLK_EXT_PLL_STEP_6  0x0006
#define            CLK_EXT_PLL_STEP_7  0x0007
#define            CLK_EXT_PLL_STEP_8  0x0008
#define            CLK_EXT_PLL_STEP_9  0x0009
#define            CLK_EXT_PLL_STEP_10  0x000A
#define            CLK_EXT_PLL_STEP_11  0x000B
#define            CLK_EXT_PLL_STEP_12  0x000C
#define            CLK_EXT_PLL_STEP_13  0x000D
#define            CLK_EXT_PLL_STEP_14  0x000E
#define            CLK_EXT_PLL_STEP_15  0x000F
#define            CLK_EXT_PLL_STEP_16  0x0010
#define            CLK_EXT_PLL_STEP_17  0x0011
#define            CLK_EXT_PLL_STEP_18  0x0012
#define            CLK_EXT_PLL_STEP_19  0x0013
#define            CLK_EXT_PLL_STEP_20  0x0014
#define            CLK_EXT_PLL_STEP_21  0x0015
#define            CLK_EXT_PLL_STEP_22  0x0016
#define            CLK_EXT_PLL_STEP_23  0x0017
#define            CLK_EXT_PLL_STEP_24  0x0018
#define            CLK_EXT_PLL_STEP_25  0x0019
#define            CLK_EXT_PLL_STEP_26  0x001A
#define            CLK_EXT_PLL_STEP_27  0x001B
#define            CLK_EXT_PLL_STEP_28  0x001C
#define            CLK_EXT_PLL_STEP_29  0x001D
#define            CLK_EXT_PLL_STEP_30  0x001E
#define            CLK_EXT_PLL_STEP_31  0x001F



#define OSC_FREE_RUN  0x0800
#define            PLL_OSC_SEL_PLL_OSC_OFF  0x0000
#define            PLL_OSC_SEL_USE_PLL  0x1000
#define            PLL_OSC_SEL_USE_OSC  0x2000
#define            CLK_SEL_MCLK_IN  0x0000
#define            CLK_SEL_TDM0_SCLK  0x4000
#define            SRC_SET_KEEP_CUR_CLK_SRC  0x0000
#define            SRC_SET_CONFIG_CLK_ACCORDING_TO_CLK_SEL  0x8000
                               
#define AUDIO_ROUTING_CONFIG  0x1F
#define            D2_SET_TDM0_FOR_BYPASS  0x0004
#define            D2_SET_TDM1_FOR_BYPASS  0x0008
#define            D2_SET_TDM2_FOR_BYPASS  0x0010
#define            D2_SET_TDM3_FOR_BYPASS  0x0020
#define            TDM_BYPASS_EN  0x0040
#define            TDM_BYPASS_MODE_TDM0_TO_TDM1_TDM2_TO_TDM3  0x0000
#define            TDM_BYPASS_MODE_TDM0_TO_TDM3_TDM2_TO_TDM1  0x0080
#define            MUSIC_IN_TDM0  0x0000
#define            MUSIC_IN_TDM1  0x0100
#define            MUSIC_IN_TDM2  0x0200
#define            MUSIC_IN_TDM3  0x0300
#define            USE_TDM_MUSIC_TO_SYNC  0x0400
#define            TDM_SYNC_LEFT_CH  0x0000
#define            TDM_SYNC_RIGHT_CH  0x0800

#define            TDM_SYNC_DELAY_1_CLKS_CYCLES 0x1000
#define            TDM_SYNC_DELAY_2_CLKS_CYCLES 0x2000
#define            TDM_SYNC_DELAY_3_CLKS_CYCLES 0x3000
#define            TDM_SYNC_DELAY_4_CLKS_CYCLES 0x4000
#define            TDM_SYNC_DELAY_5_CLKS_CYCLES 0x5000
#define            TDM_SYNC_DELAY_6_CLKS_CYCLES 0x6000
#define            TDM_SYNC_DELAY_7_CLKS_CYCLES 0x7000
#define            TDM_SYNC_DELAY_8_CLKS_CYCLES 0x8000
#define            TDM_SYNC_DELAY_9_CLKS_CYCLES 0x9000
#define            TDM_SYNC_DELAY_10_CLKS_CYCLES 0xA000
#define            TDM_SYNC_DELAY_11_CLKS_CYCLES 0xB000
#define            TDM_SYNC_DELAY_12_CLKS_CYCLES 0xC000
#define            TDM_SYNC_DELAY_13_CLKS_CYCLES 0xD000
#define            TDM_SYNC_DELAY_14_CLKS_CYCLES 0xE000
#define            TDM_SYNC_DELAY_15_CLKS_CYCLES 0xF000
                               


#define READ_AUDIO_BUFFER  0x20
#define GENERAL_CONFIG_1  0x22
#define            MAX_NUMBER_OF_MIC_IS_0  0x0000
#define            MAX_NUMBER_OF_MIC_IS_2  0x0001
#define            MAX_NUMBER_OF_MIC_IS_4  0x0002
#define            DSP_CLK_GEN_PLL  0x0000
#define            DSP_CLK_GEN_RC_OSC  0x0020
#define            IF_REQUIRED_WAKEUP  0x0040
#define            AUD_BUF_READ_MODE_MODE_2  0x0000
#define            AUD_BUF_READ_MODE_MODE_1_AND_2  0x0080
#define            WAKEUP_POLARITY_ACTIVE_LOW  0x0000
#define            WAKEUP_POLARITY_ACTIVE_HIGH  0x0100
#define            MEMORY_SLEEP_EN  0x0200
#define            INVERT_DETECTION_GPIOS_ACTIVE_LOW   0x0400
#define            INVERT_DETECTION_GPIOS_ACTIVE_HIGH   0x0000
#define            SEND_UART_CHG_SEQ  0x0800
#define            SPLIT_MIC_BUFFER  0x1000

#define GENERAL_CONFIG_2  0x23
#define            FW_VAD_TYPE_NO_VAD  0x0000
#define            FW_VAD_TYPE_HW_VAD0  0x0020
#define            FW_VAD_TYPE_FW_OR_VESPER  0x0040
#define            FW_VAD_TYPE_D6_HW_VAD1  0x0060
#define            DDF_SAMPLE_WIDTH_16_BIT  0x0000
#define            DDF_SAMPLE_WIDTH_24_BIT  0x0080
#define            MIC_SAMPLE_RATE_16K  0x0000
#define            MIC_SAMPLE_RATE_44_1K  0x0100
#define            MIC_SAMPLE_RATE_48K  0x0200
#define            MIC_SAMPLE_RATE_8K  0x0300
#define            AUTO_MIPS_HWVAD_ENABLED  0x0400
#define            RESET_DDF_DC  0x0800
#define            WRITE_AUD_BUF_HDR  0x1000
#define            D6_LDO_SETTING_INPUT_MIN_0_96V  0x2000
#define            D6_LDO_SETTING_INPUT_MIN_1_03V  0x4000
#define            D6_LDO_SETTING_INPUT_MIN_1_1V  0x6000

#define FIRST_MICROPHONE_CONFIG  0x24
#define SECOND_MICROPHONE_CONFIG  0x25
#define THIRD_MICROPHONE_CONFIG  0x26
#define FOURTH_MICROPHONE_CONFIG  0x27
#define            D2_MIC_SRC_LEFT_DMIC_DM0  0x0001
#define            D2_MIC_SRC_RIGHT_DMIC_DM0  0x0002
#define            D2_MIC_SRC_LEFT_DMIC_DM1  0x0003
#define            D2_MIC_SRC_RIGHT_DMIC_DM1  0x0004
#define            D2_MIC_SRC_ANALOG_MIC_DM2  0x0005
#define            D2_DM_DATA_SRC_DM0_TDM2_RX  0x0000
#define            D2_DM_DATA_SRC_DM1_TDM0_FSYNC  0x0000
#define            D2_DM_DATA_SRC_DM2_HIGH_IMPEDANCE  0x0000
#define            D2_DM_DATA_SRC_DM0_TDM3_FSYNC  0x0008
#define            D2_DM_DATA_SRC_DM1_TDM3_RX  0x0008
#define            D2_DM_DATA_SRC_DM2_MID_IMPEDANCE  0x0008
#define            D2_DM_CLK_SRC_DM0_TDM2_TX  0x0000
#define            D2_DM_CLK_SRC_DM1_TDM0_TX  0x0000
#define            D2_DM_CLK_SRC_DM0_TDM3_SCLK  0x0010
#define            D2_DM_CLK_SRC_DM1_TDM3_TX  0x0010

#define            DDF_AND_DM_CONFIG_DDF0_DM0  0x0001
#define            DDF_AND_DM_CONFIG_DDF0_DM1  0x0002
#define            DDF_AND_DM_CONFIG_DDF1_DM0  0x0003
#define            DDF_AND_DM_CONFIG_DDF1_DM1  0x0004
#define            DDF_AND_DM_CONFIG_SD_DDF_DM0  0x0005
#define            DDF_AND_DM_CONFIG_SD_DDF_DM1  0x0006
#define            DDF_AND_DM_CONFIG_SD_DDF_SD_ADC  0x0007
#define            DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC  0x0008
#define            CLOCK_POLARITY_RISING_EDGE  0x0000
#define            CLOCK_POLARITY_FALLING_EDGE  0x0010
                               
#define            DM_CLK_FREQ_512_SR_8KHz_16KHz  0x0000
#define            DM_CLK_FREQ_768_SR_8KHz_16KHz  0x0020
#define            DM_CLK_FREQ_1024_1040_SR_16KHz_32KHz_48KHz  0x0040
#define            DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz  0x0060
#define            DM_CLK_FREQ_2304_2352_SR_16KHz_32KHz_48KHz  0x0080
#define            DM_CLK_FREQ_3072_SR_16KHz_32KHz_48KHz  0x00a0
#define            SAR_128Khz  0x0000
#define            SAR_512_Khz  0x00f0
#define            SYNCED_START  0x0100
#define            DDF_AUDIO_ATTENUATION_6dB_GAIN  0x0000 
#define            DDF_AUDIO_ATTENUATION_0dB  0x0200 
#define            DDF_AUDIO_ATTENUATION_MINUS_6dB  0x0400
#define            DDF_AUDIO_ATTENUATION_MINUS_12dB  0x0600
                               
#define            DM_DATA_SRC_DM0_GPIO2_DM1_GPIO1  0x0000
#define            DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11  0x1000
#define            DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13  0x2000
#define            DM_DATA_SRC_DM0_GPIO20_DM1_GPIO18  0x3000
#define            DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO0  0x0000
#define            DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12  0x4000
#define            DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14  0x8000
#define            DM_CLOCK_SRC_DM0_GPIO19_DM1_GPIO17  0xC000


// de-configure MIC 
#define            CLOSING_MICS_NO_DM_CLOCK  0x0200

#define VESPER_MIC_CONFIG  0x28
#define                        MODE_GPIO_NUM_0   0x0000
#define                        MODE_GPIO_NUM_1   0x0001
#define                        MODE_GPIO_NUM_2   0x0002
#define                        MODE_GPIO_NUM_3   0x0003
#define                        MODE_GPIO_NUM_4   0x0004
#define                        MODE_GPIO_NUM_5   0x0005
#define                        MODE_GPIO_NUM_6   0x0006
#define                        MODE_GPIO_NUM_7   0x0007
#define                        MODE_GPIO_NUM_8   0x0008
#define                        MODE_GPIO_NUM_9   0x0009
#define                        MODE_GPIO_NUM_10   0x000a
#define                        MODE_GPIO_NUM_11   0x000b
#define                        MODE_GPIO_NUM_12   0x000c
#define                        MODE_GPIO_NUM_13   0x000d
#define                        MODE_GPIO_NUM_14   0x000e
#define                        MODE_GPIO_NUM_15   0x000f
#define                        MODE_GPIO_NUM_16   0x0010
#define                        MODE_GPIO_NUM_17   0x0011
#define                        MODE_GPIO_NUM_18   0x0012
#define                        MODE_GPIO_NUM_19   0x0013
#define                        MODE_GPIO_NUM_20   0x0014
#define                        MODE_GPIO_NUM_21   0x0015
#define                        MODE_GPIO_NUM_22   0x0016
#define                        MODE_GPIO_NUM_23   0x0017
#define                        MODE_GPIO_NUM_24   0x0018

#define                        ZERO_MIC_OUT_0_MS   0x0000
#define                        ZERO_MIC_OUT_1_5_MS   0x0020
#define                        ZERO_MIC_OUT_3_MS   0x0040
#define                        ZERO_MIC_OUT_4_5_MS   0x0060
#define                        ZERO_MIC_OUT_6_MS   0x0080
#define                        ZERO_MIC_OUT_7_5_MS   0x00a0
#define                        ZERO_MIC_OUT_9_MS   0x00c0
#define                        ZERO_MIC_OUT_10_5_MS   0x00e0

#define                        DET_GPIO_NUM_0   0x0000
#define                        DET_GPIO_NUM_1   0x0100
#define                        DET_GPIO_NUM_2   0x0200
#define                        DET_GPIO_NUM_3   0x0300
#define                        DET_GPIO_NUM_4   0x0400
#define                        DET_GPIO_NUM_5   0x0500
#define                        DET_GPIO_NUM_6   0x0600
#define                        DET_GPIO_NUM_7   0x0700
#define                        DET_GPIO_NUM_8   0x0800
#define                        DET_GPIO_NUM_9   0x0900
#define                        DET_GPIO_NUM_10   0x0a00
#define                        DET_GPIO_NUM_11   0x0b00
#define                        DET_GPIO_NUM_12   0x0c00
#define                        DET_GPIO_NUM_13   0x0d00
#define                        DET_GPIO_NUM_14   0x0e00
#define                        DET_GPIO_NUM_15   0x0f00
#define                        DET_GPIO_NUM_16   0x1000
#define                        DET_GPIO_NUM_17   0x1100
#define                        DET_GPIO_NUM_18   0x1200
#define                        DET_GPIO_NUM_19   0x1300
#define                        DET_GPIO_NUM_20   0x1400
#define                        DET_GPIO_NUM_21   0x1500
#define                        DET_GPIO_NUM_22   0x1600
#define                        DET_GPIO_NUM_23   0x1700
#define                        DET_GPIO_NUM_24   0x1800

#define                        SIL_TIME_THD_250MS   0x0000
#define                        SIL_TIME_THD_500MS   0x2000
#define                        SIL_TIME_THD_1000MS   0x4000
#define                        SIL_TIME_THD_2000MS   0x6000
#define                        SIL_TIME_THD_4000MS   0x8000

                               
#define HOST_INTERFACE_SUPPORT  0x29
#define            UART0_ENABLE_FOR_HOST_D2_TDM2_D4_D6_GPIO_17_18  0x0001
#define            I2C_D2_ON_I2C_SDA_SCK_D4_D6_GPIO_1_2  0x0002
#define            SLIMBUS_D2_TDM0_D4_D6_GPIO_13_15  0x0004
#define            UART_D2_0_UART_TDI_RTCK_D4_1_GPIO_14_15  0x0008
#define            UART_D2_0_UART_Rx_Tx_D4_1_GPIO_8_9  0x0010
#define            SPI_D2_TDM1_D4_GPIO_4_5_6_7  0x0020
#define            SPI_D2_SPI_PINS_D6_GPIO_4_5_6_7  0x0040
#define            UART_D2_0_ON_I2C_SDA_SCK  0x0080
#define            SPI_ACK  0x0800
#define            UART_DEBUG_MODE_0  0x1000
#define            UART_DEBUG_MODE_1  0x2000
#define            UART_EN_UART_1_Rx  0x4000

#define TONE_GENERATOR  0x2A
#define            TONE_GEN_EN  0x0001
#define            TONE_WORD_WIDTH_16_BITS  0x0000
#define            TONE_WORD_WIDTH_32_BITS  0x0002

#define TRIGGER_WORD_ID_FOR_BUFFERING  0x2B
#define            WORD_ID_SEL_REG_5C  0x0000
#define            WORD_ID_SEL_REG_5B  0x000

#define VT_COMMAND_TIMEOUT  0x2C
#define                        TIMEOUT_3_SEC  0xBB8
#define                        TIMEOUT_5_MIN  0x4e20


#define SPL_METER_CONFIG  0x2D
#define SPL_METER_STATUS  0x2E

#define UART_DEBUG_RECORDING  0x30
#define            DISABLE_RECORDING  0x0000
#define            ENABLE_RECORDING  0x0001
#define            ENLARGE_BUFFER_BY_1_5  0x0002
#define            ENLARGE_BUFFER_BY_2  0x0004
#define            ALLOCATE_BUFFER_IN_AHB_MEMORY  0x0008
#define            ADD_BUFFER_TO_HW_VAD  0x0010
#define            ENABLE_CALCULATING_MAX_VALUES  0x0100
#define            PRINT_MAX_VALUES  0x0200
#define            RECORD_MIC0_INT  0x0001
#define            RECORD_MIC1_INT  0x0002
#define            RECORD_MIC2_INT  0x0004
#define            RECORD_MIC3_INT  0x0008
#define            RECORD_MIC0_FRM  0x0010
#define            RECORD_MIC1_FRM  0x0020
#define            RECORD_MIC2_FRM  0x0040
#define            RECORD_MIC3_FRM  0x0080
#define            RECORD_TDM0_Rx_INT  0x0100
#define            RECORD_TDM1_Rx_INT  0x0200
#define            RECORD_TDM2_Rx_INT  0x0400
#define            RECORD_TDM3_Rx_INT  0x0800
#define            RECORD_TDM0_Tx_INT  0x1000
#define            RECORD_TDM1_Tx_INT  0x2000
#define            RECORD_TDM2_Tx_INT  0x4000
#define            RECORD_TDM3_Tx_INT  0x8000
#define            RECORD_TDM0_Rx_FRM  0x0001
#define            RECORD_TDM1_Rx_FRM  0x0002
#define            RECORD_TDM2_Rx_FRM  0x0004
#define            RECORD_TDM3_Rx_FRM  0x0008
#define            RECORD_TDM0_Tx_FRM  0x0010
#define            RECORD_TDM1_Tx_FRM  0x0020
#define            RECORD_TDM2_Tx_FRM  0x0040
#define            RECORD_TDM3_Tx_FRM  0x0080
#define            RECORD_BUF1_IN_FRM  0x0100
#define            RECORD_BUF2_IN_FRM  0x0200
#define            RECORD_BUF3_IN_FRM  0x0400
#define            RECORD_BUF4_IN_FRM  0x0800
#define            RECORD_BUF1_OUT_FRM  0x1000
#define            RECORD_BUF2_OUT_FRM  0x2000
#define            RECORD_BUF3_OUT_FRM  0x4000
#define            RECORD_BUF4_OUT_FRM  0x8000
#define            RECORD_VT_IN  0x0001
#define            RECORD_BUF_8_OUT  0x0002
#define            RECORD_BUF_9_OUT  0x0004
#define            RECORD_DECIM_IN_0  0x0008
#define            RECORD_DECIM_IN_1  0x0010

#define TDM_ACTIVATION_CONTROL  0x31
#define            CONFIG_TDM_0  0x0000
#define            CONFIG_TDM_1  0x0001
#define            CONFIG_TDM_2  0x0002
#define            CONFIG_TDM_3  0x0003
#define            PROC_EN_DISABLE  0x0000
#define            PROC_EN_SWITCH_FW_TO_DETECTION_MODE  0x0004
#define            PROC_EN_SWITCH_FW_TO_BUFFERING_MODE  0x0008
#define            TDM0_RX_EN  0x0100
#define            TDM0_TX_EN  0x0200
#define            TDM1_RX_EN  0x0400
#define            TDM1_TX_EN  0x0800
#define            TDM2_RX_EN  0x1000
#define            TDM2_TX_EN  0x2000
#define            TDM3_RX_EN  0x4000
#define            TDM3_TX_EN  0x8000

#define MEMORY_CONFIG  0x33
#define            DTCM_SIZE_64_KW  0x0000
#define            DTCM_SIZE_D4_D6_96_KW_D8_128KW  0x0001
#define            DTCM_SIZE_D4_D6_96_KW_D8_192KW  0x0002
#define            AUDIO_BUF_LOC_DTCM_USING_MEM_ALLOCATION  0x0000
#define            AUDIO_BUF_LOC_AHB_USING_MEM_ALLOCATION  0x0004
#define            AUDIO_BUF_LOC_AHB_3_SEC_BUFFER  0x0008
#define            AUDIO_BUF_LOC_AHB_7_SEC_BUFFER_D8  0x000C
#define            AHB_ON_SIZE_NO_AHB  0x0000
#define            AHB_ON_SIZE_32KW  0x0010
#define            AHB_ON_SIZE_64KW_32KW_CACHE  0x0020
#define            AHB_ON_SIZE_80KW_16KW_CACHE  0x0030
#define            AHB_ON_SIZE_112KW_32KW_CACHE  0x0040
#define            AHB_ON_SIZE_128KW_16KW_CACHE  0x0050
#define            AHB_ON_SIZE_160KW_32KW_CACHE_D8  0x0060
#define            AHB_ON_SIZE_192KW_32KW_CACHE_D8  0x0070
#define            AMODEL_0_LOC_DTCM  0x0000
#define            AMODEL_1_LOC_DTCM  0x0000
#define            AMODEL_2_LOC_DTCM  0x0000
#define            AMODEL_3_LOC_DTCM  0x0000
#define            AMODEL_4_LOC_DTCM  0x0000
#define            AMODEL_0_LOC_AHB  0x0100
#define            AMODEL_1_LOC_AHB  0x0200
#define            AMODEL_2_LOC_AHB  0x0400
#define            AMODEL_3_LOC_AHB  0x0800
#define            AMODEL_4_LOC_AHB  0x1000
#define                        MIC_AUDIO_BUF_LOC_DTCM  0x0000
#define                        MIC_AUDIO_BUF_LOC_AHB  0x2000

#define AUDIO_PROCESSING_CONFIG  0x34
#define            ALGO1_EN_FW_MODE_1_ONLY  0x0001
#define            ALGO1_EN_FW_MODE_2_ONLY  0x0002
#define            ALGO1_EN_FW_MODE_1_AND_MODE_2  0x0003
#define            ALGO2_EN_FW_MODE_1_ONLY  0x0004
#define            ALGO2_EN_FW_MODE_2_ONLY  0x0008
#define            ALGO2_EN_FW_MODE_1_AND_MODE_2  0x000C
#define            ALGO3_EN_FW_MODE_1_ONLY  0x0010
#define            ALGO3_EN_FW_MODE_2_ONLY  0x0020
#define            ALGO3_EN_FW_MODE_1_AND_MODE_2  0x0030
#define            VT_PROC_EN  0x0040
#define            EN_441_EC_REF_DECIM  0x0080
#define            VT_CP0  0x0000
#define            VT_CP1  0x0100
#define            VT_CP2  0x0200
#define            VT_CP3  0x0300
#define            VT_CP4  0x0400
#define            VT_CP5  0x0500
#define            VT_CP6  0x0600
#define            VT_CP7  0x0700
#define            AUTO_SET_HIGH_MIPS  0x1000
#define            POST_DETECTION_MODE_SWITCH_TO_STREAMING  0x0000
#define            POST_DETECTION_MODE_REMAIN_IN_DETECTION  0x2000
#define            EN_48_EC_REF_DECIM  0x4000
#define            EN_COPY_HIST  0x8000

#define TDM_RX_CONFIG  0x36
#define TDM_TX_CONFIG  0x37
#define            CLEAR_TDM_CFG  0x0
#define            RX_TX_CP0  0x0000
#define            RX_TX_CP1  0x0001
#define            RX_TX_CP2  0x0002
#define            RX_TX_CP3  0x0003
#define            RX_TX_CP4  0x0004
#define            RX_TX_CP5  0x0005
#define            RX_TX_CP6  0x0006
#define            RX_TX_CP7  0x0007
#define            RX_TX_CP8  0x0008
#define            RX_TX_CP9  0x0009
#define            RX_TX_CP_VT_AUDIO_BUFFER  0x000f
#define            HW_BLOCK_EN  0x0010
#define            RESAMPLE_RATIO_NO_RESAMPLING  0x0000
#define            RESAMPLE_RATIO_2_AND_1_OR_44_1  0x0020
#define            RESAMPLE_RATIO_3_1  0x0040
#define            RESAMPLE_TYPE_DECIMATION  0x0000
#define            RESAMPLE_TYPE_INTERPOLATION  0x0080
#define            RX_TX_I2S_CH_USE_LOW_WORD_ONLY  0x0000
#define            RX_TX_I2S_CH_USE_HIGH_WORD_ONLY  0x0100
#define            RX_TX_I2S_CH_USE_I2S_STEREO  0x0200
#define            RX_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE  0x0300
#define            INPUT_OUTPUT_SAMPLE_RATE_16_KHz  0x0000
#define            INPUT_OUTPUT_SAMPLE_RATE_44_1_KHz  0x0400
#define            INPUT_OUTPUT_SAMPLE_RATE_48_KHz  0x0800
#define            INPUT_OUTPUT_SAMPLE_RATE_8_KHz  0x0C00
#define            NUM_OF_CHANNELS_1_CH  0x0000
#define            NUM_OF_CHANNELS_2_CH  0x1000
#define            NUM_OF_CHANNELS_4_CH  0x2000
#define            SAMPLE_WIDTH_16_BIT  0x0000
#define            SAMPLE_WIDTH_32_BIT  0x4000
#define            DEMUX_MUX_DISABLE  0x0000
#define            DEMUX_MUX_ENABLE  0x8000

#define BUFFERING_NORMAL_AMPLITUDE  0x38
#define            NORMALIZE_TO_MINUS_1dB  0xfff0
#define            NORMALIZE_TO_MINUS_3dB  0xffd0
#define            NORMALIZE_TO_MINUS_6dB  0xffa0
#define            NORMALIZE_TO_MINUS_9dB  0xff7f
#define            NORMALIZE_TO_MINUS_18dB  0xfee0
#define            NO_NORMALIZATION  0x0

#define INDIRECT_ACCESS_REGISTER_NUMBER  0x3D
#define INDIRECT_ACCESS_REGISTER_WRITE  0x3E
#define INDIRECT_ACCESS_REGISTER_READ  0x3F


//////////////////////////////////////////////////////////////
/* VT Engine  commands registers */
#define VT_ENGINE_RECOGMODE 0x40
#define VT_ENGINE_INITIALIZED 0x41
#define VT_ENGINE_LPSD_VAD 0x42
#define VT_ENGINE_DUALTHRESHOLD 0x45
#define VT_ENGINE_WORDID 0x5B
#define VT_ENGINE_PHRASE_LENGTH 0x67

//#define SENSORY_INITIALIZED 			0x41
//#define SENSORY_DUALTHRESHOLD 			0x45
//#define SENSORY_WORDID					0x5B

// CPLD address & registers
#define           CPLD_I2C_ADDR  0x33
#define           CPLD_REG0  0x00
#define           CPLD_REG1  0x01
#define           CPLD_REG2  0x02
#define           CPLD_REG3  0x03
#define           CPLD_REG4  0x04
#define           CPLD_REG5  0x05
#define           CPLD_REG6  0x06



/* ASRP Registers for QED */

#define ASRP_USER_DEFINED_DELAY  0x107
#define ASRP_BLOCKID_LOW_PART 0x124
#define ASRP_BLOCKID_HIGH_PART 0x125
#define ASRP_ENABLE_OFFSET 0x126
#define ASRP_VALUE  0x127
#define ASRP_RECORDS_CHANNELS 0x128

#define ASRP_QED_Trigger_mode			0x140
#define ASRP_QED_Trigger_offset_mode 	0x141
#define ASRP_QED_Trigger_length 		0x142
#define ASRP_QED_OUT_detection 			0x143




/* DBMDX Operation Modes command  */
#define           IDLE_MODE		0x00
#define           DETECTION_MODE 	0x01
#define           BUFFERING_MODE 	0x02
#define           HIBERNATE_MODE	0x06

#define           GAIN_OF_12_DB 					0x18
                              
/* command for AUDIO_BUFFER_SIZE (0x9) */
#define           BUFFER_SIZE_3_SEC 				0x1770
#define           BUFFER_SIZE_5_SEC 				0x2724
                              
/* command for LOAD_NEW_ACOUSTIC_MODEL (0xF) */
#define           LOAD_TRIGGER_ACOUSTIC_MODEL 					0x00
#define           LOAD_COMMAND_ACOUSTIC_MODEL 					0x01
#define           LOAD_COMMAND_ASRP_PARAM 					0x04
#define           AUDIO_BUFFER_READOUT_START_FROM_POINT_OF_SWITCHING_TO_BUFFERING 0x00
#define           AUDIO_BUFFER_READOUT_START_FROM_PHRASE_END_POINT 		0x01
#define           AUDIO_BUFFER_READOUT_START_FROM_PHRASE_START_POINT 		0x02
                              
/* command for HOST_INTERFACE_SUPPORT (0x29) */
#define           HOST_INTERFACE_I2C  0x02
#define           HOST_INTERFACE_UART 0x01
#define           HOST_INTERFACE_SPI  0x20

#define DBMD2             2
#define DBMD4 	        4

/* tdm addr */
#define            D4_TDM_0_RX_ADDR   0x80800000
#define            D4_TDM_1_RX_ADDR   0x80801000
                               
#define            D4_TDM_0_TX_ADDR   0x80804000
#define            D4_TDM_1_TX_ADDR   0x80805000
                               
#define            D2_TDM_0_RX_ADDR   0x80800000
#define            D2_TDM_1_RX_ADDR   0x80801000
#define            D2_TDM_2_RX_ADDR   0x80802000
#define            D2_TDM_3_RX_ADDR   0x80803000
                               
#define            D2_TDM_0_TX_ADDR   0x80804000
#define            D2_TDM_1_TX_ADDR   0x80805000
#define            D2_TDM_2_TX_ADDR   0x80806000
#define            D2_TDM_3_TX_ADDR   0x80807000
                               
#define            TDM3_TX_EN_BIT 0x8000
#define            TDM3_RX_EN_BIT 0x4000
                               
#define            TDM2_TX_EN_BIT 0x2000
#define            TDM2_RX_EN_BIT 0x1000
                               
                               
#define            TDM1_TX_EN_BIT 0x0800
#define            TDM1_RX_EN_BIT 0x0400
                               
#define            TDM0_TX_EN_BIT 0x0200
#define            TDM0_RX_EN_BIT 0x0100
                               
                               
#define            MODE_0_EN_BIT   0x0000
#define            MODE_1_EN_BIT   0x0004
#define            MODE_2_EN_BIT   0x0008

