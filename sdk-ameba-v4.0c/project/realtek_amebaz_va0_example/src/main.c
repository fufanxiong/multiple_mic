#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"
#include <example_entry.h>

#include "Dbmd_init.h"

extern void console_init(void);

extern void tcp_socket_select(void);			// for tcp 
extern void baidu_asr(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	/* Initialize log uart and at command service */
	//console_init();	
	ReRegisterPlatformLogUart();

	/* pre-processor of application example */
	pre_example_entry();
	
	/*******d4p init***********/
	DBMD_INIT_ENTYR();

	/* wlan intialization */
#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
	wlan_network();
#endif

	/* Execute application example */
	example_entry();

	baidu_asr();
	/*socket to write what we recv*/
	//tcp_socket_select();

    	/*Enable Schedule, Start Kernel*/
#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
	#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
	#endif
#else
	RtlConsolTaskRom(NULL);
#endif
}
