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


int main()
{
	//Declaration
    int f;
    char* ptr;

    //Initial print
    printf("Hello World\n");

    //Open UIO device
    f = open("/dev/uio0", O_RDWR);
    if (f < 0) {
    	printf("Failed to open\n");
    }

    //Map memory to user space
    ptr = mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (ptr == MAP_FAILED) {
    	printf("Failed to map\n");
    }

    //Memory can now be accessed
    //Read
    uint32_t version = *((uint32_t*)&ptr[0x00]);
    uint32_t year = *((uint32_t*)&ptr[0x04]);
    printf("version=0x%08X\n", version);
    printf("year=%d\n", year);
    //Write
    *((uint32_t*)&ptr[0x18]) = 0xABCD;
    version = *((uint32_t*)&ptr[0x18]);
    printf("sw-version=0x%08X\n", version);
    return 0;
}
