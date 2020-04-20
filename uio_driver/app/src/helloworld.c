/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

#define uio

int main()
{
    printf("Hello World\n");

    int f;
    char* ptr;

#ifdef uio
    f = open("/dev/uio0", O_RDWR);
    if (f < 0) {
    	printf("Failed to open\n");
    }
    ptr = mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (ptr == MAP_FAILED) {
    	printf("Failed to map\n");
    }
    uint32_t version = *((uint32_t*)&ptr[0x00]);
    uint32_t year = *((uint32_t*)&ptr[0x04]);
    printf("version=0x%08X\n", version);
    printf("year=%d\n", year);
#endif

#ifndef uio
    typedef struct date_s {
    	int year;
    	int month;
    	int day;
    	int hour;
    	int minute;
    } date_t;

    date_t date;

	#define WR_SW_VERSION _IOW('a', 'a', uint32_t)
	#define RD_SW_VERSION _IOR('a', 'b', uint32_t*)
	#define RD_FW_VERSION _IOR('a', 'c', uint32_t*)
	#define RD_DATE _IOR('a', 'd', date_t*)

    f = open("/dev/fpga_base", O_RDWR);
    if (f < 0) {
    	printf("Failed to open\n");
    }
    uint32_t version;
    ioctl(f, RD_FW_VERSION, &version);
    printf("version=0x%08X\n", version);
    ioctl(f, RD_DATE, &date);
    printf("year=%d\n", date.year);
    ioctl(f, WR_SW_VERSION, 123);
    ioctl(f, RD_SW_VERSION, &version);
    printf("swversion=0x%d\n", version);
#endif

    return 0;
}
