#ifndef DBMD_INIT_H
#define DBMD_INIT_H

#include "spi_api.h"

#define	DBMD_PRINTF		1
#if DBMD_PRINTF
#define	DBMD_PRINTF(fmt,args...) \
			printf("[%d]%s():"fmt,__LINE__,__FUNCTION__,##args);
#else
#define	DBMD_PRINTF(fmt,args...)
#endif

extern spi_t spi_master;
extern void wait_ms(u32);

void DBMD_INIT_ENTYR();

#endif
