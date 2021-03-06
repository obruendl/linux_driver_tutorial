

//Struct used as parameter to one IOCTL call
typedef struct date_s {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} date_t;


//IOCTL commands
#define WR_SW_VERSION _IOW('a','a',uint32_t)
#define RD_SW_VERSION _IOR('a','b',uint32_t*)
#define RD_FW_VERSION _IOR('a','c',uint32_t*)
#define RD_DATE _IOR('a','d', date_t*)
 

