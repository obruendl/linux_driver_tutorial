

//Struct used as parameter to one IOCTL call
typedef struct date_s {
	int year;
	int month;
	int day;
	int hour;
	int minute;
} date_t;

//Callback function
typedef void (*callback_f)(void*);


//IOCTL commands
#define WR_SW_VERSION _IOW('a','a',uint32_t)
#define RD_SW_VERSION _IOR('a','b',uint32_t*)
#define RD_FW_VERSION _IOR('a','c',uint32_t*)
#define RD_DATE _IOR('a','d', date_t*)
#define WAIT_IRQ _IOR('a', 'e', void*)
#define CLEAR_IRQCNT _IOR('a', 'f', void*)

