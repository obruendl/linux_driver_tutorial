#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


int main()
{
	//Declaration
    int f;
    char* ptr;
    int i;

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

    //Test IRQ
    for (i=0; i<10; i++) {
    	uint32_t intInfo;

    	//Acknowledge IRQ
    	if (write(f, &intInfo, sizeof(intInfo)) < 0) {
    		printf("Failed to acknowledge IRQ: %s\n", strerror(errno));
    		return -1;
    	}

    	//Wait for next IRQ
    	if (read(f, &intInfo, sizeof(intInfo)) <0) {
    		printf("Failed to wait for IRQ: %s\n", strerror(errno));
    		return -1;
    	}
    	//... The read value is the number of IRQs that occurred.
    	//... not that select() can be used as well.

    	//Print IRQ Info
    	printf("Received IRQ\n");
    }


    return 0;
}
