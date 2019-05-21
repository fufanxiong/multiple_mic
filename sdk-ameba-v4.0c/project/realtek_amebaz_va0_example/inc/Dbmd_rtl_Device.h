#ifndef DBMD_RTL_DEVICE_H
#define DBMD_RTL_DEVICE_H
#include "device.h"
#include "spi_api.h"
#include "spi_ex_api.h"

int device_read_spi(spi_t *spi_obj,char *buf,unsigned int len);
int device_write_spi(spi_t *spi_obj,char * buf,unsigned int len);
uint16_t read_register_spi(spi_t *spi_obj,int16_t reg);
int device_special_read_register_spi(spi_t *spi_obj,char *txbuf,char *rxbuf,unsigned int len);
void write_register_spi(spi_t *spi_obj,int16_t reg, int16_t val);
int16_t ASRP_read_register(spi_t *spi_obj,int16_t reg);
void ASRP_write_register(spi_t *spi_obj,int16_t reg, int16_t val);
unsigned int atoh__(const char *String);

#endif