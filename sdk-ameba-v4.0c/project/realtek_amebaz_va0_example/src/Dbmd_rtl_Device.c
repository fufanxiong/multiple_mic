#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"


#include "sys_api.h"
#include "osdep_service.h"
#include "flash_api.h"
#include "device_lock.h"



#include "dbmdx_melon_def.h"
#include "Dbmd_rtl_Device.h"
#include "Dbmd_init.h"
#include "UserCase_Commom.h"

volatile int TrDone;
volatile int masterRxDone;


void master_tr_done_callback(void *pdata, SpiIrq event)
{
	switch(event){
        case SpiRxIrq:
            DBMD_PRINTF("master rx ok***\n");
            masterRxDone = 1;
            break;
        case SpiTxIrq:
            DBMD_PRINTF("master tx ok***\n");
            TrDone = 1;
            break;
        default:
            DBMD_PRINTF("unknown interrput evnent!\n");
    }
}


int device_read_spi(spi_t *spi_obj,char *buf,unsigned int len)
{
	int i = 0;
	//printf("%s @@\n",__func__);
	spi_irq_hook(spi_obj, (spi_irq_handler)master_tr_done_callback, (uint32_t)(spi_obj));
	masterRxDone = 0;
	spi_flush_rx_fifo(spi_obj);
	//printf("%s ##\n",__func__);
	spi_master_read_stream(spi_obj,buf,len);
	//printf("%s &&\n",__func__);
	while(masterRxDone ==0)
	{
#if 1
		i++;
		if(i>1000)
		{
			//break;
			printf("spi timeout read\n");
			return -1;		//read err
		}
//		else
//		{
//			if((i%10) == 0)
//				printf("wait %d\n",i);
//		}
		wait_ms(5);
		//wait_ms(10);
#endif
	}
	return len;
}

#if 0
int device_special_read_register_spi(spi_t *spi_obj,char *txbuf,char *rxbuf,unsigned int len)
{
	int i;
	spi_irq_hook(spi_obj, (spi_irq_handler)master_tr_done_callback, (uint32_t)(spi_obj));
	masterRxDone = 0;
	spi_flush_rx_fifo(spi_obj);
	spi_master_write_read_stream(spi_obj,txbuf,rxbuf,len);
	while(masterRxDone ==0)
	{
		i++;
		if(i>1000)
		{
			//break;
			return -1;		//read err
		}
		wait_ms(5);
	}
	
	return len;
}
#endif

int device_write_spi(spi_t *spi_obj,char * buf,unsigned int len)
{
	int i;
	//ASSERT(buf);
	spi_irq_hook(spi_obj, (spi_irq_handler)master_tr_done_callback, (uint32_t)(spi_obj));
	TrDone = 0;
	spi_master_write_stream(spi_obj,buf,len);
	while(TrDone == 0)
	{
		//wait_ms(100);
		wait_ms(5);
	}
	//printf("read flash to send buf[0]=%d,buf[end]=%d\n",buf[0],buf[len-1]);
	//DBMD_PRINTF("read flash to send buf[0]=%d,buf[end]=%d\n",buf[0],buf[len-1]);
	return len;
}



/***********************************************************************
*  FUNCTION NAME: read_register_spi()
*
* DESCRIPTION:
*	Send via SPI ascii buffer of 3 bytes, format is:
* 	2 bytes - register address in ascii format.
* 	1 byte contains the character 'r' for read command
*
* 	Receive via SPI ascii buffer
* 	Read via SPI 5 bytes
* 	byte[0] - not relevant
* 	bytes[1-4] - ascii string
*
* PARAMETERS:
* 	int fd_spi int16_t reg
*
* RETURNS:
*
***********************************************************************/
uint16_t read_register_spi(spi_t *spi_obj,int16_t reg)
{
	uint16_t val = 0;
	int ret;
	unsigned char Buff[10] = {0};
	char RegSet[18] = {0};

	sprintf(RegSet, "%03xr", reg);
#if 1
	ret = device_write_spi(spi_obj,RegSet,strlen(RegSet));
	//DBMD_PRINTF("reg send ret = %d\n",ret);
	//wait_ms(10);
	//vTaskDelay(10 / portTICK_RATE_MS); 
	ret = device_read_spi(spi_obj,Buff,5);
	//DBMD_PRINTF("reg read ret = %d\n",ret);
#else
	//printf("read register %s\n",RegSet);
	device_special_read_register_spi(spi_obj,RegSet,Buff,5);
#endif
	Buff[5] = 0;
	val = atoh__((char*)&Buff[1]);
	if(val < 0) {
		printf("[%s]: device_read error.\n", __func__);
	}

	return val;
}



/***********************************************************************
*  FUNCTION NAME: write_register_spi()
*
* DESCRIPTION:
*	Send via SPI ascii buffer of 7 bytes, format is:
* 	2 bytes - register address in ascii format
* 	1 byte contains the character 'w' for write command
* 	4 bytes contains the value to be written to register in ascii format.
*
* PARAMETERS:
* 	int fd_spi, uint8_t reg, int16_t val
*
* RETURNS:
*
***********************************************************************/
void write_register_spi(spi_t *spi_obj,int16_t reg, int16_t val)
{
	int ret;
	char str[16];
	//printf("%s &&^^\n",__func__);
	sprintf(str, "%03xw%04x", reg, (val)&0xffff);

	DBMD_PRINTF("D? [%s]: %s\n",__func__, str);
	ret = device_write_spi(spi_obj,str,strlen(str));
	DBMD_PRINTF("reg send ret = %d\n",ret);
	
}

/***********************************************************************
* FUNCTION NAME: ASRP_read_register()
*
* DESCRIPTION:
*	read dbmdx ASRP register value using SPI.
*	using indirect commands since the address of registers are above 0x100
* PARAMETERS:
*	int fd, int16_t reg
* RETURNS:
*	register value
***********************************************************************/
int16_t ASRP_read_register(spi_t *spi_obj,int16_t reg)
{
	int16_t val;
	write_register_host_interface(spi_obj,INDIRECT_ACCESS_REGISTER_NUMBER, reg);
	val = read_register_host_interface(spi_obj,INDIRECT_ACCESS_REGISTER_READ);
	DBMD_PRINTF("Register 0x%d = 0x%X = %d\n",reg, (val&0xffff), (val&0xffff));
	return val;
}


/***********************************************************************
* FUNCTION NAME: ASRP_write_register()
*
* DESCRIPTION:
*	write dbmdx ASRP register value using SPI.
*	using indirect commands since the address of registers are above 0x100
* PARAMETERS:
*	int fd, int16_t reg, int16_t val
* RETURNS:
***********************************************************************/
void ASRP_write_register(spi_t *spi_obj,int16_t reg, int16_t val)
{
	write_register_host_interface(spi_obj,INDIRECT_ACCESS_REGISTER_NUMBER, reg);
	write_register_host_interface(spi_obj,INDIRECT_ACCESS_REGISTER_WRITE, val);
	DBMD_PRINTF("Register 0x%d = 0x%X = %d\n",reg, (val&0xffff), (val&0xffff));
}

/* FUNCTION NAME: atoh__(const char *String)
*
* DESCRIPTION:
*	Convert Ascci to Hex.
*
* PARAMETERS:
*	const char *String
*
* RETURNS:
*	integer value
***********************************************************************/
//static 
unsigned int atoh__(const char *String)
{
    int Value = 0, Digit;
    char c;

    while ((c = *String++) != '\0')
    {
        if (c >= '0' && c <= '9')
            Digit = (uint) (c - '0');
        else if (c >= 'a' && c <= 'f')
            Digit = (uint) (c - 'a') + 10;
        else if (c >= 'A' && c <= 'F')
            Digit = (uint) (c - 'A') + 10;
        else
            break;

        Value = (Value << 4) + Digit;
    }

    return Value;
}


