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


#include "dbmdx_melon_def.h"
#include "LoadFile.h"
#include "Dbmd_init.h"
#include "UserCase_Commom.h"


#define FLASH_DATA_LEN		4096

#define FLASH_FIRMWARE_ADDR	0x000F5000			//user data base addr	

#define FLASH_A_MODEL_ADDR	0x00140000			//300k space for firwware

#define FLASH_MIC_ADDR		0X00159000			//100k space for A_Model

//#define MIC_DATA_ADDR		0x00160000

//#define FIRMWARE_TYPE		0
//#define ASRP_TYPE			1

/**********************
*****invoke load firmware **
**********************/
int load_file(int load_type,int skip_bytes)
{
	char *checksum;
	int rc;
	u32 base_addr;
	long err = 1;
	const char clr_crc_d4[2] = {0x5a,0x0f};
	int firmware_len,tmp_len,read_len,addr_offset_len;
	flash_t flash;
	char *flash_buffer = (char *) malloc(FLASH_DATA_LEN);
	if(flash_buffer == NULL)
	{
		printf("%s malloc failed\n",__func__);
		return -1;
	}
	if(load_type == FIRMWARE_TYPE)
	{
		firmware_len = 211240;
		tmp_len = 211240;
		base_addr = FLASH_FIRMWARE_ADDR;
	}
	else if(load_type == ASRP_TYPE)
	{
#if 1
		firmware_len = 2540;
		tmp_len = 2540;
#else
		firmware_len = 2530;
		tmp_len = 2530;
#endif
		base_addr = FLASH_MIC_ADDR;
	}
	addr_offset_len = 0;
	if(skip_bytes > 0)
	{
		printf("start load file\n");
		device_write_spi(&spi_master,clr_crc_d4,2);	//send upload command
	}
	
	do
	{
		if(tmp_len < FLASH_DATA_LEN)
		{
			read_len = tmp_len;		
		}
		else
		{
			read_len = FLASH_DATA_LEN;
		}
		
		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_stream_read(&flash, base_addr+addr_offset_len, read_len, flash_buffer);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		tmp_len -= read_len;
		addr_offset_len += read_len;

		//send firmware
		//we can judge it ok or not
		//when read last packet tmp_len will be zero
		if((read_len < FLASH_DATA_LEN) || (tmp_len == 0))
			device_write_spi(&spi_master,flash_buffer,(read_len-skip_bytes));
		else
			device_write_spi(&spi_master,flash_buffer,read_len);
		//printf("read flash to send buf[0]=%d,buf[end]=%d\n",flash_buffer[0],flash_buffer[read_len-1]);
		
	}while(tmp_len > 0);

	if(skip_bytes > 0)
	{
		checksum = &flash_buffer[read_len - skip_bytes];
		rc = read_checksum(checksum);
		if(rc < 0)
		{
			printf("[%s]: fail read checksum.", __func__);
			err = -1;
			//goto error_2;
		}
	}

//error_2:
	free(flash_buffer);
	
	return err;
}

/***********************************************************************
* FUNCTION NAME: load_amodel_file()
*

* DESCRIPTION:
*		load acoustic model file into buffer
* 		and transmit it to DBMDX over the SPI.
* 		Separately header / data.
* PARAMETERS:
*		char *filename, int file descriptor
* RETURNS:
*		0 on sucsses
*		error otherwise
***********************************************************************/
int load_amodel_file()
{
	//LAST 2 SIZE NOT SEND ACCORD D4P
	int rc;
	long err = 1;
	int firmware_len,tmp_len,read_len,addr_offset_len;
	unsigned char header_buffer[10];
	flash_t flash;
	uint8_t *flash_buffer = (uint8_t *) malloc(FLASH_DATA_LEN);
	if(flash_buffer == NULL)
	{
		printf("%s malloc failed\n",__func__);
		return -1;
	}
#if 1
	firmware_len = 68470;
	tmp_len = 68470;
#else
	firmware_len = 68460;		//it wrong length to certify what will happen
	tmp_len = 68460;
#endif
	addr_offset_len = 0;
	printf("start A MODEL file\n");
	
	do
	{
		if(tmp_len < FLASH_DATA_LEN)
		{
			read_len = tmp_len;		
		}
		else
		{
			read_len = FLASH_DATA_LEN;
		}
		
		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_stream_read(&flash, FLASH_A_MODEL_ADDR+addr_offset_len, read_len, flash_buffer);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		tmp_len -= read_len;
		addr_offset_len += read_len;

		//send firmware
		//we can judge it ok or not

		device_write_spi(&spi_master,flash_buffer,read_len);
		//printf("read flash to send buf[0]=%d,buf[end]=%d\n",flash_buffer[0],flash_buffer[read_len-1]);
		
	}while(tmp_len > 0);


	free(flash_buffer);
	return 0;
}


/***********************************************************************
* FUNCTION NAME: Load_A_Model(char *primary, char *secondary)
*
* DESCRIPTION:
*	Load VT acoustic model to memory
* PARAMETERS:
*	char *filename
* RETURNS:
*	0 on sucsses else error
***********************************************************************/
int Load_A_Model()
{
	int value;
	int err = 0;

	/* Load Trigger Acoustic Model */
	write_register_host_interface(&spi_master,LOAD_NEW_ACOUSTIC_MODEL, LOAD_TRIGGER_ACOUSTIC_MODEL);
	printf("[%s]: Start uploading Trigger Acoustic Model to D4\n", __func__);

	//load_amodel_file(Acoustic_Model_filename, instance_D4);
	load_amodel_file();
	printf("[%s]: Done uploading Trigger Acoustic model.\n", __func__);

	Run_D4();
	//D4P verify a model file is correct
	//above func test when file is not complete
	/* verify loading acoustic model */
	value = read_register_host_interface(&spi_master,VT_ENGINE_INITIALIZED);
	if (value != 0x01) {
		printf("[%s]: Trigger Acoustic model was not successfully uploaded %d !\n", __func__, value);
		err = -1;
		goto out;
	} else {
		printf("[%s]: Trigger Acoustic model was successfully uploaded!\n", __func__);
	}
	
#if 1
	printf("****fu debug test**************\n");
	value = read_register_host_interface(&spi_master,VT_ENGINE_RECOGMODE);
	printf("VT_ENGINE_RECOGMODE value = %d\n",value);

	value = read_register_host_interface(&spi_master,VT_ENGINE_LPSD_VAD);
	printf("VT_ENGINE_LPSD_VAD value = %d\n",value);

	value = read_register_host_interface(&spi_master,VT_ENGINE_DUALTHRESHOLD);
	printf("VT_ENGINE_DUALTHRESHOLD value = %d\n",value);

	value = read_register_host_interface(&spi_master,VT_ENGINE_WORDID);
	printf("VT_ENGINE_WORDID value = %d\n",value);

	value = read_register_host_interface(&spi_master,VT_ENGINE_PHRASE_LENGTH);
	printf("VT_ENGINE_PHRASE_LENGTH value = %d\n",value);
	printf("******************\n");
#endif
out:
	return err;
}

/***********************************************************************
* FUNCTION NAME: Load_ASRP_paramD4()
*
* DESCRIPTION:
*	Config DBMD2
*
* PARAMETERS:
*	int fd, int param
* RETURNS:
* 	0 on sucsses elese device_abort
***********************************************************************/
int Load_ASRP_paramD4()
{
	int value;
	long err = 1;

	wait_ms(10);
	write_register_host_interface(&spi_master,LOAD_NEW_ACOUSTIC_MODEL, LOAD_COMMAND_ASRP_PARAM);
	printf("[%s]: Start uploading ASRP param.\n", __func__);
	wait_ms(100);

	if(!load_file(ASRP_TYPE,0)) {
		perror ("fail uploading ASRP MODEL.\n");
		err = -1;
		goto out;
	}

	printf("[%s]: Done uploading ASRP model.\n", __func__);
	wait_ms(100);

//debug find when load_file ,reg100,reg101 is not zero ,or it will zero
#if	1

	value = read_register_host_interface(&spi_master,0x19);
	if (value != 0)	{
		printf("[%s]: Chip version = 0x%04X\n", __func__, value & 0xffff);
	} else {
		printf("[%s]: FAIL read_register_host_interface(0x19)\n", __func__);
	}

	/*	at the moment the param file containt the run command
		verify loading acoustic model */
	value = ASRP_read_register(&spi_master,0x100);
	printf ("ASRP_read reg 0x100=  0x%X \n",value&0xffff);

	value = ASRP_read_register(&spi_master,0x101);
	printf ("ASRP_read reg 0x101 =  0x%X \n",value&0xffff);

	value = ASRP_read_register(&spi_master,0x103);
	printf ("ASRP_read reg 0x103=  0x%X \n",value&0xffff);

	value = ASRP_read_register(&spi_master,0x104);
	printf ("ASRP_read reg 0x104 =  0x%X \n",value&0xffff);

	value = ASRP_read_register(&spi_master,0x105);
	printf ("ASRP_read reg 0x105 =  0x%X \n",value&0xffff);

#endif
	wait_ms(10);
//	write_register_host_interface(fd, 0x18, 0xa);

out:
	return err;
}

int read_checksum(char *checksum)
{
	char buf[2] = {0x5A, 0x0E};
	char c[7];
	int rc;

	rc = device_write_spi(&spi_master,buf,2);
	if(-1 == rc)
		goto out;
	wait_ms(1);
	rc = device_read_spi(&spi_master,c,7);

	if(-1 == rc)
		goto out;

	rc = strncmp(&c[3],checksum, 4);
	if (rc)
		printf ("[%s]: checksum error : got: = %2x %2x %2x %2x expected : %2x %2x %2x %2x\n",
			__func__, c[3],c[4],c[5],c[6],checksum[0],checksum[1],checksum[2],checksum[3]);
	else
		printf("[%s]: checksum pass\n", __func__);

out:
	return rc;
}



