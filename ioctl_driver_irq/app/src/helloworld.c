#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

//Include driver header
#include "../../fpga_base_ioctl_irq.h"


int main()
{
    //Declarations
    int f;
    date_t date;
    uint32_t version;
    int i;

    //Initial print
    printf("Hello World\n");

    //Open device
    f = open("/dev/fpga_base", O_RDWR);
    if (f < 0) {
        printf("Failed to open\n");
    }
    //Operrate driver using IOCTL
    ioctl(f, RD_FW_VERSION, &version);
    printf("version=0x%08X\n", version);
    ioctl(f, RD_DATE, &date);
    printf("year=%d\n", date.year);
    ioctl(f, WR_SW_VERSION, 123);
    ioctl(f, RD_SW_VERSION, &version);
    printf("swversion=0x%d\n", version);

    //Test IRQ
    ioctl(f, CLEAR_IRQCNT, NULL); //First clear the counter to flush all IRQs detected before this line is executed
    for (i=0; i<10; i++) {
        //Wait for next IRQ
        ioctl(f, WAIT_IRQ, NULL);

        //Print IRQ Info
        printf("Received IRQ\n");
    }


    return 0;
}
