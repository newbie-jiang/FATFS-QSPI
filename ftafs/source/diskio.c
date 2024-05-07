/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "bsp_qspi.h"
#include "stdio.h"


/* Definitions of physical drive number for each drive */

#define DEV_CHIP_FLASH		0	

#define DEV_SPI		        1	

    

 	
#define qspi_file_sys_base_address		0x00000000	 /* base address 6M */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) 
	{

			
		case DEV_SPI :
			 result = 0;
			 stat = (DSTATUS)result;
			 return stat;			
       
	}
	  
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;
    uint16_t i;
	switch (pdrv) {
		

		case DEV_SPI :
			QSPI_W25Qxx_Init();
			i=500;
			while(--i);	
			QSPI_W25Qxx_Reset();
		    result = 0;
			stat = (DSTATUS)result;
			return stat;
		
	}

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	
	case DEV_SPI :
		for(;count>0;count--)
		{
			QSPI_W25Qxx_ReadBuffer(buff,(sector*4096),4096);
			sector++;
			buff+=4096;
		}				
		 result = 0;
	     res = (DRESULT)result;
		 return res;       	
	}
	
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {	

	
		
	case DEV_SPI :
		for(;count>0;count--)
		{			
            QSPI_W25Qxx_SectorErase(sector*4096);			
			QSPI_W25Qxx_WriteBuffer((uint8_t*)buff,(sector*4096),4096);
			sector++;
			buff+=4096;
		}
      
			result = 0;
			res = (DRESULT)result;
			return res;
  }	
	  

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	

	
		  
		 case DEV_SPI :
			switch (cmd) 
			{
				case GET_SECTOR_COUNT:
    	//         	  *(DWORD*)buff =15360;//7.86M
					  *(DWORD*)buff =2048;
					  result = 0;
				
					break;
			    case GET_SECTOR_SIZE :
					  *(WORD*)buff = 4096; 
					  result = 0;
					break;
				case GET_BLOCK_SIZE :
					  *(WORD*)buff = 128;
					  result = 0;
					break;        
          }
				result = 0;
				res = (DRESULT)result;
				return res; 			
          
	}
	

	return RES_PARERR;
}





FATFS fss;           // FatFs file system object
FIL fil;            // File object
FRESULT fr;         // To store the result of FatFs functions
UINT bw, br;        // File read/write count
char my_buffer[1024];  // File read/write buffer



/* 调用该函数可能会导致open失败 由于多线程影响 */
void my_test_fatfs(const char* drive) {
    // Mount or format the drive

    fr = f_mount(&fss, drive, 1);
    if (fr == FR_NO_FILESYSTEM) {
        // No filesystem, need to format the drive
        printf("No filesystem found on %s, need to format the drive...\r\n", drive);
        fr = f_mkfs(drive, 0, 0, 4096);		
        if (fr != FR_OK) {
            printf("Error formatting the drive %s: %d\r\n", drive, fr);
            return;
        }
        printf("Drive %s formatted successfully.\r\n", drive);
    } else if (fr != FR_OK) {
        printf("Failed to mount the filesystem on %s: %d\r\n", drive, fr);
        return;
    }
        printf("Filesystem mounted successfully on %s.\r\n", drive);

    // Create or open a file
    char filepath[30];
    sprintf(filepath, "%stest.txt", drive);
    fr = f_open(&fil, filepath, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    if (fr != FR_OK) {
        printf("Failed to open '%s': %d\r\n", filepath, fr);
        return;
    }

    // Write data to the file
    fr = f_write(&fil, "Hello, world!", 13, &bw);
    if (fr != FR_OK) {
        printf("Failed to write to '%s': %d\r\n", filepath, fr);
        f_close(&fil);
        return;
    }
    printf("Written to '%s': %u bytes\r\n", filepath, bw);

    // Close the file to save the data
    f_close(&fil);

    // Open the file to read
    fr = f_open(&fil, filepath, FA_READ);
    if (fr != FR_OK) {
        printf("Failed to open '%s' for reading: %d\r\n", filepath, fr);
        return;
    }

    // Read data from the file
    fr = f_read(&fil, my_buffer, sizeof(my_buffer), &br);
    if (fr != FR_OK) {
        printf("Failed to read from '%s': %d\r\n", filepath, fr);
        f_close(&fil);
        return;
    }
    my_buffer[br] = 0; // Ensure string is null-terminated
    printf("Read from '%s': %s\r\n", filepath, my_buffer);

    // Close the file
    f_close(&fil);

    // Optionally, unmount the filesystem
    f_mount(NULL, drive, 0);
}




