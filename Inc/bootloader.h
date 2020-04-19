#ifndef _BOOTLOADER_H
#define _BOOTLOADER_H

#define APP_ADDRESS (uint32_t)0x08008000
#define END_ADDRESS (uint32_t)0x080FFFFB
#define CRC_ADDRESS (uint32_t)0x080FFFFC

void bootloaderJump(void);
	
#endif
