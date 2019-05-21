#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"


#include "device.h"
#include "spi_api.h"
#include "gpio_api.h"
#include "gpio_irq_api.h"
#include "timer_api.h"
#include "pwmout_api.h"
#include "sys_api.h"
#include "spi_ex_api.h"

#include "semphr.h"
#include "osdep_service.h"
#include "timers.h"
#include "flash_api.h"
#include "device_lock.h"

#include "Dbmd_rtl_Device.h"
#include "dbmdx_melon_def.h"
#include "Dbmd_init.h"
#include "UserCase_Commom.h"


#define TEST_STACK			2048
//PIN ASSIGN
#define D4P_GPIO_WAKEUP 	PA_15
#define D4P_GPIO_RSTN		PA_14
#define D4P_GPIO_BUTTON		PA_23
#define D4P_GPIO_DBMD_INT	PA_5
//#define D4P_PWM_BUZ			PA_12

#define SPI1_MOSI  PA_4
#define SPI1_MISO  PA_3
#define SPI1_SCLK  PA_1
#define SPI1_CS    PA_2

//another def
#define		DETECT_KEY_TIME		10
#define		BUZ_PWM_PERIOD		500
#define		SPI_DATA_BIT		8
#define		SPI_FREQUENCY		3000000
#define		HW_ANALOG_MIC
#define 	NO_USE_ANALOG_MIC	0
#define		USE_ANALOG_MIC		1
#define		NO_USE_TDM_CODEC	0
#define		USE_TDM_CODEC		1		

/*****var obj********/
//wait wlan init ok sema
_sema init_finish_sema;
//interrupt sema
_sema action_sema_control;
//rtos soft timer
TimerHandle_t	detectHandle;		//button detect soft timer


//dev init
gpio_t gpio_d4p_wakeup;

gpio_t gpio_d4p_rstn;

gpio_t gpio_d4p_button;

gpio_irq_t gpio_d4p_irq;

pwmout_t pwm_buz;

spi_t spi_master;

/*******var************/
//PinName  pwm_led_pin =  D4P_PWM_BUZ;


extern void wait_ms(u32);
void d4p_irq_handle(uint32_t id,gpio_irq_event event);

/********************
when load d4p firmware ,it,s gpio is high
so we need init irq when we load ok
*******************/
void rtl_int_init()
{
	gpio_irq_init(&gpio_d4p_irq,D4P_GPIO_DBMD_INT,d4p_irq_handle,(uint32_t)(&gpio_d4p_irq));
	gpio_irq_set(&gpio_d4p_irq,IRQ_RISE,1);
	gpio_irq_enable(&gpio_d4p_irq);
}

void rtl_dev_init()
{
	sys_jtag_off();	//close jtag func
	
	// gpio init
	gpio_init(&gpio_d4p_wakeup,D4P_GPIO_WAKEUP);
	gpio_dir(&gpio_d4p_wakeup,PIN_OUTPUT);
	gpio_mode(&gpio_d4p_wakeup,PullNone);
	gpio_write(&gpio_d4p_wakeup,1);		//wakeup pin

	gpio_init(&gpio_d4p_rstn,D4P_GPIO_RSTN);
	gpio_dir(&gpio_d4p_rstn,PIN_OUTPUT);
	gpio_mode(&gpio_d4p_rstn,PullNone);

#if 0
	gpio_init(&gpio_d4p_button,D4P_GPIO_BUTTON);
	gpio_dir(&gpio_d4p_button,PIN_INPUT);
	gpio_mode(&gpio_d4p_button,PullNone);
#endif
#if 0
	gpio_irq_init(&gpio_d4p_irq,D4P_GPIO_DBMD_INT,d4p_irq_handle,(uint32_t)(&gpio_d4p_irq));
	gpio_irq_set(&gpio_d4p_irq,IRQ_RISE,1);
	gpio_irq_enable(&gpio_d4p_irq);
#endif	
	//pwm init
#if 0
	pwmout_init(&pwm_buz,pwm_led_pin);
	pwmout_period_us(&pwm_buz,BUZ_PWM_PERIOD);
	pwmout_stop(&pwm_buz);
#endif
	//spi init
	/******************************************
	DATASHEET PORT PA_18 PA_19 PA_22 PA_23
	when init spi1  it only can master
	when init spi0  it only can slave
	*******************************************/
	//printf("start init^^^\n");
	spi_master.spi_idx = MBED_SPI1;
	spi_init(&spi_master,SPI1_MOSI,SPI1_MISO,SPI1_SCLK,SPI1_CS);
	spi_format(&spi_master,SPI_DATA_BIT,SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_MIDDLE,0);
	spi_frequency(&spi_master,SPI_FREQUENCY);

	//spi handshark will start when system init ,debuging find wlan init will affact spi hand
	//so we delay to wait wlan init 
	//wait_ms(1000);
	
}

void d4p_irq_handle(uint32_t id,gpio_irq_event event)
{
	DBMD_PRINTF("*****************\n");
	DBMD_PRINTF("get rising interrupt\n");
	DBMD_PRINTF("^^^^^^^^^^^^^^^^^\n");
	rtw_up_sema(&action_sema_control);
}

unsigned int key_down_handle_time()
{
	unsigned int tmp;
	static unsigned int press_time = 0;

	if(gpio_read(&gpio_d4p_button)){		//key up 
		if(press_time > 0){
			tmp = press_time;
			press_time = 0;

			return tmp;
		}
	}
	else{							//key down 
		press_time++;

	}
    return 0;
}

void button_detect_handler()		//Keyboard scan£¬ble/wifi ota, identily timeout
{
	unsigned int press_hold_time;
	press_hold_time = key_down_handle_time();	//get key down time
	//DBMD_PRINTF("KEY DOWM (%d*10)ms\n",press_hold_time);	
}

void set_d4p_reset()
{
	gpio_write(&gpio_d4p_rstn,0);
	//vTaskDelay(100/portTICK_RATE_MS);
	wait_ms(100);
	gpio_write(&gpio_d4p_rstn,1);
	//vTaskDelay(100/portTICK_RATE_MS);
	wait_ms(100);
}

void dbmd_task(void *param)
{
	int value,cnt=0;
	while(rtw_down_sema(&init_finish_sema) == pdTRUE){
		DBMD_PRINTF("Read flash finish, OneSec_TimerTask\n");
		break;
	}
	rtl_dev_init();
	/******set d4p reset*********/
	set_d4p_reset();
	/**init d4p */
	if(Init_D4() < 0)
		goto err;
	
	/* START DBMD4 */
	Run_D4();
	vTaskDelay(10/portTICK_RATE_MS);

	//query fw version
	value = read_register_host_interface(&spi_master,FW_VERSION_NUMBER);
	if(value != 0) {
		printf("[%s]: D4 Firmware version 0x%X\n", __func__, value & 0xffff);
	} else {
		printf("[%s]: Failed read D4 Firmware version \n", __func__);
//		goto err;
	}

	//config set
	Config_D4();

	wait_ms(100);

	//load a model
	Load_A_Model();

	//when load fail(for example load uncomplete can not read chip ver and fw ver but can continue load model)
	/* verify DBMD4 is alive and read version */
	value = read_register_host_interface(&spi_master,0x19);
	if(value != 0) {
		printf ("*********************************\n");
		printf ("*********************************\n");
		printf ("******     D4 IS RUNNING     ****\n");
		printf ("*********************************\n");
		printf ("*********************************\n");
		printf ("Chip version = 0x%04X\n", value & 0xffff);

	} else {
		printf ("FAIL read_register(0x19)\n");
	}

#if 1
	wait_ms(10);
	value = read_register_host_interface(&spi_master,FW_VERSION_NUMBER);
	if(value != 0) {
		printf ("FirmWare version = 0x%X\n", value & 0xffff);
	} else {
		printf ("FAIL read_register(FW_VERSION_NUMBER)\n");
	}
#endif

	//in func_test_init we have init gpio_irq

	/*load asrp*/
#ifdef HW_ANALOG_MIC
	use_case_AEC_1mic_mono(NO_USE_TDM_CODEC, USE_ANALOG_MIC);
//#else
	//use_case_AEC_1mic_mono(0, 0);
#endif

	/*init timer to detect button*/
	rtw_init_sema(&action_sema_control, 0);

	//IQR init
	rtl_int_init();

	//create a soft time for button detect
	detectHandle = xTimerCreate ((const char*)"detect_handle", (DETECT_KEY_TIME/portTICK_RATE_MS),
            1, (void *) NULL, button_detect_handler);
	xTimerStart(detectHandle, 0);

#if 0
int j= 0;
	for(j=0;j<10;j++)
{
	if(j%2)
	{
		write_register_host_interface(&spi_master,IO_PORT_ADDRESS_LOW,SET_PRI_GPIO | MODE_GPIO_NUM_14);
		write_register_host_interface(&spi_master,IO_PORT_ADDRESS_HIGH,SET_SEC_GPIO | DET_GPIO_NUM_14);
		write_register_host_interface(&spi_master,IO_PORT_VALUE_LOW,0x0000);
		//write_register_host_interface(&spi_master,IO_PORT_VALUE_HIGH,0x0000);
#if 0
			write_register_host_interface(&spi_master,PHRASE_DETECTION_GPIO_CONFIG,	SET_SEC_GPIO			|
																			MODE_GPIO_NUM_14		|
																			SET_PRI_GPIO			|
																			DET_GPIO_NUM_14);
#endif
	}
	else
	{
		write_register_host_interface(&spi_master,IO_PORT_ADDRESS_LOW,SET_PRI_GPIO | MODE_GPIO_NUM_14);
		write_register_host_interface(&spi_master,IO_PORT_ADDRESS_HIGH,SET_SEC_GPIO | MODE_GPIO_NUM_14);
		write_register_host_interface(&spi_master,IO_PORT_VALUE_LOW,0x0001);
		//write_register_host_interface(&spi_master,IO_PORT_VALUE_HIGH,0x0000);
	}
	wait_ms(2000);
}
#endif

#if 1
	while(rtw_down_sema(&action_sema_control) == pdTRUE)
//	while(1)		//fu test to read
	{

		value = read_register_host_interface(&spi_master,VT_ENGINE_WORDID);

		if(value != 0) {
			//printf("[%s]:  !!!!!!!!! D4 VT_ENGINE_WORDID 0x%X !!!!!!!! \n", __func__, value & 0xffff);
			printf("[%s]: D4 VT_ENGINE_WORDID 2 0x%X\n", __func__, value & 0xffff);
		} else {
			printf("[%s]: Failed read D4 VT_ENGINE_WORDID 1 0x%X\n", __func__, value & 0xffff);
		}


		if(value < 99 && value > 0) 
		{
			uint16_t val;
#if 1 //xxxxx #ifdef DBG_ADD_LOCAL_COMMAND
			uint16_t op_mode;
#endif			
			cnt++;
			DBMD_PRINTF("[%s]: Got a trigger!- Number of detections %d\n", __func__, cnt);
			val = read_register_host_interface(&spi_master,OPERATION_MODE);
			DBMD_PRINTF("[%s]: Operation mode : 0x%x\n",__func__, val);

		
			val = read_register_host_interface(&spi_master,AUDIO_STREAMING_SOURCE_SELECT);
			DBMD_PRINTF("[%s]: AUDIO_STREAMING_SOURCE_SELECT : 0x%x\n",__func__, val);


			op_mode = read_register_host_interface(&spi_master,OPERATION_MODE);
			if((val != 0xffff) && (op_mode == BUFFERING_MODE)){
				/* current_timestamp(); */
				//TickDelay(DEL_1000);
				DBMD_PRINTF("get into pull buff\n");
				DBMD_PRINTF("AUDIO_STREAMING_SOURCE_SELECT val=%d\n",val);
				op_mode = read_register_host_interface(&spi_master,OPERATION_MODE);
				printf("into buffer op_mode=%d\n",op_mode);
				//why we need to delay 1s
				//wait_ms(1000);
			#if 1
				pull_buffer();		//recv spi data
			#else
				int k;
			for(k=0; k<300 ; k++)
			{
				printf("k=%d\n",k);
				wait_ms(100);
			}
			#endif
			}

			write_register_host_interface(&spi_master,OPERATION_MODE, DETECTION_MODE);


		}
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
#else
//fu test for buffer mode to read by spi
while(1)
{
#if 0
	int j=0;
	j++;
	if(j>3)
	{
		printf("start set into buffer mode\n");
		write_register_host_interface(&spi_master,OPERATION_MODE, BUFFERING_MODE);
		if(j > 6)
		{
			j = 0;
			//write_register_host_interface(&spi_master,OPERATION_MODE, DETECTION_MODE);
		}
	}
#endif	
	test_read();

	vTaskDelay(20000);
}
#endif
	rtw_free_sema(&action_sema_control);
	
err:
	spi_free(&spi_master);
	vTaskDelete(NULL);
}

#if 0
int test_read()
{
	int remained_samples_in_d4,n_read,i,retries=0,value;
	char get_buf[128];
	do {
		remained_samples_in_d4 = read_register_host_interface(&spi_master,NUMBER_OF_SAMPLES_IN_BUFFER);
		printf("get sample=%d\n",remained_samples_in_d4);
		/* current_timestamp(); */ /* debug */
		if(remained_samples_in_d4 == 0xffff) {
				printf("audio buffer is not in streaming.\n");
				return 0; /* no more data. */
		}

		if(remained_samples_in_d4 == 0x0000) {
			/* printf("."); */ /* debug */
#if 1
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
#else	
		printf("even read 0,still read\n");
		goto exit;
#endif
		} else {
			printf("remained_samples_in_d4 =%d\n",remained_samples_in_d4);
			break;
		}
	} while(1);
	//printf()
exit:
	memset(get_buf,0,128);
	write_register_host_interface(&spi_master,READ_AUDIO_BUFFER, 128/16);
	n_read = device_read_spi(&spi_master,get_buf,128);
	for(i=0;i<128;i++)
		printf("get buf[%d]=%d\n",i,get_buf[i]);
	return 0;

}
#endif

void DBMD_INIT_ENTYR()
{
	rtw_init_sema(&init_finish_sema,0);
	if(xTaskCreate(dbmd_task, ((const char*)"dbmd_task"), TEST_STACK, NULL, tskIDLE_PRIORITY + 4, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(dbmd_func) failed", __FUNCTION__);
}


