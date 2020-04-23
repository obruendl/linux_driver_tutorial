#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

//Include driver header
#include "../../fpga_base_ioctl.h"


int main()
{
    //Declarations
    int f;
    date_t date;
    uint32_t version;

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

    return 0;
}
